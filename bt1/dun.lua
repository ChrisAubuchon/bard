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
		bigpic.dun_add(quad, tileSet, "portal", sq"ceil")
	end

	if (sq.hasFloorPortal) then
		bigpic.dun_add(quad, tileset, "portal", sq"floor")
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
		bigpic.dun_add(quad, tileSet, "front", sq[wallDir])
	end
	if (sideDepth < sideLimit[depth]) then
		if (sq[pathDir].gfx) then
			bigpic.dun_add(quad, tileSet, face, sq[pathDir])
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
		bigpic.dun_add(dunViewMiddle[depth], tileSet, "front", 
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

----------------------------------------
-- Dungeon class
----------------------------------------
dun	= {}
dun.new = function(inName, startLevel, startX, startY, startDirection)
	local self		= level.new()
	local squares		= {}
	local edges		= {}

	local function __init()
		local label
		local object
		local dunLevel

		self.name		= inName
		self.currentLevel	= startLevel
		self.direction		= startDirection
		self.isPhasedFlag	= false
		self.squareFlags	= {}
		

		assert(dunData[inName] ~= nil, "Unknown dungeon name: ", 
			tostring(inName))

		if (type(dunData[inName][startLevel].level) == "string") then
			dunData[inName][startLevel].level = read_table(dunData[inName][startLevel].level, false)
		end

		dunLevel = dunData[inName][startLevel].level

		for label, object in pairs(dunLevel.squares) do
			squares[label] = dunSq.new(label, object)
		end

		for label, object in pairs(squares) do
			object.north = dunEdge.new(label, 
				dunLevel.edges[object.north], squares)
			object.south = dunEdge.new(label,
				dunLevel.edges[object.south], squares)
			object.east = dunEdge.new(label,
				dunLevel.edges[object.east], squares)
			object.west = dunEdge.new(label,
				dunLevel.edges[object.west], squares)
		end

		for label, object in pairs(dunLevel) do
			if ((label ~= "squares") and (label ~= "edges")) then
				self[label] = object
			end
		end

		self.currentSquare = string.format("x%02d%02s", startX, startY)
		self.currentSquare = squares[self.currentSquare]
		setmetatable(self.squareFlags, { __index = __false_mt })
	end

	function self.isDungeon()
		return true
	end

	function self.getSq(label)
		return squares[label]
	end

	function self.resetBigpic()
		self.buildView()
		self.setTitle()
	end

	function self.setTitle()
		bigpic.setTitle(self.title)
	end

	function self.getCoordinates()
		return self.currentSquare.toCoordinates()
	end

	function self.getNumLevels()
		return #dunData[self.name]
	end

	function self.canTeleportTo(inLevel)
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
			text_print("\n")
			if ((found.stairs) and (party.detect.stairs)) then
				text_print("\nThere are stairs near.\n")
			end
			if ((found.traps) and (party.detect.traps)) then
				text_print("\nThere is a trap near.\n")
			end
			if ((found.special) and (party.detect.special)) then
				text_print("\nThere is something special near.\n")
			end
			text_print("\n")
		end
	end

	function self.buildView()
		bigpic.dun_background(self.tileSet)
		bigpic.dun_display()
		if (party.light.active) then
			__buildView(self.currentSquare, self.direction, 
				self.tileSet, party.light.distance, 1)
		end
		bigpic.dun_display()

		if (party.detect.active) then
			doDetect()
		end
	end

	function self.turnParty(inRelDirection)
		self.direction = directions[inRelDirection][self.direction]
		self.isPhasedFlag = false
		text_clear()
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
			party.compass.update(self.direction)
		end

		if (self.currentSquare.isPhased) then
			self.currentSquare.isPhased = nil
		end

		if (self.currentSquare.isLifeDrain) then
			local XXX = "doLifeDrain"
		end

		self.buildView()
	end

	--function self.moveToNewSquare(direction)
	function self.moveForward()
		local curSq = self.currentSquare
		local edge = curSq[self.direction]

		text_clear()
		self.squareFlags = {}
		if (curSq.isStuck) then
			return
		end

		if ((edge.isWall) and not (curSq.isPhased)) then
			text_print("\n\nOuch!!")
			return
		end

		self.currentSquare.isPhased = nil

		self.currentSquare = edge.path
		self.runSquareCode()
	end

	function self.runSquareCode()
		local curSq = self.currentSquare

		self.buildView()

		if (curSq.isRandomBattle) then
			text_cdprint(true, false, "Random battle")
			curSq.isRandomBattle = false
		end
		if (not (self.squareFlags["seenDarkness"]) and curSq.isDarkness) then
			party.light.deactivate()
			text_print("\n\nDarkness!")
			self.buildView()
			self.squareFlags["seenDarkness"] = true
		end
		if (curSq.onEnter) then
			text_cdprint(true, false, "onEnter")
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
			text_print("\nSmoke in your eyes!")
			self.buildView()
			self.squareFlags["seenSmoke"] = true
		end
		if (curSq.isTeleport) then
			self.currentSquare = self.getSq(curSq.isTeleport)
			text_clear()
			bigpic.setTitle(self.title)
			self.buildView()
			self.runSquareCode()
			return
		end
		if (curSq.isLifeDrain) then
			text_cdprint(true, false, "isLifeDrain")
		end
		if (curSq.isMessage) then
			if not (self.squareFlags["seenMessage"]) then
				text_print(curSq.isMessage)
				
				self.squareFlags.seenMessage = true
			end
		end
		if (curSq.isForcedBattle) then
			text_cdprint(true, false, "isForcedBattle")
		end
		if (curSq.isTrap) then
			text_cdprint(true, false, "isTrap")
		end
		if (curSq.isStairs) then
			if not (self.squareFlags["seenStairs"]) then
				executeString(curSq.isStairs)
			end
		end
	end

	function self.main()
		local inkey

		text_clear()
		self.resetBigpic()

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
		until (self.exit)
	end

	__init()

	return self
end

local function __init()
	dunData = read_table("dungeons", false)
end

__init()
