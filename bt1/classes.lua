local __classes = read_table("classes")


classes = {}
function classes:setClass(inClass)
	local k
	local v

	self.class	= inClass
	table.copy(__classes[self.class], self)
end

----------------------------------------
-- selectClass()
----------------------------------------
function classes:selectClass(inRace)
	local i
	local inkey
	local classList

	classList = race.startingClasses(inRace)

	for i = 1,#classList do
		text:print("\n%d) %s", i, classList[i])
	end

	text:setCursor(0, 10)
	text:print("\n   (REROLL)")
	repeat
		inkey = getkey()
		if (key_in_num_range(inkey, 1, #classList)) then
			inkey = tonumber(inkey)
			return classList[inkey]
		end
	until (inkey == "R")

	return false
end

----------------------------------------
-- getXpForLevel()
----------------------------------------
function classes:getXpForLevel(inLevel)
	if (inLevel < 13) then
		return self.xpreq[inLevel]
	else
		return self.xpreq[12] * (inLevel - 11)
	end
end





























