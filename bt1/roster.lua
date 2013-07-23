require "character"

local __roster = {}
function __roster:new(inFileName)
	local self = read_table(inFileName, true)
	self.fileName = inFileName

	if (not self.characters) then
		self.characters = {}
		self.parties = {}
		write_table(inFileName)
	end

	btTable.addParent(self, __roster)
	btTable.setClassMetatable(self)

	return self
end

function __roster:write()
	local t = {}
	t.characters = self.characters
	t.parties = self.parties
	write_table(t, self.fileName)
end

----------------------------------------
-- toArray()
--
-- Return an array with a sorted list
-- of the parties and characters
----------------------------------------
function __roster:toArray()
	local t = {}
	local k

	dprint("roster.toArray()")

	for k,_ in pairs(self.characters) do
		table.insert(t, k)
	end

	for k,_ in pairs(self.parties) do
		table.insert(t, k)
	end

	table.sort(t)

	return t
end

roster = __roster:new("roster")

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
-- remove()
----------------------------------------
function roster:remove(inName)
	if (self.characters[inName] ~= nil) then
		self.characters[inName] = nil
	else
		self.parties[inName] = nil
	end
end














