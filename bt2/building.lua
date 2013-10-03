----------------------------------------
-- building class
--
-- Base class for a building in the
-- city
----------------------------------------
building = {}
function building:new(inTitle, inBigpic)
	local self = {
		title	= inTitle,
		bigpic	= inBigpic
	}

	btTable.addParent(self, building)
	btTable.setClassMetatable(self)

	return self
end

function building:resetBigpic(inNightAware)
	bigpic:drawImage(self.bigpic, inNightAware)
	bigpic:setTitle(self.title)
end

function building:enter()
	error("Must overload enter() function", 2)
end
