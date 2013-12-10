require "bICharacter"

roster = btroster:new("roster", "bt1")

function roster:new(inVersion)
	local self = btroster:new("roster", inVersion)

	btTable.addParent(self, roster)
	btTable.setClassMetatable(self)

	return self
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















