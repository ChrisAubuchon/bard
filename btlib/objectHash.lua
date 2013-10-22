require "random"
require "bttable"

local objects = {}

objectHash = {}
function objectHash:new(inObject)
	local self = {
		key = false
	}

	btTable.addParent(self, objectHash)
	btTable.setClassMetatable(self)

	while true do
		self.key = random:hash()
		if (not objects[self.key]) then
			objects[self.key] = inObject
			return self
		end
	end
end

function objectHash:deleteKey()
	if (not self.key) then
		error("No key set in object", 2)
	end

	if (objects[self.key]) then
		objects[self.key] = nil
	end
end

function objects.dump()
	for key,obj in pairs(objects) do
		print("Key: " .. tostring(key) .. ", Object: " .. tostring(obj))
	end
end

