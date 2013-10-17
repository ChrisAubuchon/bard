----------------------------------------
-- battlePlayer class
--
-- Methods specific to characters
----------------------------------------
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

----------------------------------------
-- doAction()
----------------------------------------
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
			if (currentBattle.monParty) then
				inAction.target=currentBattle.monParty:getLeadGroup()
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

----------------------------------------
-- getMeleeAttackString()
----------------------------------------
function battlePlayer:getMeleeAttackString(inAction)
	local function getWeaponString()
		if (self:isTypeEquipped("Weapon")) then
			return " swings at "
		else
			return " chops at "
		end
	end

	return string.format("%s%s%s",
				self.name,
				getWeaponString(),
				inAction.target:getTargetString()
				)
end

----------------------------------------
-- getMeleeDamage()
----------------------------------------
function battlePlayer:getMeleeDamage(inAction)
	local target		= inAction.target
	local outData		= inAction.outData
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


----------------------------------------
-- getCombatSpell()
----------------------------------------
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
		inAction.target = self:getActionTarget(s.targetted) 
		if (not inAction.target) then
			return false
		end
	end

	return true
end

----------------------------------------
-- doCombatSpell()
----------------------------------------
function battlePlayer:doCombatSpell(inAction)
	text:print(self.name)
	self:castSpell(inAction)
end

----------------------------------------
-- doUseItem()
----------------------------------------
function battlePlayer:doUseItem(inAction)
	local inData	= inAction.inData
	local target	= inAction.target
	local item	= inData.item

	use.doUse(inAction)
end

----------------------------------------
-- battlePlayer:getBattleTune()
----------------------------------------
function battlePlayer:getBattleTune(inAction)
	local tune

	tune = self:getTune()
	if (not tune) then
		return false
	end

	inAction.func = tune.combatFunction.func
	inAction.inData = tune.combatData[currentLevel.level].inData

	return true
end

----------------------------------------
-- doBardSong()
----------------------------------------
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
		timer:delay()
	end
end

local paladinSavingBonus = {
	1,1,2,2,2,3,3,3,3,4,4,4,4,5,5,6
}

----------------------------------------
-- calculateSavingThrow()
----------------------------------------
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
		value = value + self.saveBonus
	end

	if (self.lk > 15) then
		value = value + (self.lk - 15)
	end

	return value
end

----------------------------------------
-- attackSpell()
----------------------------------------
function battlePlayer:attackSpell(inAction)
	local inData		= inAction.inData
	local outData		= inAction.outData
	local source		= inAction.source
	local target		= inAction.target

	dprint("battlePlayer:attackSpell()")
	-- printEllipsis if a group or allFoes spell was cast
	-- and all of the monster groups are dead.
	--
	if ((inData.group and target.size == 0) or
	    (inData.allFoes and not currentBattle.monParty:isAlive())) then
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
			text:ctdprint(false, true, " but it fizzles!\n\n")
			party:display()
			return
		end
	end

	if (inData.allFoes) then
		local mgroup

		for mgroup in currentBattle.monParty:iterator() do
			inAction.target = mgroup
			inAction:multiTargetSpell()
			if (not mgroup:isLast()) then
				text:print("and")
			end
		end
	elseif (inData.group) then
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










