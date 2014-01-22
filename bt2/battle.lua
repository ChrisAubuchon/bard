require "character"
require "monster"
require "btdebug"

-- Public battle interface
--
battle = {}

----------------------------------------
-- battle:init()
--
-- Initialize battle data structure
----------------------------------------
function battle:init()
	local self = object:new()

	self:addParent(battle)

	self.isPartyAttack	= false
	self.skipParty		= false
	self.monDisbelieve	= false
	self.numGroups		= 0
	self.monParty		= nil
	self.priorityQueue	= {}
	self.actionBySource	= {}
	self.actionList		= linkedList:new()
	self.killCount		= {}
	table.setDefault(self.killCount, 0)
	self.songToHitBonus	= 0
	self.songDamageBonus	= 0
	self.songToHitPenalty	= 0
	self.preBattleSong	= false

	self.currentBigpic	= false

	self.song = battleBonus:new()

	party.battle.antiMagic	= 0
	party.battle.acBonus	= 0

	return self
end

----------------------------------------
--
-- Battle Entry points
--
----------------------------------------

----------------------------------------
-- random()
--
-- Levelled random battle
----------------------------------------
function battle:random()
	local mgroups
	local mgroup

	currentBattle = battle:init()

	currentBattle.isPartyAttack = false
	currentBattle.monParty = monsterParty.new()
	mgroups = currentLevel:getBattleOpponents()
	for _,mgroup in ipairs(mgroups) do
		currentBattle.monParty:addMonsterGroup(monsterGroup:new(mgroup, false))
	end

	return currentBattle:start()
end

----------------------------------------
-- partyAttack()
----------------------------------------
function battle:partyAttack()
	currentBattle = battle:init()

	currentBattle.isPartyAttack = true
	currentBattle.battleGroup	= false

	return currentBattle:start()
end

----------------------------------------
-- new()
--
-- Predefined battle
-- Function arguments are
-- Monster type, Group size pairs
----------------------------------------
function battle:new(...)
	local name
	local count
	local i

	currentBattle = battle:init()
	currentBattle.isPartyAttack = false
	currentBattle.monParty = monsterParty:new()

	log:print(log.LOG_DEBUG, select('#', ...))
	for i = 1,select('#', ...),2 do
		name,count = select(i, ...)

		log:print(log.LOG_DEBUG, "name: %s, count: %d", name, count)

		if (type(name) ~= "string") then
			error("Invalid monster type: "..tostring(type(name)),2)
		end

		if ((type(count) ~= "number")and(type(count) ~= "boolean")) then
			error("Invalid monster count: "..tostring(count))
		end

		local g = monsterGroup:new(name, count)
		currentBattle.monParty:addMonsterGroup(g)
	end

	return currentBattle:start()
end


----------------------------------------
-- Internal battle Classes
----------------------------------------

----------------------------------------
-- battle class
--
-- This class holds all of the data
-- for a battle
----------------------------------------
if false then
function battle:new()
	local self = {
		isPartyAttack		= false,
		isPartyBigpicDone	= false,
		skipParty		= false,
		monDisbelieve		= false,
		numGroups		= 0,
		monParty		= false,
		actionHead		= false,
		actionTail		= false,
		actionBySource	= {},
		killCount		= btTable.new(true),
		songToHitBonus		= 0,
		sontDamageBonus		= 0,

		--
		-- The songToHitPenalty isn't used in the DOS version
		-- of BT1. I've added it here but it isn't used by this
		-- code.
		--
		songToHitPenalty	= 0,
		preBattleSong		= false
	}

	btTable.addParent(self, battle)
	btTable.setClassMetatable(self)

	party.battle.antiMagic = 0
	party.battle.acBonus = 0


	return self
end
end

----------------------------------------
-- battle:addAction()
----------------------------------------
function battle:addAction(inSource, inAction)
	self.actionBySource[inSource.key] = inAction
end

function battle:removeAction(inSource)
	self.actionBySource[inSource.key] = nil
end

function battle:resetPriority()
	self.actionBySource	= {}
	self.priorityQueue	= {}
end

----------------------------------------
-- battle:addPriority()
----------------------------------------
function battle:addPriority(inSource, inPriority)
	local action = self.actionBySource[inSource.key]

	action.priority = inPriority

	self.actionBySource[inSource.key] = action
	table.insert(self.priorityQueue, action)

if false then
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
end

----------------------------------------
-- battle:removePriority()
----------------------------------------
function battle:removePriority(inSource)
	local action = self.actionBySource[inSource.key]

	if (not action) then
		return
	end

	self.actionBySource[inSource.key] = nil
	if (action.next) then
		action.next.prev = action.prev
	end
	if (action.prev) then
		action.prev.next = action.next
	end
end

----------------------------------------
-- battle:sortPriorities()
----------------------------------------
function battle:sortPriorities()
	local action

	local function priorityCompare(a,b)
		return a.priority > b.priority
	end

	table.sort(self.priorityQueue, priorityCompare)

	-- Link up the priority queue nodes
	for _,action in ipairs(self.priorityQueue) do
		self.actionList:insertTail(action)
	end
end

function battle:dumpPriorities()
	local action

	for action in self.actionList:iterator() do
		print("Source: " .. action.source:getSingularName())
		print("Priority: " .. tostring(action.priority))
		print("---")
	end
end

----------------------------------------
-- battle:start()
--
-- 
----------------------------------------
function battle:start()
	local mgroup
	local continue = true
	local partyRan = false

	self:convertPreBattleSong()

	if (not self.isPartyAttack) then
		for mgroup in self.monParty:iterator() do
			self.killCount[mgroup:getSingularName()] = 0
		end
	end

	self:printEncounter()

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
		elseif (self.monParty:isAlive()) then
			text:cdprint(true, false, "\nYou still face ")
			self:printOpponents()

			self.monParty:advance()
		else
			break
		end
	until (continue == false)

	self:postCleanup()

	if ((not partyRan) and (not self.isPartyAttack)) then
		self:doReward()
	end

	self:stop()

	currentBattle = false

	return not partyRan
end

function battle:stop()
	text:clear()
	party:display()
end

----------------------------------------
-- battle:updateBigpic()
----------------------------------------
function battle:updateBigpic()
	if (self.isPartyAttack) then
		if (self.currentBigpic ~= "PIC_WARRIOR") then
			bigpic:setBigpic("PIC_WARRIOR", "PARTY")
			self.currentBigpic = "PIC_WARRIOR"
		end
	else
		local leadGroup = self.monParty:getNearestGroup()

		if (leadGroup.picture == self.currentBigpic) then
			return
		end

		self.currentBigpic = leadGroup.picture

		if (leadGroup.size == 1) then
			bigpic:setTitle(leadGroup.singular)
		else
			bigpic:setTitle(leadGroup.plural)
		end
		bigpic:drawImage(leadGroup.picture)
	end
end

local encounterStringList = {
	"\n\"Give me blood, men!\" you scream as you face ",
	"\nYour foes seem endless, their tales unknown. You face ",
	"\n\"Death and drek!\" you curse, as you see ",
	"\nSnarls of defiance are heard from "
}

----------------------------------------
-- battle:printEncounter()
----------------------------------------
function battle:printEncounter()
	text:clear()

	if (self.isPartyAttack) then
		text:print("\nDissention in your ranks...\n\n")
	else
		text:print(random:randomMember(encounterStringList))
		self:printOpponents()
	end
end

----------------------------------------
-- battle:printOpponents()
----------------------------------------
function battle:printOpponents()
	if (self.isPartyAttack) then
		text:print("\nhostile party members!\n\n")
		return
	end

	self.monParty:printGroups()
end

----------------------------------------
-- battle:doRound()
----------------------------------------
function battle:doRound()
	local action

	text:setCursor(0, 11)
	if (party.advance) then
		text:print("\nThe party advances!!\n\n")
		local m
		for m in self.monParty:iterator() do
			if (m.range > 10) then
				m.range = m.range - 10
			end
		end
		party.advance = false
	end

	for action in self.actionList:iterator() do
		action.source:doAction(action)
		if (globals.partyDied) then
			return
		end
		self.actionList:remove(action)
	end
end

----------------------------------------
-- battle.endRound()
----------------------------------------
function battle:endRound()
	local mgroup

	self:resetPriority()

	if (self.monParty) then
		for mgroup in self.monParty:iterator() do
			mgroup.missTurn = false
		end
	end

	party:doDisbelieve(self)
	if (globals.partyDied) then
		return
	end

	-- This looks like it is skipped in the DOS version.
	-- It is triggered by "cmp byte_3B76E, 0" but byte_3B76E
	-- is never set anywhere...
	--
	--if (self.monParty) then
		-- The DOS code for a successful disbelieve doesn't
		-- even remove the summoned illusion. Hence this
		-- entire check is commented out.
		--
		--self.monParty:disbelieve(self)
	--end

	party:doPoison()
	party:doEquippedEffects()

	if (self.song.hpRegen > 0) then
		local char

		for char in party:iterator("skipDisabled") do
			if (char.maxHp > char.currentHp) then
				char.currentHp = char.currentHp + self.song.hpRegen
				if (char.currentHp > char.maxHp) then
					char.currentHp = char.maxHp
				end
			end
		end
	end

	self:postRoundCleanup()
	if (globals.partyDied) then
		return
	end
end

function battle:postRoundCleanup()
	local i
	local c

	c = party.summon
	if ((c) and ((c.currentHp == 0) or (c.isStoned) or (c.isParalyzed) or
		     (c.isDead))) then
		log:print(log.LOG_DEBUG, "Removing summon")
		party:removeSummon()
	end

	for c in party:iterator() do
		if (c.isDoppleganger) then
			if (c.isDead or c.isStoned or c.isParalyzed) then
				party:removeCharacter(c)
			end
		end
	end
	if (not party:isLive()) then
		return
	end

	party:sort()
end

----------------------------------------
-- battle:getPriorities()
----------------------------------------
function battle:getPriorities()
	local p
	local c

	if ((not party.missTurn) and (not party.advance)) then
		for c in party:characterIterator("skipDisabled") do
			self:addPriority(c, c:getBattlePriority())
		end
	end
	party.missTurn = false

	if (self.isPartyAttack) then
		return
	end

	for p in self.monParty:iterator() do
		for c in p:iterator() do
			self:addPriority(c, c:getBattlePriority())
			c.beenAttacked = false
		end
	end

	self:sortPriorities()
	self:dumpPriorities()
end


----------------------------------------
--
-- Player battle option section
--
----------------------------------------
function battle:getPlayerOptions()
	local c
	local done = false
	local action

	if (self:getRunFightOption()) then
		return false
	end

	if (party.advance) then
		return true
	end

	repeat
		for c in party:iterator("skipDisabled") do
			action = self:getPlayerOption(c)
			if (c.isDoppleganger) then
				action.action = "possessedAttack"
			end
			self:addAction(c, action)
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

----------------------------------------
-- battle:getRunFightOption()
----------------------------------------
function battle:getRunFightOption()
	local inkey

	if (self.isPartyAttack or (not party:canRun())) then
		timer:delay(3)
		return false
	end

	text:print("Will your stalwart band choose to\n")
	text:print("Fight")
	if (not self.monParty:isInMeleeRange()) then
		text:print(",\nAdvance")
	end
	text:print(" or\nRun?")

	repeat
		inkey = getkey()

		if (inkey == "A") then
			party.advance = true
			return false
		elseif (inkey == "R") then
			local saveAction = btAction:new()
			saveAction.target = party:getFirstCharacter()
			saveAction.source = self.monParty:getLeadGroup()
			if (saveAction:savingThrow()) then
				return true
			end

			if (random:band(7) == 0) then
				return true
			end
			if (currentLevel:isCity() and not globals.isNight) then
				if (random:band(3) == 0) then
					return true
				end
			end

			return false
		elseif (inkey == "D") then
			log:print(log.LOG_DEBUG, "songHpRegen: %s", party.battle.songHpRegen)
			self:dumpBattleBonus()
		end
	until (inkey == "F")

	return false
end

----------------------------------------
-- battle:getPlayerOption
----------------------------------------
function battle:getPlayerOption(c)
	local action
	local options = {}
	local inkey

	table.setDefault(options, false)

	action = btAction:new()
	action.source = c

	if (c:isSummon()) then
		action.action = battle.doSummonAttack
		return action
	end

	if ((c.isPossessed) or (c.isNuts)) then
		action.action = "possessedAttack"
		return action
	end

	while true do
		text:clear()
		text:print(c.name.." has these options this battle round:\n\n")

		text:print("Party attack\n")
		options["P"] = true

		if ((not self.isPartyAttack) and (c.inMeleeRange)) then
			text:print("Attack foes\n")
			options["A"] = true
		end

		text:print("Defend\n")
		options["D"] = true

		if (c.currentSppt > 0) then
			text:print("Cast a spell\n")
			options["C"] = true
		end

		text:print("Use an item\n")
		options["U"] = true

		if (c.class == "Rogue") then
			text:print("Hide in shadows\n")
			options["H"] = true
			self.isHiding = false
		end

		if ((c.class == "Bard") and 
		    (c:isTypeEquipped("Instrument"))) then
			text:print("Bard Song\n")
			options["B"] = true
		end

		text:print("\nSelect an option.")

		local continue = true
		while continue do
			inkey = getkey()

			if (options[inkey]) then
				if (inkey == "A") then
					action.action = "melee"
					if (self:meleeTarget(action)) then
						return action
					end
					continue = false
				elseif (inkey == "B") then
					action.action = "sing"
					if (c:getBattleTune(action, true)) then
						return action
					end
					continue = false
				elseif (inkey == "C") then
					action.action = "cast"
					if (c:getCombatSpell(action)) then
						return action
					end
					continue = false
				elseif (inkey == "D") then
					action.action = "defend"
					return action
				elseif (inkey == "H") then
					action.source:hideInShadows()
					action.action = "hide"
					return action
				elseif (inkey == "P") then
					action.action = "partyAttack"
					if (self:meleeTarget(action)) then
						return action
					end
					continue = false
				elseif (inkey == "U") then
					action.action = "use"
					if (c:getUseItem(action)) then
						return action
					end
					continue = false
				end
			end
		end
	end
end

----------------------------------------
-- battle:meleeTarget()
----------------------------------------
function battle:meleeTarget(inAction)
	if (self.isPartyAttack or inAction.action == "partyAttack") then
		inAction.action = "melee"
		text:cdprint(true, false, "Attack:")
		inAction.target = inAction.source:getActionTarget(
					{party = true}
					)
		if (not inAction.target) then
			return false
		end
	else
		if (self.monParty.size > 1) then
			text:cdprint(true, false, "Attack:")
		end

		inAction.target=inAction.source:getActionTarget({melee = true})
		if (not inAction.target) then
			return false
		end
	end

	return true
end

----------------------------------------
-- battle:getMonsterActions()
----------------------------------------
function battle:getMonsterActions()
	local action
	local mgroup
	local m

	log:print(log.LOG_DEBUG, "getMonsterActions()")
	if (self.isPartyAttack) then
		return
	end

	for mgroup in self.monParty:iterator() do
		for m in mgroup:iterator() do
			action = btAction.new()
			action.source = m
			self:addAction(m, action)
		end
	end
end

----------------------------------------
-- battle:doReward()
----------------------------------------
function battle:doReward()
	local mgroup
	local nmonsters
	local i
	local xp		= 0
	local au		= 0
	local itemsToGive	= 0
	local giveGold		= true
	local partySize		= 0

	timer:delay()

	if (globals.gameState == globals.STATE_DUNGEON) then
		giveGold = currentLevel:doTreasureChest()
	end

	bigpic:setBigpic("PIC_TREASURE", "Treasure!")

	for mgroup,nmonsters in pairs(self.killCount) do
		for i = 1,nmonsters do
			xp = xp + monsterData:getXPReward(mgroup)
			if (random:band(7) == 0) then
				itemsToGive = itemsToGive + 1
			end

			if (giveGold) then
				if ((globals.gameState == globals.STATE_CITY)
				    and (not globals.isNight)) then
					au = au + random:betweenInclusive(1,128)
				else
					au = au + random:betweenInclusive(1,256)
				end
			end
		end
	end

	partySize = party:countLiveCharacters()
	xp = xp / partySize
	au = au / partySize

	text:cdprint(true, false, 
		"Each character receives %d experience points for " ..
		"valor and battle knowledge, and %d pieces of gold.\n\n", 
		xp, au)

	for i in party:characterIterator("isLive") do
		i.xp = i.xp + xp
		i.gold = i.gold + au
		i.battlesWon = i.battlesWon + 1
	end

	local function giveItem(inTarget, inItem)
		local isIdentified = (random:band(7) ~= 7)
		local item = items:new(inItem)

		if (not inTarget:isSummon()) then
			if (inTarget:giveItem(inItem, isIdentified)) then
				text:print("%s found a ", inTarget.name)
				if (isIdentified) then
					text:print(item.name)
				else
					text:print(item.type)
				end

				return true
			end
		end

		return false
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

		for i = 1,itemsToGive do
			local item	= currentLevel:getBattleReward()
			local c

			for c in party:iterateFrom(party:randomCharacter()) do
				if (giveItem(c, item)) then
					break
				end
			end
		end

		timer:delay(8)
	else
		timer:delay(10)
	end
end


----------------------------------------
-- battle:convertPreBattleSong
----------------------------------------
function battle:convertPreBattleSong()
	if (party.song.active) then
		local singer = party.song.singer

		self.preBattleSong = singer.song

		if (singer.song.name ~= "Sanctuary Score") then
			-- Stop the song
			singer:songTimeout()
			log:print(log.LOG_DEBUG, "Running combatFunction")
			local action = btAction.new()
			action.inData = singer.song.combatData[currentLevel.level].inData
			singer.song.combatFunction.func(action)
		end
	end
end

function battle:postCleanup()
	local c

	party:resetBattleBonus()

	for c in party:characterIterator() do
		c:resetBattleBonus()
	end
end

function battle:dumpBattleBonus()
	local c
	log:print(log.LOG_DEBUG, "Party battle bonus:")
	party:dumpBattleBonus()
	log:print(log.LOG_DEBUG, "----------------------------")
	log:print(log.LOG_DEBUG, "Character battle bonus")
	log:print(log.LOG_DEBUG, "----------------------------")
	for c in party:iterator() do
		log:print(log.LOG_DEBUG, "%s Bonus", c.name)
		c:dumpBattleBonus()
	end
	log:print(log.LOG_DEBUG, "----------------------------")
	log:print(log.LOG_DEBUG, "Song battle bonus")
	log:print(log.LOG_DEBUG, "----------------------------")
	self.song:dumpBattleBonus()
	log:print(log.LOG_DEBUG, "----------------------------")
	if (self.monParty) then
		log:print(log.LOG_DEBUG, "Monster Party battle bonus")
		self.monParty:dumpBattleBonus()
		log:print(log.LOG_DEBUG, "----------------------------")
		for c in self.monParty:iterator() do
			log:print(log.LOG_DEBUG, "%s Bonus", c.singular)
			c:dumpBattleBonus()
		end
	end
end
	
























	
