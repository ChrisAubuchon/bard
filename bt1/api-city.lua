btapi.city	= {}

----------------------------------------
-- City Functions
----------------------------------------
function btapi.city.emptyBuilding()
	if (rnd_xdy(1,4) == 1) then
		if (not battle:random()) then
			if (globals.partyDied) then
				return
			end
		end
	end

	bigpic:setTitle("Empty Building")
	bigpic:drawImage("PIC_EMPTYBLDG")

	text:clear()
	text:print("You're in an empty building.")
	text:printExit()
	getkey()
	currentLevel:turnParty("back")
end

function btapi.city.enterStables()
	bigpic:setTitle("Empty Building")
	bigpic:drawImage("PIC_EMPTYBLDG")

	text:clear()
	text:print("\nSorry, friends, all the horses have ")
	text:print("been eaten by creatures!")
	text:printExit()
	getkey()

	currentLevel:turnParty("back")
end

function btapi.city.cityGates()
	bigpic:setTitle("City Gate")
	bigpic:drawImage("PIC_CITYGATE", true)

	text:clear()
	text:print("You stand before the city gates, ")
	text:print("which are blocked by a gigantic snow drift.")
	text:printContinue()
	getkey()

	currentLevel:turnParty("back")
end

function btapi.city.enterGarths()
	if (globals.isNight) then
		text:clear()
		text:print("\nThe shoppe is closed at night.")
		text:printContinue()
		getkey()
		text:clear()
		currentLevel:turnParty("back")
		return
	end

	garths:enter()
	text:clear()
	currentLevel:turnParty("back")
end

function btapi.city.enterGuild()
	currentLevel.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false
end

function btapi.city.enterTavern(tavernName)
	tavern.enter(tavernName)
	text:clear()
	currentLevel:turnParty("back")
end

function btapi.city.enterTemple(templeName)
	temple:new(templeName)
	currentLevel:turnParty("back")
end

function btapi.city.enterRoscoes()
	roscoes.enter()
	currentLevel:turnParty("back")
end

function btapi.city.enterReview()
	if (globals.isNight) then
		text:cdprint(true, false, "The review Board is closed for the evening.")
		text:cdprint(false, true, " The guild leaders will meet with you in the morning.\n\n")
		currentLevel:turnParty("back")
		return
	end
	review.enter()
	currentLevel:turnParty("back")
end
		

function btapi.city.ironGate()
	local foo = true
end

function btapi.city.enterMadgod()
	btapi.city.enterDungeon("cellars", 1)
end

function btapi.city.enterDungeon(dunName, dunLevel)
	currentLevel.exit = true
	currentLevel.exitLocation = dunName
	globals.gameState = globals.STATE_DUNGEON
	globals.citySquare = currentLevel.currentSquare.label
	globals.cityDirection = currentLevel.direction
	currentLevel = dun:new(currentLevel.exitLocation, dunLevel, 0, 0, "north")
end
