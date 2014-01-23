require "spells"

----------------------------------------
-- songList data
----------------------------------------
local songList = {}
songList = diskio:readTable("songs")

local s
local func

for _,s in pairs(songList) do
	if (s.activate) then
		compileAction(s.activate)
		compileAction(s.deactivate)
	end
	compileAction(s.combatFunction)
	compileAction(s.toCombat)
end

----------------------------------------
-- song class
----------------------------------------
song = {}
function song:new()
	local self = object:new()

	self.active		= false
	self.duration		= 0
	self.singer		= false
	self.currentTune	= false

	-- Tune effect values
	self.lightSong		= false
	self.trapProtect	= false
	self.acBonus		= 0
	self.spptRegen		= false

	self:addParent(song)

	return self
end

----------------------------------------
-- song:__tuneNumberCheck()
--
-- Validate that a function argument
-- is a tune number
----------------------------------------
function song:__tuneNumberCheck(inTune)
	if (type(inTune) ~= "number") then
		error("song:activate() inTune not a number", 3)
	end

	if ((inTune < 1) or (inTune > #songList)) then
		error("Invalid tune number: " .. tostring(inTune), 3)
	end
end
----------------------------------------
-- song:getTune()
----------------------------------------
function song:getTune()
	local i
	local inkey

	text:cdprint(true, false, "Play tune #\n\n")
	for i = 1,#songList do
		text:print("%d)%s\n", i, songList[i].name)
	end

	inkey = tonumber(getkey())
	if (not inkey) then
		return false
	end

	if ((inkey > 0) and (inkey < #songList + 1)) then
		return inkey
	end

	return false
end

----------------------------------------
-- song:activate()
----------------------------------------
function song:activate(inSinger, inTune)
	local tune
	local action

	self:__tuneNumberCheck(inTune)

	tune = songList[inTune]
	if (tune.activate) then
		action = btAction:new()
		action.source = inSinger
		action.func = tune.activate.func
		action.inData = tune.nonCombatData[currentLevel.level].inData

		log:print(log.LOG_DEBUG, "Singing tune #%d", inTune)
		if (action.func) then
			action.func(action)
		end
	end

	self:__activate()
	self.currentTune	= inTune
	self.duration		= 12
	self.singer		= inSinger
	inSinger.isSinging	= true
end

----------------------------------------
-- song:deactivate()
----------------------------------------
function song:deactivate()
	self:__deactivate()
	self.duration	= 0
	
	local xxx_saved_party_song_active = false

	songList[self.currentTune].deactivate.func()

	self.currentTune	= 0
	self.singer.isSinging	= false
	self.singer		= false
	self.duration		= 0
end

----------------------------------------
-- song:__activate()
----------------------------------------
function song:__activate()
	local xxx_start_music_here = true

	self.active = false
end

----------------------------------------
-- song:__deactivate()
--
-- Just sets the active flag to false.
-- This is used by battle code to keep
-- the passive effect going but disable
-- the effect
----------------------------------------
function song:__deactivate()
	local xxx_this_would_disable_music = true
	self.active = false
end

----------------------------------------
-- song:convertToCombat()
----------------------------------------
function song:convertToCombat()
	if (not self.active) then
		return
	end

	self:__deactivate()

	-- Sanctuary score doesn't get converted
	if (self.currentTune ~= 3) then
		local tune	= songList[self.currentTune]

		local action = btAction:new()
		action.inData = tune.combatData[currentLevel.level].inData

		tune.toCombat.func(action)
	end
end

----------------------------------------
-- song:combatSong()
----------------------------------------
function song:activateCombatSong(inAction)
	local tune
	local action

	self:__tuneNumberCheck(inAction.func)

	tune = songList[inAction.func]

	-- Special handling for Watchwood Melody
	if (tune.name == "Watchwood Melody") then
		text:print(" but was unable to play it.")
		return
	end

	inAction.func = tune.combatFunction.func
	inAction.inData = tune.combatData[currentLevel.level].inData
	inAction.func()
	timer:delay()
end
