-- XXX
--party:addParent(battleBonus)

-- party Graphics objects
party.gfxRect		= gfxRect:New(24, 288, 628, 112)
party.gfxSurface	= gfxSurface:New(628, 112)

----------------------------------------
-- party:hasEmptySlot()
----------------------------------------
function party:hasEmptySlot()
	return (self.size <= 7)
end

----------------------------------------
-- party:printStatusLine()
----------------------------------------
function party:printStatusLine(inStrings, inSlot)
	local renderedText
	local monoP
	local varP
	local y

	y	= 16 * inSlot
	monoP	= globals.fonts.mono
	varP	= globals.fonts.var
	self.gfxSurface:Fill(gfxRect:New(0, y, 628, 16), globals.colors[8])

	if (inStrings) then
		renderedText = varP:Render(inStrings.name, globals.colors[1])
		self.gfxSurface:Blit(gfxRect:New(0, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(inStrings.ac, globals.colors[16])
		self.gfxSurface:Blit(
			gfxRect:New(240, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(inStrings.maxhp, globals.colors[16])
		self.gfxSurface:Blit(
			gfxRect:New(320, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(inStrings.curhp, globals.colors[1])
		self.gfxSurface:Blit(
			gfxRect:New(384, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(
			inStrings.maxsppt, globals.colors[16])
		self.gfxSurface:Blit(
			gfxRect:New(448, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(
			inStrings.cursppt, globals.colors[1])
		self.gfxSurface:Blit(
			gfxRect:New(512, y, 0, 0), renderedText, nil)

		renderedText = monoP:Render(inStrings.class, globals.colors[16])
		self.gfxSurface:Blit(
			gfxRect:New(576, y, 0, 0), renderedText, nil)
	end
end

----------------------------------------
-- party:display()
----------------------------------------
function party:display()
	local i
	local member
	local strings

	for i = 0,6 do
		self:printStatusLine(false, i)
	end

	i = 0
	for member in self:iterator() do
		strings = member:getStatusLine()
		self:printStatusLine(strings, i)
		i = i + 1
	end

	self.gfxSurface:Draw(self.gfxRect)
end

----------------------------------------
-- party:addParty()
----------------------------------------
function party:addParty(inParty)
	local name

	text:clear()

	for _,name in ipairs(inParty) do
		if (not self:hasEmptySlot()) then
			text:cdprint(false, true, "The roster is full.\n")
		elseif (self:findByName(name)) then
			text:cdprint(false, true,
				"%s\nis already in the party.\n",
				name)
		elseif (not roster:nameExists(name)) then
			text:cdprint(false, true,
				"%s\nThere's no one here named that!\n",
				name)
		else
			self:addMember(roster:readCharacter(name))
		end
	end

	self:display()
end
