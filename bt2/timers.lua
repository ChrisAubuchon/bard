----------------------------------------
-- timers.lua
--
-- Real time timers for Bard's Tale 2
--
-- Based on the DOS version where one
-- clock tick is ~55ms.
----------------------------------------

----------------------------------------
-- bt2_32()
--
-- Interval: ~2s
----------------------------------------
local function bt2_32()
	if (globals.doTimeEvents) then
		if (globals.isNight) then
			if (random:band(0x3f) == 2) then
				return btkeys.RANDOMBATTLE
			end
		else
			if (random:band(0x7f) == 2) then
				return btkeys.RANDOMBATTLE
			end
		end

		if (random:band(0xff) == 2) then
			return btkeys.WANDERING
		end
	end
end

----------------------------------------
-- bt2_512()
--
-- Interval: ~30s
----------------------------------------
local function bt2_512()
	log:print(log.LOG_DEBUG, "bt2_512() run")

	if ((not globals.isPaused) and (globals.doTimeEvents)) then
		party:doPoison()

		if (not party:isLive()) then
			globals.doTimeEvents = false
			return false
		end
	end

	if (not globals.isPaused) then
		if (party.song.duration > 0) then
			party.song.duration = party.song.duration - 1
			if (party.song.duration == 0) then
				party.song:deactivate()
			end
		end

		if ((party.light.active) and (party.light.duration > 0)
			and (not party.song.lightSong)) then
			party.light.duration = party.light.duration - 1
			if (party.light.duration == 0) then
				party.light.deactivate()
			end
		end

		if ((party.levitate.active) and (party.levitate.duration > 0)) then
			party.levitate.duration = party.levitate.duration - 1
			if (party.levitate.duration == 0) then
				party.levitate.deactivate()
			end
		end
		if ((party.shield.active) and (party.shield.duration > 0)) then
			party.shield.duration = party.shield.duration - 1
			if (party.shield.duration == 0) then
				party.shield.deactivate()
			end
		end
		if ((party.detect.active) and (party.detect.duration > 0)) then
			party.detect.duration = party.detect.duration - 1
			if (party.detect.duration == 0) then
				party.detect.deactivate()
			end
		end
		if ((party.compass.active) and (party.compass.duration > 0)) then
			party.compass.duration = party.compass.duration - 1
			if (party.compass.duration == 0) then
				party.compass.deactivate()
			end
		end

		if (globals.doTimeEvents) then
			if ((currentLevel.currentSquare.isSpptRegen) or
			    (currentLevel:isCity() and not globals.isNight) or
			    (party.song.spptRegen))
			    then
				party:regenSpellPoints()
			end

			party:doEquippedEffects()

			if (currentLevel.currentSquare.isLifeDrain) then
				currentLevel:doLifeDrain()
				if (not party:isLive()) then
					return false
				end
			end
		end
	end
end

----------------------------------------
-- bt2_2048()
--
-- Interval: ~2m
----------------------------------------
local function bt2_2048()
	if (not globals.isPaused) then
		if (globals.gameHour == 23) then
			globals.gameHour = 0
		else
			globals.gameHour = globals.gameHour + 1
		end

		if ((globals.gameHour < 6) or (globals.gameHour > 18)) then
			if (not globals.isNight) then
				globals.isNight = true

				if (globals.doTimeEvents) then
					if (currentLevel:isOutdoors()) then
						currentLevel:buildView()
					end
				end
			end
		else
			if (globals.isNight) then
				globals.isNight = false

				if (globals.doTimeEvents) then
					if (currentLevel:isOutdoors()) then
						currentLevel:buildView()
					end
				end
			end
		end
	end
end

local xxx_look_in_to_making_timers_global = true
-- Save off the timer somewhere so that they can be cleanly stopped

timer:new(bt2_32,	1760)
timer:new(bt2_512,	28160)
timer:new(bt2_2048,	112640)
