----------------------------------------
-- battleParty class
--
-- Class methods that are common between
-- characters and summoned monsters
----------------------------------------
battleParty	= {}

----------------------------------------
-- doMeleeAttack()
----------------------------------------
function battleParty:doMeleeAttack()
	local target	= self.action.target
	local inData	= self.action.inData

	if (not self.action:validateTarget()) then
		return
	end

	text:print(self:getMeleeAttackString())

	if (not self:checkMeleeHits()) then
		text:ctdprint(false, true, ", but misses!\n\n")
		return
	else
		text:print(", and hits ")
		if (self.numAttacks > 1) then
			text:print("%d times ", self.numAttacks)
		end

		self:getMeleeDamage()
		self.action:printDamage()
		self.action:doDamage()

		if (target:isCharacter()) then
			party:display()
		end

		if (globals.partyDied) then
			return
		end
	end

	timer:delay()
end

----------------------------------------
-- checkMeleeHits()
----------------------------------------
function battleParty:checkMeleeHits()
	local target	= self.action.target
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
	--

	sourceAttack = self.ac
	if (not self:isSummon()) then
		sourceAttack = sourceAttack - self.meleeBonus
	end
	sourceAttack = sourceAttack - currentBattle.songToHitBonus
	sourceAttack = sourceAttack + self.toHitPenalty

	if (sourceAttack >  10) then sourceAttack =  10 end
	if (sourceAttack < -20) then sourceAttack = -20 end

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

----------------------------------------
-- doDamage()
----------------------------------------
function battleParty:doDamage(inAction)
	local result	= inAction.result
	local rval

	if (self.isDead) then
		if (result.specialAttack ~= "possess") then
			return false
		end
	end

	if (self.isStoned) then
		return false
	end

	if (result.damage > 0) then
		if (self.currentHp > result.damage) then
			self.currentHp = self.currentHp - result.damage
		else
			self.currentHp = 0
			self.possessed = false
			self.isDead = true

			self:songTimeout()

			result.specialAttack = "kill"

			party:isLive()

			return true
		end
	end

	if (not result.specialAttack) then
		return false
	end

	rval = self:inflictStatus(inAction)

	party:isLive()

	return rval
end

----------------------------------------
-- inflictStatus()
----------------------------------------
function battleParty:inflictStatus(inAction)
	local source	= inAction.source
	local result	= inAction.result

	if (result.specialAttack == "poison") then
		if (not self.isDestinyKnight) then
			self.isPoisoned = true
		end
	elseif (result.specialAttack == "drain") then
		if (self:isSummon()) then
			return
		end
		if (self.cur_level ~= 1) then
			self.cur_level = self.cur_level - 1
			self.xp = self:getXpForLevel(self.cur_level)
		end
		return true
	elseif (result.specialAttack == "nuts") then
		if (not self.isDestinyKnight) then
			self.isNuts = true
		end
	elseif (result.specialAttack == "old") then
		if (self.isOld) then
			return true
		end

		-- Don't print special attack if isDestinyKnight
		if (self.isDestinyKnight) then
			return false
		end

		self.save_st = self.st
		self.save_iq = self.iq
		self.save_dx = self.dx
		self.save_cn = self.cn
		self.save_lk = self.lk
		self.st = 1
		self.iq = 1
		self.dx = 1
		self.cn = 1
		self.lk = 1
		self.isOld = true
	elseif (result.specialAttack == "possess") then
		if (not source:isMonster()) then
			if (self:hasItem("Kato's Bracer")) then
				self.isDocile = true
			end
		end
		if (self.isDestinyKnight) then
			return true
		end
		self.currentHp = 100
		self.isPossessed	= true
		self.isDead		= false
		self.isPoisoned		= false
		self.isOld		= false
		self.isStoned		= false
		self.isNuts		= false
		self.isParalyzed	= false
	elseif (result.specialAttack == "stone") then
		if (self.isDestinyKnight) then
			return true
		end
		self.currentHp		= 0
		self.isStoned		= true
		self:songTimeout()
	elseif ((result.specialAttack == "critical") or 
		(result.specialAttack == "kill")) then
		if (self.isDestinyKnight) then
			self.currentHp = self.maxHp
		else
			self.currentHp		= 0
			self.possessed		= false
			self.isDead		= true
		end
		self:songTimeout()
	elseif (result.specialAttack == "unequip") then
		local i

		for i in self.inventory:iterator() do
			i:unequip()
		end

		self:songTimeout()
		party:display()

		return false
	end

	return true
end

----------------------------------------
-- battleBonus()
----------------------------------------
function battleParty:battleBonus(inAction)
	local inData		= inAction.inData
	local inSource		= inAction.source
	local inTarget		= inAction.target
	local updateParty	= false

	----------------------------------------
	-- antiMagic battle bonus
	----------------------------------------
	if (inData.antiMagic) then
		party:addBattleBonus("antiMagic", 2, true, 6)
	end

	----------------------------------------
	-- AC Bonus
	----------------------------------------
	if (inData.acBonus) then
		if (inData.group) then
			party:addBattleBonus("acBonus", inData.acAmount,
					inData.acStack)
		else
			inSource:addBattleBonus("acBonus", inData.acAmount, 
					inData.acStack)
			updateParty = true
		end
	end

	----------------------------------------
	-- AC Penalty
	----------------------------------------
	if (inData.acPenalty) then
		if (inData.group) then
			if (inAction:savingThrow()) then
				text:ctdprint(false, true, 
					" but it had no effect!\n\n"
					)
				party:display()
				return false
			end

			inTarget:addBattleBonus("acPenalty", inData.acAmount,
					inData.acStack)
		else
			-- No single AC penalty spells in BT1
			local none = true
		end
	end

	----------------------------------------
	-- Damage Bonus
	----------------------------------------
	if (inData.damageBonus) then
		if (inData.group) then
			if (inData.damageRandom) then
				party:addBattleBonus("damageRandom",
						inData.damageAmount,
						inData.damageStack)
			else
				inTarget:addBattleBonus("damageBonus", 
						inData.damageAmount,
						inData.damageStack)
			end
		else
			if (inData.damageRandom) then
				inTarget:addBattleBonus("damageRandom",
						inData.damageAmount,
						inData.damageStack)
			else
				inTarget:addBattleBonus("damageBonus",
						inData.damageAmount,
						inData.damageStack)
			end
		end
	end

	if (inData.damagePenalty) then
		if (inData.group) then
			inTarget:addBattleBonus("damagePenalty",
						inData.damageAmount,
						inData.damageStack)
		else
			local none = true
		end
	end

	if (inData.toHitBonus) then
		if (inData.group) then
			party:addBattleBonus("toHitBonus",
						inData.toHitAmount,
						inData.toHitStack)
		else
			inTarget:addBattleBonus("toHitBonus",	
						inData.toHitAmount,
						inData.toHitStack)
		end
	end

	if (inData.toHitPenalty) then
		if (inData.group) then
			inTarget:addBattleBonus("toHitPenalty",
						inData.toHitAmount,
						inData.toHitStack)
		else
			local none = true
		end
	end

	if (updateParty) then
		party:display()
	end

	return true
end


























