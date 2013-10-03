require "bttable"

btAction = {}
function btAction:new()
	local self = {
		next		= false,
		prev		= false,
		source		= false,
		target		= false,
		func		= false,
		inData		= {},
		outData		= {}
	}

	btTable.addParent(self, btAction)
	btTable.setClassMetatable(self)

	return self
end

function btAction:dump()
	local k
	local v
	dprint("===========================")
	dprint("Source: %s", self.source:getSingularName())
	dprint("Target: %s", self.target:getSingularName())
	dprint("inData: {")
	for k,v in pairs(self.inData) do
		dprint("  %s: %s", k, v)
	end
	dprint("}")
	dprint("outData: {")
	for k,v in pairs(self.outData) do
		dprint("  %s: %s", k, v)
	end
	dprint("}")
	dprint("===========================")
end

----------------------------------------
-- validateTarget()
----------------------------------------
function btAction:validateTarget()
	if (self.target:isSummon() and self.source:isCharacter()) then
		if (not self.target.isIllusion) then
			self.target.isHostile = true
		end
	end

	if (self.target:isCharacter() and self.target:isDisabled()) then
		dprint("Character target is disabled")
		return false
	end

	if (self.target:isMonster() and self.target.size == 0) then
		dprint("Monster group is gone")
		return false
	end

	return true
end


----------------------------------------
-- savingThrow
--
-- Calculate the saving throw value for
-- a source and target.
--
-- Returns two values. The first value
-- is a pass/fail of the saving throw
-- check. The second value is a pass/fail
-- of a lower value. This is generally
-- used in spells to halve the damage.
--
----------------------------------------
function btAction:savingThrow()
	local sourceValue
	local targetValue

	sourceValue = self.source:calculateSavingThrow()
	dprint("sourceValue: %d", sourceValue)
	targetValue = self.target:calculateSavingThrow()
	dprint("targetValue: %d", targetValue)

	if (sourceValue >= targetValue) then
		return false, false
	end

	if (sourceValue >= (targetValue - 4)) then
		return true, true
	end

	return true, false
end

----------------------------------------
-- groupSavingThrow()
----------------------------------------
function btAction:groupSavingThrow()
	if (self.source:isMonster()) then
		self.target = party:getFirstCharacter()
	end

	return self:savingThrow()
end

----------------------------------------
-- printDamage()
----------------------------------------
function btAction:printDamage()
	text:print("for %d point%s of damage",
		self.outData.damage,
		string.pluralize(self.outData.damage, "", "s")
		)
end

----------------------------------------
-- doDamage()
----------------------------------------
function btAction:doDamage()
	if (self.target:doDamage(self)) then
		if (not globals.partyDied) then
			text:print(stringTables.effects[self.outData.specialAttack])
			text:print("%s!\n\n", self.target:getPronoun())
		end
	else
		text:print(".\n\n")
	end
end

----------------------------------------
-- singleTargetSpell()
----------------------------------------
function btAction:singleTargetSpell()
	local target		= self.target
	local source		= self.source
	local inData		= self.inData
	local outData		= self.outData
	local possessFlag	= false
	local save
	local half

	dprint("btAction:singleTargetSpell()")

	if (target:isSummon() and source:isCharacter()) then
		party.summon.isHostile = true
	end

	if (target:isMonster()) then
		if (target.size == 0) then
			text:printEllipsis()
			return
		end
	else
		if (target.isStoned or target.isParalyzed) then
			text:printEllipsis()
			return
		end

		if (target.isDead) then
			if (inData.specialAttack == "possess") then
				possessFlag = true
			else
				text:printEllipsis()
				return	
			end
		end

		if (inData.specialAttack == "possess" and
				source:isCharacter() and
				not possessFlag) then
			text:printEllipsis()
			return
		end
	end

	text:print(" at %s", target:getSingularName())
	save, half = self:savingThrow()
	if (save) then
		if (half) then
			bit32.rshift(outData.damage, 1)
		else
			text:ctdprint(false, true, " but it had no effect!\n\n")
			party:display()
			return
		end
	end

	if (not outData.specialAttack) then
		text:print(" and %s %s ",
			stringTables.andEffects[inData.atype],
			target:getPronoun()
			)
		self:printDamage()
	end

	self:doDamage()
	if (globals.partyDied) then
		return
	end

	party:display()
	timer:delay()
end

----------------------------------------
-- multiTargetSpell
----------------------------------------
function btAction:multiTargetSpell()
	local source		= self.source
	local target		= self.target
	local inData		= self.inData
	local outData		= self.outData
	local iter
	local t
	local lastDamage	= 0

	if (target:isCharacter()) then
		text:print(" at the party...\n\n")
		target = party
	else
		text:print(" at %s %s...\n\n",
			string.pluralize(target.size, "a", "some"),
			string.pluralize(target.size, target.singular, 
						      target.plural)
			)
		iter = target
	end

	for t in target:iterator(true) do
		local save
		local half

		repeat
			outData.damage = rnd_xdy(inData.ndice, inData.dieval)
		until (outData.damage ~= lastDamage)
		lastDamage = outData.damage

		if (t:isCharacter() or t:isSummon()) then
			text:print(t:getSingularName())
		else
			text:print("One")
		end

		if (t:isCharacter()) then
			self.target = t
		end
		save, half = self:savingThrow()
		if (save and not half) then
			text:print(" repelled the spell!\n\n")
		else
			if (half) then
				bit32.rshift(outData.damage, 1)
			end
			text:print(" is %s ",
				stringTables.isEffects[inData.atype])
			self:printDamage()
			self:doDamage()
		end
		party:display()
		timer:delay()
	end
end


























