race = object:new()

local raceData	= diskio:readTable("races")

----------------------------------------
-- randomStat()
----------------------------------------
function race:randomStat(race, stat)
	local s		= 0

	if ((not raceData[race]) or (not raceData[race][stat])) then
		return 0
	end

	s = raceData[race][stat] + random:band(7)
	if (s > 30) then
		s = 30
	end

	return s
end

----------------------------------------
-- getStartingClasses()
----------------------------------------
function race:getStartingClasses(inRace)
	if (not raceData[inRace]) then
		return nil
	end

	return table.copy(raceData[inRace].starting_classes)
end
