require "spells"
require "btdebug"

songs = diskio:readTable("songs")

local function initSongs()
	local s
	local func
	local w
	local l

	for _,s in pairs(songs) do
		if (s.activate) then
			compileAction(s.activate)
			compileAction(s.deactivate)
		end
		compileAction(s.combatFunction)
	end
end
initSongs()
