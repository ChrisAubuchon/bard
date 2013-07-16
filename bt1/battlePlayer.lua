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
		text:print("for %d ", outData.damage)
		if (outData.damage == 1) then
			text:print("point ")
		else
			text:print("points ")
		end
		text:print("of damage")

		if (target:doDamage(inAction)) then
			text:print(stringTables.effects[outData.specialAttack])
			text:print("%s!\n\n", target:getPronoun())
		else
			text:print(".\n\n")
		end

		if (target:isCharacter()) then
			party:display()
		end

		if (not party:isLive()) then
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
		targetAC = targetAC + inBattle.freezeFoePenalty[target.key]
	end

	sourceAttack = self.ac
	sourceAttack = sourceAttack - classes.get(self.class, "meleeBonus")
	sourceAttack = sourceAttack - inBattle.songToHitBonus
	sourceAttack = sourceAttack + inBattle.fearPenalty[self.key]

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
		outData.damage = outData.damage +
			inBattle.battleSkillBonus[self.key]
		outData.damage = outData.damage +
			rnd_xdy(inBattle.vorpalBonus[self.key], 8)
		outData.damage = outData.damage - inBattle.fearPenalty[self.key]
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

	if (self.cur_hp > outData.damage) then
		self.cur_hp = self.cur_hp - outData.damage
	else
		self.cur_hp = 0
		self.possessed = false
		self.isDead = true

		self:songTimeout()

		outData.specialAttack = "kill"

		return true
	end

	if (not outData.specialAttack) then
		return false
	end

	self:inflictStatus(inAction)

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
		inAction.target = getActionTarget(s.targetted, false)
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













