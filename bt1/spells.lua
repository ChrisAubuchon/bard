require "dataio"
require "bttable"
require "btdebug"

local spellList = {}

spells = {}
spells.passive = {}

local function getPassiveDuration(inData)
	local duration

	if (inData.duration > 0) then
		return inData.duration + rnd_xdy(1,16)
	else
		return -1
	end
end

spells.passive.light = function(inAction)
	local inData	= inAction.inData
	local duration	= getPassiveDuration(inAction.inData)

	party.light:activate(duration, inData.distance, inData.detectSecret)
end

spells.passive.levitate = function(inAction)
	local duration	= getPassiveDuration(inAction.inData)
	party.levitate:activate(duration)
end

spells.passive.detect = function(inAction)
	local inData	= inAction.inData
	local duration	= getPassiveDuration(inAction.inData)

	party.detect:activate(duration, inData.detectStairs,
			inData.detectTraps,
			inData.detectSpecial)
end

spells.passive.shield = function(inAction)
	local inData	= inAction.inData
	local duration	= getPassiveDuration(inAction.inData)

	party.shield:activate(duration, inData.acBonus)
end

spells.passive.compass = function(inAction)
	local duration	= getPassiveDuration(inAction.inData)

	party.compass:activate(duration)
	party.compass:update(currentLevel.direction)
	text:printEllipsis()
end

spells.trapZap = function()
	if (globals.gameState == globals.STATE_CITY) then
		text:printEllipsis()
		return
	end

	local frontDir = directions.front[currentLevel.direction]
	local sq = currentLevel.currentSquare
	local i

	for i = 1,3 do
		sq = sq[frontDir].path
		if (sq.isTrap) then
			sq.isTrap = false
		end
	end
		
end

spells.scrySite = function()
	local outString = "You face "

	if (currentLevel:isCity()) then
		text:splashMessage(outString .. currentLevel.direction .. " and are in Skara Brae.")
	else
		local east
		local north

		east,north = currentLevel:getCoordinates()

		outString = outString .. currentLevel.direction .. ", and are " 
		outString = outString .. tostring(currentLevel.dungeonLevel)
		if (currentLevel.dungeonLevel == 1) then
			outString = outString .. " level "
		else
			outString = outString .." levels "
		end
		outString = outString..currentLevel.dungeonDirection..",\n"

		outString = outString .. tostring(north)
		if (north == 1) then
			outString = outString .. " square north\n"
		else
			outString = outString .. " squares north\n"
		end
		outString = outString .. tostring(east)
		if (east == 1) then
			outString = outString .. " square "
		else
			outString = outString .. " squares "
		end
		outString = outString .. "east of the entry stairs"
		text:splashMessage(outString)
	end

	text:clear()
end

spells.phaseDoor = function()
	if (currentLevel:isDungeon()) then
		local sq = currentLevel.currentSquare

		if (sq[currentLevel.direction].canPhase) then
			currentLevel.currentSquare.isPhased = true
			currentLevel:buildView()
		end
	end
	text:printEllipsis()
end

local function teleportHelper(inValue, inLine)
	text:setCursor(108, inLine)
	text:print("     ")
	text:setColumn(9)
	if (inValue > 0) then
		text:print("+")
	end
	text:print(tostring(inValue))
end

spells.teleport = function()

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
		if ((inkey == "-") or (inkey == btkeys.BTKEY_LEFT)) then
			deltas[cursorPosition] = deltas[cursorPosition] - 1
			if (deltas[cursorPosition] < -21) then
				deltas[cursorPosition] = -21
			end
		elseif ((inkey == "+") or (inkey == "=") or
			 (inkey == btkeys.BTKEY_RIGHT)) then
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
		elseif (inkey == btkeys.BTKEY_DOWN) then
			cursorPosition = cursorPosition + 1
			if (cursorPosition > 4) then
				cursorPosition = 2
			end
		elseif (inkey == btkeys.BTKEY_UP) then
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

function spells.summon(inAction)
	local inData	= inAction.inData
	local summon	= {}

	summon.isIllusion = inData.isIllusion
	summon.type = inData.summons[rnd_xdy(1,#inData.summons)]

	if (inAction.source:isCharacter()) then
		party:doSummon(summon)
		text:printEllipsis()
	elseif (inAction.source:isSummon()) then
		text:printEllipsis()
		return
	else
		currentBattle.monParty:doSummon(summon)
	end
end

local function __doHeal(inAction, inTarget)
	local inData	= inAction.inData

	if (inData.fullHeal) then
		inTarget.cur_hp = inTarget.max_hp
	else
		local hpHealed = 0

		if (inData.randomHeal) then
			assert(inData.ndice ~= 0)
			hpHealed = rnd_xdy(inData.ndice, inData.dieval)
		end

		inTarget.cur_hp = inTarget.cur_hp + hpHealed
		if (inTarget.cur_hp > inTarget.max_hp) then
			inTarget.cur_hp = inTarget.max_hp
		end
	end

	if ((inData.dispossess) or (inData.resurrect)) then
		if (inData.dispossess) then
			if (inTarget.isPossessed) then
				inTarget.isPossessed = false
			else
				return
			end
		end

		if (inData.resurrect) then
			if (inTarget.isDead) then
				inTarget.isDead = false
			else
				return
			end
		end

		inTarget.cur_hp = 1
		inTarget.possessed = false

		if (currentBattle) then
			currentBattle:removeAction(inTarget)
		end
	end

	if (inData.poison) then inTarget.isPoisoned = false end
	if (inData.insanity) then inTarget.isNuts = false end
	if (inData.paralysis) then inTarget.isParalyzed = false end
	if (inData.old) then inTarget.isOld = false end
	if (inData.stoned) then inTarget.isStoned = false end
end

function spells.heal(inAction)
	local inData = inAction.inData

	local hpHealed = 0

	if (inData.groupHeal) then
		local c
		for c in party:characterIterator() do
			__doHeal(inAction, c)
		end
	else
		__doHeal(inAction, inAction.target)
	end

	text:printEllipsis()

end

----------------------------------------
-- spellBind()
----------------------------------------
function spells.spellBind(inAction)
	local target		= inAction.target
	local last

	if (inAction:savingThrow() 
			or target.size == 0
			or target.isIllusion
		) then

		text:ctdprint(false, true, " but it had no effect!\n\n")
		return
	end

	party:doSummon({ type = target.name })

	-- Remove the last monster from the group
	--
	last = target:getLast()
	target:removeMonster(last)
	currentBattle:removePriority(last)
	if (target.size == 0) then
		currentBattle.monParty:removeMonsterGroup(target)
	end
	text:printEllipsis()
end

function spells.mageStar(inAction)
	dprint("spells.mageStar")

	if (inAction:groupSavingThrow()) then
		text:ctdprint(false, true, " but it had no effect!\n\n")
		party:display()
		return
	end
	inAction.source:mageStar(inAction)
	text:printEllipsis()
end


function spells.attack(inAction)
	dprint("spells.attack()")
	inAction.source:attackSpell(inAction)
end

function spells.battleBonus(inAction)
	if (inAction.source:battleBonus(inAction)) then
		text:printEllipsis()
	end
end

function spells.disbelieve(inAction)
	-- disbelieve is kind of broken in the DOS version of BT1
	--
	if (inAction.source:isMonster()) then
		assert(currentBattle.monParty.disbelieve)
		currentBattle.monParty.disbelieve = true
	else
		party.disbelieve = true
		if (inAction.inData.revealDoppleganger) then
			local c

			for c in party:characterIterator() do
				if (c.isDoppleganger) then
					c.name = "DOPPLEGANGER   "
				end
			end
			party:display()
		end
	end
	text:printEllipsis()
end

function spells.getSpellByAbbreviation(abbr)
	local s

	s = spells[abbr]
	if (type(s) == "table") then
		if (s.isSpell) then
			return s
		end
	end

	return false
end


----------------------------------------
-- __initSpellList
--
-- Read the list of spells and convert
-- the function strings to the actual
-- functions
----------------------------------------
local function __initSpellList()
	local s
	local func
	local w

	spellList = read_table("spells")
	for _,s in pairs(spellList) do
		local inFunc
		local mesg

		s.isSpell = true
		if (s.action) then
			compileAction(s.action)
		end
end

end
__initSpellList()
btTable.addParent(spells, spellList)
btTable.setClassMetatable(spells)
spells.__index = spells















