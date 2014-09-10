party	= linkedList:new("partyNext", "partyPrev")
-- XXX
--party:addParent(battleBonus)

-- party Graphics objects
party.gfxRect		= gfxRect:New(24, 288, 628, 112)
party.gfxSurface	= gfxSurface:New(628, 112)

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
		self.gfxSurface:Blit(gfxRect:New(256, y, 0, 0), renderedText, nil)
		renderedText = monoP:Render(inStrings.maxhp, globals.colors[16])
		self.gfxSurface:Blit(gfxRect:New(320, y, 0, 0), renderedText, nil)
		renderedText = monoP:Render(inStrings.curhp, globals.colors[1])
		self.gfxSurface:Blit(gfxRect:New(384, y, 0, 0), renderedText, nil)
		renderedText = monoP:Render(inStrings.maxsppt, globals.colors[16])
		self.gfxSurface:Blit(gfxRect:New(448, y, 0, 0), renderedText, nil)
		renderedText = monoP:Render(inStrings.cursppt, globals.colors[1])
		self.gfxSurface:Blit(gfxRect:New(512, y, 0, 0), renderedText, nil)
		renderedText = monoP:Render(inStrings.class, globals.colors[16])
		self.gfxSurface:Blit(gfxRect:New(576, y, 0, 0), renderedText, nil)
	end
end
