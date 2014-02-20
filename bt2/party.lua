require "bttable"
require "btdebug"
require "random"

party = linkedList:new("partyNext", "partyPrev")
party:addParent(battleBonus)

-- Graphics objects for party display
party.gfxRect		= gfxRect:New(24, 288, 604, 112)
party.gfxSurface	= gfxSurface:New(604, 112)

party.size		= 0

-- Passive effects
--
party.detect	= detectEffect:new()
party.shield	= shieldEffect:new()
party.light	= lightEffect:new()
party.compass	= compassEffect:new()
party.levitate	= levitateEffect:new()

party.advance	= false
party.action	= btAction:new()
party.song		= song:new()
party.battle	= {
	acBonus		= 0,
	antiMagic	= 0,
	songHpRegen	= false
}

----------------------------------------
-- getFirstCharacter()
----------------------------------------
function party:getFirstCharacter()
	return self:getFirst()
end

----------------------------------------
-- addCharacter()
----------------------------------------
function party:addCharacter(inCharacter)
	-- Insert at the end of the list
	--
	log:print(log.LOG_DEBUG, "party:addCharacter(%s)", inCharacter.name)
	self:insertTail(inCharacter)
	self:sort()
	self.size = self.size + 1
end

----------------------------------------
-- removeCharacter()
----------------------------------------
function party:removeCharacter(inCharacter)
	local c

	if (type(inCharacter) == "number") then
		c = self:isOccupied(inCharacter)
		if (c) then
			self:remove(c)
			self.size = self.size - 1
		end
		return c
	else
		self:remove(inCharacter)
		self.size = self.size - 1
		return inCharacter
	end

	return false
end

----------------------------------------
-- updateIndices()
----------------------------------------
function party:updateIndices()
	local index = 1
	local node

	for node in self:iterator() do
		if (index < 5) then
			node.inMeleeRange = true
		else
			node.inMeleeRange = false
		end
		index = index + 1
	end
end

----------------------------------------
-- hasRoom()
----------------------------------------
function party:hasRoom()
	return (self.size <= 7)
end

----------------------------------------
-- getNumber()
----------------------------------------
function party:isOccupied(inIndex)
	local count		= 1
	local head

	if (type(inIndex) ~= "number") then
		inIndex = tonumber(inIndex)
	end

	for head in self:iterator() do
		if (inIndex == count) then
			return head
		end

		count = count + 1
	end

	return false
end

----------------------------------------
-- addParty()
--
-- Add characters to the party from a
-- given list of character names
----------------------------------------
function party:addParty(inParty)
	local i,name

	for i,name in ipairs(inParty) do
		if (not self:hasRoom()) then
			break
		end

		if ((not self:findByName(name)) and
		    (roster:nameExists(name))) then
			self:addCharacter(roster:readCharacter(name))
		end
	end

	self:display()
end

----------------------------------------
-- Party iterator functions
----------------------------------------

local iteratorConditionals = {
	default		= function () return true end,
	skipDisabled	= function (c) return not c:isDisabled() end,
	isLive		= function (c) 
				return (not c:isDisabled() and 
				        not c.isDoppleganger)
			  end,
	isHostile	= function (c)
				return (c.isPossessed or c.isDoppleganger)
			  end,
	cantRun		= function (c)
		if (c:isDisabled()) then
			return false
		end

		if (c.isDoppleganger) then
			return true
		end

		if (c:isSummon() and not c.isDocile) then
			return true
		end

		return false
	end,
	isAttackable	= function (c)
				if (c:isSummon() and c.isIllusion) then
					if (currentBattle and 
					    currentBattle.disbelieve) then
						return false
					end
				end
		
				return (not c.isDead and not c.isStoned 
						     and not c.isHiding)
			  end
}
	

----------------------------------------
-- iterator()
----------------------------------------
function party:iterator(inCondition)
	local condition	= inCondition or "default"

	if (not iteratorConditionals[condition]) then
		condition = "default"
	end

	return self:conditionalIterator(iteratorConditionals[condition])
end

----------------------------------------
-- party:targetIterator()
----------------------------------------
function party:targetIterator()
	return self:iterator()
end

local xxx_no_need_for_special_characterIterator_in_bt2 = true

----------------------------------------
-- characterIterator()
-- 
-- Iterate over the characters only
----------------------------------------
function party:characterIterator(inCondition)
	local condition = inCondition or "default"

	if (not iteratorConditionals[condition]) then
		condition = "default"
	end

	return self:conditionalIterator(iteratorConditionals[condition])
end

----------------------------------------
-- getLastLiveCharacter()
--
-- Return the last live character
----------------------------------------
function party:getLastLiveCharacter()
	local c

	for c in self:reverseIterator() do
		if ((not c:isDisabled()) and (not c.isDoppleganger)) then	
			return c
		end
	end

	error("party:getLastLiveCharacter() called with a dead party", 2)
end

----------------------------------------
-- countLiveCharacters()
----------------------------------------
function party:countLiveCharacters()
	local count = 0
	local c

	for c in self:characterIterator("isLive") do
		count = count + 1
	end

	if (count == 0) then
		error("party:countLiveCharacters() called with a dead party", 2)
	end

	return count
end

----------------------------------------
-- findByName()
--
-- Find a character by name. 
--
-- Returns:
--   slot number of character 
--   false if not found
----------------------------------------
function party:findByName(inName)
	local c

	for c in self:characterIterator() do
		if (c.name == inName) then
			return c
		end
	end

	return false
end

----------------------------------------
-- display()
--
-- Update the party display
----------------------------------------
function party:display()
	local strings
	local member
	local i			= 0

	for i = 0,6 do
		self:printStatusLine(false, i)
	end

	for member in self:iterator() do
		strings = member:getStatusLine()
		self:printStatusLine(strings, i)
		i = i + 1
	end

	self.gfxSurface:Draw(self.gfxRect)
end

----------------------------------------
-- sort()
--
-- Sort the dead, stoned and paralyzed
-- members to the bottom of the party
----------------------------------------
function party:sort()
	log:print(log.LOG_DEBUG, "party:sort() called")
	local function __sort(inAttr)
		local tail

		local function __iter(c) 
			return c[inAttr]
		end

		for tail in self:reverseConditionalIterator(__iter) do
			self:remove(tail)
			self:insertTail(tail)
		end
	end

	__sort("isDead")
	__sort("isStoned")
	__sort("isParalyzed")
	self:updateIndices()
	self:display()
end

----------------------------------------
-- toTable()
--
-- Convert the current party to a table
-- suitable for writing to disk
----------------------------------------
function party:toTable()
	local t = {}
	local c

	for c in self:characterIterator() do
		table.insert(t, c.name)
	end

	return t
end

----------------------------------------
-- party:saveState()
----------------------------------------
function party:saveState()
	local t	
	local c

	-- Save party order
	t = self:toTable()

	t.characters = {}

	-- Save characters
	for c in self:iterator() do
		t.characters[c.name] = c:toTable()
	end

	if (self.light.active) then
		t.light = {}
		t.light.duration	= self.light.duration
		t.light.distance	= self.light.distance
		t.light.seeSecret	= self.light.seeSecret
		t.light.noRandom	= true
	end

	if (self.levitate.active) then
		t.levitate = {}
		t.levitate.duration	= self.levitate.duration
		t.levitate.noRandom	= true
	end

	if (self.shield.active) then
		t.shield = {}
		t.shield.duration	= self.shield.duration
		t.shield.acBonus	= self.shield.bonus
		t.shield.noRandom	= true
	end

	if (self.detect.active) then
		t.detect = {}
		t.detect.duration	= self.detect.duration
		t.detect.stairs		= self.detect.stairs
		t.detect.traps		= self.detect.traps
		t.detect.special	= self.detect.special
		t.detect.noRandom	= true
	end

	if (self.compass.active) then
		t.compass = {}
		t.compass.duration	= self.compass.duration
		t.detect.noRandom	= true
	end

	if (self.song.active) then
		t.song = {}
		t.song.currentTune 	= self.song.currentTune
		t.song.duration		= self.song.duration
		t.song.singer		= self.song.singer.name
	end

	return t
end

----------------------------------------
-- party:restoreState()
----------------------------------------
function party:restoreState(inTable)
	local i
	local c

	-- Empty the party first
	for c in self:iterator() do
		self:removeCharacter(c)
	end

	for i,c in ipairs(inTable) do
		self:addCharacter(character:fromTable(inTable.characters[c]))
	end

	if (inTable.light) then
		self.light:activate(inTable.light)
	end

	if (inTable.levitate) then
		self.levitate:activate(inTable.levitate)
	end

	if (inTable.shield) then
		self.shield:activate(inTable.shield)
	end

	if (inTable.detect) then
		self.detect:activate(inTable.detect)
	end

	if (inTable.compass) then
		self.compass:activate(inTable.compass)
	end

	if (inTable.song) then
		self.song:activate(
			self:findByName(inTable.song.singer),
			inTable.song.currentTune, 
			inTable.song.duration
		)
	end

	self:display()
end

----------------------------------------
-- reorder()
--
-- Change the party order
----------------------------------------
function party:reorder()
	local fromNode
	local toNode
	local tmp
	local after = false

	text:clear()
	text:print("Move who?")

	fromNode = self:readSlot()
	if (not fromNode) then
		return
	end

	text:clear()
	text:print("\n\nTo where?")

	toNode = self:readSlot()
	if (not toNode) then
		return
	end

	if (fromNode == toNode) then
		return
	end

	for tmp in self:iterateFrom(fromNode) do
		if (tmp == toNode) then
			after = true
			break
		end
	end

	self:remove(fromNode)
	if (after) then
		self:insertAfter(fromNode, toNode)
	else
		self:insertBefore(fromNode, toNode)
	end

	self:sort()
	self:display()
end

----------------------------------------
-- isHostile()
--
-- Return true if there is a hostile
-- monster in the summon slot, or if
-- the party contains a doppelganger or
-- possessed party member
----------------------------------------
function party:isHostile()
	local c

	for c in self:characterIterator("isHostile") do
		return true
	end

	return false
end

----------------------------------------
-- canRun()
--
-- Returns true if the party is capable
-- of choosing Fight, Run, Advance
----------------------------------------
function party:canRun()
	local c

	for c in self:characterIterator("cantRun") do
		log:print(log.LOG_DEBUG, "%s can't run", c.name)
		return false
	end

	return true
end

----------------------------------------
-- isLive()
--
-- Return true if there is at least one
-- capable party member
----------------------------------------
function party:isLive()
	local c

	log:print(log.LOG_DEBUG, "isLive() called")
	for c in self:characterIterator("isLive") do
		return true
	end

	globals.partyDied = true
	self:died()
	return false
end

----------------------------------------
-- doPoison()
--
-- Do damage to each poisoned party
-- member
----------------------------------------
function party:doPoison()
	local c
	local doResort	= false

	for c in self:characterIterator() do
		if (c.isPoisoned) then
			doResort = true

			c.currentHp = c.currentHp - currentLevel:getPoisonDamage()

			if (c.isDestinyKnight) then
				c.currentHp = c.maxHp
			elseif (c.currentHp <= 0) then
				c.currentHp = 0
				c.isDead = true
			end
		end
	end

	if (doResort) then	
		self:sort()
	end
end

----------------------------------------
-- regenSpellPoints()
----------------------------------------
function party:regenSpellPoints()
	local c
	local update = false

	for c in self:characterIterator() do
		if (c.currentSppt < c.maxSppt) then
			c.currentSppt = c.currentSppt + 1
			update = true
		end
	end

	if (update) then
		self:display()
	end
end

----------------------------------------
-- doEquippedEffects()
--
-- Loop through the party and perform
-- effects based on equipped gear
----------------------------------------
function party:doEquippedEffects()
	local c
	local update = false

	for c in self:iterator("skipDisabled") do
		if (c:isEffectEquipped("hasSpptRegen")) then
			if (c.currentSppt < c.maxSppt) then
				c.currentSppt = c.currentSppt + 1
				update = true
			end
		end

		if (party.song.spptRegen) then
			c.currentSppt = c.currentSppt + 1
			update = true
		end

		if (c:isEffectEquipped("hasRegenHP")) then
			if (c.currentHp < c.maxHp) then
				c.currentHp = c.currentHp + 1
				update = true
			end
		end
	end

	if (update) then
		self:display()
	end
end

----------------------------------------
-- isItemEquipped()
--
-- Return true if a party member has
-- an item equipped
----------------------------------------
function party:isItemEquipped(inName)
	local c

	for c in self:characterIterator() do
		if (c:isItemEquipped(inName)) then
			return true
		end
	end

	return false
end

----------------------------------------
-- hasItem()
----------------------------------------
function party:hasItem(inName)
	local c

	for c in self:characterIterator() do
		if (c:hasItem(inName)) then	
			return c
		end
	end

	return false
end

----------------------------------------
-- randomMeleeCharacter()
--
-- Return a random melee character that
-- can be attacked
----------------------------------------
function party:randomMeleeCharacter()
	local c
	local randomCharacter	= false
	local numerator		= 1
	local denominator	= 2
	local comparator

	for c in self:iterator("isAttackable") do
		if (c.inMeleeRange) then
			if (not randomCharacter) then
				randomCharacter = c
			else
				comparator = (numerator / denominator) * 100000
				if ((random:rnd() % 100000) > comparator) then
					randomCharacter = c
				end
				numerator = numerator + 1
				denominator = denominator + 1	
			end
		end
	end

	if (not randomCharacter) then
		return self:getFirst()
	else
		return randomCharacter
	end
end

----------------------------------------
-- randomCharacter()
----------------------------------------
function party:randomCharacter(inSummonFlag)
	local c
	local randomCharacter	= false
	local numerator		= 1
	local denominator	= 2
	local comparator

	for c in self:characterIterator("isAttackable") do
		if (not randomCharacter) then
			randomCharacter = c
		else
			comparator = (numerator / denominator) * 100000
			if ((random:rnd() % 100000) > comparator) then
				randomCharacter = c
			end
			numerator = numerator + 1
			denominator = denominator + 1
		end
	end

	if (not randomCharacter) then
		return self:getFirst()
	else
		return randomCharacter
	end
end

----------------------------------------
-- summon()
--
-- Summon a monster to the summon slot
----------------------------------------
function party:doSummon(inData)
	if (not self:hasRoom()) then
		text:print(" but there was no room for a summoning!\n\n")
		self:display()
		timer:delay()
		return false
	end

	repeat
		if (not self:hasRoom()) then
			break
		end

		self:addCharacter(summon:new(inData.type, inData.isIllusion))
	until (not inData.fillParty)

	self:display()

	return true
end

----------------------------------------
-- party:releaseSummon()
----------------------------------------
function party:releaseSummon()
	local c

	text:cdprint(true, false, 
		"\nPick the monster to release from the party")

	c = self:readSlot()
	if (c) then
		if (c:isSummon()) then
			self:removeCharacter(c)
			self:sort()
			party:display()
		else
			text:csplash(true, true, "Not a monster!")
		end
	end

	text:clear()
	self:isLive()
end

----------------------------------------
-- readSlot()
--
-- Read a party slot from the player
----------------------------------------
function party:readSlot()
	local inkey

	text:printCancel()
	inkey = getkey()
	if ((inkey < "1") or (inkey > "6")) then
		return false
	end

	inkey = tonumber(inkey)
	return self:isOccupied(inkey)
end

----------------------------------------
-- readMember()
----------------------------------------
function party:readMember(inkey)
	if (not inkey) then return false end

	if ((inkey > "0") and (inkey < "8")) then
		return self:isOccupied(inkey)
	end

	return false
end

----------------------------------------
-- printStatusLine()
--
--
----------------------------------------
function party:printStatusLine(inStrings, inSlot)
	local renderedText
	local fontP
	local y

	y = 16 * inSlot
	fontP = globals.fonts.mono
	self.gfxSurface:Fill(gfxRect:New(0, y, 604, 16), globals.colors[8])

	if (inStrings) then
		renderedText = fontP:Render(inStrings.name, globals.colors[16])
		self.gfxSurface:Blit(gfxRect:New(0, y, 0, 0), renderedText, nil)
		renderedText = fontP:Render(inStrings.hp, globals.colors[1])
		self.gfxSurface:Blit(gfxRect:New(376, y, 0, 0), renderedText, nil)
		renderedText = fontP:Render(inStrings.sppt, globals.colors[1])
		self.gfxSurface:Blit(gfxRect:New(512, y, 0, 0), renderedText, nil)
	end
end

----------------------------------------
-- useItem()
--
-- 
----------------------------------------
function party:useItem()
	local char
	local action

	text:cdprint(true, false, "Who wishes to use an item?")
	char = self:readSlot()
	if (not char) then
		text:clear()
		return
	end

	if (not char:getUseItem()) then
		return	
	end

	char:useItem()
end

----------------------------------------
-- castSpell()
--
--
----------------------------------------

function party:castSpell()
	local char
	local action
	local s

	text:cdprint(true, false, "\n\nWho do you wish to cast the spell?")
	char = self:readSlot()
	if (not char) then
		text:clear()
		return
	end

	if (char:isDisabled()) then
		text:cdcprint(true, true, true,
			"Sorry, %s is in no condition to cast a spell.", 
			char.name)
		return
	end

	if (not char:isSpellCaster()) then
		text:cdcprint(true, true, true,
			"\n\nThou art not a spell caster!")
		return
	end

	s = char:getSpell(false)
	if (not s) then
		text:clear()
		return
	end

	if (not s.noncombat) then
		text:cdcprint(false, true, true, 
			"\nYou can only cast that in combat.")
		return
	end

	if (s.sppt > char.currentSppt) then
		text:cdcprint(false, true, true, 
			"\nNot enough spells points.")
		return
	end

	char.action.func	= s.action.func
	char.action.inData	= s.action.inData
	char.action.inData.sppt = s.sppt

	if (s.targetted) then
		text:cdprint(true, false, "Use on:")

		char.action.target = char:getActionTarget(s.targetted)
		if (not char.action.target) then
			text:clear()
			return
		end
	end

	text:cdprint(true, false, char.name)
	char:castSpell()
	party:sort()
end

----------------------------------------
-- singSong()
--
--
----------------------------------------
function party:singSong()
	local char
	local tune

	text:cdprint(true, false, "Who will play?")
	char = self:readSlot()
	if (not char) then
		text:clear()
		return
	end

	if ((char.class ~= "Bard") or (char:isDisabled())) then
		text:cdcprint(true, true, true, "\n\nHe can't sing")
		return
	end

	if (not char:isTypeEquipped("Instrument")) then
		text:cdcprint(true, true, true, 
			"\n\nHe has no instrument to play.")
		return
	end

	tune = song:getTune()
	if (not tune) then
		text:clear()
		return
	end

	text:cdprint(true, false, char.name)
	if (not char:doVoiceCheck()) then
		return
	end

	text:print(" plays a tune")

	if (party.song.active) then
		party.song:deactivate()()
	end

	party.song:activate(char, tune)
end

----------------------------------------
-- died()
--
-- Called when the party has died
----------------------------------------
function party:died()
--	timer:delay(5)
	self:display()
	bigpic:drawImage("PIC_GAMEOVER")
	bigpic:setTitle("Sorry, bud")
	text:csplash(true, true, "Alas, your party has expired, but " ..
				 "has gone to adventurer heaven.")
end


----------------------------------------
-- disbelieve()
----------------------------------------
function party:doDisbelieve()
	local mgroup

	if (not currentBattle.monParty) then
		return
	end

	if (not self.disbelieve) then
		return
	end

	for mgroup in currentBattle.monParty:iterator() do
		if (mgroup.isIllusion) then
			local action = btAction:new()
			action.source = self:getFirstCharacter()
			action.target = mgroup

			if (not action:savingThrow()) then
				mgroup:truncate()
				action.outData.specialAttack = "stone"
				mgroup:doDamage(action)
				text:ctdprint(false, true, 
					"\nThe party disbelieves!"
					)
			end
		end
	end
end

----------------------------------------
-- giveItem()
--
-- Try to give an item to a party
-- member. Return the character that
-- received the item.
----------------------------------------
function party:giveItem(inItem, inIsIdentified, inCount)
	local c

	for c in self:characterIterator() do
		if (c:giveItem(inItem, inIsIdentified, inCount)) then
			return c
		end
	end

	return false
end	

----------------------------------------
-- meleeMarch()
----------------------------------------
function party:meleeMarch()
	local c

	for c in self:characterIterator() do
		if ((c.currentHp + 3) <= c.maxHp) then
			c.currentHp = c.currentHp + 3
		end
	end

	self:display()
end

----------------------------------------
-- party:getBattleActions()
----------------------------------------
function party:getBattleActions()
	local c

	if (self:battleRunCheck()) then
		return false
	end

	if (self.advance) then
		return true
	end

	while true do
		for c in self:iterator("skipDisabled") do
			c:getBattleAction()
			if (c.isDoppleganger) then
				c.action.action = "possessedAttack"
			end
		end

		text:cdprint(true, false, "Use these attack commands?")
		if (not text:getYesNo()) then
			for c in self:iterator() do
				c.action:reset()
			end
		else
			text:clear()
			return true
		end
	end

	error("Should never be reached")
end

----------------------------------------
-- party:battleRunCheck()
----------------------------------------
function party:battleRunCheck()
	local inkey

	if (currentBattle.isPartyAttack) then
		return false
	end

	if (not party:canRun()) then
		timer:delay(3)
		return false
	end

	text:print("Will your stalwart band choose to\n")
	text:print("Fight")
	if (not currentBattle.monParty:isInMeleeRange()) then
		text:print(",\nAdvance")
	end
	text:print(" or\nRun?")

	repeat
		inkey = getkey()

		if (inkey == "A") then
			self.advance = true
			return false
		elseif (inkey == "R") then
			self.action.target = self:getFirstCharacter()
			self.action.source = currentBattle.monParty:getLeadGroup()
			if (self.action:savingThrow()) then
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
		end
	until (inkey == "F")

	return false
end


