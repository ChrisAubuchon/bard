----------------------------------------
-- Private classes 
----------------------------------------

local wildData = diskio:readTable("wild")

----------------------------------------
-- Base class for city squares and
-- buildings
----------------------------------------
local square = {}
square.new = function(inLabel, inSquare)
	local self = {
		label		= false,
		north		= false,
		south		= false,
		east		= false,
		west		= false,
		enterFunction	= false,
		face		= false,
		isPath		= false,
		isWall		= false
	}

	assert(type(inSquare) == "table", "inSquare not a table")

	self.label		= inLabel
	self.north		= inSquare.north
	self.south		= inSquare.south
	self.east		= inSquare.east
	self.west		= inSquare.west
	self.face		= inSquare.face
	if (inSquare.enterFunction) then
		self.enterFunction	= compileString("return " .. inSquare.enterFunction)
	end

	self.isPath		= not self.face
	self.isWall		= ((self.face == "wall") or 
				   (self.face == "entr"))

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

		if (not self.enterFunction) then
			return false
		end

		rval = self.enterFunction()

		return rval
	end

	function self.clearEnter()
		self.enterFunction = nil
	end

	return self
end

local function doWallSquare(sq, dir, depth, facet)
	local back_sq
	local front_sq
	local face

	if (sq.isWall) then
		back_sq		= sq[directions.back[dir]]
		front_sq	= sq[directions.front[dir]]

		face = sq.face
		if (back_sq.isWall) then
			if (depth < 4) then
				bigpic:wildAdd(depth, facet, face .. "-side")
			end
		else
			if (depth == 4) then
				bigpic:wildAdd(depth, facet, face)
			elseif ((depth == 2) or (depth == 3)) then
				bigpic:wildAdd(depth, facet, face .. "-corner")
			else
				bigpic:wildAdd(depth, facet, face .. "-side")
			end
		end

		return true
	end

	return false
end

----------------------------------------
-- __buildView
----------------------------------------
local function __buildView(maxdepth, sq, dir, depth)
	if ((depth == 1) and (sq.face)) then
		error("Not on previous square", 3)
	end

	if (depth >= maxdepth) then
		return
	end

	__buildView(maxdepth, sq[directions.front[dir]], dir, depth + 1)

	local left_sq	= sq[directions.left[dir]]
	local right_sq	= sq[directions.right[dir]]
	local leftDone	= false
	local rightDone	= false

	if (depth == 4) then
		local fl_sq = left_sq[directions.left[dir]]
		if (fl_sq.isWall) then
			bigpic:wildAdd(depth, "FL", fl_sq.face)
		else
			fl_sq = right_sq[directions.right[dir]]
			if (fl_sq.isWall) then
				bigpic:wildAdd(depth, "FR", fl_sq.face)
			end
		end
	end

	if (not sq.isWall) then
		leftDone	= doWallSquare(left_sq, dir, depth, "L")
		rightDone	= doWallSquare(right_sq, dir, depth, "R")
	end

	if (not leftDone) then
		if (left_sq.face) then
			bigpic:wildAdd(depth, "L", left_sq.face)
		end
	end

	if (sq.face) then
		bigpic:wildAdd(depth, "M", sq.face)
	end

	if (not rightDone) then
		if (right_sq.face) then
			bigpic:wildAdd(depth, "R", right_sq.face)
		end
	end

end

wild = {}
function wild:new()
	local self = {
		sqs	= {},
		day	= false,
		night	= false,
	}


	btTable.addParent(self, wild, level)
	btTable.setClassMetatable(self)

	self.name	= "wild"
	self.title	= wildData.title
	self.day	= wildData.day
	self.night	= wildData.night
if false then
	if (globals.isNight) then
		self.level	= 2
	else
		self.level	= 1
	end
end

	local function __initSquares()
		local label
		local s

		for label, s in pairs(wildData.squares) do
			self.sqs[label] = square.new(label, s)
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
	__linkSquares()

	return self
end

----------------------------------------
-- getSq()
----------------------------------------
function wild:getSq(inLabel)
	assert(self.sqs[inLabel], "inLabel: " .. tostring(inLabel))
	return self.sqs[inLabel]
end

----------------------------------------
-- getLabelXY()
----------------------------------------
function wild:getLabelXY(inX, inY)
	return string.format("x%02d-%02d", inX, inY)
end

----------------------------------------
-- isCity()
----------------------------------------
function wild:isCity()
	return true
end

----------------------------------------
-- buildView()
----------------------------------------
function wild:buildView()
	bigpic:wildBackground()
	__buildView(5, self.currentSquare, self.direction, 1)
	bigpic:wildDisplay()
end

----------------------------------------
-- turnParty()
----------------------------------------
function wild:turnParty(inRelDirection) 
	self.direction = directions[inRelDirection][self.direction]
	self:buildView()
	if (party.compass.active) then
		party.compass:update(self.direction)
	end
	text:clear()
end

----------------------------------------
-- moveForward
----------------------------------------
function wild:moveForward()
	local front_sq = self.currentSquare[self.direction]

	if (not front_sq.onEnter()) then
		if (self.exit) then
			return
		end
		self.currentSquare = front_sq
		self:buildView()
	end

	if (globals.partyDied) then
		return
	end

	text:clear()
end

----------------------------------------
-- getPoisonDamage()
----------------------------------------
function wild:getPoisonDamage()
	if (globals.isNight) then	
		return self.night.poisonDamage
	else
		return self.day.poisonDamage
	end
end

----------------------------------------
-- resetBigpic()
----------------------------------------
function wild:resetBigpic()
	self:buildView()
	self:setTitle()
end

----------------------------------------
-- enter()
----------------------------------------
function wild:enter(inX, inY, inDirection)
	self.direction = inDirection
	self.currentSquare = self:getSq(self:getLabelXY(inX, inY))
end

----------------------------------------
-- main()
----------------------------------------
function wild:main()
	local inkey

	text:clear()
	self:resetBigpic()

	repeat
		globals.doTimeEvents = true
		inkey = getkey()
		globals.doTimeEvents = false

		if (keyboardCommand(inkey)) then
			self:resetBigpic()
			text:cdprint(true, false, self.currentSquare.label)
		elseif (inkey == "?") then
			self:printLocation()
		elseif (inkey == "D") then
			if (globals.debug) then
				btdebug.cityDebug()
			end
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
function wild:printLocation()
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
function wild:getBattleReward()
	if (globals.isNight) then
		return self.night.items[rnd_xdy(1,#self.night.items)]
	else
		return self.day.items[rnd_xdy(1,#self.day.items)]
	end
end

----------------------------------------
-- getBattleOpponents()
----------------------------------------
function wild:getBattleOpponents()
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

function wild:printCredits()
	text:splashMessage("Bard's Tale I: Reimplemented by me")
end
	

----------------------------------------
-- enterDungeon()
----------------------------------------
function wild:enterDungeon(inName, inLevel)
	currentLevel.exit = true
	currentLevel.exitLocation = inName
	globals.gameState = globals.STATE_INGAME
	globals.citySquare = currentLevel.currentSquare.label
	globals.cityDirection = currentLevel.direction
	currentLevel = dun:new(inName, inLevel, 0, 0, "north")
end
