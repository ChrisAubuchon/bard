roscoes = object:new()

roscoes:addParent(building:new("Roscoe's", "PIC_ROSCOEINT"))

function roscoes:enter()
	self:resetBigpic()
	self:selectCharacter()
	currentLevel:turnParty("back")
end

----------------------------------------
-- doReenergzie()
----------------------------------------
function roscoes:doReenergize(inChar)
	local payer
	local rechargeCost

	text:cdprint(true, false, inChar.name)
	if (inChar.currentSppt >= inChar.maxSppt) then
		text:cdprint(false, true, " does not require restoration")
		return
	end

	rechargeCost = (inChar.maxSppt - inChar.currentSppt) * 15
	text:print(
		" has some definite spell point problems. It will cost " ..
		"%d in gold. Who will pay?", 
		rechargeCost
		)

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
	inChar.currentSppt = inChar.maxSppt
	text:cdprint(true, true, "\n\nRoscoe re-energizes him.")
	party:display()
end

----------------------------------------
-- selectCharacter()
----------------------------------------
function roscoes:selectCharacter()
	local inkey

	repeat
		text:cdprint(true, false, 
			"Welcome, my friends, to Roscoe's Energy Emporium. " ..
			"Who needeth spell points restored?"
			)
		text:printExit()

		inkey = getkey()
		if ((inkey > "0") and (inkey < "8")) then
			local c = party:readMember(inkey)
			if (c) then
				self:doReenergize(c)
			end
		end
	until (inkey == "E")
end

