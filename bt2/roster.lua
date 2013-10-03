require "bICharacter"
require "bIICharacter"

roster = btroster:new("roster", "bt2")

function roster:new(inVersion)
	local self = btroster:new("roster", inVersion)

	btTable.addParent(self, roster)
	btTable.setClassMetatable(self)

	return self
end

----------------------------------------
-- toArray()
----------------------------------------
function roster:toArray()
	local t = {}
	local k

	for k,_ in pairs(self.characters) do
		table.insert(t,k)
	end

	for k,_ in pairs(self.parties) do
		table.insert(t,k)
	end

	table.sort(t)

	return t
end

----------------------------------------
-- printMember()
--
-- Print the member line in list format
----------------------------------------
function roster:printMember(inName)
	if (self.characters[inName] ~= nil) then
		if (self.characters[inName].isDead) then
			text:print("@" .. inName)
		else
			text:print(" " .. inName)
		end
	else
		text:print("*" .. inName)
	end
end

----------------------------------------
-- nameExists()
--
--
----------------------------------------
function roster:nameExists(inName)
	if ((self.characters[inName] ~= nil) or
	    (self.parties[inName] ~= nil)) then
		return true
	end

	return false
end

----------------------------------------
-- writeCharacter()
----------------------------------------
function roster:writeCharacter(inChar)
	self.characters[inChar.name] = inChar:toTable()
	self:write()
end

----------------------------------------
-- readCharacter()
----------------------------------------
function roster:readCharacter(inName)
	local char

	assert(self.characters[inName] ~= nil)

	char = character:new()
	char:fromTable(self.characters[inName])

	return char
end

----------------------------------------
-- writeParty()
----------------------------------------
function roster:writeParty(inName)
	self.parties[inName] = party:toTable()
	self:write()
end

----------------------------------------
-- readParty()
----------------------------------------
function roster:readParty(inName)
	assert(self.parties[inName] ~= nil)

	return self.parties[inName]
end

----------------------------------------
-- isParty()
----------------------------------------
function roster:isParty(inName)
	if (self.parties[inName] ~= nil) then
		return true
	end

	return false
end

----------------------------------------
-- remove()
----------------------------------------
function roster:remove(inName)
	dprint(self.characters[inName])
	if (self.characters[inName] ~= nil) then
		self.characters[inName] = nil
	else
		self.parties[inName] = nil
	end
	self:write()
end

----------------------------------------
-- renameMember()
----------------------------------------
function roster:renameMember(inOldName, inNewName)
	if (self.characters[inOldName]) then
		self.characters[inNewName] = self.characters[inOldName]
		self.characters[inNewName].name = inNewName
		self.characters[inOldName] = nil
	else
		self.parties[inNewName] = self.parties[inOldName]
		self.parties[inOldName] = nil
	end
	self:write()
end

local itemArray = {
	"Admt Chain", "Dayblade", "Spectre Snare", "Mthr Shield",
	"Ogrewand", "Dragon Fgn", "Frost Horn", "Admt Sword",
	"Mthr Axe", "Crystal Sword", "Shield Ring", "Bracers [6]",
	"Troll Ring", "Buckler", "Kael's Axe", "Thief Dagger",
	"Ring of Power", "Mthr Gloves", "Master Key", "Robes",
	"Deathring", "Staff of Lor", "Flame Horn", "Scale Armor",
	"Dragonwand", "Halbard", "Plate Armor", "Luckshield",
	"Mthr Sword", "Mthr Scale", "Dmnd Helm", "Dmnd Shield",
	"War Staff", "Speedboots", "Pureblade", "Conjurstaff",
	"Pure Shield", "Mage Staff", "Admt Helm", "Powerstaff",
	"Helm", "Stoneblade", "Ali's Carpet", "Shield Staff",
	"Bracers [4]", "Hawkblade", "Tower Shield", "Mthr Helm",
	"Galt's Flute", "Dragonshield", "Dagger", "Mthr Dagger",
	"Soul Mace", "Giant Fgn", "Admt Gloves", "Lamp",
	"Fin's Flute", "Wargloves", "Admt Plate", "Admt Shield",
	"Chain Mail", "Elf Cloak", "Mthr Plate", "Short Sword",
	"Mthr Chain", "Leather Armor", "Pipes of Pan", "Lightwand",
	"Gauntlets", "Leather Glvs.", "War Axe", "Dmnd Plate",
	"Sorcerstaff", "Mandolin", "Broadsword", "Staff",
	"Torch", "Bardsword"
}

----------------------------------------
-- convert()
----------------------------------------
function roster:convert(inTable, inVersion)
	local c = {}
	local copyList = {
		"name", "race", "class", 
		"st", "iq", "dx", "lk", "cn",
		"save_st", "save_iq", "save_dx", "save_lk", "save_cn",
		"ac", "cur_hp", "max_hp", "cur_sppt", "max_sppt",
		"xp", "gold", "cur_level", "max_level", "rogu_level",
		"hunt_level", "songs", "isDoppelganger", "isDead",
		"isOld", "isPoisoned", "isStoned", "isParalyzed",
		"isPossessed", "isNuts", "possessed", "num_attacks",
		"bat_vict"
	}
	local k
	local v
	local i
	local itemMap = {}

	for _,v in ipairs(itemArray) do
		itemMap[v] = true
	end

	for _,k in ipairs(copyList) do
		c[k] = inTable[k]
	end

	c.spellLevel = {}
	c.spellLevel.Magician = inTable.spellLevel.Magician
	c.spellLevel.Conjurer = inTable.spellLevel.Conjurer
	c.spellLevel.Sorcerer = inTable.spellLevel.Sorcerer
	c.spellLevel.Wizard = inTable.spellLevel.Wizard
	c.spellLevel.Archmage = 0

	c.inventory = {}
	for k,v in ipairs(inTable.inventory) do
		if (itemMap[v.name]) then
			i = items.fromBTI(v.name)

			i.isIdentified = v.isIdentified
			local xxx_might_strip_equipped = true
			--i.isEquipped = v.isEquipped
			i.isEquipped = false

			table.insert(c.inventory, i)
		end
	end

	if (c.cur_level > 30) then
		c.cur_level = 30
	end

	if (c.max_level > 30) then
		c.max_level = 30
	end


	local newchar

	newchar = character:new()
	newchar:fromTable(c)
	newchar.xp = newchar:getXpForLevel(newchar.cur_level)

	return newchar
end










