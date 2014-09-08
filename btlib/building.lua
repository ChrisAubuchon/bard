----------------------------------------
-- building class
--
-- Base class for a building in the
-- city
----------------------------------------
building = {}
function building:new(inTitle, inBigpic)
	local self = object:new()

	self.title	= inTitle
	self.bigpic	= inBigpic

	self:addParent(building)

	return self
end

----------------------------------------
-- building:resetBigpic()
----------------------------------------
function building:resetBigpic(inNightAware)
	bigpic:drawImage(self.bigpic, inNightAware)
	bigpic:setTitle(self.title)
end

function building:enter()
	error("Must overload enter() function", 2)
end
