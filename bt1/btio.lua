declare_global("getkey")
declare_global("btkeys")

----------------------------------------
-- getkey()
--
-- Main IO loop
----------------------------------------
function getkey()
	local inkey

	while true do
		inkey = sdl.GetKey()

		if (inkey == sdl.SDLK_BACKSPACE) then
			return btkeys.BTKEY_BACKSPACE
		elseif (inkey == sdl.SDLK_RETURN) then
			return btkeys.BTKEY_RETURN
		elseif (inkey == sdl.SDLK_ESCAPE) then
			return btkeys.BTKEY_ESCAPE
		elseif (inkey == sdl.SDLK_UP) then
			return btkeys.BTKEY_UP
		elseif (inkey == sdl.SDLK_DOWN) then
			return btkeys.BTKEY_DOWN
		elseif (inkey == sdl.SDLK_LEFT) then
			return btkeys.BTKEY_LEFT
		elseif (inkey == sdl.SDLK_RIGHT) then
			return btkeys.BTKEY_RIGHT
		elseif (inkey == sdl.SDLK_PAGEUP) then
			return btkeys.BTKEY_PAGEUP
		elseif (inkey == sdl.SDLK_PAGEDOWN) then
			return btkeys.BTKEY_PAGEDOWN
		elseif (inkey == sdl.SDLK_F1) then
			return btkeys.BTKEY_F1
		elseif (inkey == sdl.SDLK_F2) then
			return btkeys.BTKEY_F2
		elseif (inkey == sdl.SDLK_F3) then
			return btkeys.BTKEY_F3
		elseif (inkey == sdl.SDLK_F4) then
			return btkeys.BTKEY_F4
		elseif (inkey == sdl.SDLK_F5) then
			return btkeys.BTKEY_F5
		elseif (inkey == sdl.SDLK_F6) then
			return btkeys.BTKEY_F6
		elseif (inkey == sdl.SDLK_F7) then
			return btkeys.BTKEY_F7
		elseif (inkey == 0) then
			return inkey
		elseif (inkey < 255) then
			return string.upper(string.char(inkey))
		end
	end
end

btkeys = {
	BTKEY_RETURN = ":A:",
	BTKEY_ESCAPE = ":B:",
	BTKEY_BACKSPACE = ":C:",
	BTKEY_UP = ":D:",
	BTKEY_DOWN = ":E:",
	BTKEY_LEFT = ":F:",
	BTKEY_RIGHT = ":G:",
	BTKEY_PAGEUP = ":H:",
	BTKEY_PAGEDOWN = ":I:",
	BTKEY_F1 = ":J:",
	BTKEY_F2 = ":K:",
	BTKEY_F3 = ":L:",
	BTKEY_F4 = ":M:",
	BTKEY_F5 = ":N:",
	BTKEY_F6 = ":O:",
	BTKEY_F7 = ":P:"
}

