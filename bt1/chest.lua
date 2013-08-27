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
----------------------------------------
function chest:enter()
	local inkey

	repeat
		text:cdprint(true,false, "There is a chest here. Will you:\n\n")
		text:print("Examine it\n")
		text:print("Open chest\n")
		text:print("Disarm it\n")
		text:print("Trap Zap\n")
		text:print("Leave chest\n")

		inkey = getkey()

		if (inkey == "D") then
			if (self:disarm()) then
				return true
			end
		elseif (inkey == "E") then
			self:examine()
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
		dprint(self.isTrapped)
		if (not self.isTrapped) then
			return true
		end

		dprint(answer)
		if (answer == self.name) then
			return true
		end

		if (char.lk >= rnd_xdy(1,256)) then
			return true
		end

		return false
	end

	if (doDisarm()) then
		text:cdprint(true, true, "\n\nYou disarmed it!")
		return true
	end

	if (rnd_and_x(3) ~= 0) then
		text:cdprint(true, true, "\n\nDisarm failed!")
		return false
	end

	self:setOffTrap()

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

	if ((not self.isTrapped) and (rnd_and_x(3) ~= 0)) then
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
function chest:setOffTrap()
	text:cdprint(true, true, "You set off a %s", self.name)
end




































