btapi.dun	= {}

function btapi.dun.exitToCity()
	currentLevel.exit = true

	currentLevel = city:new("skara")
	currentLevel.direction = globals.cityDirection
	currentLevel.currentSquare = currentLevel:getSq(globals.citySquare)
	globals.gameState = globals.STATE_CITY
end

function btapi.dun.changeLevel(levDelta)
	local newDun

	currentLevel.exit = true
	newDun = dun:new(currentLevel.name, currentLevel.currentLevel + levDelta, 0, 0, "north")
	newDun.currentSquare = newDun:getSq(currentLevel.currentSquare.label)
	newDun.direction = currentLevel.direction
	currentLevel = newDun
end

