----------------------------------------
-- Local variables
----------------------------------------
local cities = {}

local timeOfDay = {
	"midnight.",
	"midnight.",
	"after midnight.",
	"after midnight.",
	"after midnight.",
	"after midnight.",
	"early morning.",
	"early morning.",
	"early morning.",
	"mid-morning.",
	"mid-morning.",
	"noon.",
	"noon.",
	"noon.",
	"afternoon.",
	"afternoon.",
	"afternoon.",
	"dusk.",
	"dusk.",
	"evening.",
	"evening.",
	"evening.",
	"evening.",
	"evening.",
	"midnight."
}

----------------------------------------
-- Private classes 
----------------------------------------

----------------------------------------
-- Base class for city squares and
-- buildings
----------------------------------------
local square = {}
square.new = function(lab, inEnterFunction, inEnterArgs) 
	local self = {
		label		= lab
	}
	local enterFunction	= nil
	local enterArgs		= inEnterArgs

	enterFunction = inEnterFunction

	function self.isPath()		return false end
	function self.isBuilding()	return false end

	----------------------------------------
	-- onEnter
	--
	--   Execute code associated with the
	-- square. Errors in the square code are
	-- fatal.
	--
	--   Returns false if there is no code
	-- for the square
	----------------------------------------
	function self.onEnter(city)
		local rval
		local msg

		if (enterFunction == nil) then
			return false
		end

		executeString(enterFunction)

		return rval
	end

	return self
end

----------------------------------------
-- City path square class
----------------------------------------
local citysq = {}
citysq.new = function(inLabel, inSquare)
	local self

	self = square.new(inLabel, inSquare.enterFunction)
	self.location	= inSquare.location
	self.north	= inSquare.north
	self.south	= inSquare.south
	self.east	= inSquare.east
	self.west	= inSquare.west

	function self.isPath() 	return true 	end

	----------------------------------------
	-- link
	--
	--   Link the edges of the map with the
	-- the other nodes
	----------------------------------------
	function self.link(n,s,e,w)
		self.north	= n
		self.south	= s
		self.east	= e
		self.west	= w
	end

	return self
end

----------------------------------------
-- City building class
----------------------------------------
local citybldg = {}
citybldg.new = function(inLabel, inBuilding)
	local self

	self		= square.new(inLabel, inBuilding.enterFunction)
	self.distFace	= inBuilding.distFace
	self.nearFace	= inBuilding.nearFace

	function self.isBuilding()	return true	end

	return self
end


local function __buildView(maxdepth, sq, dir, depth, cycle, prevleft, prevright)
	local front_sq	= sq[directions.front[dir]]
	local right_sq	= sq[directions.right[dir]]
	local left_sq	= sq[directions.left[dir]]
	local quad	= cycle .. "-" .. depth

	if (front_sq == nil) then
		return
	end

	if (depth >= maxdepth) then
		return
	end

	-- If the front square is a building then the face obscures
	-- any detail further down. Show the face and return.
	--
	if (front_sq.isBuilding()) then
		if (depth == 1) then
			bigpic:cityAdd(quad, "face", front_sq.nearFace)
			return 
		else
			bigpic:cityAdd(quad, "face", front_sq.distFace)
		end
	else
		-- Recursively call with an increased depth
		--
		__buildView(maxdepth, front_sq, dir, depth + 1, cycle,
				left_sq.isBuilding(), right_sq.isBuilding())
	end


	if (left_sq.isBuilding()) then
		bigpic:cityAdd(quad, "leftfront", left_sq.distFace)
		if (not prevleft) then
			bigpic:cityAdd(quad, "leftside", left_sq.distFace)
		end
	end

	if (right_sq.isBuilding()) then
		bigpic:cityAdd(quad, "rightfront", right_sq.distFace)
		if (not prevright) then
			bigpic:cityAdd(quad, "rightside", right_sq.distFace)
		end
	end
end
			

----------------------------------------
-- city base class
----------------------------------------
city = {}
city.new = function (inName, startX, startY, startDirection)
	local self = level.new()
	local sqs		= {}
	local bldgs		= {}

	
	----------------------------------------
	-- __init
	--
	-- Initialize the internal data
	-- structures. In Bard's Tale, the city
	-- data is read from disk every time the
	-- party enters the city. We accomplish
	-- this by recreating the object from
	-- JSON table
	----------------------------------------
	local function __init()
		local label
		local square

		self.name	= inName
		self.title	= "Skara Brae"
		self.direction	= startDirection
		self.level	= 1
		self.day	= cities[inName].day
		self.night	= cities[inName].night

		for label, square in pairs(cities[inName].squares) do
			sqs[label] = citysq.new(label, square)
		end

		for label, square in pairs(cities[inName].buildings) do
			bldgs[label] = citybldg.new(label, square)
		end

		for label, square in pairs(sqs) do
			square.north = self.getSq(square.north)
			square.south = self.getSq(square.south)
			square.east = self.getSq(square.east)
			square.west = self.getSq(square.west)
		end

		self.currentSquare = self.getSq(startX.."-"..startY)
	end

	function self.isCity()
		return true
	end

	----------------------------------------
	-- newSq()
	--
	-- Create a new path square
	----------------------------------------
	function self.newSq(label, location, onEnter)
		sqs[label] = citysq.new(label, location, onEnter)
	end

	----------------------------------------
	-- getSq()
	--
	-- Get a path square
	----------------------------------------
	function self.getSq(label)
		if (sqs[label] ~= nil) then
			return sqs[label]
		else
			return bldgs[label]
		end
	end

	----------------------------------------
	-- newBldg()
	--
	-- Create a new city building
	----------------------------------------
	function self.newBldg(label, df, nf, one)
		bldgs[label] = citybldg.new(label, df, nf, one)
	end

	function self.buildView()
		bigpic:cityBackground()
		__buildView(4, self.currentSquare, self.direction, 1,
				"base", true, true)
		bigpic:cityDisplay()
	end

	----------------------------------------
	-- animateMove
	--
	--   Animate the movement of the party
	-- in the city. On fast processors you
	-- don't even really notice.
	----------------------------------------
	function self.animateMove()
		bigpic:cityBackground()
		__buildView(4, self.currentSquare, self.direction, 1,
				"1", true, true)
		bigpic:cityDisplay()

		bigpic:cityBackground()
		__buildView(4, self.currentSquare, self.direction, 1,
				"2", true, true)
		bigpic:cityDisplay()

		bigpic:cityBackground()
		__buildView(4, self.currentSquare, self.direction, 1,
				"3", true, true)
		bigpic:cityDisplay()
	end

	function self.turnParty(inRelDirection) 
		self.direction = directions[inRelDirection][self.direction]
		self.buildView()
		if (party.compass.active) then
			party.compass:update(self.direction)
		end
	end

	function self.moveForward()
		local front_sq = self.currentSquare[self.direction]

		if (front_sq.isBuilding()) then
			front_sq.onEnter(self)
			if (self.exit) then
				return true
			end
			bigpic:setTitle("Skara Brae")
		else
			self.animateMove()
			if (not front_sq.onEnter(self)) then
				self.currentSquare = front_sq
			end
			self.buildView()
		end

		text:clear()
		text:print(self.currentSquare.label)
	end

	function self.getPoisonDamage()
		if (globals.isNight) then	
			return self.night.poisonDamage
		else
			return self.day.poisonDamage
		end
	end

	function self.resetBigpic()
		self.buildView()
		self.setTitle()
	end

	function self.main()
		local inkey

		text:clear()
		self.resetBigpic()

		repeat
			globals.doTimeEvents = true
			inkey = getkey()
			globals.doTimeEvents = false

			if (keyboardCommand(inkey)) then
				self.resetBigpic()
			elseif (inkey == "?") then
				self.printLocation()
			elseif (inkey == "D") then
				if (globals.debug) then
					btdebug.cityDebug()
				end
			elseif (inkey == "Z") then
				party:doSummon("M_STONE_ELEMENTAL")
			end

			if (globals.partyDied) then
				globals.gameState = globals.STATE_PARTYDIED
				self.exit = true
			end
		until (self.exit)
	end

	function self.printLocation()
		if (self.currentSquare.location == "grand") then
			text:print("\n\nYou're in the Grand Plaz.")
		else
			text:print("\n\nYou are on " .. 
					self.currentSquare.location .. 
					" facing " .. self.direction
				)
		end

		text:print("\n\nIt's now " .. timeOfDay[globals.gameHour])
	end

	function self.getBattleReward()
		if (globals.isNight) then
			return self.night.items[rnd_xdy(1,#self.night.items)]
		else
			return self.day.items[rnd_xdy(1,#self.day.items)]
		end
	end

	function self.getBattleOpponents()
		local timeTable
		local monGroup = {}

		if (globals.isNight) then
			timeTable = self.night
		else
			timeTable = self.day
		end

		monGroup[1] = timeTable.monsters[rnd_xdy(1,#timeTable.monsters)]

		return monGroup
	end

	__init()

	return self
end

local function __init()
	cities.skara = read_table("skara", false)
end

__init()
