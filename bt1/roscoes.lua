roscoes = {}
local funcs = {}

function funcs.setRoscoesBigpic()
	bigpic:drawImage("PIC_ROSCOEINT")
	bigpic:setTitle("Roscoe's")
end

function funcs.doReenergize(characterNumber)
	local char
	local payer
	local rechargeCost

	char = party[characterNumber]

	text:cdprint(true, false, char.name)
	if (char.cur_sppt >= char.max_sppt) then
		text:cdprint(false, true, " does not require restoration")
		return
	end

	rechargeCost = (char.max_sppt - char.cur_sppt) * 15
	text:print(" has some definite spell point problems. It will cost ")
	text:print("%d in gold. Who will pay?", rechargeCost)

	payer = party:readSlot()
	if (not payer) then
		return
	end
	if (payer.gold < rechargeCost) then
		text:clearline(11)
		text:setCursor(0, 11)
		text:cdprint(false, true, "Not enough gold.   ")
		return
	end

	payer.gold = payer.gold - rechargeCost
	char.cur_sppt = char.max_sppt
	text:cdprint(true, true, "\n\nRoscoe re-energizes him.")
	party:display()
end

function funcs.selectCharacter()
	local inkey

	repeat
		text:cdprint(true, false, "Welcome, my friends, to Roscoe's Energy Emporium. Who needeth spell points restored?")
		text:printExit()

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
