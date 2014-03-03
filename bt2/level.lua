require "bttable"
level = {
	exit		= false,
	name		= false,
	title		= false,
	currentSquare	= false,
	previousSquare	= false,
	direction	= false
}

local function overloadError(inFunction)
	error("Must overload " .. inFunction, 2)
end

function level:main()		overloadError("main()")		end
function level:buildView()	overloadError("buildView()")	end
function level:buildTitle()	overloadError("buildTitle()")	end
function level:turnParty(inDir)	overloadError("turnParty()")	end
function level:moveForward()	overloadError("moveForward()")	end
function level:resetBigpic()	overloadError("resetBigpic()")	end
function level:getSq()		overloadError("getSq()")	end
function level:getBattleReward() overloadError("getBattleReward()")	end
function level:getBattleOpponents() overloadError("getBattleOpponents()") end
function level:getPoisonDamage() overloadError("getPoisonDamage()") end
function level:doLifeDrain()	return true			end


function level:isDungeon()	return false	end
function level:isCity()		return false	end
function level:isOutdoors()	return true	end
function level:setTitle()
	bigpic:setTitle(self.title)
end

function level:doStairs(inDirection)
	text:print("\nThere are stairs here, going %s. ", inDirection)
	text:print("Do you wish to take them?")
	if (text:getYesNo()) then
		return true
	else
		text:clear()
	end
end

----------------------------------------
-- level:toLevel()
----------------------------------------
function level:toLevel(inType, inName, inLevel, inX, inY, inDirection)
	if (currentLevel) then
		currentLevel.exit = true
	end

	log:print(log.LOG_DEBUG, "toLevel(%s, %s, %s, %s, %s, %s) called",
		inType, inName, inLevel, inX, inY, inDirection)
	if (inType == "city") then
		currentLevel = city:new(inName)
	elseif (inType == "wild") then
		currentLevel = wild:new()
	elseif (inType == "dun") then
		currentLevel = dun:new(inName, inLevel)
	else
		error("Invalid level type", 2)
	end

	currentLevel:enter(inX, inY, inDirection)
end

----------------------------------------
-- level:getBattleOpponents()
----------------------------------------
function level:getBattleOpponents()
	local numGroups = 1
	local monGroups	= {}
	local picked	= {}
	local monType
	local i
	local j
	local monP

	if (self:isCity()) then
		monP = self.day.monsters
		if (globals.isNight) then
			numGroups = random:band(1) + 1
		end
	else
		monP = self.monsters
		numGroups = random:xdy(1,4)
	end

	for i = 1,numGroups do
		j = ((i - 1) * 2) + 1
		repeat
			monType = random:xdy(1,16)
		until (not picked[monType])
		picked[monType] = true
		monGroups[j] = monP[monType]
		monGroups[j+1] = false
	end

	return monGroups
end
