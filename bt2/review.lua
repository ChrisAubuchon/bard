review = {}

btTable.addParent(review, building:new("Review board", "PIC_REVINT"))
btTable.setClassMetatable(review)

function review:nightCheck()
	if (globals.isNight) then
		text:csplash(true, true, 
			"The review Board is closed for the evening. " ..
			"The guild leaders will meet with you in the " ..
			"morning.\n\n"
			)
		return true
	end

	return false
end

----------------------------------------
-- advanceRandomStat()
----------------------------------------
function review:advanceRandomStat(char)
	local canAdvance = {}
	local rndStat

	if (char.st < 18) then table.insert(canAdvance, "st") end
	if (char.iq < 18) then table.insert(canAdvance, "iq") end
	if (char.dx < 18) then table.insert(canAdvance, "dx") end
	if (char.cn < 18) then table.insert(canAdvance, "cn") end
	if (char.lk < 18) then table.insert(canAdvance, "lk") end

	if (#canAdvance > 0) then
		rndStat = (random:rnd() % #canAdvance) + 1
		char[canAdvance[rndStat]] = char[canAdvance[rndStat]] + 1
		text:print("\n+1 to ")
		if (canAdvance[rndStat] == "st") then text:print("strength.") end
		if (canAdvance[rndStat] == "iq") then text:print("I.Q..") end
		if (canAdvance[rndStat] == "dx") then text:print("dexterity.") end
		if (canAdvance[rndStat] == "cn") then text:print("constitution.") end
		if (canAdvance[rndStat] == "lk") then text:print("luck.") end
	end
end

----------------------------------------
-- doAdvancement()
----------------------------------------
function review:doAdvancement()
	local char
	local xpRequired

	text:cdprint(true, false, "The Guild leaders prepare to weigh thy merits.\n\n")
	text:print("Who shall be reviewed?")

	char = party:readSlot()
	if ((not char) or (char:isSummon())) then
		return
	end
	text:cdprint(true, false, "The guild leaders deem that " .. char.name)
	xpRequired = char:getXpForLevel(char.cur_level + 1)
	if (char.xp < xpRequired) then
		text:cdprint(false, true, 
			" still needeth %d experience points prior to " ..
			"advancement...\n", 
			xpRequired - char.xp
			)
		return
	end

	text:print(" hath earned a level of advancement...")
	char.cur_level = char.cur_level + 1
	char.max_level = char.cur_level

	char.cur_hp = char.cur_hp + (random:rnd() % char.hpDice)
	if (char.cn > 14) then
		char.cur_hp = char.cur_hp + (char.cn - 14)
	end
	char.cur_hp = char.cur_hp + 1
	char.max_hp = char.cur_hp

	if (char.class == "Rogue") then
		char.rogu_level = char.rogu_level + (random:rnd() % 7)
		if (char.dx > 14) then
			char.rogu_level = char.rogu_level + (char.dx - 14)
		end
		char.rogu_level = char.rogu_level + 1
		if (char.rogu_level > 255) then
			char.rogu_level = 255
		end
	elseif (char.class == "Hunter") then
		char.hunt_level = char.hunt_level + (random:rnd() % 31)
		if (char.dx > 14) then
			char.hunt_level = char.hunt_level + (char.dx - 14)
		end
		char.hunt_level = char.hunt_level + 1
		if (char.hunt_level > 255) then
			char.hunt_level = 255
		end
	elseif ((char.class == "Paladin") or (char.class == "Warrior") or
		(char.class == "Monk")) then
		char.num_attacks = math.floor(char.cur_level / 4) + 1
		if (char.num_attacks > 8) then
			char.num_attacks = 8
		end
	elseif (char:getSpellLevel()) then
		char.cur_sppt = char.cur_sppt + (random:rnd() % 3)
		if (char.iq > 14) then
			char.cur_sppt = char.cur_sppt + (char.iq - 14)
		end
		char.cur_sppt = char.cur_sppt + 1
		char.max_sppt = char.cur_sppt
	end
	self:advanceRandomStat(char)
	party:display()
	text:printContinue()
	getkey()
end

----------------------------------------
-- doClassChange()
----------------------------------------
function review:doClassChange()
	local char
	local options = {}
	local numAvailable = 0
	local inkey

	text:cdprint(true, false, "Which mage seeks to change his class?")
	char = party:readSlot()
	if ((not char) or (char:isSummon())) then
		return
	end
	if (not char:getSpellLevel()) then
		text:cdprint(true, true, "\n\nThou are not a spell caster!\n")
		return
	end

	if (char:getSpellLevel() < 3) then
		text:csplash(false, true, 
			"\nThou must know at least 3 spell levels in your " ..
			"present art first."
			)
		return
	end

	dprint(char.spellLevel.Conjurer)
	dprint(char.spellLevel.Magician)
	dprint(char.spellLevel.Sorcerer)
	dprint(char.spellLevel.Wizard)
	dprint(char.spellLevel.Archmage)
	text:cdprint(true, false, char.name .. "\n\n")
	if (char.spellLevel.Conjurer == 0) then 
		text:print("\nConjurer")
		options["C"] = true
		numAvailable = numAvailable + 1
	end
	if (char.spellLevel.Magician == 0) then 
		text:print("\nMagician")
		options["M"] = true
		numAvailable = numAvailable + 1
	end
	if (char.spellLevel.Sorcerer == 0) then 
		text:print("\nSorcerer")
		options["S"] = true
		numAvailable = numAvailable + 1
	end
	if (char.spellLevel.Wizard == 0) then 
		-- Must have two classes to level 3 to be a wizard
		if (numAvailable < 2) then
			text:print("\nWizard")
			options["W"] = true
			numAvailable = numAvailable + 1
		end
	end
	if (char.spellLevel.Archmage == 0) then
		-- Must have three classes to level 3 to be an archmage
		if (numAvailable == 0) then
			text:print("Archmage")
			options["A"] = true
			numAvailable = numAvailable + 1
		end
	end

	if (numAvailable == 0) then
		text:csplash(false, true, 
			"\n\nThou cannot change to an old class!\n"
			)
		return
	end

	repeat
		text:printExit()
		inkey = getkey()
		if (options[inkey] ~= nil) then
			if (inkey == "A") then
				chass.class = "Archmage"
				char.spellLevel.Archmage = 1
			elseif (inkey == "C") then
				char.class = "Conjurer"
				char.spellLevel.Conjurer = 1
			elseif (inkey == "M") then
				char.class = "Magician"
				char.spellLevel.Magician = 1
			elseif (inkey == "S") then
				char.class = "Sorcerer"
				char.spellLevel.Sorcerer = 1
			elseif (inkey == "W") then
				char.class = "Wizard"
				char.spellLevel.Wizard = 1
			else
				error("Invalid class option: "..tostring(inkey))
			end
			char.cur_level = 1
			char.max_level = 1
			char.xp = 0
			party:display()
			text:cdprint(true, true, "\n\nDone!")
			return
		end
	until (inkey == "E")
end

local spellLevelCost = {
	[1] = 1000, 
	[2] = 2000, 
	[3] = 4000, 
	[4] = 7000, 
	[5] = 10000, 
	[6] = 20000,
	[7] = 40000
}

----------------------------------------
-- doSpellAcquire()
----------------------------------------
function review:doSpellAcquire()
	local char
	local spellLevel
	local cost

	text:cdprint(true, false, "Who seeks knowledge of the mystic arts?\n\n")
	char = party:readSlot()
	if ((not char) or (char:isSummon())) then
		return
	end
	spellLevel = char:getSpellLevel()
	text:clear()
	if (not spellLevel) then
		text:cdprint(false, true, "\n\nThou art not a spell caster!\n")
		return
	end

	if (spellLevel == 7) then
		text:csplash(false, true, 
			"\n\nThou art at the highest level of spell ability!"
			)
		return
	end

	if (math.floor((char.cur_level + 1) / 2) <= spellLevel) then
		text:csplash(false, true, 
			"\n\nThou cannot acquire new spells yet."
			)
		return
	end

	cost = spellLevelCost[spellLevel]
	text:print("%s %d will cost thee %d in gold. ", char.class, 
			spellLevel + 1, cost)

	if (char.gold < cost) then
		text:csplash(false, true, 
			"\n\nThe spell Sages refuse to teach you until " ..
			"you can pay!"
			)
		return
	end

	text:print("Will you pay?")
	if (text:getYesNo()) then
		char.gold = char.gold - cost
		if (char.class == "Conjurer") then
			char.spellLevel.Conjurer = char.spellLevel.Conjurer + 1
		end
		if (char.class == "Magician") then
			char.spellLevel.Magician = char.spellLevel.Magician + 1
		end
		if (char.class == "Sorcerer") then
			char.spellLevel.Sorcerer = char.spellLevel.Sorcerer + 1
		end
		if (char.class == "Wizard") then
			char.spellLevel.Wizard = char.spellLevel.Wizard + 1
		end
		if (char.class == "Archmage") then
			char.spellLevel.Archmage = char.spellLevel.Archmage + 1
		end
		text:csplash(true, true, 
			"\n\nThe Spell Sages have taught you the lore.\n"
			)
	end

end

----------------------------------------
-- selectOption()
----------------------------------------
function review:selectOption()
	local inkey
	
	repeat
		if (self:nightCheck()) then
			return
		end

		text:cdprint(true, false, 
			"Wouldst thou like to be reviewed for:\n\n"
			)
		text:print("Advancement\n")
		text:print("Spell Acquiring\n")
		text:print("Class Change\n")
		text:printExit()

		inkey = getkey()
		if ((inkey > "0") and (inkey < "8")) then
			local c = party:readMember(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		elseif (inkey == "A") then
			self:doAdvancement()
		elseif (inkey == "C") then	
			self:doClassChange()
		elseif (inkey == "S") then
			self:doSpellAcquire()
		end
	until (inkey == "E")
end

----------------------------------------
-- enter()
----------------------------------------
function review:enter()
	if (self:nightCheck()) then
		currentLevel:turnParty("back")
		return
	end
	
	self:resetBigpic()
	self:selectOption()

	currentLevel:turnParty("back")
end
