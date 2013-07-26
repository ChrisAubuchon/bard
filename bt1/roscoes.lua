roscoes = {}
local funcs = {}

function funcs.setRoscoesBigpic()
	bigpic:drawImage("PIC_ROSCOEINT")
	bigpic:setTitle("Roscoe's")
end

function funcs.doReenergize(inChar)
	local payer
	local rechargeCost

	text:cdprint(true, false, inChar.name)
	if (inChar.cur_sppt >= inChar.max_sppt) then
		text:cdprint(false, true, " does not require restoration")
		return
	end

	rechargeCost = (inChar.max_sppt - inChar.cur_sppt) * 15
	text:print(" has some definite spell point problems. It will cost ")
	text:print("%d in gold. Who will pay?", rechargeCost)

	payer = party:readSlot()
	if (not payer) then
		return
	end
	if (payer.gold < rechargeCost) then
		text:clearLine(11)
		text:setCursor(0, 11)
		text:cdprint(false, true, "Not enough gold.   ")
		return
	end

	payer.gold = payer.gold - rechargeCost
	inChar.cur_sppt = inChar.max_sppt
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
			local c = party:readMember(inkey)
			if (c) then
				funcs.doReenergize(c)
			end
		end
	until (inkey == "E")
end

function roscoes.enter()
	funcs.setRoscoesBigpic()
	funcs.selectCharacter()
end
