----------------------------------------
-- Check if a string is a number 
-- >= low and <= high
----------------------------------------
function key_in_num_range(inkey, low, high)
	local key

	if (low > high) then
		local tmp
		tmp = low
		low = high
		high = low
	end

	key = tonumber(inkey)

	if ((key) and (key >= low) and (key <= high)) then
		return true
	else
		return false
	end
end


function keyboardCommand(inkey)
	local rval

	if (inkey == nil) then
		return false
	end

	if ((inkey > "0") and (inkey < "8")) then
		local c
		c = party:isOccupied(inkey)
		if (c) then
			c:printCharacter()
			currentLevel:resetBigpic()
			text:clear()
		end
	elseif (inkey == "B") then
		party:singSong()
		text:clear()
		return true
	elseif (inkey == "C") then
		party:castSpell()
		text:clear()
		return true
	elseif (inkey == "G") then
		collectgarbage()
		object.dumpAll()
	elseif (inkey == "N") then
		party:reorder()
		text:clear()
	elseif (inkey == "P") then
		battle.partyAttack()
		if (globals.partyDied) then
			return true
		end
		local xxx_might_need_to_reset_bigpic_here = true
		currentLevel:resetBigpic()
	elseif (inkey == "T") then
		globals.isPaused = true

		text:cdprint(true, false, "\n\nPausing ! !")
		text:printContinue()
		getkey()

		text:clear()
		globals.isPaused = false
		return true
	elseif (inkey == "U") then
		text:clear()
		party:useItem()
		text:clear()
		return true
	elseif (inkey == btkeys.LEFT) then
		currentLevel:turnParty("left")
	elseif (inkey == btkeys.RIGHT) then
		currentLevel:turnParty("right")
	elseif (inkey == btkeys.DOWN) then
		currentLevel:turnParty("back")
	elseif (inkey == btkeys.UP) then
		currentLevel:moveForward()
	elseif (inkey == btkeys.RANDOMBATTLE) then
		battle:random()
		return true
	elseif (inkey == "Q") then
		os.exit(0)
	else
		return false
	end

	return true
end

function string.toLetterOption(inValue, inBase)
	return string.char(inValue + string.byte(inBase))
end

function string.pluralize(inCount, inSingular, inPlural)
	if (inCount == 1) then
		return inSingular
	else
		return inPlural
	end
end

