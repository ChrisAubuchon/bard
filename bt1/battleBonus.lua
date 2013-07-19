battleBonus = {
	antiMagic	= 0,
	acBonus		= 0,
	acPenalty	= 0,
	toHitBonus	= 0,
	toHitPenalty	= 0,
	damageRandom	= 0,
	damageBonus	= 0,
	damagePenalty	= 0,
}

function battleBonus:resetBattleBonus()
	self.antiMagic		= 0
	self.acBonus		= 0
	self.acPenalty		= 0
	self.toHitBonus		= 0
	self.toHitPenalty	= 0
	self.damageBonus	= 0
	self.damagePenalty	= 0
end

function battleBonus:addBattleBonus(inStat, inAmount, inStack)
	if ((self[inStat] == 0) or (not inStack)) then
		self[inStat] = inAmount
	else
		self[inStat] = self[inStat] + inAmount
	end
end

function battleBonus:dumpBattleBonus()
	dprint("antiMagic: %d", self.antiMagic)
	dprint("acBonus: %d", self.acBonus)
	dprint("acPenalty: %d", self.acPenalty)
	dprint("toHitBonus: %d", self.toHitBonus)
	dprint("toHitPenalty: %d", self.toHitPenalty)
	dprint("damageRandom: %d", self.damageRandom)
	dprint("damageBonus: %d", self.damageBonus)
	dprint("damagePenalty: %d", self.damagePenalty)
end
