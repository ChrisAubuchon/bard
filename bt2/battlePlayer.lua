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
-- battlePlayer:getBattleAction()
----------------------------------------
function battlePlayer:getBattleAction()
	local inkey
	local options = {}

	table.setDefault(options,false)

	if (self.isPossessed or self.isNuts) then
		self.action.action = "possessedAttack"
		return
	end

	while true do
		self:generateBattleOptions(options)

		repeat
			inkey = getkey()
		until (options[inkey])

		if (self:readBattleOption(inkey)) then
			return
		end
	end

	error("Never reached")
end

----------------------------------------
-- battlePlayer:generateBattleOptions()
----------------------------------------
function battlePlayer:generateBattleOptions(inOptions)
	text:clear()
	text:print("%s has these options this battle round:\n\n", self.name)

	text:print("Party attack\n")
	inOptions.P = true

	if ((not currentBattle.isPartyAttack) and (self.inMeleeRange)) then
		text:print("Attack foes\n")
		inOptions.A = true
	end

	text:print("Defend\n")
	inOptions.D = true

	if (self.currentSppt > 0) then
		text:print("Cast a spell\n")
		inOptions.C = true
	end

	text:print("Use an item\n")
	inOptions.U = true

	self.isHiding = false
	if (self.class == "Rogue") then
		text:print("Hide in shadows\n")
		inOptions.H = true
	end

	if ((self.class == "Bard") and (self:isTypeEquipped("Instrument"))) then
		text:print("Bard Song\n")
		inOptions.B = true
	end

	text:print("\nSelect an option.")
end

----------------------------------------
-- battlePlayer:readBattleOption()
----------------------------------------
function battlePlayer:readBattleOption(inkey)
	if (inkey == "A") then
		self.action.action = "melee"
		if (self:getMeleeTarget()) then
			return true
		end
	elseif (inkey == "B") then
		self.action.action = "sing"
		if (self:getBattleTune()) then
			return true
		end
	elseif (inkey == "C") then
		self.action.action = "cast"
		if (self:getCombatSpell()) then
			return true
		end
	elseif (inkey == "D") then
		self.action.action = "defend"
		return true
	elseif (inkey == "H") then
		self.action.action = "hide"
		self:hideInShadows()
		return true
	elseif (inkey == "P") then
		self.action.action = "partyAttack"
		if (self:getMeleeTarget()) then
			return true
		end
	elseif (inkey == "U") then
		self.action.action = "use"
		if (self:getUseItem()) then
			return true
		end
	end

	return false
end

----------------------------------------
-- battlePlayer:getMeleeTarget()
----------------------------------------
function battlePlayer:getMeleeTarget()
	local action = self.action
	if (currentBattle.isPartyAttack or action.action == "partyAttack") then
		action.action = "melee"
		text:cdprint(true, false, "Attack:")
		action.target = self:getActionTarget({party = true})
		if (not action.target) then
			return false
		end
	else
		if (currentBattle.monParty.size > 1) then
			text:cdprint(true, false, "Attack:")
		end

		action.target = self:getActionTarget({melee = true})
		if (not action.target) then
			return false
		end
	end

	return true
end

----------------------------------------
-- doAction()
----------------------------------------
function battlePlayer:doAction()
	if (self:isDisabled()) then
		return
	end

	log:print(log.LOG_DEBUG, "Action type: %s", self.action.action)

	if (self.action.action == "melee") then
		log:print(log.LOG_DEBUG, "Performing melee attack")
		self:doMeleeAttack()
	elseif (self.action.action == "cast") then
		log:print(log.LOG_DEBUG, "Performing cast a spell")
		self:doCombatSpell()
	elseif (self.action.action == "defend") then
		log:print(log.LOG_DEBUG, "Source: " .. tostring(self.name) .. " defends")
		return
	elseif (self.action.action == "use") then
		log:print(log.LOG_DEBUG, "Using an item")
		self:useItem()
	elseif (self.action.action == "sing") then
		log:print(log.LOG_DEBUG, "Singing a bard song")
		self:doBardSong()
	elseif (self.action.action == "possessedAttack") then
		local attackParty	= false

		-- A nuts attack randomly attacks player or monster
		if (self.isNuts) then
			if (random:band(1) == 1) then	
				attackParty = true
			end
		else
			attackParty = self.possessed
		end

		if (attackParty) then
			self.action.target = party:randomCharacter()
		else
			if (currentBattle.monParty) then
				self.action.target=currentBattle.monParty:getLeadGroup()
			else
				-- This should probably attack a random
				-- party member. In the DOS version it does
				-- nothing.
				--
				return
			end
		end

		self:doMeleeAttack()
	end
end

----------------------------------------
-- getMeleeAttackString()
----------------------------------------
function battlePlayer:getMeleeAttackString()
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
				self.action.target:getTargetString()
				)
end

----------------------------------------
-- getMeleeDamage()
----------------------------------------
function battlePlayer:getMeleeDamage()
	local target		= self.action.target
	local damage		= target.action.damage
	local damageBonus	= 0
	local weapon
	local ndice
	local dieValue
	local i

	damage.amount = 0
	damage.source = self

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
		damage.specialAttack = weapon.sp_attack
	end

	if (self.st > 15) then
		damageBonus = damageBonus + (self.st - 15)
	end

	for i = 1,self.numAttacks do
		damage.amount = damage.amount + damageBonus
		damage.amount = damage.amount + random:xdy(ndice, dieValue)
		damage.amount = damage.amount + self.damageBonus
		damage.amount = damage.amount + random:xdy(self.damageRandom,8)
		damage.amount = damage.amount - self.damagePenalty
	end

	if (self.class == "Hunter") then
		if (random:xdy(1,256) < self.rogu_level) then
			damage.specialAttack = "critical"
		end
	end
end


----------------------------------------
-- getCombatSpell()
----------------------------------------
function battlePlayer:getCombatSpell()
	local s
	local action	= self.action

	s = self:getSpell(false)
	if (not s) then
		return false
	end

	if (not s.combat) then
		text:cdprint(false, true, "\nNot a combat spell.")
		return false
	end

	if (self.currentSppt < s.sppt) then
		text:cdprint(false, true, "\nNot enough spell points.")
		return false
	end

	action.func		= s.action.func
	action.inData		= s.action.inData
	action.inData.sppt	= s.sppt

	if (s.targetted) then
		text:cdprint(true, false, "Use on:")
		action.target = self:getActionTarget(s.targetted) 
		if (not action.target) then
			return false
		end
	end

	return true
end

----------------------------------------
-- doCombatSpell()
----------------------------------------
function battlePlayer:doCombatSpell()
	text:print(self.name)
	self:castSpell()
end

----------------------------------------
-- doUseItem()
----------------------------------------
function battlePlayer:doUseItem()
	local inData	= self.action.inData
	local target	= self.action.target
	local item	= inData.item

	local xxx_wtf_is_use_doUse = true
	use.doUse(self.action)
end

----------------------------------------
-- battlePlayer:getBattleTune()
----------------------------------------
function battlePlayer:getBattleTune()
	local tune

	tune = song:getTune()
	if (not tune) then
		return false
	end

	self.action.func = tune

	return true
end

----------------------------------------
-- battlePlayer:doBardSong()
----------------------------------------
function battlePlayer:doBardSong()
	text:print(self.name)

	if (not self:doVoiceCheck()) then
		return
	end

	if (not self:isTypeEquipped("Instrument")) then
		text:cdprint(false, true, "\n has no instrument to play.")
		return
	end

	text:print(" plays a tune...\n\n")

	song:activateCombatSong(self.action)
	party:display()
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
function battlePlayer:attackSpell()
	local inData		= self.action.inData
	local outData		= self.action.outData
	local source		= self.action.source
	local target		= self.action.target
	local damage		= target.action.damage

	log:print(log.LOG_DEBUG, "battlePlayer:attackSpell()")
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
			self.action.target = mgroup
			self.action:multiTargetSpell()
			if (not mgroup:isLast()) then
				text:print("and")
			end
		end
	elseif (inData.group) then
		self.action:multiTargetSpell()
	else
		-- Single target spell
		--
		if (inData.levelMultiply) then
			local i
			damage.amount = 0
			for i = 1,inData.ndice do
				damage.amount = damage.amount +
					random:xdy(self.cur_level + 1, inData.dieval)
			end
		elseif (inData.specialAttack) then
			damage.specialAttack = inData.specialAttack
			damage.amount = 0
		else
			damage.amount = random:xdy(inData.ndice, inData.dieval)
		end

		self.action:singleTargetSpell()
	end
end


----------------------------------------
-- battlePlayer:hideInShadows()
----------------------------------------
function battlePlayer:hideInShadows()
	if (self:isEffectEquipped("hasAlwaysHide")) then
		self.isHiding = true
		return 
	end

	if (random:xdy(1,65535) > self.rogu_level) then
		self.isHiding = true
		return 
	end

	self.isHiding = false
end

----------------------------------------
-- battlePlayer:getBattlePriority()
----------------------------------------
function battlePlayer:getBattlePriority()
	local priority
	local levelBonus

	priority = bit32.rshift(self.battlesWon, 9)
	if (self.dx > 14) then
		priority = priority + bit32.lshift(self.dx - 14, 3)
	end
	priority = priority + (random:rnd() % 32)

	levelBonus = bit32.rshift(self.cur_level, 1)
	if ((self.class == "Conjurer") or
	    (self.class == "Magician") or
	    (self.class == "Sorcerer") or
	    (self.class == "Wizard") or
	    (self.class == "Archmage")) then
		levelBonus = bit32.rshift(levelBonus, 2)
	elseif ((self.class == "Rogue") or
		(self.class == "Bard")) then
		levelBonus = bit32.rshift(levelBonus, 1)
	elseif (self.class == "Monk") then
		levelBonus = bit32.lshift(levelBonus, 1)
	end

	priority = priority + levelBonus
	if (priority <= 0) then
		return 1
	elseif (priority > 255) then
		return 255
	else
		return priority
	end
end



