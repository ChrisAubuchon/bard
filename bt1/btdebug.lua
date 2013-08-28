btdebug = {}

local __debug = true
local __debugLog = "output.log"
--local __debugFile = io.open(__debugLog, "w+")
local __debugFile = io.stdout

----------------------------------------
-- specialSquare class
----------------------------------------
local specialSquare = {
	squares = {}
}

function specialSquare:toArray()
	return self.squares
end

function specialSquare:printName(inSquare)
	text:print(inSquare.name)
end

specialSquare.squares = {
	{
		name		= "SpiderGod",
		dungeon		= "cellars",
		level		= 2,
		y		= 10,
		x		= 4,
		direction	= "south"
	},
	{
		name		= "LightBeam",
		dungeon		= "cellars",
		level		= 3,
		y		= 5,
		x		= 0,
		direction	= "south"
	},
	{
		name		= "TarjanMouth",
		dungeon		= "cellars",
		level		= 3,
		y		= 3,
		x		= 3,
		direction	= "north"
	},
	{
		name		= "LongStairs",
		dungeon		= "cellars",
		level		= 4,
		y		= 16,
		x		= 18,
		direction	= "west"
	},
	{
		name		= "Bashar",
		dungeon		= "catacombs",
		level		= 2,
		y		= 11,
		x		= 10,
		direction	= "north"
	},
	{
		name		= "Sphynx",
		dungeon		= "catacombs",
		level		= 2,
		y		= 12,
		x		= 6,
		direction	= "north"
	},
	{
		name		= "WitchKing",
		dungeon		= "catacombs",
		level		= 3,
		y		= 18,
		x		= 20,
		direction	= "north"
	},
	{
		name		= "CrystalSword",
		dungeon		= "castle",
		level		= 1,
		y		= 0,
		x		= 20,
		direction	= "west"
	},
	{
		name		= "RobedMen",
		dungeon		= "castle",
		level		= 1,
		y		= 5,
		x		= 16,
		direction	= "north"
	},
	{
		name		= "BaronThrone",
		dungeon		= "castle",
		level		= 1,
		y		= 13,
		x		= 9,
		direction	= "north"
	},
	{
		name		= "GuardCap",
		dungeon		= "castle",
		level		= 1,
		y		= 17,
		x		= 9,
		direction	= "north"
	},
	{
		name		= "SilverSq",
		dungeon		= "castle",
		level		= 2,
		y		= 1,
		x		= 0,
		direction	= "south"
	},
	{
		name		= "ShieldMouth",
		dungeon		= "castle",
		level		= 2,
		y		= 0,
		x		= 18,
		direction	= "east"
	},
	{
		name		= "OldMan",
		dungeon		= "castle",
		level		= 2,
		y		= 9,
		x		= 8,
		direction	= "east"
	},
	{
		name		= "Eye Statue",
		dungeon		= "castle",
		level		= 3,
		y		= 0,
		x		= 21,
		direction	= "north"
	},
	{
		name		= "Tavern Q",
		dungeon		= "castle",
		level		= 3,
		y		= 5,
		x		= 2,
		direction	= "east"
	},
	{
		name		= "Robe Check",
		dungeon		= "castle",
		level		= 3,
		y		= 5,
		x		= 11,
		direction	= "east"
	},
	{
		name		= "Crystal Stat",
		dungeon		= "kylr",
		level		= 1,
		y		= 0,
		x		= 4,
		direction	= "north"
	},
	{
		name		= "Sinister Q",
		dungeon		= "kylr",
		level		= 1,
		y		= 2,
		x		= 13,
		direction	= "west"
	},
	{
		name		= "Stone Golem",
		dungeon		= "kylr",
		level		= 1,
		y		= 10,
		x		= 14,
		direction	= "west"
	},
	{
		name		= "Onyx Key",
		dungeon		= "kylr",
		level		= 1,
		y		= 12,
		x		= 17,
		direction	= "north"
	},
	{
		name		= "Silver Tri",
		dungeon		= "kylr",
		level		= 1,
		y		= 19,
		x		= 2,
		direction	= "north"
	},
	{
		name		= "Mouth Four",
		dungeon		= "mangar",
		level		= 1,
		y		= 13,
		x		= 21,
		direction	= "west"
	},
	{
		name		= "Silver Circle",
		dungeon		= "mangar",
		level		= 2,
		y		= 15,
		x		= 3,
		direction	= "east"
	},
	{
		name		= "Seven Q",
		dungeon		= "mangar",
		level		= 3,
		y		= 5,
		x		= 10,
		direction	= "south"
	},
	{
		name		= "Master Key",
		dungeon		= "mangar",
		level		= 3,
		y		= 12,
		x		= 20,
		direction	= "west"
	},
	{
		name		= "Large Coffin",
		dungeon		= "mangar",
		level		= 4,
		y		= 8,
		x		= 11,
		direction	= "east"
	},
	{
		name		= "Sleeping Dragons",
		dungeon		= "mangar",
		level		= 4,
		y		= 7,
		x		= 21,
		direction	= "north"
	},
	{
		name		= "Thor Figurine",
		dungeon		= "mangar",
		level		= 4,
		y		= 9,
		x		= 21,
		direction	= "west"
	},
	{
		name		= "Swap Doors",
		dungeon		= "mangar",
		level		= 4,
		y		= 20,
		x		= 2,
		direction	= "east"
	},
	{
		name		= "BoilingLiq",
		dungeon		= "mangar",
		level		= 5,
		y		= 10,
		x		= 20,
		direction	= "east"
	},
	{
		name		= "MangarTreas",
		dungeon		= "mangar",
		level		= 5,
		y		= 18,
		x		= 0,
		direction	= "south"
	},
	{
		name		= "SilverDest",
		dungeon		= "mangar",
		level		= 5,
		y		= 14,
		x		= 10,
		direction	= "north"
	},
	{
		name		= "PossessChar",
		dungeon		= "mangar",
		level		= 5,
		y		= 0,
		x		= 20,
		direction	= "north"
	},
	{
		name		= "DeathMouth",
		dungeon		= "mangar",
		level		= 5,
		y		= 0,
		x		= 10,
		direction	= "north"
	},
	{
		name		= "SpectreSnare",
		dungeon		= "mangar",
		level		= 5,
		y		= 21,
		x		= 11,
		direction	= "west"
	},
	{
		name		= "Mangar",
		dungeon		= "mangar",
		level		= 5,
		y		= 19,
		x		= 10,
		direction	= "north"
	}
}



----------------------------------------
-- dprint()
----------------------------------------
function dprint(format, ...)
	if (__debug) then
		local dstring
		local dinfo

		dinfo = debug.getinfo(2, "Sl")
		dstring = "DEBUG: " .. string.match(dinfo.short_src, "[^/]*$")
			.. ":" .. tostring(dinfo.currentline) .. " "

		if (select("#", ...) == 0) then
			__debugFile:write(dstring .. tostring(format).."\n")
		else
			__debugFile:write(dstring .. string.format(format,...).."\n")
		end
		__debugFile:flush()
	end
end

function btdebug.enterDungeon()
	local destination

	destination = text:scrollingSelect(specialSquare, 
						specialSquare.printName)
	if (not destination) then
		return false
	end

	city:enterDungeon(destination.dungeon, destination.level)
	currentLevel.direction = destination.direction
	currentLevel.currentSquare = currentLevel:getSqXY(destination.x, destination.y)

	return true
end

function btdebug.cityDebug() 
	local inkey

	repeat
		text:cdprint(true, false, "1. Toggle night\n")
		text:print("2. Enter dungeon\n")
		text:print("3. Random battle\n")
		text:print("4. Enter building\n")
		text:print("5. Battle\n")
		text:print("6. Add doppleganger\n")
		text:printExit()
		inkey = getkey()

		if (inkey == "1") then
			if (globals.isNight) then
				globals.isNight = false
			else
				globals.isNight = true
			end
			currentLevel:buildView()
		elseif (inkey == "2") then
			if (btdebug.enterDungeon()) then
				return 
			end
		elseif (inkey == "3") then
			battle:random()
		elseif (inkey == "4") then
			btdebug.enterBuilding()
			return
		elseif (inkey == "5") then
			btdebug.doBattle()
		elseif (inkey == "6") then
			text:clear()
			battleMonster:doDoppleganger()
			return
		end

	until (inkey == "E")

	text:clear()
end

function btdebug.enterBuilding()
	local inkey

	repeat
		text:cdprint(true, false, "1. Garth's\n")
		text:print("2. Roscoe's\n")
		text:print("3. Review\n")
		text:printExit()
		inkey = getkey()

		if (inkey == "1") then
			garths:enter()
			return
		elseif (inkey == "2") then
			roscoes:enter()
			return
		elseif (inkey == "3") then
			review:enter()
			return
		end
	until (inkey == "E")
end

function btdebug.toggleSquareFlag(inSqFlag)
	local frontSq

	if (not currentLevel:isDungeon()) then
		return
	end

	frontSq = currentLevel.currentSquare[currentLevel.direction].path
	if (frontSq[inSqFlag]) then
		frontSq[inSqFlag] = false
	else
		frontSq[inSqFlag] = true
	end
end

function btdebug.dunSqFlags()
	local inkey

	repeat
		text:cdprint(true, false, "1. isDarkness\n")
		text:print("2. isSpinner\n")
		text:print("3. isTrap\n")
		text:print("4. isStuck\n")
		text:printExit()

		inkey = getkey()

		if (inkey == "1") then
			btdebug.toggleSquareFlag("isDarkness")
		elseif (inkey == "2") then
			btdebug.toggleSquareFlag("isSpinner")
		elseif (inkey == "3") then
			btdebug.toggleSquareFlag("isTrap")
		elseif (inkey == "4") then
			btdebug.toggleSquareFlag("isStuck")
		end
	until (inkey == "E")
end

function btdebug.dunDebug()
	local inkey

	repeat
		text:cdprint(true, false, "1. Square Flags\n")
		text:print("2. Move to square\n")
		text:print("3. Random Encounter\n")
		text:print("4. Special Square\n")
		text:print("5. Do chest\n")
		text:printExit()
		inkey = getkey()

		if (inkey == "1") then
			btdebug.dunSqFlags()
		elseif (inkey == "2") then
			btdebug.moveToSquare()
			return
		elseif (inkey == "3") then
			battle:random()
			return
		elseif (inkey == "4") then
			if (btdebug.enterDungeon()) then	
				return	
			end
		elseif (inkey == "5") then
			currentLevel:doTreasureChest()
		end
	until (inkey == "E")

	text:clear()
end

function btdebug.moveToSquare()
	local inkey
	local newCoords = {
		[2] = 0,
		[3] = 0
	}
	local cursor = 2

	repeat
		text:cdprint(true, false, "\n\n  North\n  East\n")
		text:setCursor(0, cursor)
		text:print("*")
		text:setCursor(0, 11)
		text:print("Done       Cancel")
		text:setCursor(18 * 12, 2)
		text:print("+")
		text:setCursor(18 * 12, 4)
		text:print("-")
		text:setCursor(108, 2)
		text:print("     ")
		text:print(tostring(newCoords[2]))
		text:setCursor(108, 3)
		text:print("     ")
		text:print(tostring(newCoords[3]))

		inkey = getkey()
		if ((inkey == "-") or (inkey == btkeys.BTKEY_LEFT)) then
			newCoords[cursor] = newCoords[cursor] - 1
			if (newCoords[cursor] < 0) then
				newCoords[cursor] = 0
			end
		elseif ((inkey == "+") or (inkey == "=") or
			(inkey == btkeys.BTKEY_RIGHT)) then
			newCoords[cursor] = newCoords[cursor] + 1
			if (newCoords[cursor] > 21) then
				newCoords[cursor] = 21
			end
		elseif (inkey == "N") then
			cursor = 2
		elseif (inkey == "E") then
			cursor = 3
		elseif (inkey == btkeys.BTKEY_DOWN) then
			cursor = cursor + 1
			if (cursor > 3) then
				cursor = 2
			end
		elseif (inkey == btkeys.BTKEY_UP) then
			cursor = cursor - 1
			if (cursor < 1) then
				cursor = 3
			end
		elseif (inkey == "D") then
			currentLevel.currentSquare = currentLevel:getSq(string.format("x%02x%02x", newCoords[3], newCoords[2]))
			currentLevel:buildView()
			currentLevel:runSquareCode()
			return
		end
	until (inkey == "C")

	text:clear()
end

function btdebug.doBattle()
	local ngroups
	local groups = {}
	local i
	local g
	local count

	text:cdprint(true, false, "Number of groups [1-4]:")
	ngroups = text:readNumber()
	if (not ngroups) then
		return
	end

	local function printItem(_,inItem)
		if (#inItem > 14) then
			text:print(string.sub(inItem,1,14))
		else
			text:print(inItem)
		end
	end

	for i = 1,ngroups do
		g = text:scrollingSelect(monster.typeTable(), printItem)
		if (not g) then return end

		text:cdprint(true, false, "Enter group size:")
		count = text:readNumber()

		table.insert(groups, g)
		table.insert(groups, count)
	end

	battle:new(table.unpack(groups))
end
