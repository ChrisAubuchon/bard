bank = object:new()

bank:addParent(building:new("The bank", "PIC_BANKINT"))

bank.accounts = diskio:readTable("accounts", true)

----------------------------------------
-- saveAccounts()
----------------------------------------
function bank:saveAccounts()
	diskio:writeTable(self.accounts, "accounts")
end

----------------------------------------
-- bank:getAccount()
----------------------------------------
function bank:getAccount()
	local anum

	text:cdprint(true, false, 
		"Please enter your 4 digit accounts number:\n\n"
		)
	anum = text:readNumber(4)
	if (not anum) then
		return false
	end

	return tostring(anum)
end

----------------------------------------
-- bank:accountExists()
----------------------------------------
function bank:accountExists(inAccount)
	if (self.accounts[inAccount]) then
		return true
	else
		return false
	end
end

----------------------------------------
-- bank:countAccounts()
----------------------------------------
function bank:countAccounts()
	local n
	local rval

	n = next(self.accounts)
	if (not n) then
		return 0
	end

	rval = 1
	while (next(self.accounts, n)) do
		n = next(self.accounts, n)
		rval = rval + 1
	end

	return rval
end

----------------------------------------
-- bank:listAccounts()
----------------------------------------
function bank:listAccounts()
	local anum
	local amount

	text:cdprint(true, false, "Acnt   Amount")

	local xxx_check_format_look_in_DOS_version
	for anum,amount in pairs(self.accounts) do
		text:print("\n")
		text:print("%4s %8d", anum, amount)
	end
	text:printContinue()
	getkey()
end

----------------------------------------
-- bank:closeAccount()
----------------------------------------
function bank:closeAccount(inChar)
	local anum
	local gold

	anum = self:getAccount()
	if (not anum) then
		return
	end

	if (not self:accountExists(anum)) then
		text:csplash(true, true, "Sorry, I can't find that account.")
		return
	end

	inChar.gold = inChar.gold + self.accounts[anum]
	self.accounts[anum] = nil
	self:saveAccounts()

	text:csplash(true, true, "Your gold has been withdrawn, and your " ..
			"account closed.")
end

----------------------------------------
-- bank:openAccount()
----------------------------------------
function bank:openAccount(inChar)
	local anum
	local gold

	anum = self:getAccount()
	if (not anum) then
		return
	end

	if (self:accountExists(anum)) then
		text:csplash(true, true, "Sorry, that account already exists.")
		return
	end

	-- DOS version has a limit of 10 accounts. Might be able
	-- to increase.
	--
	if (self:countAccounts() >= 10) then
		text:csplash(true, true, 
			"Sorry, the bank has no spare accounts."
			)
		return
	end

	text:cdprint(true, false, 
		"Please enter the amount of gold to deposit:\n\n")
	gold = text:readNumber()
	if (not gold) then
		return	
	end

	if (gold > inChar.gold) then
		text:cdprint(false, true, "Not enough gold.   ")
		return	
	end

	self.accounts[anum] = gold
	inChar.gold = inChar.gold - gold
	text:csplash(true, true, "Your account is set up, sir.")
	self:saveAccounts()
end

----------------------------------------
-- selectOption()
----------------------------------------
function bank:selectOption(inChar)
	local inkey

	repeat
		text:cdprint(true, false, 
			"Greetings %s. Would you like to:", inChar.name
			)
		text:setCursor(0, 3)
		text:print(
			"\nOpen an account."			..
			"\nClose an account."			..
			"\nList accounts."			..
			"\nPool your gold."			..
			"\nFinished."				..
			"\n\nYou have: %d gold.",
			inChar.gold
			)
		inkey = getkey()
		if ((inkey > "0") and (inkey < "8")) then
			local c = party:readMember(inkey)
			if (c) then
				c:printCharacter()
				self:resetBigpic()
			end
		elseif (inkey == "C") then
			self:closeAccount(inChar)
		elseif (inkey == "L") then
			self:listAccounts()
		elseif (inkey == "O") then
			self:openAccount(inChar)
		elseif (inkey == "P") then
			inChar:poolGold()
		end
	until (inkey == "F")
end

----------------------------------------
-- selectCharacter()
----------------------------------------
function bank:selectCharacter()
	local inkey
	local c

	repeat
		text:cdprint(true, false, 
			"Greetings, honored ones. Welcome to Bedder's " ..
			"Bank for the Bold. Which of you is interested " ..
			"in a transaction?"
			)
		text:printExit()
		inkey = getkey()
		c = party:readMember(inkey)
		if (c) then
			self:selectOption(c)
		end
	until (inkey == "E")
end

----------------------------------------
-- nightCheck()
----------------------------------------
function bank:nightCheck()
	if (globals.isNight) then
		text:csplash(true, true, 
			"\nThe bank is closed for the night."
			)
		text:clear()
		return true
	end

	return false
end

----------------------------------------
-- enter()
----------------------------------------
function bank:enter()
	if (self:nightCheck()) then
		currentLevel:turnParty("back")
		return
	end
	
	self:resetBigpic()
	self:selectCharacter()

	currentLevel:turnParty("back")
end
