temple = {}
local funcs = {}

function funcs.setTempleBigpic()
	bigpic.drawBigpic("PIC_TEMPLEINT")
	bigpic.setTitle(funcs.templeName)
end

function funcs.selectCharacter()
	local inkey

	repeat
		text:cdprint(true,false, "Welcome, oh weary ones, to our humble temple. Who needeth healing?")
		text:printExit()
		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			inkey = tonumber(inkey)
			if (party:isOccupied(inkey)) then
				funcs.doHealing(inkey)
			end
		end
	until (inkey == "E")
end

function funcs.notEnoughGold()
	text:clearline(11)
	text:setCursor(0, 11)
	text:cdprint(false, true, "Not enough gold.")
end

function funcs.healAffliction(char)
	local multiplier
	local cost
	local payerNumber
	local payer

	text:print(" is in bad shape, indeed. It will cost ")
	if (char.isDead) then
		multiplier = 900
	elseif (char.isOld) then
		multiplier = 300
	elseif (char.isPoisoned) then
		multiplier = 500
	elseif (char.isStoned) then
		multiplier = 1120
	elseif (char.isParalyzed) then
		multiplier = 220
	elseif (char.isPossessed) then
		multiplier = 400
	elseif (char.isNuts) then
		multiplier = 600
	else
		error("char.isAfflicted without an afflication set")
	end

	if (char.cur_level < 13) then
		cost = char.cur_level * multiplier
	else
		cost = 13 * multiplier
	end

	if (not funcs.getPayer(cost)) then
		return false
	end

	if (char.isOld) then
		char.st = char.save_st
		char.iq = char.save_iq
		char.dx = char.save_dx
		char.cn = char.save_cn
		char.lk = char.save_lk
	end

	if (char.cur_hp == 0) then
		char.cur_hp = 1
	end

	char.isDead = false
	char.isOld = false
	char.isPoisoned = false
	char.isStoned = false
	char.isParalyzed = false
	char.isPossessed = false
	char.isNuts = false

	return true
end

function funcs.restoreLife(char)
	local payerNumber
	local payer
	local cost

	assert(char.max_level >= char.cur_level, "Max level > Cur level ??")
	if (char.max_level == char.cur_level) then
		return false
	end

	text:print(" has been drained of life force. It will cost ")
	if (char.cur_level < 13) then
		cost = 900 * char.cur_level
	else
		cost = 900 * 13
	end
	if (not funcs.getPayer(cost)) then
		return false
	end

	char.cur_level = char.max_level
	char.xp = classes.getXpForLevel(char.class, char.cur_level)

	return true
end

function funcs.healHp(char)
	local payerNumber
	local payer
	local cost

	cost = 10 * (char.max_hp - char.cur_hp)
	text:print(" has wounds which need tending. It will cost ")

	if (not funcs.getPayer(cost)) then
		return false
	end

	char.cur_hp = char.max_hp

	return true
end

----------------------------------------
-- getPayer()
--
--  Get the character that pays for 
-- healing. Returns true if the cost has
-- been payed and false otherwise
----------------------------------------
function funcs.getPayer(cost)
	local payer

	text:print("%d in gold. Who will pay?", cost)
	payer = party:readSlot()
	if (not payer) then
		return false
	end
	if (payer.gold < cost) then
		funcs.notEnoughGold()
		return false
	end
	payer.gold = payer.gold - cost

	return true
end

function funcs.doHealing(characterNumber)
	local char

	char = party[characterNumber]
	text:clear()
	text:print(char.name)
	if (char:isAfflicted()) then
		if (not funcs.healAffliction(char)) then
			return
		end
	elseif (char.cur_level < char.max_level) then
		if (not funcs.restoreLife(char)) then
			return
		end
	elseif (char.cur_hp < char.max_hp) then
		if (not funcs.healHp(char)) then
			return
		end
	else
		text:cdprint(false, true, " does not require any healing.")
		return
	end

	text:cdprint(true, true, "\n\nThe priests lay hands on him...\n\n")
	text:cdprint(false, true, "...and he is healed!")
	party:display()
end

function temple.enter(templeName)
	funcs.templeName = templeName
	funcs.setTempleBigpic()
	funcs.selectCharacter()
end
