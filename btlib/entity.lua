----------------------------------------
-- entity class
----------------------------------------

entity = {
	name		= "",
	class		= false,
	currentHp	= 0,
	maxHp		= 0,

	currentSppt	= 0,
	maxSppt		= 0,

	ac		= 10,

	gold		= 0,
	xp		= 0,
	battlesWon	= 0,

	isDead		= false,
	isOld		= false,
	isPoisoned	= false,
	isStoned	= false,
	isParalyzed	= false,
	isPossessed	= false,
	isNuts		= false,

	numAttacks	= 1,

	spellLevel = {
		Magician	= 0,
		Conjurer	= 0,
		Sorcerer	= 0,
		Wizard		= 0,
		Archmage	= 0
	}
}

function entity:isCharacter()
	return false
end

function entity:isMonster()
	return false
end

function entity:isSummon()
	return false
end

function entity:getBattlePriority()
	error("Must overload getBattlePriority()", 2)
end

function entity:getSingularName()
	error("Must overload getSingularName()", 2)
end

function entity:getPronoun()
	return "him"
end

