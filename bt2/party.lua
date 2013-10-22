require "bttable"
require "btdebug"
require "random"

party = {
	size			= 0,

	-- Passive effects
	--
	detect		= detectEffect:new(),
	shield		= shieldEffect:new(),
	light		= lightEffect:new(),
	compass		= compassEffect:new(),
	levitate	= levitateEffect:new(),

	song		= {
		active		= false,
		timer		= 0,
		acBonus		= 0,
		lightSong	= false,
		trapProtect	= false,
		singer		= false
	},
	battle	= {
		acBonus		= 0,
		antiMagic	= 0,
		songHpRegen	= 0
	}
}

btTable.addParent(party, battleBonus, linkedList)
btTable.setClassMetatable(party)
party.__index = party

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
		if (index < 4) then
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

	head = self:getFirst()
	while (inIndex ~= count) do
		count = count + 1
		if (head.next) then
			head = head.next
		else
			return false
		end
	end

	return head
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
--
-- Special iterator function to include
-- the "summon" slot
----------------------------------------
function party:iterator(inCondition)
	local condition	= inCondition or "default"

	if (not iteratorConditionals[condition]) then
		condition = "default"
	end

	return self:conditionalIterator(iteratorConditionals[condition])
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

--	return self:conditionalIterateFrom(self:getFirstCharacter(),
--				iteratorConditionals[condition])
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
	local i
	local strings
	local member		= self:getFirst()

	for i = 0,6 do
		if (not member) then
			strings = false
		else
			strings = member:getStatusLine()
			member = member.next
		end
		self:printStatusLine(strings, i)
	end
end

----------------------------------------
-- sort()
--
-- Sort the dead, stoned and paralyzed
-- members to the bottom of the party
----------------------------------------
function party:sort()
	dprint("party:sort() called")
	local function __sort(inAttr)
		local tail
		local current
		local save

		tail = self:getLast()
		if (not tail[inAttr]) then
			current = tail
			tail = false
		else
			while (tail[inAttr]) do
				tail = tail.prev
			end
			current = tail
		end

		while (current) do
			save = current.prev
			if (current[inAttr]) then
				self:remove(current)
				if (not tail) then
					self:insertTail(current)
					tail = current.prev
				else
					self:insertAfter(current, tail)
				end
			end
			current = save
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

	if ((self.summon) and (self.summon.isHostile)) then
		return true
	end

	for c in self:characterIterator("isHostile") do
		return true
	end

	return false
end

----------------------------------------
-- isLive()
--
-- Return true if there is at least one
-- capable party member
----------------------------------------
function party:isLive()
	local c

	dprint("isLive() called")
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

			c.cur_hp = c.cur_hp - currentLevel:getPoisonDamage()

			if (c.cur_hp <= 0) then
				c.cur_hp = 0
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
		if (c.cur_sppt < c.max_sppt) then
			c.cur_sppt = c.cur_sppt + 1
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

	for c in self:characterIterator() do
		if (c:isEffectEquipped("hasSpptRegen")) then
			if (c.cur_sppt < c.max_sppt) then
				c.cur_sppt = c.cur_sppt + 1
				update = true
			end
		end

		if (self.song.regenHp or c:isEffectEquipped("hasRegenHP")) then
			if (not c:isDisabled()) then
				if (c.cur_hp < c.max_hp) then
					c.cur_hp = c.cur_hp + 1
					update = true
				end
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
				if ((rnd() % 100000) > comparator) then
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

	-- This is only used by the addDoppleganger()
	-- function so we can skip the summon slot
	--
	for c in self:characterIterator("isAttackable") do
		if (not randomCharacter) then
			randomCharacter = c
		else
			comparator = (numerator / denominator) * 100000
			if ((rnd() % 100000) > comparator) then
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
function party:doSummon(inSummon)
	if (self.summon) then
		self:removeSummon()
	end

	self:insertHead(summon:new(inSummon.type))
	self.summon = self:getFirst()
	self.summon.isIllusion = inSummon.isIllusion or false
	self:display()
end

----------------------------------------
-- removeSummon()
--
-- Remove the summoned monster
----------------------------------------
function party:removeSummon()
	dprint(self.summon)
	if (not self.summon) then return end

	self:remove(self.summon)
	self.summon	= false
	party:display()
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

	y = 288 + (16 * inSlot)
	fontP = globals.fonts.mono
	m_window:Fill(gfxRectangle:new(24, y, 604, 16), globals.colors[8])

	if (inStrings) then
		renderedText = fontP:Render(inStrings.name, globals.colors[16])
		m_window:Draw(gfxRectangle:new(24, y, 0, 0), renderedText, nil)
		renderedText = fontP:Render(inStrings.hp, globals.colors[1])
		m_window:Draw(gfxRectangle:new(400, y, 0, 0), renderedText, nil)
		renderedText = fontP:Render(inStrings.sppt, globals.colors[1])
		m_window:Draw(gfxRectangle:new(536, y, 0, 0), renderedText, nil)
	end

	m_window:Update()
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

	action = btAction.new()
	action.source = char

	if (not char:getUseItem(action)) then
		return
	end

	char:useItem(action)
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

	if (s.sppt > char.cur_sppt) then
		text:cdcprint(false, true, true, 
			"\nNot enough spells points.")
		return
	end

	action = btAction.new()
	action.source	= char
	action.func	= s.action.func
	action.inData	= s.action.inData
	action.inData.sppt	= s.sppt

	if (s.targetted) then
		text:cdprint(true, false, "Use on:")

		action.target = char:getActionTarget(s.targetted)
		if (not action.target) then
			text:clear()
			return
		end
	end

	text:cdprint(true, false, char.name)
	char:castSpell(action)
	party:sort()
end

----------------------------------------
-- singSong()
--
--
----------------------------------------
function party:singSong()
	local char
	local action
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

	action = btAction.new()
	action.source = char

	tune = char:getTune()
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
		party.song.singer:songTimeout()
	end

	if (tune.activate) then
		action.func = tune.activate.func
		action.inData = tune.nonCombatData[currentLevel.level].inData

		if (action.func) then
			action.func(action)
		end
	end

	party.song.singer	= char
	party.song.active	= true
	party.song.timer	= 12
	char.isSinging		= true
	char.song		= tune
end

----------------------------------------
-- died()
--
-- Called when the party has died
----------------------------------------
function party:died()
--	timer:delay(5)
	bigpic:drawImage("PIC_GAMEOVER")
	bigpic:setTitle("Sorry, bud")
	text:clear()
	text:splashMessage("Alas, your party has expired, but has gone to adventurer heaven.")
end


----------------------------------------
-- disbelieve()
----------------------------------------
function party:doDisbelieve()
	local mgroup

	if (not currentBattle.monParty) then
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
		local xxx_check_melee_march_effect = true
		if ((c.max_hp - 3) <= c.cur_hp) then
			c.cur_hp = c.cur_hp + 3
		end
	end
	end
end








