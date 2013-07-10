require "bttable"
require "btdebug"
require "random"

party = {
	className	= "party",
	detect		= detectEffect,
	shield		= shieldEffect,
	light		= lightEffect,
	compass		= compassEffect,
	levitate	= levitateEffect,
	summon		= false,

	song		= {
		acBonus		= 0,
		lightSong	= false,
		singer		= false
	},
	battle	= {
		acBonus		= 0,
		antiMagic	= 0
	}
}

btTable.addParent(party, btArray)
btTable.setClassMetatable(party)
party.__index = party

function party:addCharacter(inCharacter)
	self:__add(inCharacter)
	self:sort()
end

function party:removeCharacter(inCharacter)
	if (type(inCharacter) == "number") then
		local rval = self[inCharacter]
		self:__remove(inCharacter)

		return rval
	else
		local i
		local c
		for i,c in self:ipairs() do
			if (c == inCharacter) then
				self:__remove(i)
				return c
			end
		end
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
		if (self[6]) then
			break
		end

		if ((not self:findByName(name)) and
		    (roster.nameExists(name))) then
			self:addCharacter(roster.getCharacter(name))
		end
	end

	self:display()
end



----------------------------------------
-- ipairsSummon()
--
-- Special ipairs function to include 
-- "summon" slot
----------------------------------------
function party:ipairsSummon()
	local state = -1
	local function f(_)
		if (state == -1) then
			state = state + 1
			if (self.summon) then
				return "summon", self.summon
			end
		end

		while (state < 6) do
			state = state + 1
			if (self[state]) then
				return state, self[state]
			end
			return
		end
	end
	return f, nil, state
end

----------------------------------------
-- iterator()
--
-- Special iterator function to include
-- the "summon" slot
----------------------------------------
function party:iterator()
	local state = -1
	local function f(_)
		if (state == -1) then
			state = state + 1
			if (self.summon) then
				return self.summon
			end
		end

		while (state < 6) do
			state = state + 1
			if (self[state]) then
				return self[state]
			end
		end
		return
	end
	return f, nil, state
end

----------------------------------------
-- characterIterator()
-- 
-- Iterate over the characters only
----------------------------------------
function party:characterIterator()
	local state = 0
	local function f(_)
		while (state < 6) do
			state = state + 1
			if (self[state]) then
				return self[state]
			end
		end
		return
	end
	return f, nil, state
end

----------------------------------------
-- liveCharacterIterator()
--
-- Loop over the live characters in the
-- party
----------------------------------------
function party:liveCharacterIterator()
	local state = 0
	local function f(_)
		while (state < 6) do
			state = state + 1
			if (
			    (self[state]) and
			    (not self[state]:isDisabled()) and
			    (not self[state].isDoppelganger)
			   ) then
				return self[state]
			end
		end
		return
	end
	return f,nil,state
end

----------------------------------------
-- getLastLiveCharacter()
--
-- Return the index of the last live
-- character
----------------------------------------
function party:getLastLiveCharacter()
	local i

	for i = #self,1,-1 do
		if (
		    (self[i]) and
		    (not self[i]:isDisabled()) and
		    (not self.isDoppelganger)
		   ) then
			return i
		end
	end

	error("party:getLastLiveCharacter() called with a dead party", 2)
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
	local i
	local c

	for i,c in self:ipairs() do
		if (inName == c.name) then
			return i
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
	local charString	= false
	local hpString		= false

	if (self.summon) then
		dprint(self.summon)
		charString,hpString = self.summon:getStatusLine()
	end
	self:printStatusLine(charString, hpString, 0)

	for i = 1,6 do
		if (self[i]) then
			charString, hpString = self[i]:getStatusLine()
		else
			charString = false
		end

		self:printStatusLine(charString, hpString, i)
	end
end

----------------------------------------
-- isOccupied()
----------------------------------------
function party:isOccupied(inSlot)
	if (type(inSlot) ~= "number") then
		inSlot = tonumber(inSlot)
	end
	return self[inSlot]
end

----------------------------------------
-- __sort()
--
-- Actual sorting algorithm
----------------------------------------
function party:__sort(inStatus)
	local i
	local j

	for i = 1,5 do
		if (not self[i]) then
			return
		end

		if (self[i][inStatus]) then
			for j = i+1,6 do
				if (not self[j]) then
					break
				end
				if (not party[j][inStatus]) then
					local tmp = party[j]
					party[j] = party[i]
					party[i] = tmp
				end
			end
		end
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
	self:__sort("isDead")
	self:__sort("isStoned")
	self:__sort("isParalyzed")
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
	local i

	for i = 1,6 do
		if (self[i]) then
			table.insert(t, self[i].name)
		end
	end

	return t
end

----------------------------------------
-- reorder()
--
-- Change the party order
----------------------------------------
function party:reorder()
	local fromNum
	local toNum
	local tmp

	text:clear()
	text:print("Move who?")

	fromNum = self:readSlotNumber()
	if (not fromNum) then
		return
	end

	text:clear()
	text:print("\n\nTo where?")

	toNum = getkey()
	if ((toNum < "1") or (toNum > "6")) then
		return
	end

	toNum = tonumber(toNum)
	if (toNum == fromNum) then
		return
	end

	tmp = self[fromNum]
	table.remove(self, fromNum)
	table.insert(self, toNum, tmp)

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

	for c in self:characterIterator() do
		if ((c.isPossessed) or (c.isDoppelganger)) then
			return true
		end
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

	for c in self:characterIterator() do
		if ((not c:isDisabled()) and
		    (not c.isDoppelganger)) then
			return true
		end
	end

	return false
end

----------------------------------------
-- doPoison()
--
-- Do damage to each poisoned party
-- member
--
-- xxx - might not belong here
----------------------------------------
function party:doPoison()
	local c
	local doResort	= false

	for c in self:characterIterator() do
		if (c.isPoisoned) then
			doResort = true

			c.cur_hp = c.cur_hp - currentLevel.getPoisonDamage()

			if (c.cur_hp < 0) then
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
-- doEquippedEffects()
--
-- Loop through the party and perform
-- effects based on equipped gear
----------------------------------------
function party:doEquippedEffects()
	local c

	for c in self:characterIterator() do
		if (c:isEffectEquipped("hasSpptRegen")) then
			if (c.cur_sppt < c.max_sppt) then
				c.cur_sppt = c.cur_sppt + 1
			end
		end

		local xxx_song_regen_hp = true

		if (c:isEffectEquipped("hasRegenHP")) then
			if (not c:isDisabled()) then
				if (c.cur_hp < c.max_hp) then
					c.cur_hp = c.cur_hp + 1
				end
			end
		end
	end
end

----------------------------------------
-- randomMeleeCharacter()
--
-- Return a random melee character that
-- can be attacked
----------------------------------------
function party:randomMeleeCharacter()
	local maxSize = self.size
	local minSize = self.size
	local randomCharacter
	local c
	local i
	local f

	if (maxSize > 3) then
		maxSize = 3
	end

	if (minSize > 3) then
		minSize = 3
	end

	if (self.summon) then
		maxSize = maxSize + 1
		--minSize = minSize + 1
	end

	randomCharacter = rnd_xdy(1,maxSize)
	for i = 1, maxSize do
		dprint("Random character = %d", randomCharacter)
		dprint("minSize = %d", minSize)
		if (randomCharacter > minSize) then
			c = self.summon
		else
			c = self[randomCharacter]
		end

		if (c:canBeAttacked()) then
			return c
		end

		randomCharacter = randomCharacter + 1
		if (randomCharacter > maxSize) then
			randomCharacter = 1
		end
	end
end

----------------------------------------
-- summon()
--
-- Summon a monster to the summon slot
----------------------------------------
function party:doSummon(inName, inIllusion)
	if (self.summon) then
		self.summon = false
	end

	self.summon = summon:new(inName)
	self.summon.isIllusion = inIllusion or false
	self:display()
end

----------------------------------------
-- removeSummon()
--
-- Remove the summoned monster
----------------------------------------
function party:removeSummon()
	self.summon = false
end

----------------------------------------
-- readSlot()
--
-- Read a party slot from the player
----------------------------------------
function party:readSlot()
	local cnum

	cnum = self:readSlotNumber()

	return self[cnum]
end

----------------------------------------
-- readSlotNumber()
--
-- Read a party slot number from the
-- player
----------------------------------------
function party:readSlotNumber()
	local inkey

	text:printCancel()
	inkey = getkey()
	if ((inkey < "1") or (inkey > "6")) then	
		return false
	end

	if (not self:isOccupied(inkey)) then
		return false
	end

	return tonumber(inkey)
end

----------------------------------------
-- printStatusLine()
--
--
----------------------------------------
function party:printStatusLine(inCharString, inHpString, inSlot)
	local renderedText
	local fontP
	local y

	y = 266 + (16 * inSlot)
	fontP = globals.fonts.mono
	m_window:Fill(gfxRectangle:new(32, y, 580, 16), globals.colors[8])

	if (inCharString) then
		renderedText = fontP:Render(inCharString, globals.colors[16])
		m_window:Draw(gfxRectangle:new(32, y, 0, 0), renderedText, nil)
		renderedText = fontP:Render(inHpString, globals.colors[1])
		m_window:Draw(gfxRectangle:new(416, y, 0, 0), renderedText, nil)
	end

	m_window:Update()
end




















