require "bttable"

btAction = {}
function btAction:new()
	local self = object:new()

	self:addSelf(btAction)

	self.type		= false
	self.source		= false
	self.target		= false
	self.func		= false
	self.inData		= {}
	self.result		= {}
	self.outData		= {}

	return self
end

----------------------------------------
-- btAction:delete()
----------------------------------------
function btAction:delete()
	self.source	= nil
	self.target	= nil
end

function btAction:dump()
	local k
	local v
	log:print(log.LOG_DEBUG, "===========================")
	log:print(log.LOG_DEBUG, "Source: %s", self.source:getSingularName())
	log:print(log.LOG_DEBUG, "Target: %s", self.target:getSingularName())
	log:print(log.LOG_DEBUG, "inData: {")
	for k,v in pairs(self.inData) do
		log:print(log.LOG_DEBUG, "  %s: %s", k, v)
	end
	log:print(log.LOG_DEBUG, "}")
	log:print(log.LOG_DEBUG, "result: {")
	for k,v in pairs(self.result) do
		log:print(log.LOG_DEBUG, "  %s: %s", k, v)
	end
	log:print(log.LOG_DEBUG, "}")
	log:print(log.LOG_DEBUG, "===========================")
end

----------------------------------------
-- btAction:reset()
----------------------------------------
function btAction:reset()
	self.target	= false
	self.func	= false
	self.action	= false
	self.type	= false
	self.inData	= {}
	self.result	= {}
end

----------------------------------------
-- btAction:renew()
--
-- Clear all data including the source
----------------------------------------
function btAction:renew()
	self.target	= false
	self.source	= false
	self.action	= false
	self.inData	= {}
	self.result	= {}
	self.type	= false
end

local xxx_candidate_for_battle_entity
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
		log:print(log.LOG_DEBUG, "Character target is disabled")
		return false
	end

	if (self.target:isMonster() and self.target.size == 0) then
		log:print(log.LOG_DEBUG, "Monster group is gone")
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
	log:print(log.LOG_DEBUG, "sourceValue: %d", sourceValue)
	targetValue = self.target:calculateSavingThrow()
	log:print(log.LOG_DEBUG, "targetValue: %d", targetValue)

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
	local amount = self.result.damage

	text:print("for %d point%s of damage", 
		amount,
		string.pluralize(amount, "", "s")
		)
end

----------------------------------------
-- doDamage()
----------------------------------------
function btAction:doDamage()
	local target	= self.target

	if (target:doDamage(self)) then
		local result = self.result

		if (not globals.partyDied) then
			text:print(stringTables.effects[result.specialAttack])
			text:print("%s!\n\n", target:getPronoun())
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
	local result		= self.result
	local possessFlag	= false
	local save
	local half

	log:print(log.LOG_DEBUG, "btAction:singleTargetSpell()")

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
			if (result.specialAttack == "possess") then
				possessFlag = true
			else
				text:printEllipsis()
				return	
			end
		end

		if (result.specialAttack == "possess" and
				source:isCharacter() and
				not possessFlag) then
			text:printEllipsis()
			return
		end
	end

	text:print(" at ")
	self:printTarget()
	save, half = self:savingThrow()
	if (save) then
		if (half) then
			bit32.rshift(result.damage, 1)
		else
			log:print(log.LOG_DEBUG, "%s", self.type)
			if (inData.fireString) then
				text:ctdprint(false, true, 
					" but it missed!\n\n")
			else
				text:ctdprint(false, true, 
					" but it had no effect!\n\n")
			end
			party:display()
			return
		end
	end

	if (not result.specialAttack) then
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
	local result		= self.result
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
	end

	for t in target:targetIterator() do
		local save
		local half

		repeat
			result.damage = random:xdy(inData.ndice, inData.dieval)
		until (result.damage ~= lastDamage)
		lastDamage = result.damage

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
				bit32.rshift(result.damage, 1)
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

----------------------------------------
-- btAction:perform()
----------------------------------------
function btAction:perform(inSource)
	if (type(self.func) ~= "function") then
		error("self.func not a function", 2)
	end

	if (self.func) then
		self:func(inSource)
	end
end

----------------------------------------
-- btAction:printTarget()
----------------------------------------
function btAction:printTarget()
	local target	= self.target

	if ((target:isMonster()) and (not target:isSummon())) then
		text:print("a %s", target.singular)
	else
		text:print(target.name)
	end
end






















