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
-- level:toCity()
----------------------------------------
function level:toCity(inCity, inX, inY, inDirection)
	if (not inDirection) then
		inDirection = self.direction
	end

	self.exit = true

	if (inCity == "Wild") then
		currentLevel = wild:new()
	else
		currentLevel = city:new(inCity)
	end

	currentLevel:enter(inX, inY, inDirection)
end

----------------------------------------
-- level:toDungeon()
----------------------------------------
function level:toDungeon(inName, inLevel)
	self.exit = true

	if (self:isCity()) then
		globals.citySquare = self.currentSquare.label
		globals.cityDirection = currentLevel.direction
	end

	currentLevel = dun:new(inName, inLevel, 0, 0, "north")
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
		repeat
			monType = random:xdy(1,16)
		until (not picked[monType])
		picked[monType] = true
		monGroups[i] = monP[monType]
	end

	return monGroups
end
