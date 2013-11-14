require "dataio"

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

	enterFunction = compileString(inEnterFunction)

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

		rval = enterFunction()

		return rval
	end

	function self.clearEnter()
		enterFunction = nil
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

city = {}
function city:new(inName)
	local self = {
		sqs	= {},
		bldgs	= {},
		day	= false,
		night	= false,
	}


	btTable.addParent(self, city, level)
	btTable.setClassMetatable(self)

	self.name	= inName
	self.title	= cities[inName].title
	self.day	= cities[inName].day
	self.night	= cities[inName].night
	if (globals.isNight) then
		self.level	= 2
	else
		self.level	= 1
	end

	local function __initSquares()
		local label
		local square

		for label, square in pairs(cities[inName].squares) do
			self.sqs[label] = citysq.new(label, square)
		end
	end

	local function __initBuildings()
		local label
		local square

		for label, square in pairs(cities[inName].buildings) do
			self.sqs[label] = citybldg.new(label, square)
		end
	end

	local function __linkSquares()
		local label
		local square

		for label, square in pairs(self.sqs) do
			square.north = self:getSq(square.north)
			square.south = self:getSq(square.south)
			square.east = self:getSq(square.east)
			square.west = self:getSq(square.west)
		end
	end

	__initSquares()
	__initBuildings()
	__linkSquares()

	return self
end

----------------------------------------
-- getSq()
----------------------------------------
function city:getSq(inLabel)
	if (self.sqs[inLabel] ~= nil) then
		return self.sqs[inLabel]
	else
		return self.bldgs[inLabel]
	end
end

----------------------------------------
-- getLabelXY()
----------------------------------------
function city:getLabelXY(inX, inY)
	return string.format("%d-%d", inX, inY)
end

----------------------------------------
-- isCity()
----------------------------------------
function city:isCity()
	return true
end

----------------------------------------
-- buildView()
----------------------------------------
function city:buildView()
	bigpic:cityBackground()
	__buildView(4, self.currentSquare, self.direction, 1,
			"base", true, true)
	bigpic:cityDisplay()
end

----------------------------------------
-- animateMove()
--
-- Animate the movement of the party in
-- the city. Very fast. Probably won't
-- notice.
----------------------------------------
function city:animateMove()
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

----------------------------------------
-- turnParty()
----------------------------------------
function city:turnParty(inRelDirection) 
	self.direction = directions[inRelDirection][self.direction]
	self:buildView()
	if (party.compass.active) then
		party.compass:update(self.direction)
	end
end

----------------------------------------
-- moveForward
----------------------------------------
function city:moveForward()
	local front_sq = self.currentSquare[self.direction]

	if (front_sq.isBuilding()) then
		front_sq.onEnter()
		if (self.exit) then
			return true
		end
		bigpic:setTitle("Skara Brae")
	else
		self:animateMove()
		self.previousSquare = self.currentSquare
		self.currentSquare = front_sq
		dprint("Current square: " .. self.currentSquare.label)
		if (self.currentSquare.onEnter()) then
			self.currentSquare = self.previousSquare
		end
		dprint("Current square: " .. self.currentSquare.label)
		self:buildView()
	end

	text:clear()
end

----------------------------------------
-- getPoisonDamage()
----------------------------------------
function city:getPoisonDamage()
	if (globals.isNight) then	
		return self.night.poisonDamage
	else
		return self.day.poisonDamage
	end
end

----------------------------------------
-- resetBigpic()
----------------------------------------
function city:resetBigpic()
	self:buildView()
	self:setTitle()
end

----------------------------------------
-- enter()
----------------------------------------
function city:enter(inX, inY, inDirection)
	self.direction = inDirection
	self.currentSquare = self:getSq(inX .. "-" .. inY)
end

----------------------------------------
-- main()
----------------------------------------
function city:main()
	local inkey

	text:clear()
	self:resetBigpic()

	repeat
		globals.doTimeEvents = true
		inkey = getkey()
		globals.doTimeEvents = false

		if (keyboardCommand(inkey)) then
			self:resetBigpic()
		elseif (inkey == "?") then
			self:printLocation()
		elseif (inkey == "D") then
			if (globals.debug) then
				btdebug.cityDebug()
			end
		elseif (inkey == "Z") then
			party:doSummon({ type = "M_STONE_ELEMENTAL"})
		end

		if (globals.partyDied) then
			globals.gameState = globals.STATE_PARTYDIED
			self.exit = true
		end
	until (self.exit)
end

----------------------------------------
-- printLocation()
----------------------------------------
function city:printLocation()
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

----------------------------------------
-- getBattleReward()
----------------------------------------
function city:getBattleReward()
	if (globals.isNight) then
		return self.night.items[random:xdy(1,#self.night.items)]
	else
		return self.day.items[random:xdy(1,#self.day.items)]
	end
end

----------------------------------------
-- getBattleOpponents()
----------------------------------------
function city:getBattleOpponents()
	local timeTable
	local monGroup = {}

	if (globals.isNight) then
		timeTable = self.night
	else
		timeTable = self.day
	end

	monGroup[1] = timeTable.monsters[random:xdy(1,#timeTable.monsters)]

	return monGroup
end

function city:printCredits()
	text:splashMessage("Bard's Tale I: Reimplemented by me")
end
	

----------------------------------------
-- enterDungeon()
----------------------------------------
function city:enterDungeon(inName, inLevel)
	currentLevel.exit = true
	currentLevel.exitLocation = inName
	globals.gameState = globals.STATE_DUNGEON
	globals.citySquare = currentLevel.currentSquare.label
	globals.cityDirection = currentLevel.direction
	currentLevel = dun:new(inName, inLevel, 0, 0, "north")
end

local function __init()
	cities.skara = read_table("skara", false)
end

__init()
