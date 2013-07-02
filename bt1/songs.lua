require "dataio"
require "spells"
require "btdebug"

songs = read_table("songs")

local function initSongs()
	local s
	local func
	local w
	local l

	for _,s in pairs(songs) do
		for _,l in pairs(s.combat) do
			compileAction(l)
if false then
			if (l.func) then
				func = _G
				for w in l.func:gmatch("%a+") do
					func = func[w]
				end
				l.func = func
end
			end
		end
		for _,l in pairs(s.nonCombat) do
			compileAction(l)
if false then
			if (l.func) then
				func = _G
				for w in l.func:gmatch("%a+") do
					func = func[w]
				end
				l.func = func
			end
end
		end
	end
end
initSongs()
