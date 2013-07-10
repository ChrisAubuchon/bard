timer = {}

function timer:new(inFunction, inInterval)
	local self = {
		timer	= false
	}

	self.timer = sdl.AddTimer(inFunction, inInterval)

	btTable.addParent(self, timer)
	btTable.setClassMetatable(self)

	return self
end

function timer:stop()
	sdl.removeTimer(self.timer)
end

function timer:delay(inDelayTime)
	local function __delay(inTime)
		local inkey
		local timer

		timer = sdl.Sleep(180 * inTime)

		while true do
			inkey = getkey()
			if (inkey == 0) then
				return
			else
				sdl.RemoveTimer(timer)
				return
			end
		end
	end

	__delay(inDelayTime * 4)
end
