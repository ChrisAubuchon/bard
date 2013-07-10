require "btlib"

local _race = {}

_race.init = function ()
	local self = {}
	local races = nil

	races = read_table("races")

	local function race_iter(t, state)
		state = state + 1

		local r = races[state]
		if (r) then
			return state, r
		end
	end

	----------------------------------------
	-- ipairs()
	--
	-- Iterator to print an ordered list of
	-- races
	----------------------------------------
	function self.ipairs()
		return race_iter, races, 0
	end

	----------------------------------------
	-- randomStat()
	--
	-- Generate a random statistic from a 
	-- race base stat
	----------------------------------------
	function self.randomStat(race, stat)
		local s = 0

		assert(races[race][stat] ~= nil)

		s = races[race][stat] + rnd_xdy_z(2, 7, 1)

		if (s > 18) then
			s = 18
		end

		return s
	end

	----------------------------------------
	-- select()
	--
	-- Have the player select a race
	----------------------------------------
	function self.selectRace()
		local i
		local r
		local inkey

		text:clear()
		text:print("Select a race for your new character:")
		text:setCursor(0, 3)
		for i,r in self.ipairs() do
			text:print("\n%d) %s", i, r)
		end

		printCancel()

		repeat
			inkey = getkey()

			if (key_in_num_range(inkey, 1, 7)) then
				inkey = tonumber(inkey)
				return races[inkey]
			end
		until (inkey == "C")

		return false
	end

	----------------------------------------
	-- startingClasses()
	--
	-- Return the list of available classes
	-- for a race
	----------------------------------------
	function self.startingClasses(r)
		assert(races[r] ~= nil)

		return races[r].starting_classes
	end
	
	return self
end

race = _race.init()
