----------------------------------------
-- battleMonster class
--
-- Battle functions for monsters
----------------------------------------
battleMonster	= {}

function battleMonster:doAction(inAction)
	local attack
	local i

	-- Check here if this monster groups turn is
	-- skipped
	--
	if (self.parentGroup.missTurn) then
		return
	end

	inAction.target = party:randomMeleeCharacter()

	for attack in self:attackIterator() do
		if (attack.type == "melee") then
			if (self:inMeleeRange()) then
				inAction.inData = attack.action.inData
				self:meleeAttack(inAction)
				return
			end
		elseif (attack.type == "spell") then
			if (inAction.target:isDisabled()) then
				return
			end

			inAction.inData = attack.action.inData
			inAction.func = attack.action.func
			self:combatSpell(inAction)

			return 
		elseif (attack.type == "dopple") then
			if (self:doDoppleganger(inAction)) then
				if (inAction.target:isDisabled()) then
					return
				end
				inAction.inData = attack.action.inData
				self:meleeAttack(inAction)
				return	
			end
		elseif (attack.type == "breath") then
			text:print("%s breathes", self.singular)
			inAction.inData = attack.action.inData
			inAction.func = attack.action.func
			inAction:multiTargetSpell()
			
			return
		else
			error("Unknown monster attack type: " .. 
				tostring(attack.type))
		end
	end
end

function battleMonster:meleeAttack(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	dprint("battleMonster:meleeAttack() inData: " .. tostring(inData))
	if (not inAction:validateTarget()) then
		return
	end

	text:print("A %s%s%s", 
			self.singular,
			inData.meleeString[rnd_xdy(1,2)],
			inAction.target:getSingularName()
		)

	if (self:checkMeleeHits(inAction)) then
		self:getMeleeDamage(inAction)
		text:print(", and hits for %d ", outData.damage)
		if (outData.damage == 1) then
			text:print("point ")
		else
			text:print("points ")
		end
		text:print("of damage")

		if (inAction.target:doDamage(inAction)) then
			text:print(stringTables.effects[outData.specialAttack])
			text:print("%s!\n\n", inAction.target:getPronoun())
		else
			text:print(".\n\n")
		end

		party:display()
		if (not party:isLive()) then
			return
		end
	else
		text:print(", but misses!\n\n")
	end

	timer:delay()
	return
end

function battleMonster:checkMeleeHits(inAction)
	local target	= inAction.target
	local sourceAttack
	local targetAC
	local bonus

	targetAC = target.ac

	targetAC = targetAC - self.parentGroup.toHitPenalty

	-- The DOS version has this backwards. It adds the toHitPenalty
	-- to the target's AC. This actually makes it easier to hit
	-- the party.
	--
	--targetAC = targetAC + self.parentGroup.toHitPenalty

	if (targetAC >  10) then targetAC =  10 end
	if (targetAC < -10) then targetAC = -10 end

	sourceAttack = rnd_between_xy_inc(self.toHitLo, self.toHitHi)
	sourceAttack = sourceAttack + 2 + self.parentGroup.toHitBonus
	sourceAttack = 10 - sourceAttack

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

function battleMonster:getMeleeDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	outData.damage = rnd_xdy(inData.ndice, inData.dieval)
	outData.damage = outData.damage + self.parentGroup.damageBonus

	outData.specialAttack = inData.specialAttack
end
	

function battleMonster:doDamage(inAction)
	local outData		= inAction.outData
	local target		= false
	local m

	for m in self:iterator() do
		if (not m.beenAttacked) then
			target = m
			break
		end
	end

	if (not target) then
		target = self.head
	end

	if ((outData.specialAttack ~= "stone") and
	    (outData.specialAttack ~= "critical")) then
		outData.specialAttack = false
		if (outData.damage < target.curHP) then
			target.curHP = target.curHP - outData.damage
			target.beenAttacked = true
			return false
		else
			outData.specialAttack = "kill"
		end
	end

	self:removeMonster(target)
	currentBattle:removePriority(target)
	currentBattle.killCount[self.name] = currentBattle.killCount[self.name] + 1
	if (self.size == 0) then
		currentBattle.monParty:removeMonsterGroup(self)
	end

	return true
end

function battleMonster:calculateSavingThrow()
	return rnd_between_xy_inc(self.spellSaveLo, self.spellSaveHi)
end

----------------------------------------
-- battleBonus()
----------------------------------------
function battleMonster:battleBonus(inAction)
	local inData		= inAction.inData
	local inSource		= inAction.source
	local inTarget		= inAction.target
	local updateParty	= false

	dprint("battleMonster:battleBonus()")

	if (inData.acBonus) then
		if (inData.group) then
			self.parentGroup:addBattleBonus("acBonus",
						inData.acAmount,
						inData.acStack)
		else
			local none = true
		end
	end

	if (inData.acPenalty) then
		inAction.target = party:getFirstCharacter()
		if (inAction:savingThrow()) then
			text:cdprint(false, true, " but it had no effect!\n\n")
			return false
		end
		updateParty = true
		party:addBattleBonus("acPenalty", inAction.toHitAmount,
						  inAction.toHitStack)
	end

	if (inData.toHitPenalty) then
		if (inData.group) then
			party:addBattleBonus("toHitPenalty",
						inData.toHitAmount,
						inData.toHitStack)
		else
			local none = true
		end
	end

	if (inData.damagePenalty) then
		if (inData.group) then
			party:addBattleBonus("damagePenalty",
						inData.damageAmount,
						inData.damageStack)
		else
			local none = true
		end
	end

	-- Monsters use the damageBonus value for both damage
	-- and toHit values
	--
	if (inData.damageBonus) then
		self.parentGroup:addBattleBonus("damageBonus",
						inData.damageAmount,
						inData.damageStack)
		self.parentGroup:addBattleBonus("toHitBonus",
						inData.damageAmount,
						inData.damageStack)
	end

	return true
end

function battleMonster:mageStar(inAction)
	text:print(" and the party misses an attack")
	party.missTurn = true
end

function battleMonster:combatSpell(inAction)
	if (inAction.target:isDisabled()) then
		return
	end

	text:print("%s casts a spell", self.singular)
	inAction.func(inAction)
end

function battleMonster:attackSpell(inAction)
	local inData		= inAction.inData
	local outData		= inAction.outData
	local source		= inAction.source

	dprint("battleMonster:attackSpell()")
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

function battleMonster:doDoppleganger(inAction)
	local randomCharacter
	local doppleganger

	if (not party:hasRoom()) then
		return false
	end

	randomCharacter = party:randomCharacter()
	dprint("cloneCharacter = %s", randomCharacter.name)
	doppleganger = randomCharacter:clone()
	party:addCharacter(doppleganger)
	if (rnd_and_x(1) == 1) then
		dprint("randomCharacter is the doppleganger")
		randomCharacter.isDoppleganger = true
		randomCharacter.possessed = true
	else
		dprint("doppleganger is the doppleganger")
		doppleganger.isDoppleganger = true
		doppleganger.possessed = true
	end

	party:display()
	text:ctdprint(false, true, "A Doppleganger jumps into your party!\n\n")
end






















