require "bttable"
level = {
	exit		= false,
	name		= false,
	title		= false,
	currentSquare	= false,
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





