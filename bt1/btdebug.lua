btdebug = {}

local __debug = true
local __debugLog = "output.log"
--local __debugFile = io.open(__debugLog, "w+")
local __debugFile = io.stdout

function dprint(format, ...)
	if (__debug) then
		local dstring
		local dinfo

		dinfo = debug.getinfo(2, "Sl")
		dstring = "DEBUG: " .. string.match(dinfo.short_src, "[^/]*$")
			.. ":" .. tostring(dinfo.currentline) .. " "

		if (select("#", ...) == 0) then
			__debugFile:write(dstring .. tostring(format).."\n")
			--print(dstring .. tostring(format))
		else
			__debugFile:write(dstring .. string.format(format,...).."\n")
			--print(dstring .. string.format(format, ...))
		end
		__debugFile:flush()
	end
end

function btdebug.enterDungeon()
	local inkey
	local level

	repeat
		text:cdprint(true, false, "1. Sewers\n")
		text:print("2. Catacombs\n")
		text:print("3. Castle\n")
		text:print("4. Tower\n")
		text:print("5. Mangar\n")
		inkey = getkey()

		if (inkey == "1") then
			text:cdprint(true, false, "Enter level [1-4]:\n")
			level = getkey()
			if ((level > "0") and (level < "5")) then
				city:enterDungeon("cellars", tonumber(level))
				return true
			end 
		elseif (inkey == "2") then
			text:cdprint(true, false, "Enter level [1-3]:\n")
			level = getkey()
			if ((level > "0") and (level < "4")) then
				city:enterDungeon("catacombs", tonumber(level))
				return true
			end 
		elseif (inkey == "3") then
			text:cdprint(true, false, "Enter level [1-3]:\n")
			level = getkey()
			if ((level > "0") and (level < "4")) then
				city:enterDungeon("castle", tonumber(level))
				return true
			end 
		elseif (inkey == "4") then
			city:enterDungeon("kylr", 1)
			return true
		elseif (inkey == "5") then
			text:cdprint(true, false, "Enter level [1-5]:\n")
			level = getkey()
			if ((level > "0") and (level < "6")) then
				city:enterDungeon("mangar", tonumber(level))
				return true
			end 
		end
	until (inkey == "E")

	return false
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
	if (not currentLevel:isDungeon()) then
		return
	end

	if (currentLevel.currentSquare[inSqFlag]) then
		currentLevel.currentSquare[inSqFlag] = false
	else
		currentLevel.currentSquare[inSqFlag] = true
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
		text:printExit()
		inkey = getkey()

		if (inkey == "1") then
			btdebug.dunSqFlags()
		elseif (inkey == "2") then
			btdebug.moveToSquare()
			return
		elseif (inkey == "3") then
			battle:random()
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
