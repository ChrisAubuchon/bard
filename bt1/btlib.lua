
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

	if ((inkey > "0") and (inkey < "7")) then
		inkey = tonumber(inkey) 
		if (party:isOccupied(inkey)) then
			party[inkey]:printCharacter()
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
	elseif (inkey == "N") then
		party:reorder()
		text:clear()
	elseif (inkey == "P") then
		battle.partyAttack()
		if (globals.partyDied) then
			return true
		end
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
	elseif (inkey == btkeys.BTKEY_LEFT) then
		currentLevel:turnParty("left")
	elseif (inkey == btkeys.BTKEY_RIGHT) then
		currentLevel:turnParty("right")
	elseif (inkey == btkeys.BTKEY_DOWN) then
		currentLevel:turnParty("back")
	elseif (inkey == btkeys.BTKEY_UP) then
		currentLevel:moveForward()
	elseif (inkey == "Q") then
		os.exit()
	else
		return false
	end

	return true
end

function getActionTarget(inTargetOptions, inTargets)
	local inkey
	local i
	local optionKeys = btTable.new()
	dprint("getActionTarget() called")

	if (inTargetOptions.party) then
		for i = 1,party.size do
			optionKeys[tostring(i)] = true
		end

		text:print("\nMember #[1-%d", party.size)
		if ((inTargetOptions.summon) and party.summon) then
			optionKeys["S"] = true
			text:print("S]")
		else
			text:print("]")
		end
	end

	if ((inTargets) and 
            ((inTargetOptions.monster) or (inTargetOptions.melee))) then
		local i
		local monGroup

		if ((not inTargetOptions.party) and (inTargets.size == 1)) then
			dprint("returning lead group")
			return inTargets:getLeadGroup()
		end
		
		text:print("\n")
		for i,mgroup in inTargets:ipairs() do
			if ((i > 2) and (inTargetOptions.melee)) then
				break
			end

			optionKeys[string.char((i-1) + string.byte("A"))] = true
			text:print("[%s] %d %s\n", 
				string.char((i - 1) + string.byte("a")), 
				mgroup.size,
				monster.pluralizeName(mgroup.name)
				)
		end
	end

	inkey = getkey()
	if (not optionKeys[inkey]) then
		return false
	end

	if ((inkey > "0") and (inkey < "7")) then
		return party[tonumber(inkey)]
	elseif (inkey == "S") then
		return party[1]
	elseif (inkey == "A") then
		return inTargets[1]
	elseif (inkey == "B") then
		return inTargets[2]
	elseif (inkey == "C") then
		return inTargets[3]
	elseif (inkey == "D") then
		return inTargets[4]
	end

	return false
end
