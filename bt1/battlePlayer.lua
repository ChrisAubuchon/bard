battlePlayer	= {}

local monkMeleeDamage = {
	{ ndice = 1, dieval = 4 },
	{ ndice = 2, dieval = 4 },
	{ ndice = 3, dieval = 4 },
	{ ndice = 4, dieval = 4 },
	{ ndice = 5, dieval = 4 },
	{ ndice = 4, dieval = 8 },
	{ ndice = 4, dieval = 8 },
	{ ndice = 5, dieval = 8 },
	{ ndice = 5, dieval = 8 },
	{ ndice = 6, dieval = 8 },
	{ ndice = 6, dieval = 8 },
	{ ndice = 6, dieval = 8 },
	{ ndice = 6, dieval = 8 },
	{ ndice = 7, dieval = 8 },
	{ ndice = 7, dieval = 8 },
	{ ndice = 7, dieval = 8 },
	{ ndice = 5, dieval = 16 },
	{ ndice = 5, dieval = 16 },
	{ ndice = 5, dieval = 16 },
	{ ndice = 5, dieval = 16 },
	{ ndice = 6, dieval = 16 },
	{ ndice = 6, dieval = 16 },
	{ ndice = 6, dieval = 16 },
	{ ndice = 6, dieval = 16 },
	{ ndice = 4, dieval = 32 },
	{ ndice = 4, dieval = 32 },
	{ ndice = 4, dieval = 32 },
	{ ndice = 5, dieval = 32 },
	{ ndice = 5, dieval = 32 },
	{ ndice = 5, dieval = 32 },
	{ ndice = 6, dieval = 32 },
	{ ndice = 7, dieval = 32 }
}

function battlePlayer:doAction(inAction)
	if (self:isDisabled()) then
		return
	end


	if (inAction.action == "melee") then
		dprint("Performing melee attack")
		self:doMeleeAttack(inAction)
	elseif (inAction.action == "cast") then
		dprint("Performing cast a spell")
		self:doCombatSpell(inAction)
	elseif (inAction.action == "defend") then
		dprint("Source: " .. tostring(inAction.source.name) .. " defends")
		return
	elseif (inAction.action == "use") then
		dprint("Using an item")
		self:useItem(inAction)
	elseif (inAction.action == "sing") then
		dprint("Singing a bard song")
		self:doBardSong(inAction)
	elseif (inAction.action == "possessedAttack") then
		local attackParty	= false
		local inBattle		= inAction.inBattle

		-- A nuts attack randomly attacks player or monster
		if (self.isNuts) then
			if (rnd_and_x(1) == 1) then	
				attackParty = true
			end
		else
			attackParty = self.possessed
		end

		if (attackParty) then
			inAction.target = party:randomCharacter()
		else
			if (inBattle.monParty) then
				inAction.target=inBattle.monParty:getLeadGroup()
			else
				-- This should probably attack a random
				-- party member. In the DOS version it does
				-- nothing.
				--
				return
			end
		end

		self:doMeleeAttack(inAction)
	end
end

function battlePlayer:doMeleeAttack(inAction)
	local target	= inAction.target
	local inData	= inAction.inData
	local outData	= inAction.outData

	if (target:isSummon()) then
		local xxx_isIllusion_check = true
		party.summon.isHostile = true
	end

	if (target:isCharacter() and target:isDisabled()) then
		return	
	end

	if (target.size == 0) then
		dprint("Target group is gone")
		return
	end
	dprint("target: " .. tostring(target))

	text:print(self:getSingularName())
	if (self:isTypeEquipped("Weapon")) then
		text:print(" swings at ")
	else
		text:print(" chops at ")
	end

	text:print(target:getSingularName())

	if (not self:checkMeleeHits(inAction)) then
		text:print(", but misses!\n\n")
		timer:delay(3)
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

	timer:delay(3)
end

function battlePlayer:checkMeleeHits(inAction)
	local target	= inAction.target
	local inBattle	= inAction.inBattle
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
	sourceAttack = sourceAttack - classes.get(self.class, "meleeBonus")
	sourceAttack = sourceAttack - inBattle.songToHitBonus
	sourceAttack = sourceAttack + self.toHitPenalty

	if (sourceAttack >  10) then sourceAttack =  10 end
	if (sourceAttack < -10) then sourceAttack = -10 end

	if (sourceAttack > targetAC) then
		return false
	end

	return true
end

function battlePlayer:getMeleeDamage(inAction)
	local target		= inAction.target
	local outData		= inAction.outData
	local inBattle		= inAction.inBattle
	local damageBonus	= 0
	local weapon
	local ndice
	local dieValue
	local i

	outData.damage = 0

	weapon = self:isTypeEquipped("Weapon")
	if (not weapon) then
		if (self.class == "Monk") then
			i = bit32.rshift(self.cur_level, 1) + 1
			if (i > 31) then	
				i = 31
			end
			ndice = monkMeleeDamage[i].ndice
			dieValue = monkMeleeDamage[i].dieval
		else
			ndice = 1
			dieValue = 4
		end
	else
		ndice = weapon.ndice
		dieValue = weapon.die
		damageBonus = weapon.dmg_bonus
		outData.specialAttack = weapon.sp_attack
	end

	if (self.st > 15) then
		damageBonus = damageBonus + (self.st - 15)
	end

	for i = 1,self.numAttacks do
		outData.damage = outData.damage + damageBonus
		outData.damage = outData.damage + rnd_xdy(ndice, dieValue)
		outData.damage = outData.damage + self.damageBonus
		outData.damage = outData.damage + rnd_xdy(self.damageRandom,8)
		outData.damage = outData.damage - self.damagePenalty
	end

	if (self.class == "Hunter") then
		if (rnd_xdy(1,256) < self.rogu_level) then
			outData.specialAttack = "critical"
		end
	end
end


function battlePlayer:doDamage(inAction)
	local inData	= inAction.inData
	local outData	= inAction.outData

	if (self.isDead) then
		if (inData.specialAttack ~= "possess") then
			return false
		end
	end

	if (self.isStoned) then
		return false
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

	self:inflictStatus(inAction)

	party:isLive()

	return true
end

function battlePlayer:inflictStatus(inAction)
	local inData	= inAction.outData

	if (inData.specialAttack == "poison") then
		self.isPoisoned = true
	elseif (inData.specialAttack == "drain") then
		if (self.cur_level ~= 1) then
			self.cur_level = self.cur_level - 1
			self.xp = classes.getXpForLevel(self.class, 
							self.cur_level)
		end
		return true
	elseif (inData.specialAttack == "nuts") then
		self.isNuts = true
	elseif (inData.specialAttack == "old") then
		if (self:isSummon()) then
			return
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
		self.cur_hp = 100

		-- self.possessed indicates whether or not a monster
		-- has possessed the player
		--
		if (inAction.source:isMonster()) then
			self.possessed = true
		end
		self.isPossessed	= true
		self.isDead		= false
		self.isPoisoned		= false
		self.isOld		= false
		self.isStoned		= false
		self.isNuts		= false
		self.isParalyzed	= false
	elseif (inData.specialAttack == "stone") then
		self.cur_hp		= 0
		self.isStoned		= true
		self:songTimeout()
	elseif (inData.specialAttack == "critical") then
		self.cur_hp		= 0
		self.possessed		= false
		self.isDead		= true
		self:songTimeout()
	end
end

function battlePlayer:getCombatSpell(inAction)
	local s

	s = self:getSpell(false)
	if (not s) then
		return false
	end

	if (not s.combat) then
		text:cdprint(false, true, "\nNot a combat spell.")
		return false
	end

	if (self.cur_sppt < s.sppt) then
		text:cdprint(false, true, "\nNot enough spell points.")
		return false
	end

	inAction.func		= s.action.func
	inAction.inData		= s.action.inData
	inAction.inData.sppt	= s.sppt

	if (s.targetted) then
		text:cdprint(true, false, "Use on:")
		inAction.target = getActionTarget(s.targetted, 
					inAction.inBattle.monParty)
		if (not inAction.target) then
			return false
		end
	end

	return true
end

function battlePlayer:doCombatSpell(inAction)
	text:print(self.name)
	self:castSpell(inAction)
end

function battlePlayer:doUseItem(inAction)
	local inData	= inAction.inData
	local target	= inAction.target
	local item	= inData.item

	use.doUse(inAction)
end

function battlePlayer:doBardSong(inAction)
	text:print(self.name)

	if (not self:doVoiceCheck()) then
		return
	end

	if (not self:isTypeEquipped("Instrument")) then
		text:cdprint(false, true, "\n has no instrument to play.")
		return
	end

	text:print(" plays a tune...\n\n")

	if (inAction.func) then
		inAction.func(inAction)
		party:display()
	end
end

local paladinSavingBonus = {
	1,1,2,2,2,3,3,3,3,4,4,4,4,5,5,6
}

function battlePlayer:calculateSavingThrow()
	local value = 0

	value = bit32.rshift(self.cur_level, 1)
	if (value > 18) then
		value = 18
	end

	if (self:isEffectEquipped("hasAlwaysHide")) then
		value = value + 2
	end

	if (self.class == "Paladin") then
		local plevel = bit32.rshift(self.cur_level, 1)
		if (plevel > 15) then
			plevel = 15
		end
		value = value + paladinSavingBonus[plevel]
	else
		value = value + classes.get(self.class, "saveBonus")
	end

	if (self.lk > 15) then
		value = value + (self.lk - 15)
	end

	return value
end

----------------------------------------
-- battleBonus()
----------------------------------------
function battlePlayer:battleBonus(inAction)
	local inData		= inAction.inData
	local inBattle		= inAction.inBattle
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
				text:cdprint(false, true, " but it had no effect!\n\n")
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

function battlePlayer:mageStar(inAction)
	inAction.target.missTurn = true
end

function battlePlayer:attackSpell(inAction)
	local inData		= inAction.inData
	local outData		= inAction.outData
	local source		= inAction.source
	local target		= inAction.target
	local inBattle		= inAction.inBattle

	dprint("battlePlayer:attackSpell()")
	-- printEllipsis if a group or allFoes spell was cast
	-- and all of the monster groups are dead.
	--
	if ((inData.group and target.size == 0) or
	    (inData.allFoes and not inBattle.monParty:isAlive())) then
		text:printEllipsis()
		return
	end

	-- Fizzle the spell if the target is a character or doesn't
	-- match the repel type of the spell
	--
	if (next(inData.repel)) then
		local repelType

		repelType = next(inData.repel)
		if (target:isCharacter() or not target.repel[repelType]) then
			text:print(" but it fizzles!\n\n")
			party:display()
			timer:delay(3)
			return
		end
	end

	if (inData.allFoes) then
		local mgroup

		for mgroup in inBattle.monParty:iterator() do
			inAction.target = mgroup
			inAction:multiTargetSpell()
			--self:multipleTargetSpell(inAction)
			if (not mgroup:isLast()) then
				text:print("and")
			end
		end
	elseif (inData.group) then
		--self:multipleTargetSpell(inAction)
		inAction:multiTargetSpell()
	else
		-- Single target spell
		--
		if (inData.levelMultiply) then
			local i
			outData.damage = 0
			for i = 1,inData.ndice do
				outData.damage = outData.damage +
					rnd_xdy(self.cur_level + 1, inData.dieval)
			end
		elseif (inData.specialAttack) then
			outData.specialAttack = inData.specialAttack
			outData.damage = 0
		else
			outData.damage = rnd_xdy(inData.ndice, inData.dieval)
		end

		inAction:singleTargetSpell()
	end
end

function battlePlayer:multipleTargetSpell(inAction)
	local inData		= inAction.inData
	local outData		= inAction.outData
	local target		= inAction.target
	local lastDamage	= 0
	local m

	text:print(" at %s %s...\n\n",
		pluralize(target.size, "a", "some"),
		pluralize(target.size, target.singular, 
					target.plural)
		)

	for m in target:iterator() do
		local save
		local half

		-- Prevent consecutive targets from receiving
		-- the same damage.
		--
		repeat
			outData.damage = rnd_xdy(inData.ndice, inData.dieval)
		until (outData.damage ~= lastDamage)
		lastDamage = outData.damage

		text:print("One")
		save, half = inAction:savingThrow()
		if (save and not half) then
			text:print(" repelled the spell!\n\n")
		else
			if (half) then 
				bit32.rshift(outData.damage, 1)
			end
			text:print(" is %s ", 
				stringTables.isEffects[inData.atype])
			inAction:printDamage()
			inAction:doDamage()
		end
		timer:delay(3)
	end
end









