----------------------------------------
-- Local variables
----------------------------------------
local cities = {}

----------------------------------------
-- Private classes 
----------------------------------------

----------------------------------------
-- Base class for city squares and
-- buildings
----------------------------------------
local square = {}
function square:new(inLabel, inEnterFunction)
	local self = object:new()

	self.enterFunction	= false
	self.label		= inLabel

	if (inEnterFunction) then
		self.enterFunction = compileString(
			"return " .. inEnterFunction
			)
	end

	self:addSelf(square)

	return self
			
end

----------------------------------------
-- square:isPath()
----------------------------------------
function square:isPath()
	return false
end

----------------------------------------
-- square:isBuilding()
----------------------------------------
function square:isBuilding()
	return false
end

----------------------------------------
-- square:onEnter()
----------------------------------------
function square:onEnter()
	local rval

	if (not self.enterFunction) then
		return false
	end

	rval = self.enterFunction()

	return rval
end

----------------------------------------
-- square:clearEnter()
----------------------------------------
function square:clearEnter()
	self.enterFunction = nil
end

----------------------------------------
-- City path square class
----------------------------------------
local citysq = {}
function citysq:new(inLabel, inSquare)
	local self = square:new(inLabel, inSquare.enterFunction)

	self.location	= inSquare.location
	self.north	= inSquare.north
	self.south	= inSquare.south
	self.east	= inSquare.east
	self.west	= inSquare.west

	self:addSelf(citysq)

	return self
end

----------------------------------------
-- citysq:isPath()
----------------------------------------
function citysq:isPath()
	return true
end

----------------------------------------
-- citySquare:toCoordinates()
----------------------------------------
function citysq:toCoordinates()
	local x
	local y

	x = tonumber(string.sub(self.label, 1, 2))
	y = tonumber(string.sub(self.label, 4, 5))

	return x,y
end

----------------------------------------
-- City building class
----------------------------------------
local citybldg = {}
function citybldg:new(inLabel, inBuilding)
	local self = square:new(inLabel, inBuilding.enterFunction)

	self:addSelf(citybldg)

	self.distFace	= inBuilding.distFace
	self.nearFace	= inBuilding.nearFace

	return self
end

----------------------------------------
-- citybldg:isBuilding()
----------------------------------------
function citybldg:isBuilding()
	return true
end


----------------------------------------
-- __buildView
--
-- The graphics tiles aren't as well
-- matched up on BT2 as in BT1.
-- For example, the base-2 face 
-- overlaps the base-2 leftfront and
-- rightfront images.
--
-- Because of this we have to be very
-- specific in the order that we add the
-- tiles to the view. The order has to
-- go:
-- 1. left and right side
-- 2. middle
--
-- This matches DOS version behavior
----------------------------------------
local function __buildView(maxdepth, sq, dir, depth, cycle, prevleft, prevright)
	local front_sq	= sq[directions.front[dir]]
	local right_sq	= sq[directions.right[dir]]
	local left_sq	= sq[directions.left[dir]]
	local path_sq

	if (front_sq == nil) then
		return
	end

	if (depth >= maxdepth) then
		return
	end

	----------------------------------------
	-- Recurse to the first middle building
	-- or maxdepth
	----------------------------------------
	if (not front_sq.isBuilding()) then
		__buildView(maxdepth, front_sq, dir, depth + 1, cycle)
	end

	if (left_sq.isBuilding()) then
		bigpic:cityAdd(cycle, depth, "leftfront", left_sq.distFace)
	elseif ((depth + 1) < maxdepth) then
		path_sq = left_sq[directions.front[dir]]
		if (path_sq and path_sq.isBuilding()) then
			bigpic:cityAdd(cycle, depth + 1, "leftside", 
					path_sq.distFace)
		end
	end

	if (right_sq.isBuilding()) then
		bigpic:cityAdd(cycle, depth, "rightfront", right_sq.distFace)
	elseif ((depth + 1) < maxdepth) then
		path_sq = right_sq[directions.front[dir]]
		if (path_sq and path_sq.isBuilding()) then
			bigpic:cityAdd(cycle, depth + 1, "rightside", 
				path_sq.distFace)
		end
	end

	if (front_sq.isBuilding()) then
		if (depth == 1) then
			bigpic:cityAdd(cycle, depth, "face", front_sq.nearFace)
			return 
		else
			bigpic:cityAdd(cycle, depth, "face", front_sq.distFace)
		end
	end
end

city = {}
function city:new(inName)
	local self = object:new()

	self:addParent(city)
	self:addParent(level)

	self.sqs	= {}
	self.bldgs	= {}
	self.day	= false
	self.night	= false

	self.name	= inName
	if (type(cities[inName]) == "string") then
		cities[inName] = diskio:readTable(inName)
	end

	self.title	= cities[inName].title
	self.day	= cities[inName].day
	self.night	= cities[inName].night
	self.guildExitSquare	= cities[inName].guildExitSquare
	self.guildExitDir	= cities[inName].guildExitDir
	globals.guildCity	= inName
	if (globals.isNight) then
		self.level	= 2
	else
		self.level	= 1
	end

	local function __initSquares()
		local label
		local square

		for label, square in pairs(cities[inName].squares) do
			self.sqs[label] = citysq:new(label, square)
		end
	end

	local function __initBuildings()
		local label
		local square

		for label, square in pairs(cities[inName].buildings) do
			self.sqs[label] = citybldg:new(label, square)
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
-- city:saveState()
----------------------------------------
function city:saveState(inTable)
	local t		= inTable or {}

	t.class		= "city"
	t.name		= self.name
	t.direction	= self.direction
	t.x, t.y	= self.currentSquare:toCoordinates()

	return t
end

----------------------------------------
-- city:restoreState()
----------------------------------------
function city:restoreState(inTable)
	local c

	c = city:new(inTable.name)
	c:enter(inTable.x, inTable.y, inTable.direction)

	return c
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
	return string.format("%02d-%02d", inX, inY)
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
	local rval

	if (front_sq:isBuilding()) then
		front_sq:onEnter()
		if (self.exit) then
			return true
		end
		self:resetBigpic()
	else
		self:animateMove()
		self.previousSquare = self.currentSquare
		self.currentSquare = front_sq
		if (self.currentSquare:onEnter()) then
			self.currentSquare = self.previousSquare
		end
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
	if (not inX) then
		self.direction = self.guildExitDir
		self.currentSquare = self:getSq(self.guildExitSquare)
	else
		self.direction = inDirection
		self.currentSquare = self:getSq(self:getLabelXY(inX, inY))
		log:print(log.LOG_DEBUG, "currentSquare: %s", self.currentSquare)
	end
end

----------------------------------------
-- main()
----------------------------------------
function city:main()
	local inkey

	log:print(log.LOG_DEBUG, "currentSquare: %s", self.currentSquare.label)
	text:clear()
	self:resetBigpic()

	repeat
		globals.doTimeEvents = true
		inkey = getkey()
		globals.doTimeEvents = false

		if (keyboardCommand(inkey)) then
			if (not self.exit) then
				self:resetBigpic()
				text:cdprint(true, false, self.currentSquare.label)
			end
		elseif (inkey == "?") then
			self:printLocation()
		elseif (inkey == "D") then
			if (globals.debug) then
				btdebug.cityDebug()
			end
		end

		if (globals.partyDied) then
			globals.gameState = globals.STATE_GUILD
			self.exit = true
		end
	until (self.exit)
end

----------------------------------------
-- printLocation()
----------------------------------------
function city:printLocation()
	text:clear()
	if (self.currentSquare.location == "plaza") then
		text:print("\n\nYou're in the Plaz,")
	else
		text:print("\n\nYou are on %s Street,",
				self.currentSquare.location)
	end
	text:print(" facing %s.", self.direction)
	text:print("\n\nIt's now " .. globals.timeOfDay[globals.gameHour])
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

function city:printCredits()
	text:csplash(true, true, "Bard's Tale II: Reimplemented by me")
end
	

----------------------------------------
-- enterDungeon()
----------------------------------------
function city:enterDungeon(inName, inLevel)
	currentLevel.exit = true
	globals.citySquare = currentLevel.currentSquare.label
	globals.cityDirection = currentLevel.direction
	currentLevel = dun:new(inName, inLevel, 0, 0, "north")
end

local function __init()
	cities = diskio:readTable("cities", false)
end

__init()
