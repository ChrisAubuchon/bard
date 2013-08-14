battleSummon	= {}

function battleSummon:doAction(inAction)
	local inBattle	= inAction.inBattle
	local attack

	dprint("battleSummon:doAction() called")
	if (self.isIllusion and (not inBattle.isPartyAttack)) then
		if (inBattle.monParty.disbelieve) then
			dprint("Skipping summon turn: disbelieve")
			return
		end
	end
	local xxx_summon_doAction = true

	local function rndAttack()
		local a

		for a in self:attackIterator() do
			return a
		end
	end

	attack = rndAttack()

	dprint(attack.type)
	if (attack.type == "melee") then
		if (self.isHostile) then
			inAction.target = party:randomCharacter()
		elseif (not inBattle.isPartyAttack) then
			inAction.target = inBattle.monParty:getLeadGroup()
		end

		self:meleeAttack(inAction)
	end
end

function battleSummon:meleeAttack(inAction)
	local inData		= inAction.inData
	local xxx_summon_meleeAttack = true

	if (not inAction:validateTarget) then
		return
	end

	text:print("%s%s%s", 
		self:singular,
		inData.meleeString[rnd_xdy(1,2)],
		inAction.target:getSingularName()
		)

	if (not self:checkMeleeHits(inAction)) then
		text:print(", but misses!\n")
		timer:delay(3)
		return
	else
	end
end

function battleSummon:checkMeleeHits(inAction)
	local target		= inAction.target
	local inBattle		= inAction.inBattle
	local targetAC
	local sourceAttack
	local bonus

	local xxx_summon_checkMeleeHits = true

	targetAC = target.ac
	if (not target:isCharacter()) then
		targetAC = targetAC + target.acPenalty
		if (targetAC > 10) then
			targetAC = 10
		end
	end

	sourceAttack = self.ac
end

function battleSummon:getMeleeDamage(inAction)
	local xxx_getMeleeDamage = true
end
	

function battleSummon:doDamage(inAction)
	local xxx_doDamage = true
end
