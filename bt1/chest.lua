chest = {}

----------------------------------------
-- new()
----------------------------------------
function chest:new(inChestTrap)
	local self = {
		isTrapped	= true,
		checked		= {}
	}

	if (not inChestTrap.name) then
		self.isTrapped = false
	end

	btTable.addParent(self, chest, inChestTrap)
	btTable.setClassMetatable(self)

	bigpic:setBigpic("PIC_CHEST", "Chest")

	return self:enter()
end

----------------------------------------
-- enter()
--
-- Return values:
--   true	- Party gets reward
--   false	- Party left chest
----------------------------------------
function chest:enter()
	local inkey

	repeat
		text:cdprint(true,false, 
			"There is a chest here. " ..
			"Will you:\n\n" ..
			"Examine it\n" ..
			"Open chest\n" ..
			"Disarm it\n" ..
			"Trap Zap\n" ..
			"Leave chest\n" 
		)

		inkey = getkey()

		if (inkey == "D") then
			if (self:disarm()) then
				return true
			end
		elseif (inkey == "E") then
			self:examine()
		elseif (inkey == "O") then
			if (self:open()) then
				return true
			end
		elseif (inkey == "T") then
			if (self:trapZap()) then
				return true
			end
		end
	until (inkey == "L")

	return false
end

----------------------------------------
-- getSource()
----------------------------------------
function chest:getSource(inString)
	local char

	text:cdprint(true, false, inString)
	char = party:readSlot()
	if (not char) then
		return false
	end

	if (char:isDisabled()) then
		return false
	end

	return char
end

----------------------------------------
-- disasm()
----------------------------------------
function chest:disarm()
	local char
	local answer

	char = self:getSource("\n\nWho will disarm it?")
	if (not char) then
		return false
	end

	text:cdprint(true, false, "\n\nEnter trap name:\n\n")
	answer = text:readString()
	if (not answer) then
		return false
	end
	text:print("\n\n")

	local function doDisarm()
		if (not self.isTrapped) then
			return true
		end

		if (answer == self.name) then
			return true
		end

		if (char.lk >= random:xdy(1,256)) then
			dprint("Lucky trap disarm")
			return true
		end

		return false
	end

	if (doDisarm()) then
		text:cdprint(true, true, "\n\nYou disarmed it!")
		return true
	end

	if (random:band(3) ~= 0) then
		text:cdprint(true, true, "\n\nDisarm failed!")
		return false
	end

	self:setOffTrap(char)

	return true
end

----------------------------------------
-- examine()
----------------------------------------
function chest:examine()
	local char

	char = self:getSource("\n\nWho will examine it?")
	if (not char) then
		return
	end

	if (self:hasChecked(char)) then
		text:cdprint(true, true, "\n\nHe already checked.")
		return
	end

	if ((not self.isTrapped) and (random:band(3) ~= 0)) then
		text:cdprint(true, false, "\n\nYou found nothing.")
		timer:delay(6)
		return
	end

	text:cdprint(true, false, "\n\nIt looks like a %s", self.name)
	timer:delay(6)
end

----------------------------------------
-- hasChecked()
----------------------------------------
function chest:hasChecked(inChar)
	if (self.checked[inChar.key]) then
		return true
	end

	self.checked[inChar.key] = true
	return false
end


----------------------------------------
-- setOffTrap()
----------------------------------------
function chest:setOffTrap(inTarget)
	local save
	local half
	local action
	local damage

	local function doTrap(inTarget)
		action.target = inTarget
		save, half = action:savingThrow()
		if (not save) then
			if (half) then
				action.outData.damage = bit32.rshift(damage,1)
			else
				action.outData.damage = damage
			end
			action:doDamage()
		end
	end

	text:cdprint(true, true, "You set off a %s", self.name)

	damage = random:xdy(self.ndice, self.dieval)

	action = btAction:new()
	action.source = self

	dprint(self.specialAttack)
	action.inData.specialAttack = self.specialAttack

	if (self.isPartyAttack) then
		local c

		for c in party:iterator() do
			doTrap(c)
		end
	else
		doTrap(inTarget)
	end

	party:display()
end

----------------------------------------
-- calculateSavingThrow()
----------------------------------------
function chest:calculateSavingThrow()
	return random:betweenInclusive(self.saveLo, self.saveHi)
end

----------------------------------------
-- open()
----------------------------------------
function chest:open()
	local char

	char = self:getSource("\n\nWho will open it?")
	if (not char) then
		return false
	end

	if (self.isTrapped) then
		self:setOffTrap(char)
	end

	return true
end


----------------------------------------
-- trapZap()
----------------------------------------
function chest:trapZap()
	local char
	local action

	char = self:getSource("\n\nWho will cast it?")
	if (not char) then
		return false
	end

	if (char.spellLevel.Conjurer < 1) then
		return false
	end

	action = btAction:new()
	action.inData.sppt = spells["TRZP"].sppt
	if (char:consumeSpellPoints(action)) then
		text:cdprint(true, true, "\nNot enough spell points.")
		return false
	else
		text:cdprint(true, true, "\n\nYou disarmed it!")
		return true
	end
end





























