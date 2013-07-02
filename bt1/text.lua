local textwindow

function printStatusLine(inCharString, inHpString, inSlot)
	local renderedText
	local fontP
	local y

	y = 266 + (16 * inSlot)
	fontP = globals.fonts["mono"]
	m_window:FillRect(32, y, 580, 16, globals.colors[8])

	if (inCharString) then
		renderedText = fontP:RenderText(inCharString,
					globals.colors[16])
		m_window:Blit(32, y, renderedText.w, renderedText.h,
					renderedText, nil)
		renderedText = fontP:RenderText(inHpString,
					globals.colors[1])
		m_window:Blit(416, y, renderedText.w, renderedText.h,
					renderedText, nil)
	end

	m_window:Flip()
end

----------------------------------------
-- Clear text window
----------------------------------------
function text_clear()
	m_window:FillRect(348, 30, 266, 200, 255, 255, 255)
	m_window:UpdateRect(348, 30, 266, 200)
	textwindow:Clear()
end

----------------------------------------
-- text_clearline()
--
-- Clear a specific line in the text
-- window
----------------------------------------
function text_clearline(line)
	assert((line >= 0) and (line <= 11))

	textwindow:SetCursor(0, line)
	textwindow:clearLine()
end

local function _text_delay(dtime)
	local inkey
	local timer

	assert(type(dtime) == "number")

	-- The sleep is three 60ms ticks 
	timer = sdl.Sleep(180 * dtime)

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

----------------------------------------
-- text_delay()
--
-- Mirror the functionality of the BT
-- text delay function
----------------------------------------
function text_delay(dtime)
	assert(type(dtime) == "number")

	_text_delay(dtime * 4)
end

----------------------------------------
-- printf-style text
----------------------------------------
function text_print(format, ...)
	if (format == nil) then
		error("Passed nil to text_print. Probably a bad text_cdprint call", 2)
	end

	textwindow:Print(string.format(format, ...))
end

----------------------------------------
-- text_cdprint()
--
-- ClearDelay print. 
----------------------------------------
function text_cdprint(clearFlag, delayFlag, format, ...)
	if (clearFlag) then text_clear() end
	text_print(format, ...)
	if (delayFlag) then text_delay(6) end
end

function text_setColumn(newX)
	local oldX, oldY

	oldX,oldY = textwindow:GetCursor()
	textwindow:SetCursor(newX * 12, oldY)
end

----------------------------------------
-- Set text cursor
----------------------------------------
function text_setCursor(x, line)
	if (line > 11) then
		line = 11
	end

	textwindow:SetCursor(x, line)
end

----------------------------------------
-- text_readNumber()
--
-- Reads a number from the user. 
-- Returns 0 if the string contains
-- non-numeric characters
----------------------------------------
function text_readNumber()
	local t

	text_print("\n")

	t = text_readString()
	t = string.match(t, "^%d+$")
	if (t ~= nil) then
		return tonumber(t)
	else
		return false
	end
end

----------------------------------------
-- Read a string from the user
--
-- This function assumes the cursor is
-- at the proper position
----------------------------------------
function text_readString(max_len)
	local x,y
	local inkey
	local nchars = 0
	local outstring = ""

	if ((max_len == nil) or (max_len == 0)) then
		max_len = 14
	end

	x,y = textwindow:GetCursor()

	textwindow:SetCursor(240, y)
	textwindow:Print("<")

	while true do
		textwindow:SetCursor(x, y);
		textwindow:putc(128)

		inkey = getkey();
	
		if (inkey == btkeys.BTKEY_RETURN) then
			break
		elseif (inkey == btkeys.BTKEY_BACKSPACE) then
			if (nchars ~= 0) then
				-- Get last character
				local c = outstring:sub(-1)

				-- Erase the cursor
				textwindow:eraseChar(128, x)

				-- Erase the previous character
				x = x - textwindow:eraseChar(c:byte(), x, 1)

				-- Update the output string
				outstring = outstring:sub(1, -2)

				nchars = nchars - 1
			end
		elseif (inkey == btkeys.BTKEY_ESCAPE) then
			if (nchars ~= 0) then
				textwindow:clearLine()

				textwindow:SetCursor(240, y)
				textwindow:Print("<")

				x = 0;
				textwindow:SetCursor(x, y);

				outstring = ""
			end
		else
			if ((inkey:match("%w")) or (inkey == " ") or
			    (inkey == ".") or (inkey == ",")) then
				-- Match an alphanumeric character

				-- Append the character
				outstring = outstring .. inkey

				nchars = nchars + 1

				-- Erase the cursor
				textwindow:eraseChar(128, x)

				textwindow:SetCursor(x, y)
				x = x + textwindow:putc(inkey:byte())
			end
		end
	end

	return outstring
end

local function scroll_reset(intable, top, high, pfunc)
	local i

	text_clear()

	if (#intable > 0) then
		for i = 0, 10 do
			local n = i + top

			if (i ~= 0) then
				text_print("\n")
				pfunc(intable[n])
			else
				pfunc(intable[n])
			end

			if (n >= #intable) then
				break
			end
		end
	end

	text_setCursor(0, 11)
	textwindow:putc(129)

	text_setCursor(240, 11)
	textwindow:putc(130)

	text_setCursor(0, 11)
	textwindow:Print("    (CANCEL)")

	textwindow:Highlight(high)
end

----------------------------------------
-- text_scrollSelect()
--
-- Select a member from the provided 
-- list
-- 
-- Returns:
--   key selected
--   false if none selected
----------------------------------------
function text_scrollSelect(intable, pfunc)
	local top_index = 1
	local high_index = 0
	local i
	local k
	local inkey

	scroll_reset(intable, top_index, high_index, pfunc)

	while true do
		inkey = getkey()

		if (inkey == btkeys.BTKEY_DOWN) then
			if (high_index == 10) then
				-- Increment the top index if we are at the end
				-- of the window but not at the end of the list
				--
				if ((top_index + high_index) < #intable) then
					top_index = top_index + 1
					scroll_reset(intable, top_index, 
							high_index, pfunc)
				end
			else
				-- Unhighlight the previous entry and highlight
				-- the new one
				--
				if (high_index < #intable - 1) then
					textwindow:UnHighlight(high_index)
					high_index = high_index + 1
					textwindow:Highlight(high_index)
				end
			end
		elseif (inkey == btkeys.BTKEY_UP) then
			-- Scroll the list if the highlight is at the
			-- top and there are more items above.
			--
			if (high_index == 0) then
				if (top_index ~= 1) then
					top_index = top_index - 1
					scroll_reset(intable, top_index, 
							high_index, pfunc)
				end
			else
				textwindow:UnHighlight(high_index)
				high_index = high_index - 1
				textwindow:Highlight(high_index)
			end
		elseif (inkey == btkeys.BTKEY_PAGEUP) then
			if (top_index <= 10) then
				top_index = 1
			else
				top_index = top_index - 10
			end
			scroll_reset(intable, top_index, high_index, pfunc)
		elseif (inkey == btkeys.BTKEY_PAGEDOWN) then
			if ((top_index + 20) > #intable) then
				top_index = #intable - 10
			else
				top_index = top_index + 10
			end
			scroll_reset(intable, top_index, high_index, pfunc)
		elseif ((inkey == btkeys.BTKEY_RETURN) or 
		       (inkey == " ")) then
			return (intable[top_index + high_index])
		elseif (inkey == btkeys.BTKEY_ESCAPE) then
			return false
		elseif (inkey == "C") then
			return false
		end

	end
end

function text_select(baseIndex, numItems)
	local highIndex = baseIndex
	local lineIndex = 1
	local inkey

	textwindow:Highlight(highIndex)

	while true do
		inkey = getkey()

		if (inkey == btkeys.BTKEY_DOWN) then
			if (lineIndex < numItems) then
				textwindow:UnHighlight(highIndex)
				highIndex = highIndex + 1
				lineIndex = lineIndex + 1
				textwindow:Highlight(highIndex)
			end
		elseif (inkey == btkeys.BTKEY_UP) then
			if (lineIndex > 1) then
				textwindow:UnHighlight(highIndex)
				highIndex = highIndex - 1
				lineIndex = lineIndex - 1
				textwindow:Highlight(highIndex)
			end
		elseif ((inkey == btkeys.BTKEY_RETURN) or
			 (inkey == " ")) then
			return lineIndex
		elseif ((inkey > "0") and (inkey < tostring(numItems))) then
			return tonumber(inkey)
		elseif (inkey == "C") then
			return false
		end
	end
		
				
end

local function __init()
	-- Font initialization
	globals.fonts["mono"] = sdl.font.Open("fonts/bt1font.btf",
					      sdl.font.FONT_BTF,
					      sdl.font.BTF_MONOSPACE);
	globals.fonts["var"] = sdl.font.Open("fonts/bt1font.btf",
					      sdl.font.FONT_BTF,
					      sdl.font.BTF_VARWIDTH);

	-- textwindow initiliaztion
	textwindow = sdl.NewTextbox(m_window, 352, 34, 262, 192)
	textwindow.bg_color = sdl.NewColor(255, 255, 255)
	textwindow.fg_color = sdl.NewColor(0, 0, 0)
	textwindow.hg_color = sdl.NewColor(255, 255, 0)
	textwindow:SetFont(globals.fonts["var"])
end

__init()
