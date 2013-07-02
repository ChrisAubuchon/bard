roscoes = {}
local funcs = {}

function funcs.setRoscoesBigpic()
	bigpic.drawBigpic("PIC_ROSCOEINT")
	bigpic.setTitle("Roscoe's")
end

function funcs.doReenergize(characterNumber)
	local char
	local payer
	local rechargeCost

	char = party[characterNumber]

	text_cdprint(true, false, char.name)
	if (char.cur_sppt >= char.max_sppt) then
		text_cdprint(false, true, " does not require restoration")
		return
	end

	rechargeCost = (char.max_sppt - char.cur_sppt) * 15
	text_print(" has some definite spell point problems. It will cost ")
	text_print("%d in gold. Who will pay?", rechargeCost)

	payer = party:readSlot()
	if (not payer) then
		return
	end
	if (payer.gold < rechargeCost) then
		text_clearline(11)
		text_setCursor(0, 11)
		text_cdprint(false, true, "Not enough gold.   ")
		return
	end

	payer.gold = payer.gold - rechargeCost
	char.cur_sppt = char.max_sppt
	text_cdprint(true, true, "\n\nRoscoe re-energizes him.")
	party:display()
end

function funcs.selectCharacter()
	local inkey

	repeat
		text_cdprint(true, false, "Welcome, my friends, to Roscoe's Energy Emporium. Who needeth spell points restored?")
		printExit()

		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			inkey = tonumber(inkey)
			if (party:isOccupied(inkey)) then
				funcs.doReenergize(inkey)
			end
		end
	until (inkey == "E")
end

function roscoes.enter()
	funcs.setRoscoesBigpic()
	funcs.selectCharacter()
end
