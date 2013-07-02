btapi.city	= {}

----------------------------------------
-- City Functions
----------------------------------------
function btapi.city.emptyBuilding()
	if (rnd_xdy(1,4) == 1) then
		if (not battle.randomEncounter()) then
			if (globals.partyDied) then
				return
			end
		end
	end

	bigpic.setTitle("Empty Building")
	bigpic.drawBigpic("PIC_EMPTYBLDG")

	text_clear()
	text_print("You're in an empty building.")
	printExit()
	getkey()
	currentLevel.turnParty("back")
end

function btapi.city.enterStables()
	bigpic.setTitle("Empty Building")
	bigpic.drawBigpic("PIC_EMPTYBLDG")

	text_clear()
	text_print("\nSorry, friends, all the horses have ")
	text_print("been eaten by creatures!")
	printExit()
	getkey()

	currentLevel.turnParty("back")
end

function btapi.city.cityGates()
	bigpic.setTitle("City Gate")
	bigpic.drawBigpicTimeAware("PIC_CITYGATE")

	text_clear()
	text_print("You stand before the city gates, ")
	text_print("which are blocked by a gigantic snow drift.")
	printContinue()
	getkey()

	currentLevel.turnParty("back")
end

function btapi.city.enterGarths()
	if (globals.isNight) then
		text_clear()
		text_print("\nThe shoppe is closed at night.")
		printContinue()
		getkey()
		text_clear()
		currentLevel.turnParty("back")
		return
	end

	garths.enter()
	text_clear()
	currentLevel.turnParty("back")
end

function btapi.city.enterGuild()
	currentLevel.exit = true
	globals.gameState = globals.STATE_GUILD
	currentLevel = false
end

function btapi.city.enterTavern(tavernName)
	tavern.enter(tavernName)
	text_clear()
	currentLevel.turnParty("back")
end

function btapi.city.enterTemple(templeName)
	temple.enter(templeName)
	currentLevel.turnParty("back")
end

function btapi.city.enterRoscoes()
	roscoes.enter()
	currentLevel.turnParty("back")
end

function btapi.city.enterReview()
	if (globals.isNight) then
		text_cdprint(true, false, "The review Board is closed for the evening.")
		text_cdprint(false, true, " The guild leaders will meet with you in the morning.\n\n")
		currentLevel.turnParty("back")
		return
	end
	review.enter()
	currentLevel.turnParty("back")
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
	currentLevel = dun.new(currentLevel.exitLocation, dunLevel, 0, 0, "north")
end
