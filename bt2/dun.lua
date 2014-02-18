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
		bigpic:dunAdd(quad, tileSet, "portal", sq, "ceil")
	end

	if (sq.hasFloorPortal) then
		log:print(log.LOG_DEBUG, quad)
		bigpic:dunAdd(quad, tileSet, "portal", sq, "floor")
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

	if ((depth >= 2) and (sideDepth < 3)) then
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
-- Local class for dungeon squares
----------------------------------------
local dunSq	= {}
dunSq.new = function(inLabel, inSquare)
	local self = object:new()

	self:addParent(dunSq)

	table.copy(inSquare, self)
	self.label = inLabel

	return self
end

----------------------------------------
-- dunSq:toCoordinates()
----------------------------------------
function dunSq:toCoordinates()
	local x
	local y

	x = tonumber(string.sub(self.label, 2, 3), 16)
	y = tonumber(string.sub(self.label, 4, 5), 16)

	return x,y
end

----------------------------------------
-- dunSq:clearCode()
----------------------------------------
function dunSq:clearCode()
	self.isSpecial = false
	self.code = false
end



dun = {}
function dun:new(inName, inLevel, inX, inY, inDirection)
	local self = object:new()

	self:addSelf(dun)
	self:addParent(level)
	self:addParent(dunSquares)

	self.currentLevel	= inLevel
	self.isPhasedFlag	= false
	self.squareFlags	= btDefaultTable:new(false)
	self.squares		= {}

	self.name		= inName
	self.direction		= inDirection

	if (type(dunData[inName][inLevel].level) == "string") then
		dunData[inName][inLevel].level = diskio:readTable(
				dunData[inName][inLevel].level, false
				)
	end

	self:fromTable(dunData[inName][inLevel].level)

	self.currentSquare = string.format("x%02x%02x", inX, inY)
	self.currentSquare = self.squares[self.currentSquare]

	return self
end

----------------------------------------
-- dun:fromTable()
----------------------------------------
function dun:fromTable(inTable)
	local label, object

	for label, object in pairs(inTable.squares) do
		self.squares[label] = dunSq.new(label, object)
	end

	for label, object in pairs(self.squares) do
		object.north.path = self.squares[object.north.path]
		object.south.path = self.squares[object.south.path]
		object.east.path = self.squares[object.east.path]
		object.west.path = self.squares[object.west.path]
	end

	for label, object in pairs(inTable) do
		if (label ~= "squares") then
			if (type(object) == "table") then
				self[label] = table.copy(object)
			else
				self[label] = object
			end
		end
	end
end

----------------------------------------
-- dun:saveState()
----------------------------------------
function dun:saveState(inTable)
	local t		= inTable or {}
	local label
	local square
	local attribute
	local value

	t.class		= "dun"
	t.name		= self.name
	t.currentLevel	= self.currentLevel
	t.direction	= self.direction
	t.currentSquare	= self.currentSquare.label

	local function writePath(inDest, inPath)
		local key
		local value

		for key,value in pairs(inPath) do
			if (key == "path") then
				inDest.path = inPath.path.label
			elseif (type(value) ~= "function") then
				inDest[key] = value
			end
		end

	end

	for label,square in pairs(self.squares) do
		t[label] = {}

		for attribute,value in square:pairs() do
			if ((attribute == "north") or (attribute == "south") or
			    (attribute == "east" ) or (attribute == "west"))then
				t[label][attribute] = {}
				writePath(t[label][attribute], 
					square[attribute])
			else
				t[label][attribute] = value
			end
		end
	end

	return t
end

----------------------------------------
-- dun:restoreState()
----------------------------------------
function dun:restoreState(inTable)
	local t

	
end

----------------------------------------
-- dun:isOutdoors()
----------------------------------------
function dun:isOutdoors()
	return false
end

function dun:isDungeon()
	return true
end

function dun:getSqXY(inX, inY)
	return self.squares[string.format("x%02x%02x", inX, inY)]
end

function dun:getSq(label)
	if (self == nil) or (self.squares == nil) then
		error("bad getSq call", 2)
	end
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
	return self.currentSquare:toCoordinates()
end

function dun:getNumLevels()
	return #dunData[self.name]
end

function dun:canTeleportTo(inLevel)
	return dunData[self.name][inLevel].canTeleportTo
end

----------------------------------------
-- exitToCity()
----------------------------------------
function dun:doExit()
	self.exit = true

	if (globals.guildCity == "Wild") then
		local xxx_implement_wild
		currentLevel = wild:new()
	else
		currentLevel = city:new(globals.guildCity)
	end
	currentLevel.direction = globals.cityDirection
	currentLevel.currentSquare = currentLevel:getSq(globals.citySquare)
end

----------------------------------------
-- changeLevel()
----------------------------------------
function dun:changeLevel(inLevelDelta)
	local newDun

	self.exit = true
	newDun = dun:new(currentLevel.name, 
			currentLevel.currentLevel + inLevelDelta,
			0, 0, "north"
			)
	newDun.currentSquare = newDun:getSq(currentLevel.currentSquare.label)
	newDun.direction = currentLevel.direction
	currentLevel = newDun
end

function dun:doDetect()
	local found = {}
	local i
	local sq
	
	sq = self.currentSquare[self.direction].path
	for i = 1,3 do
		if (sq.isStairs) then	found.stairs  = true end
		if (sq.isTrap) then	found.traps   = true end
		if (sq.isSpecial) then	found.special = true end
		if (sq.isSpinner) then	found.spinner = true end
		if (sq.isAntiMagic) then
			found.antimagic = true
		end
		if (sq.isLifeDrain) or (sq.isSpptDrain) then
			found.something = true
		end
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
		if (party.detect.special) then
			if (found.special) then
				text:print("\nThere is something special near.\n")
			end
			if (found.spinner) then
				text:print("\nA spinner is near.\n")
			end
			if (found.antimagic) then
				text:print("\nYou spells waver.\n")
			end
			if (found.something) then
				text:print("\nSomething ahead.\n")
			end
		end
		text:print("\n")
	end
end

function dun:buildView()
	bigpic:dunBackground(self.tileSet)
	if (party.light.active) then
		__buildView(self.currentSquare, self.direction, 
			self.tileSet, party.light.distance, 1)
	end
	bigpic:dunDisplay()

	if (party.detect.active) then
		self:doDetect()
	end
end

function dun:doSpinnerCheck()
	if (self.currentSquare.isSpinner) then
		text:cdprint(true, false, "isSpinner")
		local r = random:xdy(1,4)

		if (r == 1) then
			self.direction = "north"
		elseif (r == 2) then
			self.direction = "south"
		elseif (r == 3) then
			self.direction = "east"
		else
			self.direction = "west"
		end
		self:buildView()
	else
		party.compass:update(self.direction)
	end
end

function dun:turnParty(inRelDirection)
	self.direction = directions[inRelDirection][self.direction]
	self.isPhasedFlag = false
	text:clear()
	self:doSpinnerCheck()

	if (self.currentSquare.isPhased) then
		self.currentSquare.isPhased = nil
	end

	if (self.currentSquare.isLifeDrain) then
		self:doLifeDrain()
		if (globals.partyDied) then
			return
		end
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

	self.previousSquare = self.currentSquare
	self.currentSquare = edge.path
	self:runSquareCode()
	self:doSpinnerCheck()
end

local trapSave = {}
function trapSave:calculateSavingThrow()
	return random:betweenInclusive(self.saveLo, self.saveHi)
end

----------------------------------------
-- doFloorTrap()
----------------------------------------
function dun:doFloorTrap()
	local trap
	local action

	trap = self.floorTraps[random:xdy(1,#self.floorTraps)]
	btTable.addParent(trap, trapSave)
	btTable.setClassMetatable(trap)

	text:cdprint(false, true, trap.effectString)

	local function doTrapAttack(inTrap, inAction)
		local save
		local half

		inAction.source = inTrap
		inAction.outData.damage = random:xdy(inTrap.ndice, inTrap.dieval)
		inAction.inData.specialAttack = inTrap.specialAttack

		save, half = inAction:savingThrow()
		if (not save) then
			if (half) then
				inAction.outData.damage = bit32.rshift(inAction.outData.damage)
			end
			inAction.target:doDamage(inAction)
		end
	end


	if (trap.isPartyAttack) then
		local c

		for c in party:iterator() do
			action = btAction:new()
			action.target = c
			doTrapAttack(trap, action)
			if (globals.partyDied) then	
				return	
			end
		end
	else
		action = btAction:new()

		action.target = party:randomMeleeCharacter()
		doTrapAttack(trap, action)
		if (globals.partyDied) then
			return	
		end
	end

	party:display()
	text:clear()
end

----------------------------------------
-- runSquareCode()
----------------------------------------
function dun:runSquareCode()
	local curSq = self.currentSquare

	self:buildView()

	if (curSq.isRandomBattle) then
		text:cdprint(true, false, "Random battle")
		battle:random()
		if (globals.partyDied) then
			return
		end
		curSq.isRandomBattle = false
		text:clear()
		self:setTitle()
		self:buildView()
		
	end
	if (not (self.squareFlags["seenDarkness"]) and curSq.isDarkness) then
		party.light:deactivate()
		text:print("\n\nDarkness!")
		self:buildView()
		self.squareFlags["seenDarkness"] = true
	end
	if (curSq.code) then
		executeString(curSq.code)
		if (globals.partyDied) then
			return
		end
		text:clear()
		self:setTitle()
		self:buildView()
	end
	if (curSq.isAntiMagic) then
		party.levitate:deactivate()
		party.compass:deactivate()
		party.detect:deactivate()
		party.shield:deactivate()
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
		self.currentSquare = self:getSq(curSq.isTeleport)
		text:clear()
		bigpic:setTitle(self.title)
		self:buildView()
		self:runSquareCode()
		return
	end
	if (curSq.isLifeDrain) then
		text:cdprint(true, false, "isLifeDrain")
		self:doLifeDrain()
		if (globals.partyDied) then
			return	
		end
	end
	if (curSq.isMessage) then
		if not (self.squareFlags["seenMessage"]) then
			text:print(curSq.isMessage)
			
			self.squareFlags.seenMessage = true
		end
	end
	if (curSq.isForcedBattle) then
		executeString(curSq.isForcedBattle)
		if (globals.partyDied) then
			return
		end

		curSq.isForcedBattle = false
		text:clear()
		self:setTitle()
		self:buildView()
	end
	if (curSq.isTrap) then
		if (not (party.levitate.active and (random:band(3) ~= 0))) then
			text:cdprint(true, false, "TRAP! You've hit a ")
		
			self:doFloorTrap()
			if (globals.partyDied) then
				return
			end

			text:clear()
			self:setTitle()
			self:buildView()
			curSq.isTrap = false
		end
	end
	if (curSq.isStairs) then
		if not (self.squareFlags["seenStairs"]) then
			executeString(curSq.isStairs)
			self.squareFlags.seenStairs = true
		end
	end
end

----------------------------------------
-- main()
----------------------------------------
function dun:main()
	local inkey

	text:clear()
	self:resetBigpic()

	repeat
		globals.doTimeEvents = true
		inkey = getkey()
		globals.doTimeEvents = false

		if (keyboardCommand(inkey)) then
			--self:resetBigpic()
			local xxx_doNothing = true
		elseif (inkey == "A") then
			if (self.currentSquare.hasCeilPortal) then
				if (party.levitate.active) then
					self:doPortal(-1, 1)
				end
			end
		elseif (inkey == "D") then
			if (self.currentSquare.hasFloorPortal) then
				if (not party.levitate.active) then
					self:doLifeDrain()
					if (globals.partyDied) then
						break
					end
				end
				self:doPortal(1, -1)
			end
		elseif (inkey == btkeys.WANDERING) then
			self:wanderingMonster()
			self:resetBigpic()
		elseif (inkey == "Z") then
			if (globals.debug) then	
				btdebug.dunDebug()
				self:resetBigpic()
			end
		end

		if (globals.partyDied) then
			globals.gameState = globals.STATE_PARTYDIED
			return
		end
	until (self.exit)
end

----------------------------------------
-- doPortal()
----------------------------------------
function dun:doPortal(inDeltaUp, inDeltaDown)
	if (self.dungeonDirection == "below") then
		self:changeLevel(inDeltaUp)
	else
		self:changeLevel(inDeltaDown)
	end
end

----------------------------------------
-- doLifeDrain()
----------------------------------------
function dun:doLifeDrain()
	local c
	local action

	for c in party:iterator() do
		action = btAction.new()
		action.outData.damage = self.level
		c:doDamage(action)
		if (globals.partyDied) then
			return	
		end
	end

	party:display()
end

----------------------------------------
-- wanderingMonster()
----------------------------------------
function dun:wanderingMonster()
	local m
	local mtype
	local inkey

	mtype = self.monsters[random:xdy(1,#self.monsters)]
	m = monster:new(mtype)

	bigpic:setBigpic(m.picture, m.singular)
	text:cdprint(true, false, "A wandering creature offers to join your party. You can:\n\nAllow it to join\nFight it")
	text:printExit()

	repeat
		inkey = getkey()
		if (inkey == "A") then
			local s = {}
			s.type = mtype
			s.isIllusion = false
			party:doSummon(s)
			text:clear()
			return
		elseif (inkey == "F") then
			battle:new(mtype, 1)
			return
		end
	until (inkey == "E")

	text:clear()
end

----------------------------------------
-- getPoisonDamage()
----------------------------------------
function dun:getPoisonDamage()
	return self.poisonDamage
end

----------------------------------------
-- getBattleReward()
----------------------------------------
function dun:getBattleReward()
	return self.items[random:xdy(1,#self.items)]
end

----------------------------------------
-- doTreasureChest()
----------------------------------------
function dun:doTreasureChest()
	return chest:new(self.chestTraps[random:xdy(1,4)])
end

local function __init()
	dunData = diskio:readTable("dungeons", false)
end

__init()
