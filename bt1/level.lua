----------------------------------------
-- level class
--
-- Base class for all levels. Cities and
-- dungeons are sub-classes of this
-- class
----------------------------------------
level = {}
level.new = function()
	local self = {
		exit		= false,
		name		= false,
		title		= false,
		currentSquare	= false,
		direction	= false
	}

	function self.main() error("Must overload main()") end
	function self.buildView() error("Must overload buildView()") end
	function self.buildTitle() error("Must overload buildTitle()") end
	function self.turnParty(newDirection)
		error("Must overlaod turnParty()")
	end
	function self.moveForward() error("Must overload moveForward()") end
	function self.resetBigpic() error("Must overload resetBigpic()") end
	function self.getSq(label) error("Must overload getSq()") end


	function self.isDungeon()
		return false
	end
	function self.isCity()
		return false
	end
	function self.setTitle()
		bigpic.setTitle(self.title)
	end

	function self.getBattleReward()
		return self.items[rnd_xdy(1,#self.items)]
	end

	-- Return a list of monster types to battle
	--
	function self.getBattleOpponents()
		local numGroups
		local monGroups = {}
		local i

		numGroups = rnd_xdy(1,4)
		for i = 1,numGroups do
			monGroups[i] = self.monsters[rnd_xdy(1,#self.monsters)]
		end

		return monGroups
	end

	function self.getPoisonDamage()
		return self.poisonDamage
	end

	return self
end
