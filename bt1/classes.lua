local _class = {}

_class.init = function ()
	local self = {}
	local classes = nil

	classes = read_table("classes")

	----------------------------------------
	-- selectClass()
	--
	-- Have the player select a class
	----------------------------------------
	function self.selectClass(r)
		local i
		local inkey
		local clist

		clist = race.startingClasses(r)

		for i = 1, #clist do
			text_print("\n%d) %s", i, clist[i])
		end

		text_setCursor(0, 10)
		text_print("\n   (REROLL)")

		repeat
			inkey = getkey()
			if (key_in_num_range(inkey, 1, #clist)) then
				inkey = tonumber(inkey)
				return clist[inkey]
			end
		until (inkey == "R")

		return false

	end

	----------------------------------------
	-- get()
	--
	-- Get a member of a class
	----------------------------------------
	function self.get(c, attr)
		assert(classes[c] ~= nil, "Invalid class " .. tostring(c))
		assert(classes[c][attr] ~= nil)

		return classes[c][attr]
	end

	function self.getXpForLevel(c, level)
		if (level < 13) then
			return classes[c].xpreq[level]
		else
			return classes[c].xpreq[12] * (level - 11)
		end
	end

	return self
end

classes = _class.init()
