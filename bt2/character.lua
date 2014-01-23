require "inventory"
require "entity"
require "battleParty"
require "battlePlayer"
require "race"
require "classes"

character = {}

----------------------------------------
-- Character class
----------------------------------------
function character:new()
	local self = object:new()

	self.isHiding	= false

	self:addParent(bIICharacter)
	self:addParent(character)
	self:addParent(classes)
	self:addParent(battleParty)
	self:addParent(battlePlayer)
	self:addParent(battleBonus)
	self:addParent(linkedListNode)
	self:addParent(entity)

	return self
end

----------------------------------------
-- getHpString()
----------------------------------------
function character:getHpString()
	if (self.isDead) then
		return "DEAD"
	elseif (self.isStoned) then
		return "STON"
	elseif (self.isPoisoned) then
		return "POIS"
	elseif (self.isOld) then
		return "OLD"
	elseif (self.isParalyzed) then
		return "PARA"
	elseif (self.isPossessed) then
		return "POSS"
	else
		return sprintf("%4d", self.maxHp)
	end
end

----------------------------------------
-- getAcString()
----------------------------------------
function character:getAcString()
	self:calcAC()
	if (self.ac < -9) then
		if (self.ac < -20) then
			return "L+"
		elseif (self.ac == -20) then
			return "L-"
		else
			return string.format("L%d", self.ac + 20)
		end
	else
		return sprintf("%2d", self.ac)
	end
end


----------------------------------------
-- getStatusLine()
----------------------------------------
function character:getStatusLine()
	local ac
	local fnt
	local hpstring
	local rval = {}

	rval.name = sprintf("%-15s %2s %4s     %3d     %2s",
		self.name, 
		self:getAcString(),
		self:getHpString(),
		self.maxSppt,
		string.sub(self.class,1,2)
		)

	rval.hp = sprintf("%4d", self.currentHp)
	rval.sppt = sprintf("%3d", self.currentSppt)

	return rval
end

----------------------------------------
-- isDisabled()
----------------------------------------
function character:isDisabled()
	return (self.isDead or self.isStoned or self.isParalyzed)
end

----------------------------------------
-- fromTable()
----------------------------------------
function character:fromTable(t)
	local rval
	local k
	local v

	rval = character:new()

	for k,v in pairs(t) do
		if (k ~= "inventory") then
			if (type(v) == "table") then
				rval[k] = table.copy(v)
			else
				rval[k] = v
			end
		else
			rval.inventory = inventory:new()
			rval.inventory:fromTable(v)
		end
	end

	if (not rval.inventory) then
		log:print(log.LOG_DEBUG, "Creating empty inventory")
		rval.inventory = inventory:new()
	end

	rval:setClass(rval.class)

	return rval
end
			
----------------------------------------
-- toTable()
----------------------------------------
function character:toTable()
	local t = {}
	local k
	local v

	local function toTable(inSelf, inDest, inHeader)
		local k
		local v

		for k,_ in pairs(inHeader) do
			v = inSelf[k]

			if (type(v) == "function") then
				local doNothing = 1
			elseif (type(v) == "table") then
				if (v.toTable) then
					inDest[k] = v:toTable()
				else
					inDest[k] = table.copy(v)
				end
			else
				inDest[k] = inSelf[k]
			end
		end
	end

	toTable(self, t, bIICharacter)
	toTable(self, t, entity)

	return t
end


----------------------------------------
-- clone()
--
-- Create a clone of the current character
----------------------------------------
function character:clone()
	local rval = character:new()
	local k
	local v

	local function doClone(inSelf, inDest, inHeader)
		local k
		local v

		for k,_ in pairs(inHeader) do
			v = inSelf[k]

			if (type(v) == "table") then
				rawset(inDest, k, table.copy(v))
			else
				rawset(inDest, k, v)
			end
		end
	end

	doClone(self, rval, bIICharacter)
	doClone(self, rval, battleBonus)
	doClone(self, rval, entity)

	rval:setClass(rval.class)

	return rval
end


----------------------------------------
-- giveItem()
--
-- Give an item to the character
----------------------------------------
function character:giveItem(inName, isIdentified, inCount)
	return self.inventory:addItem(inName, isIdentified, inCount)
end

----------------------------------------
-- calcAC()
--
-- Calculate the characters AC
----------------------------------------
function character:calcAC()
	local i

	self.ac = 10
	if (self.dx > 15) then
		self.ac = self.ac - (self.dx - 15)
	end

	for i in self.inventory:iterator() do
		if (i.isEquipped) then
			self.ac = self.ac - i.ac_bonus
		end
	end

	if (party.shield.active) then
		self.ac = self.ac - party.shield.bonus
	end

	if (party.song.acBonus) then
		self.ac = self.ac - party.song.acBonus
	end

	self.ac = self.ac - party.acBonus
	self.ac = self.ac - self.acBonus

	if (self.ac < -21) then
		self.ac = -21
	end
end

function character:isCharacter()
	return true
end

function character:getSingularName()
		return self.name
end

----------------------------------------
-- isAfllicted()
--
-- Returns true if the character has
-- a special status
----------------------------------------
function character:isAfflicted()
	log:print(log.LOG_DEBUG, "isAfflicted() called")
	return (self.isDead or self.isOld or self.isPoisoned 
		or self.isStoned or self.isParalyzed
		or self.isPossessed or self.isNuts)
end

----------------------------------------
-- getSpellLevel()
--
-- Returns the spell level of the
-- current class
----------------------------------------
function character:getSpellLevel()
	log:print(log.LOG_DEBUG, self.class)
	if (self.class == "Magician") then return self.spellLevel.Magician end
	if (self.class == "Conjurer") then return self.spellLevel.Conjurer end
	if (self.class == "Sorcerer") then return self.spellLevel.Sorcerer end
	if (self.class == "Wizard") then return self.spellLevel.Wizard end
	if (self.class == "Archmage") then return self.spellLevel.Archmage end

	return false
end

----------------------------------------
-- isSpellCaster()
----------------------------------------
function character:isSpellCaster()
	if ((self.class == "Magician") or (self.class == "Conjurer") or
	    (self.class == "Sorcerer") or (self.class == "Wizard") or
	    (self.class == "Archmage")) then
		return true
	end

	return false
end

----------------------------------------
-- isEffectEquipped()
----------------------------------------
function character:isEffectEquipped(effect)
	local i

	log:print(log.LOG_DEBUG, "character:isEffectEquipped(): " .. self.name)
	if (self.name == nil) then
		error("nil name", 2)
	end
	for i in self.inventory:iterator() do
		if ((i.isEquipped) and (i[effect])) then
			return true
		end
	end

	return false
end

----------------------------------------
-- isTypeEquipped()
--
-- Return false if the item type is not
-- equipped and the inventory slot if
-- the type is equipped
----------------------------------------
function character:isTypeEquipped(inType)
	local i

	for i in self.inventory:iterator() do
		if ((i.isEquipped) and (i.type == inType)) then
			return i
		end
	end

	return false
end

----------------------------------------
-- isItemEquipped()
----------------------------------------
function character:isItemEquipped(inName)
	local i

	for i in self.inventory:iterator() do
		if ((i.name == inName) and (i.isEquipped)) then
			return true
		end
	end

	return false
end

----------------------------------------
-- hasItem()
----------------------------------------
function character:hasItem(inName)
	local i

	for i in self.inventory:iterator() do
		if (i.name == inName) then
			return i
		end
	end

	return false
end

----------------------------------------
-- createCharacter()
--
-- Create a character table that can
-- be used to create a character object
----------------------------------------
function character.createCharacter()
	local inkey
	local newchar
	local newclass
	local i
	local r

::restart::
	newchar = character.new()

	r = race.selectRace()
	if (not r) then
		return false
	end

	newchar.race = r

	-- Generate random attributes
	newchar.st = race.randomStat(r, "base_st")
	newchar.iq = race.randomStat(r, "base_iq")
	newchar.dx = race.randomStat(r, "base_dx")
	newchar.lk = race.randomStat(r, "base_lk")
	newchar.cn = race.randomStat(r, "base_cn")

	----------------------------------------
	-- Set hit points
	----------------------------------------
	newchar.maxHp = random:xdy_z(2, 16, 1) + 14
	newchar.currentHp = newchar.maxHp

	text:clear()
	character.printAttributes(newchar)

	newclass = newchar:selectClass(newchar.race)
	if (not newclass) then
		goto restart
	end
	newchar:setClass(newclass)

	bigpic:drawImage(newchar.pic)
	bigpic:setTitle(newchar.class)

	newchar.name = character.getCharacterName()
	if (not newchar.name) then
		bigpic:drawImage("PIC_GUILDINT")
		bigpic:setTitle("The Guild")
		goto restart
	end

	----------------------------------------
	-- Initial gold
	----------------------------------------
	newchar.gold = random:xdy_z(2, 60, 1) + 110

	----------------------------------------
	-- Assign initial songs for bard
	----------------------------------------
	if (newchar.class == "Bard") then
		newchar.songs = 1
	end

	----------------------------------------
	-- Assign spell level
	----------------------------------------

	----------------------------------------
	-- Set initial spell points if magic user
	----------------------------------------
	if ((newchar.class == "Conjurer") or 
	    (newchar.class == "Magician")) then
		newchar.spellLevel[newchar.class] = 1

		if (newchar.iq > 15) then
			newchar.maxSppt = 10 + (18 - newchar.iq)
		else
			newchar.maxSppt = 10
		end

		newchar.maxSppt = newchar.maxSppt + (random:rnd() % 8)
		newchar.currentSppt = newchar.maxSppt
	end

	newchar.inventory = inventory:new()

	return newchar
end

----------------------------------------
-- poolGold
--
-- Give all of the party's gold to the
-- character
----------------------------------------
function character:poolGold()
	local c

	for c in party:characterIterator() do
		if (c.name ~= self.name) then
			self.gold = self.gold + c.gold
			c.gold = 0
		end
	end
end


----------------------------------------
-- printAttributes()
--
-- Print a character's attributes
----------------------------------------
function character:printAttributes()
	text:print("St: %2d  IQ: %2d", self.st, self.iq)
	text:print("\nDx: %2d  Cn: %2d", self.dx, self.cn)
	text:print("\nLk: %2d  HP: %d", self.lk, self.maxHp)
end

----------------------------------------
-- character:setBigpic()
----------------------------------------
function character:setBigpic()
	bigpic:setBigpic(self.pic, self.class)
end

----------------------------------------
-- character:printAttributeScreen()
----------------------------------------
function character:printAttributeScreen()
	text:clear()
	text:print(self.name)
	text:print("\nRace:  " .. self.race)
	text:print("\nClass: " .. self.class)
	text:print("\n")
	self:printAttributes()
	text:print("\nLvl:%2d", self.cur_level)
	text:print(" SpPt:%3d", self.maxSppt)
	text:print("\nExper:%9d", self.xp)
end

----------------------------------------
-- printCharacter()
--
-- Print the character screen
----------------------------------------
function character:printCharacter()
	local inkey
	local inv

	self:setBigpic()

	repeat
		self:printAttributeScreen()

		text:setCursor(0, 7)
		text:print("\nGold:%10d", self.gold)
		text:print("\n   (POOL GOLD)\n   (TRADE GOLD)")
		text:printContinue()

		inkey = getkey()
	
		if (inkey == "P") then
			self:poolGold()
		elseif (inkey == "T") then
			self:tradeGold()
		end
	until (inkey == "C")

	-- Inventory screen
	while true do
		local i
		local slot

		text:clear()
		text:print(self.name)

		self.inventory:printScreen(self)

		text:print("\nConj:" .. self.spellLevel.Conjurer)
		text:print(" Sorc:" .. self.spellLevel.Sorcerer)
		text:print("\nMagi:" .. self.spellLevel.Magician)
		text:print(" Wizd:" .. self.spellLevel.Wizard)
		text:print("\n     (DONE)")

		inkey = getkey()
		if ((inkey < "1") or (inkey > "8")) then
			break
		end
	
		slot = tonumber(inkey)
		if (self.inventory[slot]) then
			text:clear()
			text:print(self.name)

			text:print(" do you wish to:")
			text:setCursor(0, 3)
			text:print("\nTrade the item")
			text:print("\nDrop the item")

			inv = self.inventory[slot]
			if (inv:canEquip(self.class)) then
				if (inv.isEquipped) then
					text:print("\nUne")
				else
					text:print("\nE")
				end
				text:print("quip the item")
			end

			text:printCancel()

			local continue = true
			repeat
				inkey = getkey()

				if (inkey == "C") then
					continue = false
				elseif (inkey == "X") then
					local a = btAction.new()
					a.source = self
					use.consumeItem(a, inv)
					continue = false
				elseif (inkey == "D") then
					self:unequipItem(slot)
					self.inventory:dropItem(slot)
					continue = false
				elseif (inkey == "E") then
					self:equipItem(slot)
					continue = false
				elseif (inkey == "T") then
					self:tradeItem(slot)
					continue = false
				elseif (inkey == "U") then
					self:unequipItem(slot)
					continue = false
				end
			until (not continue)
		end
	end
end

----------------------------------------
-- tradeGold()
--
-- Do the actual trading of gold
----------------------------------------
function character:tradeGold()
	local amount
	local toc

	text:clear()
	text:print("Trade gold to who?")

	toc = party:readSlot()
	if (not toc) then
		return
	end

	text:clear()
	text:print("How much will you give to him?\n")

	amount = text:readNumber()
	if (amount == false) then
		return
	end

	if (amount > self.gold) then
		text:print("\nNot enough gold.   ")
		timer:delay(6)
		return
	end

	toc.gold = amount + toc.gold
	self.gold = self.gold - amount
end

----------------------------------------
-- tradeItem()
--
-- Trade an item between characters
----------------------------------------
function character:tradeItem(slot)
	local item
	local inkey
	local toc

	item = self.inventory[slot]
	text:clear()
	text:print("\nWho does " .. self.name .. " want to give the ")
	text:print(item.name .. " to?")

	toc = party:readSlot()
	if (not toc) then
		return
	end

	-- Can't trade to yourself
	if (self.name == toc.name) then
		return
	end

	if (not toc.inventory:addItem(item.name, item.isIdentified)) then
		text:clear()
		text:print(toc.name .. " has no room for that item!")
		timer:delay(6)

		return
	end

	self:unequipItem(slot)
	self.inventory:dropItem(slot)
end

-----------------------------------------
-- getCharacterName()
--
-- Get the character name from the player
-----------------------------------------
function character.getCharacterName()
	local name

	while true do
		text:clear()

		text:print("Enter the new member's name.\n\n")

		name = text:readString()

		if (name:len() == 0) then
			return false
		end
		if (roster:nameExists(name)) then
			text:splashMessage("\n\nThere is a already a person by that name.\n")
		else
			return name
		end
	end
end


----------------------------------------
-- equipItem()
--
-- Equip an item
----------------------------------------
function character:equipItem(slot)
	local inv
	local itype
	local tinv
	local i

	itype = self.inventory[slot].type

	for i in self.inventory:iterator() do
		if ((i.type == itype) and (i.isEquipped)) then
			i:unequip()
		end
	end

	-- Equip the item
	self.inventory:equipItem(slot)

	-- Update the character's AC
	party:display()
end

----------------------------------------
-- unequipItem()
--
-- Equip an item
----------------------------------------
function character:unequipItem(slot)
	self.inventory:unequipItem(slot)

	if (not self:isTypeEquipped("Instrument")) then
		self:songTimeout()
	end

	party:display()
end

----------------------------------------
-- canBeAttacked()
--
-- Return true if the character
-- can be attacked
----------------------------------------
function character:canBeAttacked()
	if ((self.isDead) or (self.isStoned)) then
		return false
	end

	return true
end

----------------------------------------
-- readInventorySlot()
--
-- Read an inventory slot from the user
----------------------------------------
function character:readInventorySlot()
	local slot

	self.inventory:printScreen(self)
	text:printCancel()

	while true do
		slot = getkey()
		if ((slot < "1") or (slot > "8")) then
			return false
		end

		slot = tonumber(slot)
		if (self.inventory[slot]) then
			return self.inventory[slot]
		end
	end
end


----------------------------------------
-- getUseItem()
--
-- Get an item to use from the character
----------------------------------------
function character:getUseItem(inAction)
	local target	= false
	local item

	if (currentBattle) then
		target = currentBattle.monParty
	end

	if (self:isDisabled()) then
		return false
	end

	text:cdprint(true, false, "Use item #(1-8)\n")
	item = self:readInventorySlot()
	if (not item) then
		text:clear()
		return false
	end

	local function cantUse()
		text:cdprint(true, true, "\nYou can't use that.")
		text:clear()
	end

	if ((not item.isEquipped) or (not item.usable)) then
		cantUse()
		return false
	end

	if ((item.type == "Quiver") and (not self:isTypeEquipped("Bow"))) then
		cantUse()
		return false
	end

	inAction.inData = item.action.inData
	inAction.inData.item = item
	inAction.func = item.action.func

	if (item.targetted) then
		text:cdprint(true, false, "Use on:")

		inAction.target = self:getActionTarget(item.targetted)
		if (not inAction.target) then
			text:clear()
			return false
		end
	end

	return true
end


----------------------------------------
-- useItem()
--
-- Perform the function of a used item
----------------------------------------
function character:useItem(inAction)
	self:consumeItem(inAction)

	text:cdprint(true, false, "%s%s", self.name,
				inAction.inData.item.useString)

	if (inAction.func) then
		inAction.func(inAction)
	end
end

----------------------------------------
-- consumeItem()
--
-- Consume an item based on the number
-- of charges of the item
----------------------------------------
function character:consumeItem(inAction)
	local inItem	= inAction.inData.item

	if ((inItem.max_charges == 1) or
	    ((inItem.max_charges == 0) and (random:band(63) == 1))) then
		self.inventory:dropItem(self.inventory:findItem(inItem))
	end
end

----------------------------------------
-- consumeSpellPoints
----------------------------------------
function character:consumeSpellPoints(inAction)
	local req	= inAction.inData.sppt

	log:print(log.LOG_DEBUG, "consumeSpellPoints() called")

	if (self:isEffectEquipped("hasHalfSppt")) then
		req = bits32.arshift(req, 1)
	end

	if (req > self.currentSppt) then
		return true
	end

	self.currentSppt = self.currentSppt - req

	return false
end

----------------------------------------
-- getSpell()
--
-- Get a spell to cast from the player
----------------------------------------
function character:getSpell(mouseFlag)
	local spellAbbr
	local s

	log:print(log.LOG_DEBUG, "getSpell() called")

	if (mouseFlag) then
		local xxx_do_mouse_menu = true
		return
	end

	text:cdprint(true, false, "\nSpell to cast:\n")
	spellAbbr = text:readString(4)
	s = spells.getSpellByAbbreviation(spellAbbr)
	if (not s) then
		text:cdprint(false, true, "\nNo spell by that name.")
		return false
	end

	if (s.level > self.spellLevel[s.class]) then
		text:cdprint(false, true, "\nYou don't know this spell")
		return false
	end

	return s
end


----------------------------------------
-- castSpell
--
-- Do the actual casting of a spell
----------------------------------------
function character:castSpell(inAction)
	local fizzle = false

	log:print(log.LOG_DEBUG, "castSpell() called")

	text:print(" casts a spell")

	fizzle = self:consumeSpellPoints(inAction)

	if (currentLevel.currentSquare.isAntiMagic) then
		fizzle = true
	end

	if (fizzle) then
		party:display()
		text:ctdprint(false, true, " but it fizzles!\n")
		return
	end

	if (inAction.func) then
		inAction.func(inAction)
	end
end

----------------------------------------
-- doVoiceCheck()
----------------------------------------
function character:doVoiceCheck()
	if (not self:isEffectEquipped("hasFreeSinging")) then
		if (self.songs == 0) then
			text:cdprint(false, true, " lost his voice!\n\n")
			return false
		end

		self.songs = self.songs - 1
	end

	return true
end

----------------------------------------
-- songTimeout()
--
-- Deactivate the currently playing song
-- if this character is singing. Since
-- there can only be only song playing
-- at a time, if this character is 
-- singing we can safely deactivate the
-- party's song
----------------------------------------
function character:songTimeout()
	if (not self.isSinging) then
		return	
	end

	party.song:deactivate()
end

----------------------------------------
-- getMultiString()
----------------------------------------
function character:getMultiString()
	return self.name
end

----------------------------------------
-- getTargetString()
----------------------------------------
function character:getTargetString()
	return self.name
end

----------------------------------------
-- getActionTarget()
----------------------------------------
function character:getActionTarget(inTargetOptions)
	local optionKeys	= btTable:new()
	local inkey
	local i

	if (inTargetOptions.party) then
		local count = 1

		for i in party:characterIterator() do
			optionKeys[tostring(count)] = i
			count = count + 1
		end

		text:print("\nMember #[1-%d]", party.size)
	end

	if (currentBattle and (not currentBattle.isPartyAttack)) then
		if (inTargetOptions.melee or inTargetOptions.monster) then
			local i
			local monGroup

			if ((not inTargetOptions.party) and 
			    (currentBattle.monParty.size == 1)) then
				return currentBattle.monParty:getLeadGroup()
			end

			text:print("\n")
			for i,monGroup in currentBattle.monParty:ipairs() do
				local key = string.toLetterOption(i - 1, "A")
				optionKeys[key]=monGroup
				text:print("[%s] %d %s (%d')\n",
					string.toLetterOption(i - 1, "a"),
					monGroup.size,
					string.pluralize(monGroup.size,
						monGroup.singular,
						monGroup.plural),
					monGroup.range
					)
			end
		end
	end

	inkey = getkey()
	if (not optionKeys[inkey]) then
		return false
	end

	return optionKeys[inkey]
end






