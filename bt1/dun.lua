----------------------------------------
-- Local data
----------------------------------------
local dunData = {}

----------------------------------------
-- Local __index function that returns
-- false instead of nil for undefined
-- members
----------------------------------------
local function __false_mt(t, key)
	local val

	val = rawget(t, key)
	if (val == nil) then
		return false
	end

	return val
end

local function __addPortal(sq, quad, tileSet)
	if (sq.hasCeilPortal) then
		bigpic:dunAdd(quad, tileSet, "portal", sq"ceil")
	end

	if (sq.hasFloorPortal) then
		bigpic:dunAdd(quad, tileset, "portal", sq"floor")
	end
end

local sideLimit = {
	[1] = 2,
	[2] = 2,
	[3] = 3,
	[4] = 4,
	[5] = 3
}

local sideNames = {
	leftside = {
		[1] = "M",
		[2] = "L",
		[3] = "FL",
		[4] = "WL"
	},
	rightside = {
		[1] = "M",
		[2] = "R",
		[3] = "FR",
		[4] = "WR"
	}
}

local dunViewMiddle = {
	[1]	= "1-M",
	[2]	= "2-M",
	[3]	= "3-M",
	[4]	= "4-M",
	[5]	= "5-M"
}

local function __buildSide(sq, tileSet, pathDir, wallDir, depth, 
				sideDepth, face)
	local quad = tostring(depth) .. "-" .. sideNames[face][sideDepth]

	if ((depth > 2) and (sideDepth < 3)) then
		__addPortal(sq, quad, tileSet)
	end
	

	if ((sq[wallDir].gfx) and (sideDepth ~= 1)) then
		bigpic:dunAdd(quad, tileSet, "front", sq[wallDir])
	end
	if (sideDepth < sideLimit[depth]) then
		if (sq[pathDir].gfx) then
			bigpic:dunAdd(quad, tileSet, face, sq[pathDir])
		else
			__buildSide(sq[pathDir].path, tileSet, pathDir, wallDir,
				depth, sideDepth + 1, face)
		end
	end
end

----------------------------------------
-- Local buildView function
----------------------------------------
local function __buildView(sq, dir, tileSet, maxDepth, depth)
	local frontDir	= directions.front[dir]
	local rightDir	= directions.right[dir]
	local leftDir	= directions.left[dir]
	local sideDepth = 1
	local tmpSq
	local recurse = true

	if (depth > maxDepth) then
		return
	end

	if ((sq[frontDir].gfx) and not (sq.isPhased)) then
		bigpic:dunAdd(dunViewMiddle[depth], tileSet, "front", 
				sq[frontDir])
		recurse = false
	else
		__buildView(sq[frontDir].path, dir, tileSet, maxDepth, depth+1)
	end

	__buildSide(sq, tileSet, leftDir, frontDir, depth, sideDepth, "leftside")
	__buildSide(sq, tileSet, rightDir, frontDir, depth, sideDepth, "rightside")

end

----------------------------------------
-- Local class for dungeon edges
----------------------------------------
local dunEdge	= {}
dunEdge.new = function (inSquare, inEdge, inSquareList)
	local self = {
		path	= false
		}

	local function __init()
		local attr
		local value

		self.path = inSquareList[inEdge[inSquare].path]
		for attr, value in pairs(inEdge[inSquare]) do
			if (attr ~= "path") then
				self[attr] = value
			end
		end
		setmetatable(self, { __index = __false_mt })
	end

	__init()

	return self
end

----------------------------------------
-- Local class for dungeon squares
----------------------------------------
local dunSq	= {}
dunSq.new = function(inLabel, inSquare)
	local self = {
		label		= inLabel,
		north		= false,
		south		= false,
		east		= false,
		west		= false,
	}

	local function __init()
		local attr
		local value

		for attr, value in pairs(inSquare) do
			self[attr] = value
		end

		setmetatable(self, { __index = __false_mt })
	end

	function self.toCoordinates()
		local x
		local y

		x = tonumber(string.sub(self.label, 2, 3), 16)
		y = tonumber(string.sub(self.label, 4, 5), 16)

		return x,y
	end

	__init()

	return self
end

dun = {}
function dun:new(inName, inLevel, inX, inY, inDirection)
	local self = {
		currentLevel	= inLevel,
		isPhasedFlag	= false,
		squareFlags	= btDefaultTable:new(false),
		squares		= {},
		edges		= {}
	}

	btTable.addParent(self, dun, level)
	btTable.setClassMetatable(self)

	self.name		= inName
	self.direction		= inDirection

	if (type(dunData[inName][inLevel].level) == "string") then
		dunData[inName][inLevel].level = read_table(
				dunData[inName][inLevel].level, false
				)
	end

	local dunLevel = dunData[inName][inLevel].level

	local function __initSquares()
		local label
		local object

		for label, object in pairs(dunLevel.squares) do
			self.squares[label] = dunSq.new(label, object)
		end
	end

	local function __initEdges()
		local label
		local object

		for label, object in pairs(self.squares) do
			object.north = dunEdge.new(label,
				dunLevel.edges[object.north], self.squares)
			object.south = dunEdge.new(label,
				dunLevel.edges[object.south], self.squares)
			object.east = dunEdge.new(label,
				dunLevel.edges[object.east], self.squares)
			object.west = dunEdge.new(label,
				dunLevel.edges[object.west], self.squares)
		end
	end

	local function __initVars()
		local label
		local object

		for label, object in pairs(dunLevel) do
			if ((label ~= "squares") and (label ~= "edges")) then
				self[label] = object
			end
		end
	end

	__initSquares()
	__initEdges()
	__initVars()

	self.currentSquare = string.format("x%02d%02d", inX, inY)
	self.currentSquare = self.squares[self.currentSquare]

	return self
end

function dun:isDungeon()
	return true
end

function dun:getSq(label)
	return self.squares[label]
end

function dun:resetBigpic()
	self:buildView()
	self:setTitle()
end

function dun:setTitle()
	bigpic:setTitle(self.title)
end

function dun:getCoordinates()
	return self.currentSquare.toCoordinates()
end

function dun:getNumLevels()
	return #dunData[self.name]
end

function dun:canTeleportTo(inLevel)
	return dunData[self.name][inLevel].canTeleportTo
end

local function doDetect()
	local found = {}
	local i
	local sq
	
	sq = self.currentSquare[self.direction].path
	for i = 1,3 do
		if (sq.isStairs) then	found.stairs  = true end
		if (sq.isTrap) then	found.traps   = true end
		if (sq.isSpecial) then	found.special = true end
		sq = sq[self.direction].path
	end

	if (next(found) ~= nil) then
		text:print("\n")
		if ((found.stairs) and (party.detect.stairs)) then
			text:print("\nThere are stairs near.\n")
		end
		if ((found.traps) and (party.detect.traps)) then
			text:print("\nThere is a trap near.\n")
		end
		if ((found.special) and (party.detect.special)) then
			text:print("\nThere is something special near.\n")
		end
		text:print("\n")
	end
end

function dun:buildView()
	bigpic:dunBackground(self.tileSet)
	bigpic:dunDisplay()
	if (party.light.active) then
		__buildView(self.currentSquare, self.direction, 
			self.tileSet, party.light.distance, 1)
	end
	bigpic:dunDisplay()

	if (party.detect.active) then
		doDetect()
	end
end

function dun:turnParty(inRelDirection)
	self.direction = directions[inRelDirection][self.direction]
	self.isPhasedFlag = false
	text:clear()
	if (self.currentSquare.isSpinner) then
		local r = rnd_xdy(1, 4)
		if (r == 1) then
			self.direction = "north"
		elseif (r == 2) then
			self.direction = "south"
		elseif (r == 3) then	
			self.direction = "east"
		else
			self.direction = "west"
		end
	else
		party.compass:update(self.direction)
	end

	if (self.currentSquare.isPhased) then
		self.currentSquare.isPhased = nil
	end

	if (self.currentSquare.isLifeDrain) then
		self:doLifeDrain()
		party:isLive()
		return
	end

	self:buildView()
end

function dun:moveForward()
	local curSq = self.currentSquare
	local edge = curSq[self.direction]

	text:clear()
	self.squareFlags = btDefaultTable:new(false)
	if (curSq.isStuck) then
		return
	end

	if (globals.swapWallsAndDoors) then
		if (edge.isDoor and not curSq.isPhased) then
			text:print("\n\nOuch!!")
			return
		end
	else
		if ((edge.isWall) and not (curSq.isPhased)) then
			text:print("\n\nOuch!!")
			return
		end
	end

	self.currentSquare.isPhased = nil

	self.currentSquare = edge.path
	self:runSquareCode()
end

function dun:runSquareCode()
	local curSq = self.currentSquare

	self:buildView()

	if (curSq.isRandomBattle) then
		text:cdprint(true, false, "Random battle")
		curSq.isRandomBattle = false
	end
	if (not (self.squareFlags["seenDarkness"]) and curSq.isDarkness) then
		party.light.deactivate()
		text:print("\n\nDarkness!")
		self:buildView()
		self.squareFlags["seenDarkness"] = true
	end
	if (curSq.onEnter) then
		text:cdprint(true, false, "onEnter")
	end
	if (curSq.isAntiMagic) then
		party.levitate.deactivate()
		party.compass.deactivate()
		party.detect.deactivate()
		party.shield.deactivate()
	end
	if (not (self.squareFlags["seenSmoke"]) and curSq.isSmoke) then
		if (party.light.active) then
			party.light.distance = 1
		end
		text:print("\nSmoke in your eyes!")
		self:buildView()
		self.squareFlags["seenSmoke"] = true
	end
	if (curSq.isTeleport) then
		self.currentSquare = self.getSq(curSq.isTeleport)
		text:clear()
		bigpic:setTitle(self.title)
		self:buildView()
		self:runSquareCode()
		return
	end
	if (curSq.isLifeDrain) then
		text:cdprint(true, false, "isLifeDrain")
	end
	if (curSq.isMessage) then
		if not (self.squareFlags["seenMessage"]) then
			text:print(curSq.isMessage)
			
			self.squareFlags.seenMessage = true
		end
	end
	if (curSq.isForcedBattle) then
		text:cdprint(true, false, "isForcedBattle")
	end
	if (curSq.isTrap) then
		text:cdprint(true, false, "isTrap")
	end
	if (curSq.isStairs) then
		if not (self.squareFlags["seenStairs"]) then
			executeString(curSq.isStairs)
		end
	end
end

function dun:main()
	local inkey

	text:clear()
	self:resetBigpic()

	repeat
		globals.doTimeEvents = true
		inkey = getkey()
		globals.doTimeEvents = false

		if (not keyboardCommand(inkey)) then
			if (inkey == "D") then
				if (globals.debug) then	
					btdebug.dunDebug()
				end
			end
		end

		if (globals.partyDied) then
			globals.gameState = globals.STATE_PARTYDIED
			return
		end
	until (self.exit)
end

function dun:doLifeDrain()
	local c
	local action

	for c in party:iterator() do
		action = btAction.new()
		action.outData.damage = self.level
		c:doDamage(action)
	end

	party:display()
end

local function __init()
	dunData = read_table("dungeons", false)
end

__init()
