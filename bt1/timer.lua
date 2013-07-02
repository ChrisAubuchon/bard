timer = {}

function timer.new(inFunction, interval)
	sdl.AddTimer(inFunction, interval)
end

function timer.bt1_16()
	if (globals.doTimeEvents) then
		local xxx_bt1_16 = true
	end
end

function timer.bt1_256()
	if ((not globals.isPaused) and (globals.doTimeEvents)) then
		local xxx_bt1_256 = true
	end
end

function timer.bt1_512()
	if (not globals.isPaused) then

		if ((party.light.active) and (party.light.duration > 0)) then
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
	end
end

function timer.bt1_2048()
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
					if (currentLevel.isCity()) then
						currentLevel.buildView()
					end
				end
			end
		else
			if ((globals.isNight) and (not globals.inDungeon)) then
				globals.isNight = false

				currentLevel.level = 1
				if (globals.doTimeEvents) then
					if (currentLevel.isCity()) then
						currentLevel.buildView()
					end
				end
			end
		end
	end
end

local function __init()
	timer.new(timer.bt1_16,	880)
	timer.new(timer.bt1_256, 14080)
	timer.new(timer.bt1_512, 28160)
	timer.new(timer.bt1_2048, 112640)
end

__init()
