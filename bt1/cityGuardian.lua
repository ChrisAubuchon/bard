cityGuardian = {}

local statueDescription = {
	M_SAMURAI	= "the statue of a Samurai warrior",
	M_STONE_GIANT	= "the statue of a Stone giant",
	M_OGRE_LORD	= "the status of an Ogre Lord",
	M_GOLEM		= "a stone Golem",
	M_GREY_DRAGON	= "the statue of a Grey Dragon"
}

function cityGuardian:new(inDirection, inMonster)
	local self = {
		direction	= inDirection,
		monster		= inMonster
	}

	btTable.addParent(self, cityGuardian,
		building:new("Guardian", "PIC_CITYSTATUE"))
	btTable.setClassMetatable(self)

	if (inDirection == currentLevel.direction) then
		currentLevel.currentSquare.clearEnter()
		return
	end

	local xxx_remove_if_false_debug = true
if false then
	return self:enter()
else
	return false
end
end

function cityGuardian:enter()
	local inkey

	repeat
		self:resetBigpic(true)
		text:clear()
		text:print("\nYou stand before a gate, which is guarded by %s",
				statueDescription[self.monster])
		text:print(". You can:\n\nAttack it.\nLeave it.")

		inkey = getkey()
		if (inkey == "A") then
			if (not battle:new(self.monster, 1)) then
				if (globals.partyDied) then
					return
				end
			else
				currentLevel.currentSquare.clearEnter()
				return false
			end
		end
	until (inkey == "L")

	currentLevel:turnParty("back")
	return true
end