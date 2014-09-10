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

