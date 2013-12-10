btdebug = {}

local __debug = true
local __debugLog = "output.log"
--local __debugFile = io.open(__debugLog, "w+")
local __debugFile = io.stdout

----------------------------------------
-- specialSquare class
----------------------------------------
local specialSquare = {} 
function specialSquare:new()
	local self = {
		squares	= {}
	}

	btTable.addParent(self, specialSquare)
	btTable.setClassMetatable(self)

	return self
end

function specialSquare:toArray()
	return self.squares
end

function specialSquare:printName(inSquare)
	text:print(inSquare.name)
end

function dwhere(inDepth)
	local dstring
	local dinfo

	dinfo = debug.getinfo(inDepth, "Sl")
	dstring = string.format("%s:%s", 
			string.match(dinfo.short_src, "[^/]*$"),
			dinfo.currentline
			)
	__debugFile:write(dstring .. "\n")
	__debugFile:flush()
end
		

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

local cityList = specialSquare:new()
cityList.squares = {
	{
		name	= "Colosse",
		city	= "Colosse"
	},
	{
		name	= "Corinth",
		city	= "Corinth"
	},
	{
		name	= "Ephesus",
		city	= "Ephesus"
	},
	{
		name	= "Philippi",
		city	= "Philippi"
	},
	{
		name	= "Tangramayne",
		city	= "Tangramayne"
	},
	{
		name	= "Thessalonica",
		city	= "Thessalonica"
	},
	{
		name	= "Dargoth Tower",
		city	= "Philippi",
		x	= 13,
		y	= 3,
		dir	= "south"
	},
	{
		name	= "Dark Domain",
		city	= "Tangramayne",
		x	= 14,
		y	= 8,
		dir	= "east"
	},
	{
		name	= "Maze of Dread",
		city	= "Thessalonica",
		x	= 10,
		y	= 14,
		dir	= "east"
	},
	{
		name	= "Oscon Fortress",
		city	= "Corinth",
		x	= 8,
		y	= 14,
		dir	= "south"
	},
	{
		name	= "Stone",
		city	= "Colosse",
		x	= 1,
		y	= 13,
		dir	= "east"
	},
	{
		name	= "Temple of Dark",
		city	= "Ephesus",
		x	= 8,
		y	= 6,
		dir	= "north"
	}
}

local wildList = specialSquare:new()
wildList.squares = {
	{
		name	= "Fanskar",
		city	= "Wild",
		x	= 16,
		y	= 26,
		dir	= "east"
	},
	{
		name	= "Grey Crypt",
		city	= "Wild",
		x	= 8,
		y	= 32,
		dir	= "south"
	},
	{
		name	= "Kazdek",
		city	= "Wild",
		x	= 25,
		y	= 18,
		dir	= "north"
	},
	{
		name	= "Narn",
		city	= "Wild",
		x	= 10,
		y	= 2,
		dir	= "west"
	},
	{
		name	= "Sage hut",
		city	= "Wild",
		x	= 0,
		y	= 1,
		dir	= "south"
	}
}


function btdebug.toCity(inObject)
	local newCity

	newCity = text:scrollingSelect(inObject, inObject.printName)
	if (not newCity) then
		text:clear()
		return false
	end

	currentLevel:toCity(newCity.city, newCity.x, newCity.y, newCity.dir)
end

function btdebug.cityDebug() 
	local inkey

	repeat
		text:cdprint(true, false, 
			"1. Change City\n" ..
			"2. Enter Wild\n" ..
			"3. Enter Dungeon\n" ..
			"4. Random battle\n" ..
			"5. Toggle night\n"  ..
			"6. Battle\n"
			)
		
		text:printExit()

		inkey = getkey() 

		if (inkey == "1") then
			btdebug.toCity(cityList)
			return
		elseif (inkey == "2") then
			btdebug.toCity(wildList)
			return
		elseif (inkey == "4") then
			battle:random()
		elseif (inkey == "5") then
			if (globals.isNight) then
				globals.isNight = false
			else
				globals.isNight = true
			end
			currentLevel:buildView()
		elseif (inkey == "6") then
			btdebug.doBattle()
		end
if false then
		text:cdprint(true, false, "1. Toggle night\n")
		text:print("2. Enter dungeon\n")
		text:print("3. Random battle\n")
		text:print("4. Enter building\n")
		text:print("5. Battle\n")
		text:print("6. Add doppleganger\n")
		text:print("7. Enter wild\n")
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
		elseif (inkey == "7") then
			text:clear()
			
			return
end
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
		text:print("3. Random Enc\n")
		text:print("4. Special Square\n")
		text:print("5. Do chest\n")
		text:print("6. Wandering Mon\n")
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
		elseif (inkey == "6") then
			currentLevel:wanderingMonster()
			return
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
		if ((inkey == "-") or (inkey == btkeys.LEFT)) then
			newCoords[cursor] = newCoords[cursor] - 1
			if (newCoords[cursor] < 0) then
				newCoords[cursor] = 0
			end
		elseif ((inkey == "+") or (inkey == "=") or
			(inkey == btkeys.RIGHT)) then
			newCoords[cursor] = newCoords[cursor] + 1
			if (newCoords[cursor] > 21) then
				newCoords[cursor] = 21
			end
		elseif (inkey == "N") then
			cursor = 2
		elseif (inkey == "E") then
			cursor = 3
		elseif (inkey == btkeys.DOWN) then
			cursor = cursor + 1
			if (cursor > 3) then
				cursor = 2
			end
		elseif (inkey == btkeys.UP) then
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
