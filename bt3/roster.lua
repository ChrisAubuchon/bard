require "bICharacter"
require "bIICharacter"

roster = btroster:new("roster", "bt3")

----------------------------------------
-- roster:writeCharacter()
----------------------------------------
function roster:writeCharacter(inChar, inIsTable)
	if (inIsTable) then
		self.characters[inChar.name] = inChar
		self:write()
	end
end

----------------------------------------
-- roster:readCharacter()
----------------------------------------
function roster:readCharacter(inName)
	if (not self.characters[inName]) then
		return false
	end

	if (self.characters[inName].class == "Il") or 
	   (self.characters[inName].class == "Mn") then
		return summon:fromTable(self.characters[inName])
	else
		return character:fromTable(self.characters[inName])
	end

end


----------------------------------------
-- roster:printMember()
----------------------------------------
function roster:printMember(inName)
	if (self.characters[inName] ~= nil) then
		text:print(inName)
	else
		text:print("> " .. inName)
	end
end

----------------------------------------
-- roster:toArray()
----------------------------------------
function roster:toArray()
	local rval	= {}
	local key

	local tmp	= {}


	for key,_ in pairs(self.parties) do
		table.insert(rval, key)
	end

	table.sort(rval)

	for key,_ in pairs(self.characters) do
		table.insert(tmp, key)
	end

	table.sort(tmp)

	for _,key in ipairs(tmp) do
		table.insert(rval, key)
	end

	return rval
end
