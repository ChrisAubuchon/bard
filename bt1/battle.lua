require "objectHash"
require "character"
require "monster"
require "btdebug"

local battleData = {}

battleSummon	= {}

-- Public battle interface
--
battle = {}
function battle:random()
	local b = battleData:new()
	local mgroups
	local mgroup

	b.isPartyAttack = false
	b.monGroups = monsterParty.new()
	mgroups = currentLevel:getBattleOpponents()
	for _,mgroup in ipairs(mgroups) do
		b.monGroups:addMonsterGroup(monsterGroup:new(mgroup, false))
	end

	return b:start()
end

function battle:partyAttack()
	local b = battleData:new()

	b.isPartyAttack = true
	b.battleGroup	= false

	return b:start()
end

function battle:new(...)
	local b = battleData:new()
	local name
	local count
	local i

	b.isPartyAttack = false

	b.monGroups = monsterParty:new()

	for i = 1,select('#', ...),2 do
		name,count = select(i, ...)

		if (type(name) ~= "string") then
			error("Invalid monster type: "..tostring(type(name)),2)
		end

		if ((type(count) ~= "number")and(type(count) ~= "boolean")) then
			error("Invalid monster count: "..tostring(count))
		end

		local g = monsterGroup:new(name, count)
		b.monGroups:addMonsterGroup(g)
	end

	return b:start()
end


----------------------------------------
-- Internal battle Classes
----------------------------------------

----------------------------------------
-- battleData class
--
-- This class holds all of the data
-- for a battle
----------------------------------------
function battleData:new()
	local self = {
		isPartyAttack		= false,
		isPartyBigpicDone	= false,
		skipParty		= false,
		numGroups		= 0,
		monGroups	 	= false,
		actionHead		= false,
		actionTail		= false,
		songToHitBonus		= 0,
		songDamageBonus		= 0,
		battleDataBySource	= {},
		fearPenalty		= btTable.new(true),
		battleSkillBonus	= btTable.new(true),
		freezeFoePenalty	= btTable.new(true),
		vorpalBonus		= btTable.new(true),
		killCount		= btTable.new(true),
		mageStarFlag		= {},

		--
		-- The songToHitPenalty isn't used in the DOS version
		-- of BT1. I've added it here but it isn't used by this
		-- code.
		--
		songToHitPenalty	= 0,
		preBattleSong		= false
	}

	btTable.addParent(self, battleData)
	btTable.setClassMetatable(self)

	party.battle.antiMagic = 0
	party.battle.acBonus = 0


	return self
end

function battleData:addAction(inSource, inAction)
	self.battleDataBySource[inSource.key] = inAction
end

function battleData:removeAction(inSource)
	self.battleDataBySource[inSource.key] = nil
end

function battleData:resetPriority()
	self.battleDatabySource	= {}
end

function battleData:addPriority(inSource, inPriority)
	local action = self.battleDataBySource[inSource.key]

	dprint("inSource.singular: " .. tostring(inSource:getSingularName()))
	dprint("inSource.key: " .. inSource.key)
	action.priority = inPriority

	self.battleDataBySource[inSource.key] = action

	if (not self.actionHead) then
		self.actionHead = action
		self.actionTail = action
		return
	end

	local current = self.actionHead
	while (current.priority > action.priority) do
		current = current.next
		if (not current) then break end
	end

	-- New tail
	if (not current) then
		action.prev = self.actionTail
		self.actionTail.next = action
		self.actionTail = action
		return
	end

	if (current == self.actionHead) then
		self.actionHead = action
	end

	action.next = current
	if (current.prev) then
		current.prev.next = action
	end
	action.prev = current.prev
	current.prev = action
end

function battleData:removePriority(inSource)
	local action = self.battleDataBySource[inSource.key]

	if (not action) then
		return
	end

	self.battleDataBySource[inSource.key] = nil
	if (action.next) then
		action.next.prev = action.prev
	end
	if (action.prev) then
		action.prev.next = action.next
	end
end

function battleData:dumpPriorities()
	local a

	a = self.actionHead
	while (a) do
		print("Source: " .. a.source:getSingularName())
		print("Priority: " .. tostring(a.priority))
		print("---")
		a = a.next
	end
end

----------------------------------------
-- battleData class
--
-- This class holds all of the functions
-- used during battle. This is to
-- separate data from functions
----------------------------------------

function battleData:start()
	local mgroup
	local continue = true
	local partyRan = false

	self:convertPreBattleSong()

	if (not self.isPartyAttack) then
		for mgroup in self.monGroups:iterator() do
			self.killCount[mgroup:getSingularName()] = 0
		end
	end

	self:printEncounter()

	dprint("party.battle.antiMagic = %d", party.battle.antiMagic)
	dprint("self.songDamageBonus = %d", self.songDamageBonus)
	repeat
		self:updateBigpic()
		if (not self:getPlayerOptions()) then
			partyRan = true
			break
		end

		self:getMonsterActions()
		self:getPriorities()

		self:doRound()
		self:endRound()
		if (globals.partyDied) then
			return false
		end

		if (self.isPartyAttack) then
			if (not party:isHostile()) then
				text:cdprint(true, false, 
					"Do you wish to continue?")
				if (not text:getYesNo()) then
					break
				end
			end
		elseif (self.monGroups.size >= 1) then
			text:cdprint(true, false, "\nYou still face ")
			self:printMonsterGroups()

			local xxx_monsters_advance = true
		else
			break
		end
	until (continue == false)

	if ((not partyRan) and (not self.isPartyAttack)) then
		self:doReward()
	end

	self:stop(partyRan)

	return true
end

function battleData:stop()
	text:clear()
	party:display()
end

function battleData:updateBigpic()
	if (self.isPartyAttack) then
		if (not self.isPartyBigpicDone) then
			bigpic:setTitle("PARTY")
			bigpic:drawImage("PIC_WARRIOR")
			self.isPartyBigpicDone = true
		end
	else
		local leadGroup = self.monGroups:getLeadGroup()
		if (leadGroup.size == 1) then
			bigpic:setTitle(leadGroup.singular)
		else
			bigpic:setTitle(leadGroup.plural)
		end
		bigpic:drawImage(leadGroup.picture)
	end
end

local encounterStringList = {
	"\nBefore you, you see ",
	"\nThe sudden scream of battle brings your party to a halt. You face ",
	"\nThe denizens of this mystic place assault you without warning. You see ",
	"\nYou face death itself in the form of "
}

function battleData:printEncounter()
	text:clear()

	if (self.isPartyAttack) then
		text:print("\nThere is dissention in your ranks...\n\n")
	else
		text:print(encounterStringList[rnd_xdy(1,#encounterStringList)])
		self:printMonsterGroups()
	end
end

function battleData:printMonsterGroups()
	local i
	local mgroup

	if (self.isPartyAttack) then
		text:print("\nhostile party members!\n\n")
		return
	end

	for i,mgroup in self.monGroups:ipairs() do
		if (i ~= 1) then
			if ((i == 4) or (not self.monGroups:isGroup(i+1))) then
				text:print(", and ")
			else
				text:print(", ")
			end
		end

		text:print("%d ", mgroup.size)
		if (mgroup.size == 1) then
			text:print(mgroup.singular)
		else
			text:print(mgroup.plural)
		end
	end
	text:print(".\n\n")
end

----------------------------------------
--
-- Battle Round section
--
----------------------------------------

function battleData:doRound()
	local currentAction

	currentAction = self.actionHead
	text:setCursor(0, 11)
	while (currentAction) do
		currentAction.inBattle = self
		dprint(tostring(currentAction.source))
		currentAction.source:doAction(currentAction)

		if (globals.partyDied) then
			return
		end

		currentAction = currentAction.next
	end
	if (self.monGroups) then
		self.monGroups:adjustMeleeGroups()
	end
end

function battleData:endRound()
	local mgroup

	self.actionHead = false
	self.actionTail = false
	self.battleDataBySource = {}

	if (self.monGroups) then
		for mgroup in self.monGroups:iterator() do
			self.mageStarFlag[mgroup.key] = false
		end
	end

	local xxx_party_checkDisbelieve = true
	if (globals.partyDied) then
		return
	end

	-- This looks like it is skipped in the DOS version.
	-- It is triggered by "cmp byte_3B76E, 0" but byte_3B76E
	-- is never set anywhere...
	--
	local xxx_monster_disbelieve = true

	party:doPoison()
	party:doEquippedEffects()

	if (party.songHpRegen) then
		local char

		for char in party:iterator() do
			if (not char:isDisabled()) then
				if (char.max_hp > char.cur_hp) then
					char.cur_hp = char.cur_hp + 1
				end
			end
		end
	end

	self:postRoundCleanup()
	if (globals.partyDied) then
		return
	end
end

function battleData:postRoundCleanup()
	local i
	local c

	c = party.summon
	if ((c) and ((c.cur_hp == 0) or (c.isStoned) or (c.isParalyzed) or
		     (c.isDead))) then
		party:removeSummon()
	end

	for i,c in party:ipairs() do
		if (c.isDoppelganger) then
			if (c.isDead or c.isStoned or c.isParalyzed) then
				party:removeCharacter(i)
			end
		end
	end

	if (not party:isLive()) then
		return
	end

	party:sort()
end

function battleData:getPriorities()
	local p
	local c

	if (not self.skipParty) then
		for c in party:characterIterator() do
			if (not c:isDisabled()) then
				p = c:getBattlePriority()
				self:addPriority(c, p)
			end
		end
	end

	if (party.summon) then
		p = party.summon:getBattlePriority()
		self:addPriority(party.summon, p)
	end

	if (self.isPartyAttack) then
		return
	end

	for p in self.monGroups:iterator() do
		for c in p:iterator() do
			self:addPriority(c, c:getBattlePriority())
			c.beenAttacked = false
		end
	end
end


----------------------------------------
--
-- Player battle option section
--
----------------------------------------
function battleData:getPlayerOptions()
	local i
	local c
	local done = false

	if (self:getRunFightOption()) then
		return false
	end

	repeat
		if (party.summon) then
			dprint("Adding a summon option")
			local a = btAction:new()
			a.source = party.summon
			a.action = battle.doSummonAttack
			self:addAction(a.source, a)
		end

		for i,c in party:ipairs() do
			self:addAction(c, self:getPlayerOption(i,c))
		end

		text:cdprint(true, false, "Use these attack commands?")
		if (not text:getYesNo()) then
			for c in party:iterator() do
				self:removeAction(c)
			end
		else
			text:clear()
			done = true
		end	
	until (done)

	return true
end

function battleData:getRunFightOption()
	local inkey

	if (self.isPartyAttack) then
		timer:delay(3)
		return false
	end


	if (party:isHostile()) then
		return false
	end

	text:print("Will your stalwart band choose to\n")
	text:print("Fight or\n")
	text:print("Run?")

	repeat
		inkey = getkey()

		if (inkey == "R") then
			local xxx_run_check = true
			return true
		end
	until (inkey == "F")

	return false
end

function battleData:getPlayerOption(partySlot, c)
	local action
	local options = {}
	local inkey

	false_table(options)

	if (c:isDisabled()) then
		return false
	end

	action = btAction.new()
	action.source = c

	if ((c.isPossessed) or (c.isNuts)) then
		local xxx_possessedAttack = true
		return false
	end

	while true do
		text:clear()
		text:print(c.name.." has these options this battle round:\n\n")

		text:print("Party attack\n")
		options["P"] = true

		if ((not self.isPartyAttack) and (partySlot < 4)) then
			text:print("Attack foes\n")
			options["A"] = true
		end

		text:print("Defend\n")
		options["D"] = true

		if (c.cur_sppt > 0) then
			text:print("Cast a spell\n")
			options["C"] = true
		end

		text:print("Use an item\n")
		options["U"] = true

		if (c.class == "Rogue") then
			text:print("Hide in shadows\n")
			options["H"] = true
		end

		if ((c.class == "Bard") and 
		    (c:isTypeEquipped("Instrument"))) then
			text:print("Bard Song\n")
			options["B"] = true
		end

		text:print("\nSelect an option.")

		while true do
			inkey = getkey()

			if (options[inkey]) then
				if (inkey == "A") then
					action.action = "melee"
					if (self:meleeTarget(action)) then
						return action
					end
				elseif (inkey == "B") then
					action.action = "sing"
					if (action.source:getTune(action, true)) then
						return action
					end
				elseif (inkey == "C") then
					action.action = "cast"
					if (action.source:getCombatSpell(action)) then
						return action
					end
				elseif (inkey == "D") then
					action.action = "defend"
					return action
				elseif (inkey == "H") then
					action.action = "hide"
					return action
				elseif (inkey == "P") then
					action.action = "melee"
					if (self:meleeTarget(action)) then
						return action
					end
				elseif (inkey == "U") then
					action.action = "use"
					if (c:getUseItem(action)) then
						return action
					end
				end
			end
		end
	end
end

function battleData:meleeTarget(inAction)
	if (self.isPartyAttack) then
		text:cdprint(true, false, "Attack:")
		inAction.target = getActionTarget({party = true, summon = true},
						self.monGroups)
		if (not inAction.target) then
			return false
		end
	else
		if (self.monGroups.size > 1) then
			text:cdprint(true, false, "Attack:")
		end

		inAction.target = getActionTarget({melee = true}, 
							self.monGroups)
		if (not inAction.target) then
			return false
		end
	end

	return true
end

function battleData:getMonsterActions()
	local action
	local mgroup
	local m

	if (self.isPartyAttack) then
		return
	end

	for mgroup in self.monGroups:iterator() do
		for m in mgroup:iterator() do
			action = btAction.new()
			action.source = m
			action.action = self.doMonsterAttack
			self:addAction(m, action)
		end
	end
end

function battleData:doReward()
	local mgroup
	local nmonsters
	local i
	local xp		= 0
	local au		= 0
	local itemsToGive	= 0
	local giveGold		= true
	local partySize		= 0

	if (globals.gameState == globals.STATE_DUNGEON) then
		local xxx_do_treasure_chest = true
	end

	bigpic:drawImage("PIC_TREASURE")
	bigpic:setTitle("Treasure!")

	for mgroup,nmonsters in pairs(self.killCount) do
		for i = 1,nmonsters do
			xp = xp + monster.getXPReward(mgroup)
			if (rnd_and_x(7) == 0) then
				itemsToGive = itemsToGive + 1
			end

			if (giveGold) then
				if ((globals.gameState == globals.STATE_CITY)
				    and (not globals.isNight)) then
					au = au + rnd_between_xy_inc(1,128)
				else
					au = au + rnd_between_xy_inc(1,256)
				end
			end
		end
	end

	partySize = party:getLastLiveCharacter()
	xp = xp / partySize
	au = au / partySize

	text:cdprint(true, false, "Each character receives %d experience points for valor and battle knowledge, and %d pieces of gold.\n\n", xp, au)

	for i in party:liveCharacterIterator() do
		i.xp = i.xp + xp
		i.gold = i.gold + au
		i.battlesWon = i.battlesWon + 1
	end

	if (giveGold) then
		if ((itemsToGive == 0) and (currentLevel.level > 4)) then
			itemsToGive = 1
		end

		if (currentLevel.level < 5) then
			if (itemsToGive > 1) then
				itemsToGive = 1
			end
		else
			if (itemsToGive > 2) then
				itemsToGive = 2
			end
		end

		if (itemsToGive == 0) then
			timer:delay(8)
			return
		end
	else
		timer:delay(10)
	end
end


function battleData:convertPreBattleSong()
	if (party.song.active) then
		local singer = party.song.singer

		self.preBattleSong = singer.song

		if ((singer.song.name == "Falkens Fury") or
		    (singer.song.name == "Lucklaran")) then
			dprint("Running combatFunction")
			local action = btAction.new()
			action.inBattle = self
			action.inData = singer.song.combatData[currentLevel.level].inData
			singer.song.combatFunction.func(action)
		end
	end
end
