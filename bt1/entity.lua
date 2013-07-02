----------------------------------------
-- entity class
-- Base class for a monster or character
----------------------------------------

entity = {
	className	= "entity"
}

function entity.isCharacter()
	return false
end

function entity.isMonster()
	return false
end

function entity.isSummon()
	return false
end

function entity.getBattlePriority()
	error("Must overload getBattlePriority()", 2)
end

function entity.getSingularName()
	error("Must overload getSingularName()", 2)
end

function entity.getPronoun()
	return "him"
end

