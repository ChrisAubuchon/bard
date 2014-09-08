camp = {}

----------------------------------------
-- camp:enter()
----------------------------------------
function camp:enter()
	local self	= object:new()
	local inkey

	self:addParent(camp)
	self:addParent(building:new("The Ruin", "PIC_CAMP"))


--	XXX - Uncomment when ready
--	party:songTimeout()
--	party:disablePassiveEffects()

	self:resetBigpic()

	repeat
		text:cdprint(true, false, 
			"Thou art in the Camp of Skara Brae.\n\n")

--		if (party:hasEmptySlot()) then
--			text:print("Add a member\n")
--		end

--		if (not party:isEmpty()) then
--			text:print("Remove a member\n")
--		else
--			text:print("Rename a member\n")
--		end

		text:print("Create a member\n" ..
				"Transfer characters\n" ..
				"Delete a member\n")

--		if (not party:isEmpty()) then
--			text:print("Save the party\n")
--			text:print("Enter Wilderness")
--		else

		text:print("Leave the game\n")

		inkey = getkey()

		log:print(log.LOG_DEBUG, inkey)
	until (inkey == "L")

	globals.gameState = globals.STATE_EXIT
end
