----------------------------------------
-- splashMessage()
--
-- Print a message and wait for a key
-- to continue
----------------------------------------
function splashMessage(format, ...)
	text_clear()
	text_print(format, ...)

	printContinue()

	getkey()
end

----------------------------------------
-- printContinue()
--
-- Helper function to print continue
-- at the bottom of the screen
----------------------------------------
function printContinue()
	text_setCursor(0, 11)
	text_print("   (CONTINUE)")
end

----------------------------------------
-- printCancel()
--
-- Helper function to print cancel at
-- the bottom of the text window
----------------------------------------
function printCancel()
	text_setCursor(0, 11)
	text_print("    (CANCEL)")
end

function printExit()
	text_setCursor(0, 11)
	text_print("     (EXIT)")
end

----------------------------------------
-- getYesNo()
--
-- Get a Yes or No answer from the 
-- player
----------------------------------------
function getYesNo()
	local inkey

	text_print("\n\nYes\nNo")

	while true do 
		inkey = getkey()

		if (inkey == "N") then
			return false
		elseif (inkey == "Y") then
			return true
		end
	end
end

----------------------------------------
-- printEllipsis
--
-- Print "...", update roster and delay
-- according to current delay setting
----------------------------------------
function printEllipsis()
	text_print("...\n\n")
	party:display()
	local XXX_text_table_delay = true
end
