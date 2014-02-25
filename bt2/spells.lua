spells = {}

require "spell_apar"

local spellList = {}


----------------------------------------
-- Constant effect spells
----------------------------------------
function spells:light(inAction)
	party.light:activate(inAction.inData)
	text:printEllipsis()
end

function spells:levitate(inAction)
	party.levitate:activate(inAction.inData)
	text:printEllipsis()
end

function spells:detect(inAction)
	party.detect:activate(inAction.inData)
	text:printEllipsis()
end

function spells:shield(inAction)
	party.shield:activate(inAction.inData)
	text:printEllipsis()
end

function spells:compass(inAction)
	party.compass:activate(inAction.inData)
	party.compass:update(currentLevel.direction)
	text:printEllipsis()
end

----------------------------------------
-- spells:trapZap()
----------------------------------------
function spells:trapZap()
	if (not currentLevel:isDungeon()) then
		text:printEllipsis()
		return
	end

	local frontDir = directions.front[currentLevel.direction]
	local sq = currentLevel.currentSquare
	local i

	for i = 1,3 do
		sq = sq[frontDir].path
		if (sq.isTrap) then
			sq.isTrap = false
		end
	end
		
end

----------------------------------------
-- spells.scrySite()
----------------------------------------
function spells:scrySite()
	local outString = "You face "

	text:cdprint(true, false, "You face %s", currentLevel.direction)

	if (currentLevel.name == "wild") then
		local x
		local y

		x,y = currentLevel.currentSquare.toCoordinates()
		text:csplash(false, true, 
			", %d square%s north\n %d square%s east of the sages hut.",
			y,
			string.pluralize(y, "", "s"),
			x,
			string.pluralize(x, "", "s")
			)
			
	elseif (currentLevel:isCity()) then
		text:csplash(false, true, " and are in %s.", 
			currentLevel.title)
	else
		local x
		local y

		x,y = currentLevel:getCoordinates()

		text:csplash(false, true,
			", and are %d level%s %s,\n%d square%s north\n" ..
			"%d square%s east of the entry stairs",
			currentLevel.dungeonLevel,
			string.pluralize(currentLevel.dungeonLevel, "", "s"),
			currentLevel.dungeonDirection,
			y,
			string.pluralize(y, "", "s"),
			x,
			string.pluralize(x, "", "s")
			)
	end
			
	text:clear()
end

----------------------------------------
-- spells:phaseDoor()
----------------------------------------
function spells:phaseDoor()
	if (currentLevel:isDungeon()) then
		local sq = currentLevel.currentSquare

		if (sq[currentLevel.direction].canPhase) then
			currentLevel.currentSquare.isPhased = true
			currentLevel:buildView()
		end
	end
	text:printEllipsis()
end

----------------------------------------
-- spells.summon()
----------------------------------------
function spells:summon(inAction)
	local source	= inAction.source
	local inData	= inAction.inData

	if (source:isCharacter()) then
		inData.type = inData.summons[1]
		party:doSummon(inData)
		text:printEllipsis()
	elseif (source:isSummon()) then
		local xxx_add_party_monster_summon_spell = true

		text:printEllipsis()
		return
	else
		local xxx_fix_monParty_doSummon = true

		currentBattle.monParty:doSummon(inData)
	end
end

----------------------------------------
-- __doHeal()
----------------------------------------
local function __doHeal(inAction, inTarget)
	local inData	= inAction.inData

	if (inData.fullHeal) then
		inTarget.currentHp = inTarget.maxHp
	else
		local hpHealed = 0

		if (inData.randomHeal) then
			assert(inData.ndice ~= 0)
			hpHealed = random:xdy(inData.ndice, inData.dieval)
		else
			hpHealed = inData.ndice
		end

		inTarget.currentHp = inTarget.currentHp + hpHealed
		if (inTarget.currentHp > inTarget.maxHp) then
			inTarget.currentHp = inTarget.maxHp
		end
	end

	if ((inData.dispossess) or (inData.resurrect)) then
		if (inData.dispossess) then
			if (inTarget.isPossessed) then
				inTarget.isPossessed = false
			else
				return
			end
		end

		if (inData.resurrect) then
			if (inTarget.isDead) then
				inTarget.isDead = false
			else
				return
			end
		end

		inTarget.currentHp = 1
		inTarget.possessed = false

		if (currentBattle) then
			currentBattle:removeAction(inTarget)
		end
	end

	if (inData.poison) then inTarget.isPoisoned = false end
	if (inData.insanity) then inTarget.isNuts = false end
	if (inData.paralysis) then inTarget.isParalyzed = false end
	if (inData.old) then inTarget:cureOld() end
	if (inData.stoned) then inTarget.isStoned = false end
end

----------------------------------------
-- spells.heal
----------------------------------------
function spells:heal(inAction)
	local inData = inAction.inData

	local hpHealed = 0

	if (inData.groupHeal) then
		local c
		for c in party:characterIterator() do
			__doHeal(inAction, c)
		end
	else
		__doHeal(inAction, inAction.target)
	end

	text:printEllipsis()

end

----------------------------------------
-- spells:spellBind()
----------------------------------------
function spells:spellBind(inAction)
	local target		= inAction.target
	local last

	if (inAction:savingThrow() 
			or target.size == 0
			or target.isIllusion
			or target.singular == "Lagoth Zanta"
		) then

		text:ctdprint(false, true, " but it had no effect!\n\n")
		return
	end

	if (not party:doSummon({ type = target.name })) then
		return
	end

	-- Remove the last monster from the group
	--
	last = target:getLast()
	target:removeMonster(last)
	currentBattle:removePriority(last)
	if (target.size == 0) then
		currentBattle.monParty:removeMonsterGroup(target)
	end
end

----------------------------------------
-- spells:mageStar()
----------------------------------------
function spells:mageStar(inAction)
	local source	= inAction.source

	log:print(log.LOG_DEBUG, "spells.mageStar")

	-- groupSavingThrow() needs a target. The Halt Foe
	-- spell is untargetted. This looks to be a bug since
	-- the target is never set for the spell. It probably
	-- will just use the last target. We'll use the lead
	-- monster group instead.
	--
	if (source:isCharacter()) then
		inAction.target = currentBattle.monParty:getLeadGroup()
	end

	if (inAction:groupSavingThrow()) then
		text:ctdprint(false, true, " but it had no effect!\n\n")
		party:display()
		return
	end

	if (source:isCharacter()) then
		local mgroup

		for mgroup in currentBattle.monParty:iterator() do
			mgroup.missTurn = true
		end
	else
		text:print(" and the party misses an attack")
		party.missTurn = true
	end
	text:printEllipsis()
end


----------------------------------------
-- spells:attack()
----------------------------------------
function spells:attack(inAction)
	local source	= inAction.source

	log:print(log.LOG_DEBUG, "spells.attack()")
	source:attackSpell(inAction)
end

----------------------------------------
-- spells:battleBonus()
----------------------------------------
function spells:battleBonus(inAction)
	local source	= inAction.source

	if (source:battleBonus(inAction)) then
		text:printEllipsis()
	end
end

----------------------------------------
-- spells:batchSpell()
----------------------------------------
function spells:batchSpell(inAction)
	-- GRRE
	inAction.inData.duration	= 80
	inAction.inData.distance	= 5
	inAction.inData.detectSecret	= true
	party.light:activate(inAction.inData)

	-- MALE
	inAction.inData.duration	= -1
	party.levitate:activate(inAction.inData)

	-- MACO
	party.compass:activate(inAction.inData)
	party.compass:update(currentLevel.direction)

	-- SOSI
	inAction.inData.detectStairs	= true
	inAction.inData.detectTraps	= true
	inAction.inData.detectSpecial	= true
	party.detect:activate(inAction.inData)

	-- YMCA
	inAction.inData.acBonus		= 2
	party.shield:activate(inAction.inData)

	party:display()
	text:printEllipsis()
end

----------------------------------------
-- spells:disbelieve()
----------------------------------------
function spells:disbelieve(inAction, inSource)
	-- disbelieve is kind of broken in the DOS version of BT1
	--
	if (inSource:isMonster()) then
		assert(currentBattle.monParty.disbelieve)
		currentBattle.monParty.disbelieve = true
	else
		party.disbelieve = true
		if (inAction.inData.revealDoppleganger) then
			local c

			for c in party:characterIterator() do
				if (c.isDoppleganger) then
					c.name = "DOPPLEGANGER   "
				end
			end
			party:display()
		end
	end
	text:printEllipsis()
end

----------------------------------------
-- spells:camaraderie()
----------------------------------------
function spells:camaraderie()
	local c

	local function condition(c)
		if (not c:isSummon()) then
			return false
		end

		if (c.isDocile) then
			return false
		end

		if (c.isDead or c.isPoisoned or c.isStoned or c.isParalyzed or
		    c.isPossessed or c.isNuts) then
			return false
		end

		return true
	end

	for c in party:conditionalIterator(condition) do
		if (rnd:band(3) ~= 0) then
			c.isDocile = true
		end
	end
end

----------------------------------------
-- spells:dreamSpell()
----------------------------------------
function spells:dreamSpell(inAction)
	local source	= inAction.source
	local c

	if (not source.learnedZZGO) then
		text:print(" but it fizzles!\n\n")
		timer:delay()
		return
	end

	if (currentBattle) then
		for c in party:iterator() do
			if (c.isOld) then
				c:cureOld()
			end

			if (c.isDead) then
				local xxx_remove_battle_option_from_queue
			end

			c.isDead	= false
			c.isOld		= false
			c.isPoisoned	= false
			c.isStoned	= false
			c.isParalyzed	= false
			c.isPossessed	= false
			c.isNuts	= false

			c.currentHp	= c.maxHp

			if (c:isSummon() and c.isIllusion) then
				c.class		= "Mn"
				c.isIllusion	= false
			end
		end

		party.damageBonus	= 20
		party.toHitBonus	= 20
		party.antiMagic		= 20
		party.acBonus		= 20
		party.extraAttacks	= 8

		-- MIBL
		inAction.inData = table.copy(spellList.MIBL.action.inData)
		spells:attack(inAction)
	else
		local inkey

		text:clear()
		text:print(	"\n1) Dark Domain" ..
				"\n2) The Tombs" ..
				"\n3) The Castle" ..
				"\n4) The Tower" ..
				"\n5) Maze of Dread" ..
				"\n6) Oscon's Fort" ..
				"\n7) Grey Crypt" ..
				"\n8) Destiny Stone"
			)
		text:printExit()

		inkey = getkey()

		if ((inkey < "1") or (inkey > "8")) then
			return
		end

		if (inkey == "1") then
			currentLevel:toLevel("dun", "domain", 1, 0, 0, "north")
		elseif (inkey == "2") then
			currentLevel:toLevel("dun", "tombs", 1, 0, 0, "north")
		elseif (inkey == "3") then
			currentLevel:toLevel("dun", "castle", 1, 0, 0, "north")
		elseif (inkey == "4") then
			currentLevel:toLevel("dun", "tower", 1, 0, 0, "north")
		elseif (inkey == "5") then
			currentLevel:toLevel("dun", "maze", 1, 0, 0, "north")
		elseif (inkey == "6") then
			currentLevel:toLevel("dun", "fort", 1, 0, 0, "north")
		elseif (inkey == "7") then
			currentLevel:toLevel("dun", "crypt", 1, 0, 0, "north")
		elseif (inkey == "8") then
			currentLevel:toLevel("dun", "stone", 1, 0, 0, "north")
		end
	end
end

----------------------------------------
-- spells:range()
----------------------------------------
function spells:range(inAction)
	local target	= inAction.target
	local source	= inAction.source
	local inData	= inAction.inData

	if (target:isCharacter()) then
		text:print(" but it had no effect!\n\n")
		timer:delay()
		return
	end

	if (inAction:groupSavingThrow()) then
		text:print(" but it had no effect!\n\n")
		timer:delay()
		return
	end

	if (inData.addDistance) then
		target.range = target.range + inData.distance
		if (target.range > 90) then
			target.range = 90
		end
	else
		target.range = inData.distance
	end

	text:printEllipsis()
end

----------------------------------------
-- spells:safetySpell()
----------------------------------------
function spells:safetySpell()
	local c

	if ((random:band(7) < 3) or (globals.inSnare)) then
		text:print(" but it fizzles!\n\n")
		party:display()
		timer:delay()
		return
	end

	if (currentBattle) then
		globals.partyDied = true
	end

	globals.gameState = globals.STATE_GUILD
	currentLevel.exit = true

	for c in party:iterator() do
		c.gold = 0
	end
end

----------------------------------------
-- spells:magm()
----------------------------------------
function spells:magm(inAction)
	local inData	= inAction.inData
	local result	= inAction.result
	local r

	result.damage = 0

	r = random:band(3)
	if (r == 0) then
		result.specialAttack = "stone"
	elseif (r == 1) then
		result.specialAttack = "crit"
	end

	inAction:multiTargetSpell()
		
end

----------------------------------------
-- spells:getSpellByAbbreviation()
----------------------------------------
function spells:getSpellByAbbreviation(abbr)
	local s

	s = spells[abbr]
	if (type(s) == "table") then
		if (s.isSpell) then
			return s
		end
	end

	return false
end


----------------------------------------
-- __initSpellList
--
-- Read the list of spells and convert
-- the function strings to the actual
-- functions
----------------------------------------
local function __initSpellList()
	local s
	local func
	local w

	spellList = diskio:readTable("spells")
	for _,s in pairs(spellList) do
		local inFunc
		local mesg

		s.isSpell = true
		if (s.action) then
			compileAction(s.action)
		end
end

end
__initSpellList()
btTable.addParent(spells, spellList)
btTable.setClassMetatable(spells)
spells.__index = spells















