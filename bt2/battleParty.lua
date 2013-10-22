----------------------------------------
-- battleParty class
--
-- Class methods that are common between
-- characters and summoned monsters
----------------------------------------
battleParty	= {
	advance		= false
}

----------------------------------------
-- doMeleeAttack()
----------------------------------------
function battleParty:doMeleeAttack(inAction)
	local target	= inAction.target
	local inData	= inAction.inData
	local outData	= inAction.outData

	if (not inAction:validateTarget()) then
		return
	end

	text:print(self:getMeleeAttackString(inAction))

	if (not self:checkMeleeHits(inAction)) then
		text:ctdprint(false, true, ", but misses!\n\n")
		return
	else
		text:print(", and hits ")
		if (self.numAttacks > 1) then
			text:print("%d times ", self.numAttacks)
		end

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

	timer:delay()
end

----------------------------------------
-- checkMeleeHits()
----------------------------------------
function battleParty:checkMeleeHits(inAction)
	local target	= inAction.target
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
	if (sourceAttack < -10) then sourceAttack = -10 end

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

----------------------------------------
-- doDamage()
----------------------------------------
function battleParty:doDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData
	local rval

	if (self.isDead) then
		if (inData.specialAttack ~= "possess") then
			return false
		end
	end

	if (self.isStoned) then
		return false
	end

	if (inData.specialAttack) then
		outData.specialAttack = inData.specialAttack
	end

	if (outData.damage > 0) then
		if (self.cur_hp > outData.damage) then
			self.cur_hp = self.cur_hp - outData.damage
		else
			self.cur_hp = 0
			self.possessed = false
			self.isDead = true

			self:songTimeout()

			outData.specialAttack = "kill"

			party:isLive()

			return true
		end
	end

	if (not outData.specialAttack) then
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
	local inData	= inAction.outData

	if (inData.specialAttack == "poison") then
		if (not self.isDestinyKnight) then
			self.isPoisoned = true
		end
	elseif (inData.specialAttack == "drain") then
		if (self:isSummon()) then
			return
		end
		if (self.cur_level ~= 1) then
			self.cur_level = self.cur_level - 1
			self.xp = self:getXpForLevel(self.cur_level)
		end
		return true
	elseif (inData.specialAttack == "nuts") then
		if (not self.isDestinyKnight) then
			self.isNuts = true
		end
	elseif (inData.specialAttack == "old") then
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
	elseif (inData.specialAttack == "possess") then
		if (not inAction.source:isMonster()) then
			if (self:hasItem("Kato's Bracer")) then
				self.isDocile = true
			end
		end
		if (self.isDestinyKnight) then
			return true
		end
		self.cur_hp = 100
		self.isPossessed	= true
		self.isDead		= false
		self.isPoisoned		= false
		self.isOld		= false
		self.isStoned		= false
		self.isNuts		= false
		self.isParalyzed	= false
	elseif (inData.specialAttack == "stone") then
		if (self.isDestinyKnight) then
			return true
		end
		self.cur_hp		= 0
		self.isStoned		= true
		self:songTimeout()
	elseif ((inData.specialAttack == "critical") or 
		(inData.specialAttack == "kill")) then
		if (self.isDestinyKnight) then
			self.cur_hp = self.max_hp
		else
			self.cur_hp		= 0
			self.possessed		= false
			self.isDead		= true
		end
		self:songTimeout()
	elseif (inData.specialAttack == "unequip") then
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
		party.antiMagic = party.antiMagic + 2
		if (party.antiMagic > 6) then
			party.antiMagic = 6
		end
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

----------------------------------------
-- mageStar()
----------------------------------------
function battleParty:mageStar(inAction)
	inAction.target.missTurn = true
end


























