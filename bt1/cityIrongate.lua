cityIrongate = {}

function cityIrongate:new(inDirection, inCastle)
	local self = {
		direction	= inDirection,
		castle		= inCastle
	}

	btTable.addParent(self, cityIrongate,
		building:new("Iron Gate", "PIC_IRONGATE"))
	btTable.setClassMetatable(self)

	if (party:isItemEquipped("Master Key")) then	
		return false
	end

	if (inDirection ~= currentLevel.direction) then
		dprint(inDirection)
		dprint(currentLevel.direction)
		return false
	end

	return self:enter()
end

function cityIrongate:enter()

	self:resetBigpic(true)
	text:clear()
	text:splashMessage(
		"\nYou stand before an iron gate, beyond which " ..
		"stands %s tower.", 
		self.castle
		)

	return true
end
