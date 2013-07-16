require "bt1test"
require "random"

local i
local results = {}

for i = 1,1000000 do
	local r = rnd_and_x(0x7f)

	if (not results[r]) then
		results[r] = 1
	else
		results[r] = results[r] + 1
	end
end

for i,v in ipairs(results) do
	dprint("%d: %d", i, v)
end
