require "bttable"

btAction = {}
function btAction:new()
	local self = {
		next		= false,
		prev		= false,
		source		= false,
		target		= false,
		func		= false,
		inBattle	= false,
		inData		= {},
		outData		= {}
	}

	btTable.addParent(self, btAction)
	btTable.setClassMetatable(self)

	return self
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
