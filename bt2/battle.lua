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
	self.actionList		= linkedList:new("battleNext", "battlePrev")
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
	currentBattle.monParty = monsterParty:new()
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
-- battle:resetPriority()
----------------------------------------
function battle:resetPriority()
	self.priorityQueue	= {}
end

----------------------------------------
-- battle:addPriority()
----------------------------------------
function battle:addPriority(inSource)
	inSource.action.priority = inSource:getBattlePriority()

	table.insert(self.priorityQueue, inSource)
end

----------------------------------------
-- battle:removePriority()
----------------------------------------
function battle:removePriority(inSource)
	log:print(log.LOG_DEBUG, "removePriority(%s)", inSource.key)
	log:print(log.LOG_DEBUG, "inSource.action: %s", inSource.action)
	if (not inSource.action) then
		return
	end

	log:print(log.LOG_DEBUG, "doing remove")

	-- Remove the action from the list
	self.actionList:remove(inSource)
end

----------------------------------------
-- battle:sortPriorities()
----------------------------------------
function battle:sortPriorities()
	local action

	local function priorityCompare(a,b)
		return a.action.priority > b.action.priority
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
		log:print(log.LOG_DEBUG, "Source: %s (%s)",
			action:getSingularName(),
			action.key
			)
		log:print(log.LOG_DEBUG, "Priority: %s", action.action.priority)
		log:print(log.LOG_DEBUG, "---")
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
			log:print(log.LOG_DEBUG, "mgroup: %s", mgroup)
			self.killCount[mgroup:getSingularName()] = 0
		end
	end

	self:printEncounter()

	repeat
		self:updateBigpic()
		if (not party:getBattleActions()) then
			partyRan = true
			break
		end

		self:getPriorities()

		self:doRound()
		log:print(log.LOG_DEBUG, "doRound() over")
		self:endRound()
		if (globals.partyDied) then
			return false
		end

		if (self.isPartyAttack) then
			if (party:canRun()) then
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

	if ((not partyRan) and (not self.isPartyAttack)) then
		self:doReward()
	end

	self:postCleanup()

	self:stop()

	currentBattle = false

	return not partyRan
end

----------------------------------------
-- battle:stop()
----------------------------------------
function battle:stop()
	text:clear()
	party:display()

	party.action:renew()

	if (not self.isPartyAttack) then
		self.monParty:delete()
		self.monParty = nil
	end

	self.actionList:delete()
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
		timer:delay(3)
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
	local char

	text:setCursor(0, 11)
	if (party.advance) then
		text:print("\nThe party advances!!\n\n")
		local mgroup
		for mgroup in self.monParty:iterator() do
			if (mgroup.range > 10) then
				mgroup.range = mgroup.range - 10
			end
		end
		party.advance = false
	end

	for char in self.actionList:iterator() do
		log:print(log.LOG_DEBUG, "doRound(%s (%s))",
			char:getSingularName(),
			char.key)
		char:doAction()
		if (globals.partyDied) then
			return
		end
		self.actionList:remove(char)
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

----------------------------------------
-- battle:postRoundCleanup()
----------------------------------------
function battle:postRoundCleanup()
	local i
	local c

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
	party:display()

	-- Something very weird in the DOS version. What it looks like
	-- is that if the reset of the part is disabled, then the first
	-- character is stripped of being possessed.
	--
	c = party:isOccupied(2)
	if (c) then
		if (c:isDisabled()) then
			party.head.isPossessed = false
			party:display()
		end
	end
end

----------------------------------------
-- battle:getPriorities()
----------------------------------------
function battle:getPriorities()
	local p
	local c

	if ((not party.missTurn) and (not party.advance)) then
		for c in party:characterIterator("skipDisabled") do
			self:addPriority(c)
		end
	end
	party.missTurn = false

	if (not self.isPartyAttack) then
		for p in self.monParty:iterator() do
			for c in p:iterator() do
				self:addPriority(c)
				c.beenAttacked = false
			end
		end
	end

	self:sortPriorities()
	self:dumpPriorities()
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

	--timer:delay()

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
				timer:delay(6)
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

	log:print(log.LOG_DEBUG, "itemsToGive: %d", itemsToGive)
	if (giveGold) then
		if ((itemsToGive == 0) and (currentLevel.level > 4)) then
			itemsToGive = 1
		end

		if (itemsToGive == 0) then
			timer:delay(10)
			return
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

		timer:delay(10)
	else
		timer:delay(10)
	end
end


----------------------------------------
-- battle:convertPreBattleSong
----------------------------------------
function battle:convertPreBattleSong()
	party.song:convertToCombat()
end

----------------------------------------
-- battle:postCleanup()
----------------------------------------
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
	
























	
