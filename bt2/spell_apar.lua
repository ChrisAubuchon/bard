local function cityTeleport()
	local inkey
	local i
	local c

	local cities = {
		"Tangramayne",
		"Ephesus",
		"Philippi",
		"Colosse",
		"Corinth",
		"Thessalonica"
	}

	text:clear()

	for i,c in ipairs(cities) do
		text:print("\n%d) %s", i, c)
	end
	text:printExit()

	repeat
		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			currentLevel.exit = true
			globals.guildCity = cities[tonumber(inkey)]
			globals.gameState = globals.STATE_GUILD
			text:clear()
			return
		end
	until (inkey == "E")

	text:clear()
end

local function dungeonTeleport()
	local inkey
	local cursorPosition = 2
	local deltas = {
		[2] = 0,		-- Y
		[3] = 0,		-- X
		[4] = 0			-- Z
	}

	if (not currentLevel:isDungeon()) then
		text:cdprint(false, true, " but it fizzles!\n\n")
		text:clear()
		return
	end

	repeat
		text:cdprint(true, false, "\n\n  North\n  East\n  Up\n")
		text:setCursor(0, cursorPosition)
		text:print("*")
		text:setCursor(0, 11)
		text:print("Done         Cancel")
		text:setCursor(18 * 12, 2)
		text:print("+")
		text:setCursor(18 * 12, 4)
		text:print("-")
		teleportHelper(deltas[2], 2)
		teleportHelper(deltas[3], 3)
		teleportHelper(deltas[4], 4)
	
		inkey = getkey()
		if ((inkey == "-") or (inkey == btkeys.LEFT)) then
			deltas[cursorPosition] = deltas[cursorPosition] - 1
			if (deltas[cursorPosition] < -21) then
				deltas[cursorPosition] = -21
			end
		elseif ((inkey == "+") or (inkey == "=") or
			 (inkey == btkeys.RIGHT)) then
			deltas[cursorPosition] = deltas[cursorPosition] + 1
			if (deltas[cursorPosition] > 21) then
				deltas[cursorPosition] = 21
			end
		elseif (inkey == "N") then
			cursorPosition = 2
		elseif (inkey == "E") then
			cursorPosition = 3
		elseif (inkey == "U") then	
			cursorPosition = 4
		elseif (inkey == btkeys.DOWN) then
			cursorPosition = cursorPosition + 1
			if (cursorPosition > 4) then
				cursorPosition = 2
			end
		elseif (inkey == btkeys.UP) then
			cursorPosition = cursorPosition - 1
			if (cursorPosition < 2) then
				cursorPosition = 4
			end
		elseif (inkey == "D") then
			text:clear()
			if (deltas[4] == 0) then
				-- Inter level teleport
				local x
				local y

				if (not currentLevel.canTeleportFrom) then
					return
				end

				x,y = currentLevel:getCoordinates()
				x = (x + deltas[3]) % 22
				y = (y + deltas[2]) % 22

				currentLevel.currentSquare = currentLevel:getSq(string.format("x%02x%02x", x, y))
				currentLevel.squareFlags = {}
				currentLevel:runSquareCode()
				return
			else
				local x
				local y
				local newLevel = currentLevel.currentLevel
				local numLevels = currentLevel:getNumLevels()

				if (currentLevel.dungeonDirection == "below") then
					newLevel = newLevel - deltas[4]
				else
					newLevel = newLevel + deltas[4]
				end
				newLevel = newLevel - 1

				while (newLevel < 0) do
					newLevel = newLevel + numLevels
				end

				newLevel = (newLevel % numLevels) + 1

				x,y = currentLevel:getCoordinates()
				x = (x + deltas[3]) % 22
				y = (y + deltas[2]) % 22

				if (currentLevel:canTeleportTo(newLevel)) then
					currentLevel.currentLevel = newLevel
					currentLevel.currentSquare = currentLevel:getSq(string.format("x%02x%02x", x, y))
					currentLevel:changeLevel(0)
					currentLevel.clearFlags = true
				end

				return
			end
		end
	until (inkey == "C")

	text:clear()

end

function spells:teleport()
	if (currentLevel:isCity()) then
		cityTeleport()
	else
		dungeonTeleport()
	end
end
