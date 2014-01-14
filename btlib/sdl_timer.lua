timer = {}

----------------------------------------
-- new()
----------------------------------------
function timer:new(inFunction, inInterval)
	local self = object:new()

	self:addParent(timer)

	self.timer = SDL.AddTimer(inFunction, inInterval)

	return self
end

----------------------------------------
-- stop()
---------------------------------------- 
function timer:stop()
	SDL.removeTimer(self.timer)
end

local delayIndex	= 8
local delayTable	= { 1, 4, 7, 11, 14, 17, 20, 23, 26, 29 }

----------------------------------------
-- delay()
--
-- Delay for a certain number of clock
-- cycles
----------------------------------------
function timer:delay(inDelayTime)
	local function __delay(inTime)
		local inkey
		local timer

		timer = SDL.Sleep(180 * inTime)

		while true do
			inkey = getkey()
			if (inkey == 0) then
				return
			elseif ((inkey == ">") or (inkey == ".")) then
				self:defaultFaster()
			elseif ((inkey == "<") or (inkey == ",")) then
				self:defaultSlower()
			else
				SDL.RemoveTimer(timer)
				return
			end
		end
	end

	if (not inDelayTime) then
		inDelayTime = delayTable[delayIndex]
	else
		inDelayTime = inDelayTime * 4
	end

	__delay(inDelayTime)
end

function timer:defaultFaster()
	if (delayIndex == 1) then
		return
	end

	delayIndex = delayIndex - 1

	text:print("\n<FASTER...>\n")
end

function timer:defaultSlower()
	if (delayIndex == 10) then
		return
	end

	delayIndex = delayIndex + 1

	text:print("\n<SLOWER...>\n")
end
