temple = {}
function temple:new(inName)
	local self = {
		name	= inName
	}

	dprint(self.name)
	btTable.addParent(self, temple)
	btTable.setClassMetatable(self)

	self:enter()
end

----------------------------------------
-- setBigpic()
----------------------------------------
function temple:setBigpic()
	bigpic:drawImage("PIC_TEMPLEINT")
	bigpic:setTitle(self.name)
end

----------------------------------------
-- enter()
----------------------------------------
function temple:enter()
	local inkey

	dprint(self.name)
	self:setBigpic()

	repeat
		text:cdprint(true, false, "Welcome, oh weary ones, to our humble temple. Who needeth healing?")
		text:printExit()
		inkey = getkey()
		if ((inkey > "0") and (inkey < "7")) then
			local c = party:readMember(inkey)
			if (c) then
				self:doHealing(c)
			end
		end
	until (inkey == "E")
end

----------------------------------------
-- doHealing()
----------------------------------------
function temple:doHealing(inChar)
	text:clear()
	text:print(inChar.name)
	if (inChar:isAfflicted()) then
		if (not self:healAffliction(inChar)) then
			return
		end
	elseif (inChar.cur_level < inChar.max_level) then
		if (not self:restoreLife(inChar)) then
			return
		end
	elseif (inChar.cur_hp < inChar.max_hp) then
		if (not self:healHp(inChar)) then
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

----------------------------------------
-- healAffliction()
----------------------------------------
function temple:healAffliction(inChar)
	local multiplier
	local cost

	text:print(" is in bad shape, indeed. It will cost ")
	if (inChar.isDead) then
		multiplier = 900
	elseif (inChar.isOld) then
		multiplier = 300
	elseif (inChar.isPoisoned) then	
		multiplier = 500
	elseif (inChar.isStoned) then
		multiplier = 1120
	elseif (inChar.isParalyzed) then
		multiplier = 220
	elseif (inChar.isPossessed) then
		multiplier = 400
	elseif (inChar.isNuts) then
		multiplier = 600
	else
		error("inChar.isAfflicted without an afflication set")
	end

	if (inChar.cur_level < 13) then
		cost = inChar.cur_level * multiplier
	else
		cost = 13 * multiplier
	end

	if (not self:getPayer(cost)) then
		return false
	end

	if (inChar.isOld) then
		inChar.st = inChar.save_st
		inChar.iq = inChar.save_iq
		inChar.dx = inChar.save_dx
		inChar.cn = inChar.save_cn
		inChar.lk = inChar.save_lk
	end

	if (inChar.cur_hp == 0) then
		inChar.cur_hp = 1
	end

	inChar.isDead = false
	inChar.isOld = false
	inChar.isPoisoned = false
	inChar.isStoned = false
	inChar.isParalyzed = false
	inChar.isPossessed = false
	inChar.isNuts = false

	return true
end
		
----------------------------------------
-- getPayer()
----------------------------------------
function temple:getPayer(inCost)
	local payer

	text:print("%d in gold. Who will pay?", inCost)
	payer = party:readSlot()
	if (not payer) then
		return false
	end
	if (payer.gold < inCost) then
		self:notEnoughGold()
		return false
	end
	payer.gold = payer.gold - inCost

	return true
end

----------------------------------------
-- notEnoughGold()
----------------------------------------
function temple:notEnoughGold()
	text:clearLine(11)
	text:setCursor(0, 11)
	text:cdprint(false, true, "Not enough gold.")
end

----------------------------------------
-- restoreLife()
----------------------------------------
function temple:restoreLife(inChar)
	local cost

	if (inChar.max_level == inChar.cur_level) then
		return false
	end

	text:print(" has been drained of life force. It will cost ")
	if (inChar.cur_level < 13) then
		cost = 900 * inChar.cur_level
	else
		cost = 900 * 13
	end
	if (not self:getPayer(cost)) then
		return false
	end

	inChar.cur_level = inChar.max_level
	inChar.xp = inChar:getXpForLevel(inChar.cur_level)

	return true
end

----------------------------------------
-- healHp()
----------------------------------------
function temple:healHp(inChar)
	local cost

	cost = 10 * (inChar.max_hp - inChar.cur_hp)
	text:print(" has wounds which need tending. It will cost ")

	if (not self:getPayer(cost)) then
		return false
	end

	inChar.cur_hp = inChar.max_hp

	return true
end





















