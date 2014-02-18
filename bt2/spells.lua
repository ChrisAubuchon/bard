require "bttable"
require "btdebug"

local spellList = {}

spells = {}
spells.passive = {}


----------------------------------------
-- Constant effect spells
----------------------------------------
spells.passive.light = function(inAction)
	party.light:activate(inAction.inData)
	text:printEllipsis()
end

spells.passive.levitate = function(inAction)
	party.levitate:activate(inAction.inData)
	text:printEllipsis()
end

spells.passive.detect = function(inAction)
	party.detect:activate(inAction.inData)
	text:printEllipsis()
end

spells.passive.shield = function(inAction)
	party.shield:activate(inAction.inData)
	text:printEllipsis()
end

spells.passive.compass = function(inAction)
	party.compass:activate(inAction.inData)
	party.compass:update(currentLevel.direction)
	text:printEllipsis()
end

----------------------------------------
-- spells.trapZap()
----------------------------------------
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

----------------------------------------
-- spells.scrySite()
----------------------------------------
spells.scrySite = function()
	local outString = "You face "

	text:cdprint(true, false, "You face %s", currentLevel.direction)

	if (currentLevel.name == "wild") then
		local x
		local y

		x,y = currentLevel.currentSquare.toCoordinates()
		text:csplash(false, true, 
			", %d square%s north\n %d square%s east of the sages hut.",
			y,
			string.pluralize(y, "", "s"),
			x,
			string.pluralize(x, "", "s")
			)
			
	elseif (currentLevel:isCity()) then
		text:csplash(false, true, " and are in %s.", 
			currentLevel.title)
	else
		local x
		local y

		x,y = currentLevel:getCoordinates()

		text:csplash(false, true,
			", and are %d level%s %s,\n%d square%s north\n" ..
			"%d square%s east of the entry stairs",
			currentLevel.dungeonLevel,
			string.pluralize(currentLevel.dungeonLevel, "", "s"),
			currentLevel.dungeonDirection,
			y,
			string.pluralize(y, "", "s"),
			x,
			string.pluralize(x, "", "s")
			)
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

----------------------------------------
-- spells.summon()
----------------------------------------
function spells.summon(inAction)
	local source	= inAction.source
	local inData	= inAction.inData

	if (source:isCharacter()) then
		party:doSummon(inData)
	elseif (source:isSummon()) then
		local xxx_add_party_monster_summon_spell = true

		text:printEllipsis()
		return
	else
		local xxx_fix_monParty_doSummon = true

		currentBattle.monParty:doSummon(inData)
	end
end

----------------------------------------
-- __doHeal()
----------------------------------------
local function __doHeal(inAction, inTarget)
	local inData	= inAction.inData

	if (inData.fullHeal) then
		inTarget.currentHp = inTarget.maxHp
	else
		local hpHealed = 0

		if (inData.randomHeal) then
			assert(inData.ndice ~= 0)
			hpHealed = random:xdy(inData.ndice, inData.dieval)
		else
			hpHealed = inData.ndice
		end

		inTarget.currentHp = inTarget.currentHp + hpHealed
		if (inTarget.currentHp > inTarget.maxHp) then
			inTarget.currentHp = inTarget.maxHp
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

		inTarget.currentHp = 1
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

----------------------------------------
-- spells.heal
----------------------------------------
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

function spells.mageStar(inAction, inSource)
	log:print(log.LOG_DEBUG, "spells.mageStar")

	if (inAction:groupSavingThrow()) then
		text:ctdprint(false, true, " but it had no effect!\n\n")
		party:display()
		return
	end
	inSource:mageStar(inAction)
	text:printEllipsis()
end


----------------------------------------
-- spells.attack()
----------------------------------------
function spells.attack(inAction)
	local source	= inAction.source

	log:print(log.LOG_DEBUG, "spells.attack()")
	source:attackSpell(inAction)
end

----------------------------------------
-- spells:battleBonus()
----------------------------------------
function spells:battleBonus(inAction)
	local source	= inAction.source

	if (source:battleBonus(inAction)) then
		text:printEllipsis()
	end
end

function spells.disbelieve(inAction, inSource)
	-- disbelieve is kind of broken in the DOS version of BT1
	--
	if (inSource:isMonster()) then
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

	spellList = diskio:readTable("spells")
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















