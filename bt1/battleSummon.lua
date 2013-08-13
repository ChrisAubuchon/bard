battleSummon	= {}

function battleSummon:doAction(inAction)
	local inBattle	= inAction.inBattle
	local attack

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
	dprint("battleSummon:doAction() called")
end

function battleSummon:meleeAttack(inAction)
	local xxx_summon_meleeAttack = true
end

function battleSummon:checkMeleeHits(inAction)
	local xxx_checkMeleeHits = true
end

function battleSummon:getMeleeDamage(inAction)
	local xxx_getMeleeDamage = true
end
	

function battleSummon:doDamage(inAction)
	local xxx_doDamage = true
end
