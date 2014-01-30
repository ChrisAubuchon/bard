----------------------------------------
-- battleMonster class
--
-- Battle functions for monsters
----------------------------------------
battleMonster	= {}

function battleMonster:doAction()
	local attack
	local i

	-- Check here if this monster groups turn is
	-- skipped
	--
	if (self.parentGroup.missTurn) then
		return
	end

	log:print(log.LOG_DEBUG, "%s (%s) turn", self.singular, self.key)

	self.action.target = party:randomMeleeCharacter()

	for attack in self:attackIterator() do
		log:print(log.LOG_DEBUG, "attack.type: %s", attack.type)
		if (attack.type == "melee") then
			if (self:inMeleeRange()) then
				self.action.inData = attack.action.inData
				self:meleeAttack()
				return
			else
				self:advanceGroup()
				timer:delay()
				return
			end
		elseif (attack.type == "spell") then
			if (self.action.target:isDisabled()) then
				return
			end

			self.action.inData = attack.action.inData
			self.action.func = attack.action.func
			self:combatSpell()

			return 
		elseif (attack.type == "dopple") then
			if (self:doDoppleganger()) then
				if (self.action.target:isDisabled()) then
					return
				end
				self.action.inData = attack.action.inData
				self:meleeAttack()
				return	
			end
		elseif (attack.type == "breath") then
			text:print("%s breathes", self.singular)
			self.action.inData = attack.action.inData
			self.action.func = attack.action.func
			self.action:multiTargetSpell()
			
			return
		elseif (attack.type == "fire") then
			self.action.type = "fire"
			self.action.inData = attack.action.inData

			text:print("%s %s", 
				self.singular, self.action.inData.fireString)
			self:attackSpell()
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
function battleMonster:meleeAttack()
	local inData	= self.action.inData
	local result	= self.action.result
	local target	= self.action.target

	log:print(log.LOG_DEBUG, "battleMonster:meleeAttack() inData: " .. tostring(inData))
	if (not self.action:validateTarget()) then
		return
	end

	text:print("A %s%s%s", 
			self.singular,
			inData.meleeString[random:xdy(1,2)],
			target:getSingularName()
		)

	if (self:checkMeleeHits()) then
		self:getMeleeDamage()
		text:print(", and hits for %d point%s of damage", 
			result.damage,
			string.pluralize(result.damage, "", "s")
			)

		if (target:doDamage(self.action)) then
			text:print(stringTables.effects[result.specialAttack])
			text:print("%s!\n\n", self.action.target:getPronoun())
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
function battleMonster:checkMeleeHits()
	local target	= self.action.target
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
function battleMonster:getMeleeDamage()
	local inData	= self.action.inData
	local result	= self.action.result

	result.damage = random:xdy(inData.ndice, inData.dieval)
	result.damage = result.damage + self.parentGroup.damageBonus

	if (random:band(1) == 0) then
		result.specialAttack = inData.specialAttack
	end
end
	

----------------------------------------
-- battleMonster:doDamage()
----------------------------------------
function battleMonster:doDamage(inAction)
	local result		= inAction.result
	local target		= false
	local m

	target = self:randomMember()
	if (not target) then
		error("randomMember() failed to select a member")
	end

	if ((result.specialAttack ~= "stone") and
	    (result.specialAttack ~= "critical")) then
		result.specialAttack = false
		if (result.damage < target.currentHp) then
			target.currentHp = target.currentHp - result.damage
			target.beenAttacked = true
			return false
		else
			result.specialAttack = "kill"
		end
	end

	log:print(log.LOG_DEBUG, "Removing monster: %s", target.key)
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
function battleMonster:battleBonus()
	local inData		= self.action.inData
	local inSource		= self.action.source
	local inTarget		= self.action.target
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
		self.action.target = party:getFirstCharacter()
		if (self.action:savingThrow()) then
			text:cdprint(false, true, " but it had no effect!\n\n")
			return false
		end
		updateParty = true
		party:addBattleBonus("acPenalty", self.action.toHitAmount,
						  self.action.toHitStack)
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

function battleMonster:mageStar()
	text:print(" and the party misses an attack")
	party.missTurn = true
end

----------------------------------------
--
----------------------------------------
function battleMonster:combatSpell()
	if (self.action.target:isDisabled()) then
		return
	end

	text:print("%s casts a spell", self.singular)
	self.action:perform(self.action)
end

function battleMonster:attackSpell()
	local inData		= self.action.inData
	local result		= self.action.result
	local source		= self.action.source

	log:print(log.LOG_DEBUG, "battleMonster:attackSpell()")
	if (inData.group or inData.allFoes) then
		self.action:multiTargetSpell()
	else
		if (inData.specialAttack) then
			result.specialAttack = inData.specialAttack
			result.damage = 0
		else
			result.damage = random:xdy(inData.ndice, inData.dieval)
		end
		self.action:singleTargetSpell()
	end
end

----------------------------------------
-- battleMonster:doDoppleganger()
----------------------------------------
function battleMonster:doDoppleganger()
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

----------------------------------------
-- battleMonster:fireWeapon()
----------------------------------------
function battleMonster:fireWeapon()
	text:print(inData.fireString)

	self:attackSpell()
end


















