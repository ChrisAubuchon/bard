----------------------------------------
-- entity class
----------------------------------------

entity = {
	name		= "",
	class		= false,
	currentHp	= 0,
	maxHp		= 0,
	ac		= 10,

	gold		= 0,

	isDead		= false,
	isOld		= false,
	isPoisoned	= false,
	isStoned	= false,
	isParalyzed	= false,
	isPossessed	= false,
	isNuts		= false,

	numAttacks	= 1
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

