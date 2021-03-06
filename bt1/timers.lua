----------------------------------------
-- timers.lua
--
-- Real time timers for Bard's Tale 1
--
-- Based on the DOS version where one
-- clock tick is ~55ms.
----------------------------------------

----------------------------------------
-- bt1_16()
--
-- Interval: ~1s
----------------------------------------
local function bt1_16()
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
-- bt1_256()
--
-- Interval: ~15s
----------------------------------------
local function bt1_256()
	if ((not globals.isPaused) and (globals.doTimeEvents)) then
		party:doPoison()

		if (not party:isLive()) then
			globals.doTimeEvents = false
			return false
		end
	end
end

----------------------------------------
-- bt1_512()
--
-- Interval: ~30s
----------------------------------------
local function bt1_512()
	if (not globals.isPaused) then
		if ((party.song.active) and (party.song.timer > 0)) then
			party.song.timer = party.song.timer - 1
			if (party.song.timer == 0) then
				party.singer:songTimeout()
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
			    (currentLevel:isCity() and not globals.isNight))
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
-- bt1_2048()
--
-- Interval: ~2m
----------------------------------------
local function bt1_2048()
	dprint("bt1_2048()")
	if (not globals.isPaused) then
		if (globals.gameHour == 23) then
			globals.gameHour = 0
		else
			globals.gameHour = globals.gameHour + 1
		end

		if ((globals.gameHour < 6) or (globals.gameHour > 18)) then
			if ((not globals.isNight) and (not globals.inDungeon)) then
				globals.isNight = true
				currentLevel.level = 2

				if (globals.doTimeEvents) then
					if (currentLevel:isCity()) then
						currentLevel:buildView()
					end
				end
			end
		else
			if ((globals.isNight) and (not globals.inDungeon)) then
				globals.isNight = false

				currentLevel.level = 1
				if (globals.doTimeEvents) then
					if (currentLevel:isCity()) then
						currentLevel:buildView()
					end
				end
			end
		end
	end
end

local function __init()
	timer:new(bt1_16,	880)
	timer:new(bt1_256,	14080)
	timer:new(bt1_512,	28160)
	timer:new(bt1_2048,	112640)
end

__init()
