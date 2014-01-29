----------------------------------------
-- battleEntity class
--
-- This class has methods that are used
-- by any kind of object that causes
-- damage.
--
-- Most of these functions were
-- originally part of the action class
----------------------------------------

battleEntity = {}

----------------------------------------
-- battleEntity:validateTarget()
----------------------------------------
function battleEntity:validateTarget()
	local action	= self.action

	-- Make a summon hostile if attacked by a party member
	--
	if (action.target:isSummon() and self:isCharacter()) then
		if (not action.target.isIllusion) then
			action.target.isDocile = false
		end
	end

	-- Character target is disabled
	--
	if (action.target:isCharacter() and action.target:isDisabled()) then
		return false
	end

	-- Monster group has been destroyed
	--
	if (action.target:isMonster() and action.target.size == 0) then
		return false
	end

	return true
end

----------------------------------------
-- battleEntity:savingThrow
--
-- Returns two values. The first value
-- is a pass/fail of the saving throw
-- check. The second value is a pass/fail
-- of a lower value. This is generally
-- used in spells to halve damage.
----------------------------------------
function battleEntity:savingThrow()
	local sourceValue
	local targetValue
	local target		= self.action.target

	sourceValue = self:calculateSavingThrow()
	targetValue = target:calculateSavingThrow()

	if (sourceValue >= targetValue) then
		return false, false
	end

	if (sourceValue >= (targetValue - 4)) then
		return true, true
	end

	return true, false
end

----------------------------------------
-- battleEntity:groupSavingThrow()
----------------------------------------
function battleEntity:groupSavingThrow()
	if (self:isMonster()) then
		self.action.target = party:getFirstCharacter()
	end

	return self:savingThrow()
end

