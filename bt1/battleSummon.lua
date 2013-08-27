----------------------------------------
-- battleSummon class
--
-- Class methods specific to summoned
-- monsters
----------------------------------------
battleSummon	= {}

----------------------------------------
-- doAction()
----------------------------------------
function battleSummon:doAction(inAction)
	local attack

	dprint("battleSummon:doAction() called")

	if (self:isDisabled()) then
		return
	end

	if (self.isIllusion and (not currentBattle.isPartyAttack)) then
		dprint(currentBattle.monParty.disbelieve)
		if (currentBattle.monParty.disbelieve) then
			dprint("Skipping summon turn: disbelieve")
			return
		end
	end

	local function rndAttack()
		local a

		for a in self:attackIterator() do
			return a
		end
	end

	attack = rndAttack()

	if (not self:getTarget(inAction)) then
		return
	end
	if (attack.type == "melee" or attack.type == "dopple") then
		inAction.inData = attack.action.inData

		self:doMeleeAttack(inAction)
	elseif (attack.type == "spell") then
		inAction.inData = attack.action.inData
		inAction.func = attack.action.func

		self:combatSpell(inAction)
	elseif (attack.type == "breath") then
		text:print("%s breathes", self.singular)

		inAction.inData = attack.action.inData
		inAction.func = attack.action.func
		inAction:multiTargetSpell()
	end
end

----------------------------------------
-- getTarget()
----------------------------------------
function battleSummon:getTarget(inAction)
	if (self.isHostile) then
		inAction.target = party:randomCharacter()
	elseif (not currentBattle.isPartyAttack) then
		inAction.target = currentBattle.monParty:getLeadGroup()
	else
		return false
	end

	return true
end

----------------------------------------
-- getMeleeAttackString
----------------------------------------
function battleSummon:getMeleeAttackString(inAction)
	return string.format("%s%s%s",
			self.singular,
			inAction.inData.meleeString[rnd_xdy(1,2)],
			inAction.target:getTargetString()
			)
end

----------------------------------------
-- getMeleeDamage()
----------------------------------------
function battleSummon:getMeleeDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	-- Summoned monsters use d8 instead of d4 for their
	-- attack
	--
	outData.damage = rnd_xdy(inData.ndice, 8)

	outData.damage = outData.damage + self.damageBonus
	outData.damage = outData.damage + rnd_xdy(self.damageRandom, 8)
	outData.damage = outData.damage - self.damagePenalty
	outData.specialAttack = inData.specialAttack
end


----------------------------------------
-- calculateSavingThrow()
----------------------------------------
function battleSummon:calculateSavingThrow()
	return rnd_between_xy_inc(self.spellSaveLo, self.spellSaveHi)
end

----------------------------------------
-- combatSpell()
----------------------------------------
function battleSummon:combatSpell(inAction)
	if (inAction.target:isCharacter() and inAction.target:isDisabled()) then
		return
	end

	text:print("%s casts a spell", self. singular)
	inAction.func(inAction)
end

----------------------------------------
-- attackSpell()
----------------------------------------
function battleSummon:attackSpell(inAction)
	local inData		= inAction.inData
	local outData		= inAction.outData
	local target		= inAction.target

	if (inData.group or inData.allFoes) then
		inAction:multiTargetSpell()
	else
		if (inData.specialAttack) then
			outData.specialAttack = inData.specialAttack
			outData.damage = 0
		else
			outData.damage = rnd_xdy(inData.ndice, inData.dieval)
		end
		inAction:singleTargetSpell()
	end
end





























