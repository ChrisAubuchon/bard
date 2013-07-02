----------------------------------------
-- Package variables
----------------------------------------
local rost

----------------------------------------
-- Package functions
----------------------------------------
roster = {}

----------------------------------------
-- nameExists()
--
-- Check if the name already exists in
-- the roster
----------------------------------------
function roster.nameExists(name)
	for i,v in pairs(rost.characters) do
		if (i == name) then
			return true
		end
	end

	for i,v in pairs(rost.parties) do
		if (i == name) then
			return true
		end
	end

	return false

end

----------------------------------------
-- saveCharacter()
--
-- Add a character to the roster
----------------------------------------
function roster.saveCharacter(c)
	rost.characters[c.name] = c:toTable()

	write_table(rost, "roster")
end

----------------------------------------
-- saveParty()
--
-- Add party to the roster
----------------------------------------
function roster.saveParty(name)
	rost.parties[name] = party:toTable()

	write_table(rost, "roster")
end

----------------------------------------
-- getCharacter()
--
-- Return the character structure for
-- member "name"
----------------------------------------
function roster.getCharacter(name)
	local c

	assert(rost.characters[name] ~= nil)

	c = character:new()
	c:fromTable(rost.characters[name])

	return c
end

----------------------------------------
-- getParty()
--
-- Return the party structure for
-- member "name"
----------------------------------------
function roster.getParty(name)
	assert(rost.parties[name] ~= nil)

	return rost.parties[name]
end

----------------------------------------
-- isParty()
--
-- Return true if the roster member is
-- a party
----------------------------------------
function roster.isParty(name)
	if (rost.parties[name] ~= nil) then
		return true
	else
		return false
	end
end

----------------------------------------
-- listRoster()
--
-- Return a table of roster entries
----------------------------------------
function roster.listRoster()
	local rtable = {}

	for k,v in pairs(rost.characters) do
		if (k ~= "struct") then
			-- Dead characters have an '@' in front
			if (rost.characters[k].isDead) then
				rtable[k] = "@" .. k
			else
				rtable[k] = " " .. k
			end
		end
	end

	for k,v in pairs(rost.parties) do
		if (k ~= "struct") then
			rtable[k] = "*" .. k
		end
	end

	return rtable
end

function roster.printMember(member)
	if (rost.characters[member] ~= nil) then
		if (rost.characters[member].isDead) then
			text_print("@" .. member)
		else
			text_print(" " .. member)
		end
	else
		text_print("*" .. member)
	end
end

function roster.toArray()
	local rtable = {}
	local k

	for k,_ in pairs(rost.characters) do
		table.insert(rtable, k)
	end	

	for k,_ in pairs(rost.parties) do
		table.insert(rtable, k)
	end

	table.sort(rtable)

	return rtable
end

----------------------------------------
-- removeCharacter()
--
-- Remove a character from the roster
----------------------------------------
function roster.removeCharacter(member)

	if (rost.characters[member] ~= nil) then
		rost.characters[member] = nil
	else
		rost.parties[member] = nil
	end

	write_table(rost, "roster")
end

----------------------------------------
-- updateCharacter()
--
-- Update a character in the roster
-- from the given table
----------------------------------------
function roster.updateCharacter(member)
	local k
	local v
	local name

	assert(member:isCharacter())
	assert(rost.characters[member.name] ~= nil)

	rost.characters[member.name] = member:toTable()

	write_table(rost, "roster")
end

function roster.copyCharacter(member)
	local k
	local v
	local c = {}

	assert(rost.characters[member] ~= nil)

	for k,v in pairs(rost.characters[member]) do
		c[k] = v
	end

	return c
end

----------------------------------------
-- Package initialization
----------------------------------------
local function __init()
	local diff

	rost = read_table("roster", true)

	if (rost["characters"] == nil) then
		rost.characters = {}
		rost.parties = {}

		write_table(rost, "roster")
	end
end


__init()

