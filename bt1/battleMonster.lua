battleMonster	= {}

function battleMonster:doAction(inAction)
	local target		= inAction.target
	local attackIndex
	local i

	attackIndex = rnd_xdy(1,4)
	for i = 1,4 do
		local attacks = self.attacks

		local xxx_delete_block_when_all_attacks_are_filled = true
		if (not attacks[attackIndex]) then
			return
		end

		if (attacks[attackIndex].type == "melee") then
			dprint("Attack type: " .. tostring(attacks[attackIndex].type))
			if (not self:inMeleeRange()) then
				attackIndex = attackIndex + 1
				if (attackIndex > 4) then
					attackIndex = 1
				end
			else
				inAction.inData = attacks[attackIndex].action.inData
				self:meleeAttack(inAction)
				return
			end
		else
			local xxx_otherMonsterAttackTypes = true
		end
	end
end

function battleMonster:meleeAttack(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	if (not self:inMeleeRange()) then
		return
	end

	inAction.target = party:randomMeleeCharacter()
	if (inAction.target:isDisabled()) then
		return
	end

	dprint("battleMonster:meleeAttack() inData: " .. tostring(inData))
	text:print("A " .. self.singular .. " " ..
			inData.meleeString[rnd_xdy(1,2)] ..
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

	timer:delay(3)
	return
end

function battleMonster:checkMeleeHits(inAction)
	local target	= inAction.target
	local inBattle	= inAction.inBattle
	local sourceAttack
	local targetAC
	local bonus

	targetAC = target.ac

	targetAC = targetAC + inBattle.fearPenalty[self.parentGroup.key]
	if (targetAC >  10) then targetAC =  10 end
	if (targetAC < -10) then targetAC = -10 end

	sourceAttack = rnd_between_xy_inc(self.toHitLo, self.toHitHi)
	sourceAttack = sourceAttack + 2 +
			inBattle.battleSkillBonus[self.parentGroup.key]
	sourceAttack = 10 - sourceAttack

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

function battleMonster:getMeleeDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData
	local inBattle	= inAction.inBattle

	outData.damage = rnd_xdy(inData.ndice, inData.dieval)
	outData.damage = outData.damage +
		inBattle.battleSkillBonus[self.parentGroup.key]

	outData.specialAttack = inData.specialAttack
end
	

function battleMonster:doDamage(inAction)
	local outData		= inAction.outData
	local inBattle		= inAction.inBattle
	local target		= false
	local m

	dprint(self.singular)
	for m in self:iterator() do
		if (not m.beenAttacked) then
			target = m
			break
		end
	end

	if (not target) then
		target = self[1]
	end

	dprint(outData.specialAttack)
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
	inBattle:removePriority(target)
	inBattle.killCount[self.name] = inBattle.killCount[self.name] + 1
	if (self.size == 0) then
		inBattle.monGroups:removeMonsterGroup(self)
	end

	return true
end
