require "btTable"
require "inventory"
require "entity"
require "dataio"
require "battlePlayer"

character = {}

baseCharacter = {
	race	 	= "",
	name		= "",
	class		= false,
	numAttacks	= 1,
	st		= 0,
	iq		= 0,
	dx		= 0,
	lk		= 0,
	cn		= 0,
	save_st		= 0,
	save_iq		= 0,
	save_dx		= 0,
	save_lk		= 0,
	save_cn		= 0,
	max_sppt	= 0,
	cur_sppt	= 0,
	max_hp		= 0,
	cur_hp		= 0,
	xp		= 0,
	gold		= 0,
	cur_level	= 1,
	max_level	= 1,
	rogu_level	= 0,
	hunt_level	= 0,
	num_attacks	= 1,
	bat_vict	= 0,
	isDead		= false,
	isOld 		= false,
	isPoisoned	= false,
	isStoned	= false,
	isParalyzed	= false,
	isPossessed	= false,
	isNuts		= false,
	isDoppelganger	= false,
	songs		= 0,
	battlesWon	= 0,
	possessed	= false,
	ac		= 10,
	spellLevel = {
		Magician = 0,
		Conjurer = 0,
		Sorcerer = 0,
		Wizard   = 0
	},
	inventory	= false
}

function character:getStatusLine()
	local ac
	local fnt
	local hpstring
	local outString
	local outHpString

	ac = self.ac
	if (party.shield.active) then
		ac = ac - party.shield.bonus
	end

	if (party.songAcBonus) then
		ac = ac - party.songAcBonus
	end

	if (party.spellAcBonus) then
		ac = ac - party.spellAcBonus
	end

	if (ac >= -9) then
		ac = string.format("%2d", ac)
	else
		ac = "L0"
	end
		
	if (self.isDead) then
		hpstring = "DEAD"
	elseif (self.isStoned) then
		hpstring = "STON"
	elseif (self.isPoisoned) then
		hpstring = "POIS"
	elseif (self.isOld) then
		hpstring = "OLD"
	elseif (self.isParalyzed) then
		hpstring = "PARA"
	else
		hpstring = string.format("%4d", self.max_hp)
	end

	outString = string.format("%-15s %2s %4s     %4d %2s",
		self.name, ac, hpstring, self.cur_sppt,
		string.sub(self.class,1,2))

	outHpString = string.format("%4d", self.cur_hp)

	return outString,outHpString
end

function character:isDisabled()
	return (self.isDead or self.isStoned or self.isParalyzed)
end

function character:fromTable(t)
	local k
	local v

	for k,v in pairs(t) do
		if (k ~= "inventory") then
			if (type(v) == "table") then
				self[k] = table.copy(v)
			else
				self[k] = v
			end
		else
			self.inventory = inventory:new()
			self.inventory:fromTable(v)
		end
	end
end
			
function character:toTable()
	local t = {}
	local k
	local v

	for k,v in pairs(baseCharacter) do
		if (k == "inventory") then
			t["inventory"] = self.inventory:toTable()
		elseif (type(v) == "function") then
			dprint("Doing nothing for key: " .. tostring(k))
			local doNothing = 1
		else
			t[k] = self[k]
		end
	end

	return t
end

----------------------------------------
-- Character class
----------------------------------------
function character:new()
	local self = {
	}

	btTable.addParent(self, baseCharacter, character, entity, 
			objectHash:new(self), battlePlayer
			)
	btTable.setClassMetatable(self)

	self.__index = self

	return self
end

----------------------------------------
-- giveItem()
--
-- Give an item to the character
----------------------------------------
function character:giveItem(inName, isIdentified)
	return self.inventory:addItem(inName, isIdentified)
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

	-- XXX - Magic bonuses

	-- The party display is always updated after calculating
	-- the AC
	party:display()
end

function character:isCharacter()
	return true
end

function character:getSingularName()
		return self.name
end

function character:isAfflicted()
	dprint("isAfflicted() called")
	return (self.isDead or self.isOld or self.isPoisoned 
		or self.isStoned or self.isParalyzed
		or self.isPossessed or self.isNuts)
end

function character:getSpellLevel()
	if (self.class == "Magician") then return self.magi_level end
	if (self.class == "Conjurer") then return self.conj_level end
	if (self.class == "Sorcerer") then return self.sorc_level end
	if (self.class == "Wizard") then return self.wiza_level end

	return false
end

function character:isSpellCaster()
	if ((self.class == "Magician") or (self.class == "Conjurer") or
	    (self.class == "Sorcerer") or (self.class == "Wizard")) then
		return true
	end

	return false
end

function character:isEffectEquipped(effect)
	local i

	dprint("character:isEffectEquipped(): " .. self.name)
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

function character:getBattlePriority()
	local priority
	local levelBonus

	priority = bit32.rshift(self.battlesWon, 9)
	if (self.dx > 14) then
		priority = priority + bit32.lshift(self.dx - 14, 3)
	end
	priority = priority + (rnd() % 32)

	levelBonus = bit32.rshift(self.cur_level, 1)
	if ((self.class == "Conjurer") or
	    (self.class == "Magician") or
	    (self.class == "Sorcerer") or
	    (self.class == "Wizard")) then
		levelBonus = bit32.rshift(levelBonus, 2)
	elseif ((self.class == "Rogue") or
		(self.class == "Bard")) then
		levelBonus = bit32.rshift(levelBonus, 1)
	elseif (self.class == "Monk") then
		levelBonus = bit32.lshift(levelBonus, 1)
	end

	priority = priority + levelBonus
	if (priority <= 0) then
		return 1
	elseif (priority > 255) then
		return 255
	else
		return priority
	end
end

function character:doEquippedEffect()
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
	newchar.max_hp = rnd_xdy_z(2, 16, 1) + 14
	newchar.cur_hp = newchar.max_hp

	text_clear()
	character.printAttributes(newchar)

	newchar.class = classes.selectClass(newchar.race)
	if (not newchar.class) then
		goto restart
	end

	bigpic.drawBigpic(classes.get(newchar.class, "pic"))
	bigpic.setTitle(newchar.class)

	newchar.name = character.getCharacterName()
	if (not newchar.name) then
		bigpic.drawBigpic("PIC_GUILDINT")
		bigpic.setTitle("The Guild")
		goto restart
	end

	----------------------------------------
	-- Initial gold
	----------------------------------------
	newchar.gold = rnd_xdy_z(2, 60, 1) + 110

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
			newchar.max_sppt = 10 + (18 - newchar.iq)
		else
			newchar.max_sppt = 10
		end

		newchar.max_sppt = newchar.max_sppt + (rnd() % 8)
		newchar.cur_sppt = newchar.max_sppt
	end

	newchar.inventory = inventory.new()

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
	text_print("St: %2d  IQ: %2d", self.st, self.iq)
	text_print("\nDx: %2d  Cn: %2d", self.dx, self.cn)
	text_print("\nLk: %2d  HP: %d", self.lk, self.max_hp)
end

----------------------------------------
-- printCharacter()
--
-- Print the character screen
----------------------------------------
function character:printCharacter()
	local inkey
	local inv

	bigpic.drawBigpic(classes.get(self.class, "pic"))
	bigpic.setTitle(self.class)

	repeat
		text_clear()
		text_print(self.name)
		text_print("\nRace:  " .. self.race)
		text_print("\nClass: " .. self.class)
		text_print("\n")
		self:printAttributes()
		text_print("\nLvl:%2d", self.cur_level)
		text_print(" SpPt:%3d", self.max_sppt)
		text_print("\nExper:%9d\n", self.xp)

		text_print("Gold:%10d", self.gold)
		text_print("\n   (POOL GOLD)\n   (TRADE GOLD)")
		printContinue()

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

		text_clear()
		text_print(self.name)

		self.inventory:printScreen(self)

		text_print("\nConj:" .. self.spellLevel.Conjurer)
		text_print(" Sorc:" .. self.spellLevel.Sorcerer)
		text_print("\nMagi:" .. self.spellLevel.Magician)
		text_print(" Wizd:" .. self.spellLevel.Wizard)
		text_print("\n     (DONE)")

		inkey = getkey()
		if ((inkey < "1") or (inkey > "8")) then
			break
		end
	
		slot = tonumber(inkey)
		if (self.inventory[slot]) then
			text_clear()
			text_print(self.name)

			text_print(" do you wish to:")
			text_setCursor(0, 3)
			text_print("\nTrade the item")
			text_print("\nDrop the item")

			inv = self.inventory[slot]
			if (inv:canEquip(self.class)) then
				if (inv.isEquipped) then
					text_print("\nUne")
				else
					text_print("\nE")
				end
				text_print("quip the item")
			end

			printCancel()

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

	text_clear()
	text_print("Trade gold to who?")

	toc = party:readSlot()
	if (not toc) then
		return
	end

	text_clear()
	text_print("How much will you give to him?\n")

	amount = text_readNumber()
	if (amount == false) then
		return
	end

	if (amount > self.gold) then
		text_print("\nNot enough gold.   ")
		text_delay(6)
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
	text_clear()
	text_print("\nWho does " .. self.name .. " want to give the ")
	text_print(item.name .. " to?")

	toc = party:readSlot()
	if (not toc) then
		return
	end

	-- Can't trade to yourself
	if (self.name == toc.name) then
		return
	end

	if (not toc.inventory:addItem(item.name, item.isIdentified)) then
		text_clear()
		text_print(toc.name .. " has no room for that item!")
		text_delay(6)

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
		text_clear()

		text_print("Enter the new member's name.\n\n")

		name = text_readString()

		if (name:len() == 0) then
			return false
		end
		if (roster.nameExists(name)) then
			splashMessage("\n\nThere is a already a person by that name.\n")
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
	self:calcAC()
end

----------------------------------------
-- unequipItem()
--
-- Equip an item
----------------------------------------
function character:unequipItem(slot)
	self.inventory:unequipItem(slot)

	-- XXX Check if bard and stop the song. In the DOS version
	-- it doesn't check if the unequipped item is the instrument.
	-- It looks like it stops the song regardless of what item is
	-- unequipped.

	self:calcAC()
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
	printCancel()

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
	local item

	if (self:isDisabled()) then
		return false
	end

	text_cdprint(true, false, "Use item #(1-8)\n")
	item = self:readInventorySlot()
	if (not item) then
		text_clear()
		return false
	end

	local function cantUse()
		text_cdprint(true, true, "\nYou can't use that.")
		text_clear()
	end

	if ((not item.isEquipped) or (not item.usable)) then
		cantUse()
		return false
	end

	inAction.inData = item.action.inData
	inAction.inData.item = item
	inAction.func = item.action.func

	if (item.targetted) then
		text_cdprint(true, false, "Use on:")

		inAction.target = getActionTarget(item.targetted, false)
		if (not inAction.target) then
			text_clear()
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

	text_cdprint(true, false, "%s%s", self.name,
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
	    ((inItem.max_charges == 0) and (rnd_and_x(63) == 1))) then
		self.inventory:dropItem(self.inventory:findItem(inItem))
	end
end

----------------------------------------
-- consumeSpellPoints
----------------------------------------
function character:consumeSpellPoints(inAction)
	local req	= inAction.inData.sppt

	dprint("consumeSpellPoints() called")

	if (self:isEffectEquipped("hasHalfSppt")) then
		req = bits32.arshift(req, 1)
	end

	if (req > self.cur_sppt) then
		return true
	end

	self.cur_sppt = self.cur_sppt - req

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

	dprint("getSpell() called")

	if (mouseFlag) then
		local xxx_do_mouse_menu = true
		return
	end

	text_cdprint(true, false, "\nSpell to cast:\n")
	spellAbbr = text_readString(4)
	s = spells.getSpellByAbbreviation(spellAbbr)
	if (not s) then
		text_cdprint(false, true, "\nNo spell by that name.")
		return false
	end

	if (s.level > self.spellLevel[s.class]) then
		text_cdprint(false, true, "\nYou don't knw this spell")
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

	dprint("castSpell() called")

	text_print(" casts a spell")

	fizzle = self:consumeSpellPoints(inAction)

	if (currentLevel.currentSquare.isAntiMagic) then
		fizzle = true
	end

	if (fizzle) then
		party:display()
		text_print(false, true, " but it fizzles!\n")
		return
	end

	if (inAction.func) then
		inAction.func(inAction)
	end
end

----------------------------------------
-- getTune()
----------------------------------------
function character:getTune(inAction, combatFlag)
	local i
	local inkey

	text_cdprint(true, false, "Play tune #\n\n")
	for i = 1,#songs do
		text_print("%d)%s\n", i, songs[i].name)
	end

	inkey = getkey()
	if ((inkey > "0") and (inkey < tostring(#songs))) then
		local tune

		inkey = tonumber(inkey)
		if (combatFlag) then
			tune = songs[inkey].combat[currentLevel.level]
		else
			tune = songs[inkey].nonCombat[currentLevel.level]
		end

		if (tune) then
			inAction.func = tune.func
			inAction.inData = tune.inData
		end
		return true
	end

	return false
end


----------------------------------------
-- doVoiceCheck()
----------------------------------------
function character:doVoiceCheck()
	if (not self:isEffectEquipped("hasFreeSinging")) then
		if (self.songs == 0) then
			text_cdprint(false, true, " lost his voice!\n\n")
			return false
		end

		self.songs = self.songs - 1
	end

	return true
end















