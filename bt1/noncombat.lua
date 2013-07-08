noncombat = {}

----------------------------------------
-- These functions could probably be
-- moved to the party class.
----------------------------------------

function noncombat.useItem()
	local char
	local action

	text_cdprint(true, false, "Who wishes to use an item?")
	char = party:readSlot()
	if (not char) then
		text_clear()
		return	
	end

	action = btAction.new()
	action.source = char

	if (not char:getUseItem(action)) then
		return
	end

	char:useItem(action)
end

function noncombat.castSpell()
	local char
	local action
	local s

	text_cdprint(true, false, "\n\nWho do you wish to cast the spell?")
	char = party:readSlot()
	if (not char) then
		text_clear()
		return
	end


	if (char:isDisabled()) then
		text_cdprint(true, true, "Sorry, " .. char.name .. " is in no condition to cast a spell.")
		text_clear()
		return
	end

	if (not char:isSpellCaster()) then
		text_cdprint(true, true, "\n\nThou art not a spell caster!")
		text_clear()
		return
	end

	s = char:getSpell(false)
	if (not s) then
		text_clear()
		return
	end

	if (not s.noncombat) then
		text_cdprint(false, true, "\nYou can only cast that in combat.")
		text_clear()
		return
	end

	if (s.sppt > char.cur_sppt) then
		text_cdprint(false, true, "\nNot enough spell points.")
		text_clear()
		return
	end

	action = btAction.new()
	action.source = char
	action.func = s.action.func
	action.inData = s.action.inData
	action.inData.sppt = s.sppt

	if (s.targetted) then
		text_cdprint(true, false, "Use on:")

		action.target = getActionTarget(s.targetted, false)
		if (not action.target) then
			text_clear()
			return
		end
	end

	text_cdprint(true, false, char.name)
	char:castSpell(action)
	party:sort()
end

function noncombat.singSong()
	local char
	local action
	local tune

	text_cdprint(true, false, "Who will play?")
	char = party:readSlot()
	if (not char) then
		text_clear()
		return
	end

	if ((char.class ~= "Bard") or (char:isDisabled())) then
		text_cdprint(true, true, "\n\nHe can't sing")
		text_clear()
		return
	end

	if (not char:isTypeEquipped("Instrument")) then
		text_cdprint(true, true, "\n\nHe has no instrument to play.")
		text_clear()
		return
	end

	action = btAction.new()
	action.source = char

	tune = char:getTune()
	if (not tune) then
		text_clear()
		return
	end

	text_cdprint(true, false, char.name)
	if (not char:doVoiceCheck()) then
		text_delay(3)
		return
	end

	text_print(" plays a tune")

	if (party.songPlaying) then
		party.singer:songTimeout()
	end

	if (tune.activate) then
		action.func = tune.activate.func
		action.inData = tune.nonCombatData[currentLevel.level].inData

		if (action.func) then
			action.func(action)
		end
	end

	party.singer = char
	party.songPlaying = true
	party.songTime = 12
	char.isSinging = true
	char.song = tune

	dprint("party.songPlaying" .. tostring(party.songPlaying))
end















