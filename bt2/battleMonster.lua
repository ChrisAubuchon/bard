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
			else
				self:advanceGroup(inAction)
				timer:delay()
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

----------------------------------------
-- battleMonster:meleeAttack()
----------------------------------------
function battleMonster:meleeAttack(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	log:print(log.LOG_DEBUG, "battleMonster:meleeAttack() inData: " .. tostring(inData))
	if (not inAction:validateTarget()) then
		return
	end

	text:print("A %s%s%s", 
			self.singular,
			inData.meleeString[random:xdy(1,2)],
			inAction.target:getSingularName()
		)

	if (self:checkMeleeHits(inAction)) then
		self:getMeleeDamage(inAction)
		text:print(", and hits for %d point%s of damage", 
			outData.damage,
			string.pluralize(outData.damage, "", "s")
			)

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

----------------------------------------
-- battleMonster:checkMeleeHits()
----------------------------------------
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
	if (targetAC < -20) then targetAC = -20 end

	sourceAttack = random:betweenInclusive(self.toHitLo, self.toHitHi)
	sourceAttack = sourceAttack + 2 + self.parentGroup.toHitBonus
	sourceAttack = 10 - sourceAttack

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

----------------------------------------
-- battleMonster:getMeleeDamage()
----------------------------------------
function battleMonster:getMeleeDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	outData.damage = random:xdy(inData.ndice, inData.dieval)
	outData.damage = outData.damage + self.parentGroup.damageBonus

	if (random:band(1) == 0) then
		outData.specialAttack = inData.specialAttack
	end
end
	

----------------------------------------
-- battleMonster:doDamage()
----------------------------------------
function battleMonster:doDamage(inAction)
	local outData		= inAction.outData
	local target		= false
	local m

	target = self:randomMember()
	if (not target) then
		error("randomMember() failed to select a member")
	end

	if ((outData.specialAttack ~= "stone") and
	    (outData.specialAttack ~= "critical")) then
		outData.specialAttack = false
		if (outData.damage < target.currentHp) then
			target.currentHp = target.currentHp - outData.damage
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
	return random:betweenInclusive(self.spellSaveLo, self.spellSaveHi)
end

----------------------------------------
-- battleBonus()
----------------------------------------
function battleMonster:battleBonus(inAction)
	local inData		= inAction.inData
	local inSource		= inAction.source
	local inTarget		= inAction.target
	local updateParty	= false

	log:print(log.LOG_DEBUG, "battleMonster:battleBonus()")

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

	log:print(log.LOG_DEBUG, "battleMonster:attackSpell()")
	if (inData.group or inData.allFoes) then
		inAction:multiTargetSpell()
	else
		if (inData.specialAttack) then
			outData.specialAttack = inData.specialAttack
			outData.damage = 0
		else
			outData.damage = random:xdy(inData.ndice, inData.dieval)
		end
		inAction:singleTargetSpell()
	end
end

----------------------------------------
-- battleMonster:doDoppleganger()
----------------------------------------
function battleMonster:doDoppleganger(inAction)
	local randomCharacter
	local doppleganger

	if (not party:hasRoom()) then
		return false
	end

	randomCharacter = party:randomCharacter()
	log:print(log.LOG_DEBUG, "cloneCharacter = %s", randomCharacter.name)
	doppleganger = randomCharacter:clone()
	party:addCharacter(doppleganger)
	if (random:band(1) == 1) then
		log:print(log.LOG_DEBUG, "randomCharacter is the doppleganger")
		randomCharacter.isDoppleganger = true
		randomCharacter.possessed = true
	else
		log:print(log.LOG_DEBUG, "doppleganger is the doppleganger")
		doppleganger.isDoppleganger = true
		doppleganger.possessed = true
	end

	party:display()
	text:ctdprint(false, true, "A Doppleganger jumps into your party!\n\n")
end






















