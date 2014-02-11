battleBonus = {
	antiMagic	= 0,
	acBonus		= 0,
	acPenalty	= 0,
	toHitBonus	= 0,
	toHitPenalty	= 0,
	damageRandom	= 0,
	damageBonus	= 0,
	damagePenalty	= 0,
	hpRegen		= 0,
	missTurn	= false,
	disbelieve	= false,
	extraAttacks	= false
}

function battleBonus:new()
	local self = object:new()

	self:addSelf(battleBonus)

	return self
end

----------------------------------------
-- battleBonus:resetBattleBonus()
----------------------------------------
function battleBonus:resetBattleBonus()
	self.antiMagic		= 0
	self.acBonus		= 0
	self.acPenalty		= 0
	self.toHitBonus		= 0
	self.toHitPenalty	= 0
	self.damageRandom	= 0
	self.damageBonus	= 0
	self.damagePenalty	= 0
	self.hpRegen		= 0
	self.missTurn		= false
	self.disbelieve		= false
	self.extraAttacks	= false
end

----------------------------------------
-- battleBonus:addBattleBonus()
----------------------------------------
function battleBonus:addBattleBonus(inStat, inAmount, inStack, inMax)
	log:print(log.LOG_DEBUG, "inStat: %s\tinAmount: %d\t inStack: %s", 
		inStat, inAmount, inStack)

	if ((self[inStat] == 0) or (not inStack)) then
		self[inStat] = inAmount
	else
		self[inStat] = self[inStat] + inAmount
	end

	if (inMax) then
		if (self[inStat] > inMax) then
			self[inStat] = inMax
		end
	end
end

function battleBonus:dumpBattleBonus()
	log:print(log.LOG_DEBUG, "antiMagic: %d", self.antiMagic)
	log:print(log.LOG_DEBUG, "acBonus: %d", self.acBonus)
	log:print(log.LOG_DEBUG, "acPenalty: %d", self.acPenalty)
	log:print(log.LOG_DEBUG, "toHitBonus: %d", self.toHitBonus)
	log:print(log.LOG_DEBUG, "toHitPenalty: %d", self.toHitPenalty)
	log:print(log.LOG_DEBUG, "damageRandom: %d", self.damageRandom)
	log:print(log.LOG_DEBUG, "damageBonus: %d", self.damageBonus)
	log:print(log.LOG_DEBUG, "damagePenalty: %d", self.damagePenalty)
	log:print(log.LOG_DEBUG, "hpRegen: %d", self.hpRegen)
end
