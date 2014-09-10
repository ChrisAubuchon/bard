btkeys = {
	RETURN = ":A:",
	ESCAPE = ":B:",
	BACKSPACE = ":C:",
	UP = ":D:",
	DOWN = ":E:",
	LEFT = ":F:",
	RIGHT = ":G:",
	PAGEUP = ":H:",
	PAGEDOWN = ":I:",
	F1 = ":J:",
	F2 = ":K:",
	F3 = ":L:",
	F4 = ":M:",
	F5 = ":N:",
	F6 = ":O:",
	F7 = ":P:",
	RANDOMBATTLE = ":Q:",
	WANDERING = ":R:"
}

----------------------------------------
-- getkey()
--
-- Main IO loop
----------------------------------------
function getkey(inCaseSensitive)
	local inkey

	while true do
		inkey = SDL.GetKey()

		if (not inkey) then
			return nil
		elseif (inkey == 0) then
			return inkey
		elseif (inkey == SDL.K.RETURN) then
			return btkeys.RETURN
		elseif (inkey == SDL.K.BACKSPACE) then
			return btkeys.BACKSPACE
		elseif (inkey == SDL.K.ESCAPE) then
			return btkeys.ESCAPE
		elseif (inkey == SDL.K.UP) then
			return btkeys.UP
		elseif (inkey == SDL.K.DOWN) then
			return btkeys.DOWN
		elseif (inkey == SDL.K.LEFT) then
			return btkeys.LEFT
		elseif (inkey == SDL.K.RIGHT) then
			return btkeys.RIGHT
		elseif (inkey == SDL.K.PAGEUP) then
			return btkeys.PAGEUP
		elseif (inkey == SDL.K.PAGEDOWN) then
			return btkeys.PAGEDOWN
		elseif (inkey == SDL.K.F1) then
			return btkeys.F1
		elseif (inkey == SDL.K.F2) then
			return btkeys.F2
		elseif (inkey == SDL.K.F3) then
			return btkeys.F3
		elseif (inkey == SDL.K.F4) then
			return btkeys.F4
		elseif (inkey == SDL.K.F5) then
			return btkeys.F5
		elseif (inkey == SDL.K.F6) then
			return btkeys.F6
		elseif (inkey == SDL.K.F7) then
			return btkeys.F7
		elseif (inkey == btkeys.RANDOMBATTLE) then
			return btkeys.RANDOMBATTLE
		elseif (inkey == btkeys.WANDERING) then
			return btkeys.WANDERING
		elseif (inkey < 255) then
			if (inCaseSensitive) then
				return string.char(inkey)
			else
				return string.upper(string.char(inkey))
			end
		end
	end
end

