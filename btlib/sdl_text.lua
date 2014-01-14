require "sdl_video"
require "bttable"

textBox = {}
function textBox:new(inRect, inChars, inFont)
	local self = object:new()

	self:addParent(textBox)

	self.rect	= inRect
	self.tb = SDL.NewTextbox(gfx.renderer, gfx.texture, gfx.surface, 
				self.rect, inChars)
	self.tb.bg_color = { 255, 255, 255, 255 }
	self.tb.fg_color = {   0,   0,   0, 255 }
	self.tb.hg_color = { 255, 255,   0, 255 }
	self.tb:SetFont(inFont.font)

	return self
end

----------------------------------------
-- clear()
----------------------------------------
function textBox:clear()
	self.tb:Clear()
end

----------------------------------------
-- clearLine()
----------------------------------------
function textBox:clearLine(inLine)
	assert((inLine >= 0) and (inLine <= 11))

	self.tb:SetCursor(0, inLine)
	self.tb:clearLine()
end

----------------------------------------
-- putc
----------------------------------------
function textBox:putc(inChar)
	self.tb:putc(inChar)
end

----------------------------------------
-- print()
----------------------------------------
function textBox:print(format, ...)
	if (type(format) ~= "string") then
		error("nil format in textBox:print(). Probably a bad cdprint() call", 2)
	end

	self.tb:Print(sprintf(format, ...))
end

----------------------------------------
-- cdprint()
--
-- Clear, Delay, print
----------------------------------------
function textBox:cdprint(clearFlag, delayFlag, format, ...)
	if (clearFlag) then 
		self:clear()
	end

	self:print(format, ...)

	if (delayFlag) then
		timer:delay(6)
	end
end

----------------------------------------
-- ctdprint()
--
-- Clear, Print, Table Delay
----------------------------------------
function textBox:ctdprint(clearFlag, delayFlag, format, ...)
	if (clearFlag) then
		self:clear()
	end

	self:print(format, ...)

	if (delayFlag) then
		timer:delay()
	end
end

----------------------------------------
-- cdcprint()
--
-- Clear, Delay, Clear, Print
----------------------------------------
function textBox:cdcprint(preClear, delay, postClear, format, ...)
	if (preClear) then
		self:clear()
	end

	self:print(format, ...)
	if (delay) then
		timer:delay(6)
	end

	if (postClear) then
		self:clear()
	end
end

----------------------------------------
-- setColumn
----------------------------------------
function textBox:setColumn(inX)
	local oldX, oldY

	oldX, oldY = self.tb:GetCursor()
	self.tb:SetCursor(inX * 12, oldY)
end

----------------------------------------
-- setCursor()
----------------------------------------
function textBox:setCursor(inX, inLine)
	if (inLine > 11) then
		inLine = 11
	end

	self.tb:SetCursor(inX, inLine)
end

----------------------------------------
-- readString()
----------------------------------------
function textBox:readString(inMaxLen)
	local x
	local y
	local inkey
	local nchars	= 0
	local outstring = ""

	if ((inMaxLen == nil) or (inMaxLen == 0)) then
		inMaxLen = 14
	end

	self.tb:Print("\n")
	x,y = self.tb:GetCursor()
	self.tb:SetCursor(240, y)
	self.tb:Print("<")

	while true do
		self.tb:SetCursor(x, y)
		self.tb:putc(128)

		inkey = getkey()

		if (inkey == btkeys.RETURN) then
			self.tb:eraseChar(128, x)
			self.tb:putc(32)
			break
		elseif (inkey == btkeys.BACKSPACE) then
			if (nchars ~= 0) then
				-- Get the last character
				local c = outstring:sub(-1)

				-- Erase the cursor
				self.tb:eraseChar(128, x)

				-- Erase the previous character
				x = x - self.tb:eraseChar(c:byte(), x, 1)

				-- Update the output string
				outstring = outstring:sub(1, -2)

				nchars = nchars - 1
			end
		elseif (inkey == btkeys.ESCAPE) then
			if (nchars ~= 0) then
				self:clearLine()
				self.tb:SetCursor(240, y)
				self.tb:Print("<")

				x = 0
				self.tb:SetCursor(x, y)

				outstring = ""
			end
		else
			if ((inkey:match("%w")) or (inkey == " ") or
			    (inkey == ".") or (inkey == ",")) then
				outstring = outstring .. inkey
				nchars = nchars + 1
				self.tb:eraseChar(128, x)
				self.tb:SetCursor(x, y)
				x = x + self.tb:putc(inkey:byte())
			end
		end
	end

	return outstring
end

----------------------------------------
-- readNumber()
----------------------------------------
function textBox:readNumber(inMaxLen)
	local t

	self:print("\n")
	t = self:readString(inMaxLen)
	t = string.match(t, "^%d+$")
	if (t ~= nil) then
		return tonumber(t)
	else
		return false
	end
end

----------------------------------------
-- scrollingSelect()
--
----------------------------------------
function textBox:scrollingSelect(inTable, inFunc)
	local topIndex		= 1
	local highIndex		= 0
	local i
	local k
	local inkey
	local listItems

	listItems = inTable:toArray()

	local function scroll_reset()
		local i

		self:clear()

		if (#listItems > 0) then
			for i = 0,10 do
				local n = i + topIndex
	
				if (i ~= 0) then
					self:print("\n")
					inFunc(inTable, listItems[n])
				else
					inFunc(inTable, listItems[n])
				end

				if (n >= #listItems) then
					break
				end
			end
		end

		self:setCursor(0, 11)
		self:putc(129)

		self:setCursor(240, 11)
		self:putc(130)


		self:setCursor(0, 11)
		self:print("    (CANCEL)")

		self.tb:Highlight(highIndex)
	end

	scroll_reset()

	while true do
		inkey = getkey()

		if (inkey == btkeys.DOWN) then
			if (highIndex == 10) then
				-- Increment the top index if we are at the end
				-- of the window but not at the end of the list
				--
				if ((topIndex + highIndex) < #listItems) then
					topIndex = topIndex + 1
					scroll_reset()
				end
			else
				if (highIndex < #listItems - 1) then
					self.tb:UnHighlight(highIndex)
					highIndex = highIndex + 1
					self.tb:Highlight(highIndex)
				end
			end
		elseif (inkey == btkeys.UP) then
			if (highIndex == 0) then
				if (topIndex ~= 1) then
					topIndex = topIndex - 1
					scroll_reset()
				end
			else
				self.tb:UnHighlight(highIndex)
				highIndex = highIndex - 1
				self.tb:Highlight(highIndex)
			end
		elseif (inkey == btkeys.PAGEUP) then
			if (topIndex <= 10) then
				topIndex = 1
			else
				topIndex = topIndex - 10
			end
			scroll_reset()
		elseif (inkey == btkeys.PAGEDOWN) then
			if ((topIndex + 20) > #listItems) then
				topIndex = #listItems - 10
			else
				topIndex = topIndex + 10
			end
			scroll_reset()
		elseif ((inkey == btkeys.RETURN) or
			(inkey == " ")) then
			return listItems[topIndex + highIndex]
		elseif ((inkey == btkeys.ESCAPE) or
			(inkey == "C")) then
			return false
		end
	end
end

----------------------------------------
-- select()
--
-- Non-scrolling select from a list
----------------------------------------
function textBox:select(inHighIndex, inNumItems)
	local highIndex	= inHighIndex
	local lineIndex	= 1
	local inkey

	self.tb:Highlight(highIndex)

	while (true) do
		inkey = getkey()

		if (inkey == btkeys.DOWN) then
			if (lineIndex < inNumItems) then
				self.tb:UnHighlight(highIndex)
				highIndex = highIndex + 1
				lineIndex = lineIndex + 1
				self.tb:Highlight(highIndex)
			end
		elseif (inkey == btkeys.UP) then
			if (lineIndex > 1) then
				self.tb:UnHighlight(highIndex)
				highIndex = highIndex - 1
				lineIndex = lineIndex - 1
				self.tb:Highlight(highIndex)
			end
		elseif ((inkey == btkeys.RETURN) or
			(inkey == " ")) then
			return lineIndex
		elseif ((inkey > "0") and (inkey < tostring(numItems))) then
			return tonumber(inkey)
		elseif (inkey == "C") then
			return false
		end
	end
end


----------------------------------------
-- Some convenience functions for
-- repeated actions
----------------------------------------

function textBox:csplash(inClear, inPrintContinue, format, ...)
	self:cdprint(inClear, false, format, ...)
	if (inPrintContinue) then
		self:printContinue()
	end
	getkey()
end

if false then
function textBox:splash(format, ...)
	self:print(format, ...)
	self:printContinue()
	getkey()
end

function textBox:splashMessage(format, ...)
	self:clear()
	self:print(format, ...)
	self:printContinue()
	getkey()
end
end

function textBox:printContinue()
	self:setCursor(0, 11)
	self:print("   (CONTINUE)")
end

function textBox:printCancel()
	self:setCursor(0, 11)
	self:print("    (CANCEL)")
end

function textBox:printExit()
	text:setCursor(0, 11)
	self:print("     (EXIT)")
end

function textBox:getYesNo()
	local inkey

	self:print("\n\nYes\nNo")

	while true do
		inkey = getkey()

		if (inkey == "N") then
			return false
		elseif (inkey == "Y") then
			return true
		end
	end
end

function textBox:printEllipsis()
	self:print("...\n\n")	
	party:display()
	timer:delay()
end






























