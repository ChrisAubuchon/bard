battleSummon	= {}

----------------------------------------
-- doAction()
----------------------------------------
function battleSummon:doAction(inAction)
	local inBattle	= inAction.inBattle
	local attack

	dprint("battleSummon:doAction() called")

	if (self:isDisabled()) then
		return
	end

	if (self.isIllusion and (not inBattle.isPartyAttack)) then
		dprint(inBattle.monParty.disbelieve)
		if (inBattle.monParty.disbelieve) then
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

		self:meleeAttack(inAction)
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
	elseif (not inAction.inBattle.isPartyAttack) then
		inAction.target = inAction.inBattle.monParty:getLeadGroup()
	else
		return false
	end

	return true
end

----------------------------------------
-- meleeAttack()
----------------------------------------
function battleSummon:meleeAttack(inAction)
	local inData		= inAction.inData
	local target		= inAction.target

	if (not inAction:validateTarget()) then
		return
	end

	text:print("%s%s%s", 
		self.singular,
		inData.meleeString[rnd_xdy(1,2)],
		inAction.target:getTargetString()
		)

	if (not self:checkMeleeHits(inAction)) then
		text:print(", but misses!\n")
		timer:delay(3)
		return
	else
		text:print(", and hits ")
		self:getMeleeDamage(inAction)
		inAction:printDamage()
		inAction:doDamage()

		if (target:isCharacter()) then
			party:display()
		end

		if (globals.partyDied) then
			return
		end
	end

	timer:delay(3)
end

----------------------------------------
-- checkMeleeHits()
----------------------------------------
function battleSummon:checkMeleeHits(inAction)
	local target		= inAction.target
	local inBattle		= inAction.inBattle
	local targetAC
	local sourceAttack
	local bonus

	targetAC = target.ac
	if (not target:isCharacter()) then
		targetAC = targetAC + target.acPenalty
		if (targetAC > 10) then
			targetAC = 10
		end
	end

	sourceAttack = self.ac
	sourceAttack = sourceAttack - inBattle.songToHitBonus
	sourceAttack = sourceAttack + self.toHitPenalty

	if (sourceAttack > 10) then sourceAttack = 10 end
	if (sourceAttack < -10) then sourceAttack = -10 end

	if (sourceAttack > targetAC) then
		return false
	end

	return true
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
-- Exactly the same as
-- battlePlayer:doDamage()
----------------------------------------
function battleSummon:doDamage(inAction)
	return battlePlayer.doDamage(self, inAction)
end


----------------------------------------
-- Same as battlePlayer:inflictStatus()
-- except that level drain and old 
-- attacks do nothing.
--
-- Would be easy to add:
-- if self:isSummon() then return end
-- to unify the functions
----------------------------------------
function battleSummon:inflictStatus(inAction)
	return battlePlayer.inflictStatus(self, inAction)
end

----------------------------------------
-- calculateSavingThrow()
----------------------------------------
function battleSummon:calculateSavingThrow()
	return battleMonster.calculateSavingThrow(self)
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
	local inBattle		= inAction.inBattle

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

----------------------------------------
-- battleBonus()
----------------------------------------
function battleSummon:battleBonus(inAction)
	return battlePlayer.battleBonus(self, inAction)
end

----------------------------------------
-- mageStar()
----------------------------------------
function battleSummon:mageStar(inAction)
	return battlePlayer.mageStar(self, inAction)
end





























