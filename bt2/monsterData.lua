----------------------------------------
-- monsterData class
----------------------------------------
monsterData = object:new(diskio:readTable("monsters"))

local m
local a

for _,m in pairs(monsterData) do
	if (m.attacks) then
		for _,a in ipairs(m.attacks) do
			compileAction(a.action)
		end
	end
end

----------------------------------------
-- monsterData:toArray()
----------------------------------------
function monsterData:toArray()
	local rval	= {}
	local m
	local value

	for m,value in pairs(self) do
		if (type(value) ~= "function") then
			table.insert(rval, m)
		end
	end

	table.sort(rval)
	return rval
end

----------------------------------------
-- monsterData:getXPReward()
----------------------------------------
function monsterData:getXPReward(inType)
	assert(self[inType])

	return self[inType].reward
end
