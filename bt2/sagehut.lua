sagehut = building:new("The Sage", "PIC_SAGE")
function sagehut:enter()
--	if (party:hasWinner()) then
--		bigpic:setBigpic("Empty Hut", "PIC_EMPTYHUT")
--		text:csplash(true, true, "\nYou're in an empty hut.")
--		text:clear()
--	elseif (party:hasItem("The Scepter")) then
--		self:resetBigpic()
--		text:csplash(true, true,
--			"\"Welcome, my friends, to the home of Lagoth " ..
--			"Zanta! You have been cunning, it is true, " ..
--			"buy your days have now come to an end. Your " ..
--			"quest is through, fools. Prepare to die.\""
--			)
--		if (not battle:new("Balder Guard", 16, "Balder Guard", 16, 
--				"Lagoth Zanta", 1)) then
--			if (globals.partyDied) then
--				return false
--			end
--			currentLevel:turnParty("back")
--			return true
--		else
--			local xxx_add_one_million_xp_to_party
--			local xxx_set_hasWon_flag_on_party
--
--			bigpic:drawImage("PIC_VICTORY")
--			text:csplash(true, true, 
--				"Well done! At the death of Zanta the armies "..
--				"of the Realm and march to defeat the " ..
--				"invading hordes, who will easily fall. " ..
--				"The King himself enters the hut and salutes "..
--				"you."
--				)
--			text:csplash(true, true,
--				"\"Oh mighty ones, I thank thee truly for " ..
--				"they service to the realm. May the All High "..
--				"bless thee and keep thee strong and "..
--				"prosperous. In my own little way, I give "..
--				"you all a little bonus."
--			)
--			text:csplash(true, true,
--				"Congratulations, your quest is over. But a " ..
--				"new one is bound to begin..."
--				)
--			text:clear()
--			self.exit = true
--			globals.gameState = globals.STATE_GUILD
--			return true
--		end
--	else
		return self:askSage()
--	end
end

function sagehut:askSage()
	local inkey

	self:resetBigpic()
	repeat
		text:cdprint(true, false, 
			"The Sage says, \"Gold can buy thee the wisdom " ..
			"of the ancients, my friends.\"" ..
			"\n\nYou can:\n\n" ..
			"(T)alk to the Sage\n" ..
			"(E)xit his hut"
			)
		inkey = getkey()

		if (inkey == "T") then
			self:talkToSage()
		end
	until (inkey == "E")

	return true
end

local sageTopics = {}
sageTopics["TOMBS"] = {
	text =	"\"Hidden beneath the city of Ephesus, the " ..
		"Tombs have been closed for centuries. Seek " ..
		"the entrance at the Temple of Darkness, in " ..
		"the city's center.\"",
	cost =	1000
}

sageTopics["FANSKAR"] = {
	text =	"\"Fanskar is a deadly warrior-mage who " ..
		"tried once to claim the title of Destiny " ..
		"Knight and failed. His castle is a " ..
		"well-guarded fortress, located south-west " ..
		"of Colosse.\"",
	cost =	5500
}

sageTopics["DARGOTH"] = {
	text =	"\"Dargoth is a creature more animal than " ..
		"man, whose tower is hidden in the city of " ..
		"Philippi. Take care, friends, for he is " ..
		"death itself.\"",
	cost =	10000
}

sageTopics["MAZE OF DREAD"] = {
	text =	"\"The Maze is said to be the home of the " ..
		"Graphnar Fist, a group of elite " ..
		"mercenaries who kill for both gold and " ..
		"pleasure. The Maze itself exists from " ..
		"ancient times. Seek it in Thessalonica.\"",
	cost =	48000
}

sageTopics["OSCON"] = {
	text = 	"\"In the city of Corinth is the Fortress " ..
		"of the mage Oscon, whose strength is " ..
		"legendary and whose skill surpasses even " ..
		"Mangar's. Only an item of Kazdek will gain " ..
		"you entrance.\"",
	cost =	120000
}

sageTopics["KAZDEK"] = {
	text =	"\"Ah, yes, Kazdek. The immortal one, as he " ..
		"is called, he is in fact a living statue. " ..
		"Seek him between Thessalonica and Colosse, " ..
		"with the word of his name.\"",
	cost =	5000
}

sageTopics["GREY CRYPT"] = {
	text =	"\"Take care, my friends. None has left " ..
		"the Grey Crypt alive. Know you, however, " ..
		"that no magical spells will work therein.\"",
	cost =	125000
}

sageTopics["DESTINY STONE"] = {
	text =	"\"A rock it is, in fact, yet much more. " ..
		"Ponder this, my friends:\nNever find and " ..
		"never fool, the last one drinks the poison " ..
		"pool, the first one reads the ancient " ..
		"tome, the third one breaks the double dome.\"",
	cost =	230000
}

sageTopics["LAGOTH ZANTA"] = {
	text =	"\"An evil genius is Lagoth, else he'd not " ..
		"have posed the threat to the Land that he " ..
		"has. Yet, an enemy to be respected, I " ..
		"think. His whereabouts are unknown, but I'd "..
		"not be surprised to find him near.\"",
	cost =	10000
}

sageTopics["LUCKSHIELD"] = {
	text =	"\"Ah, that one makes you lucky at " ..
		"avoiding bad spells, thrown weapons, and dragon breath.\"",
	cost =	500
}

sageTopics["DEATH STARS"] = {
	text =	"\"A ninja weapon, they do 16-64 damage " ..
		"and have a range of 60 feet.\"",
	cost =	500
}

sageTopics["MAGE STAFF"] = {
	text =	"The Mage Staff regenerates spell points " ..
		"and boosts the AC by 2.\"",
	cost =	500
}

sageTopics["THIEF DAGGER"] = {
	text =	"\"A thief with that has an increased " ..
		"chance of hiding in shadows.\"",
	cost =	500
}

sageTopics["KATOS BRACER"] = {
	text =	"\"An ingenious device! If given to a " ..
		"monster ally, he is not likely to turn on you.\"",
	cost = 	500
}

sageTopics["AGS ARROWS"] = {
	text =	"\"Ag's arrows do 24 to 96 in damage and " ..
		"have a range of 90 feet.\"",
	cost =	500
}

sageTopics["ELF BOOTS"] = {
	text =	"\"Ah, that one makes you lucky at " ..
		"avoiding bad spells, thrown weapons, and dragon breath.\"",
	cost =	500
}

sageTopics["RING OF RETURN"] = {
	text =	"\"That ring does a Safety Spell when you " ..
		"use it. Careful of that!\"",
	cost =	500
}

sageTopics["BREATHRING"] = {
	text =	"\"That ring protects the wearer from " ..
		"breath attacks. Most useful!\"",
	cost =	500
}

sageTopics["WIZHELM"] = {
	text =	"\"Ah, that one casts a Wizard War when you use it.\"",
	cost =	500
}

sageTopics["SPEEDBOOTS"] = {
	text =	"\"Those boots aid you in running from " ..
		"fights. Not a manly item...\"",
	cost =	500
}

sageTopics["ZEN ARROWS"] = {
	text =	"\"Zen Arrows...oh yes. They do 40-160 " ..
		"in damage, to a range of 70 feet.\"",
	cost =	500
}

sageTopics["DRUMS OF DEATH"] = {
	text =	"\"The Drums of Death cast a Deathstrike " ..
		"spell on an opponent.\"",
	cost =	500
}

sageTopics["SONG AXE"] = {
	text =	"\"The Song Axe is a wonderous item which " ..
		"always returns to the thrower, and does " ..
		"64-256 hits at a range of 80 feet.\"",
	cost =	500
}

sageTopics["TRICK BRICK"] = {
	text =	"\"That one makes a Wizardwall appear.\"",
	cost =	500
}

sageTopics["ARAMS KNIFE"] = {
	text =	"\"Aram's Knife? Oh yes. 128-512 points " ..
		"of damage, thrown at any range. Not too shabby.\"",
	cost =	500
}

sageTopics["ANGRAS EYE"] = {
	text =	"\"The Eye does Batchspells, as needed.\"",
	cost =	500
}

sageTopics["THORS HAMMER"] = {
	text =	"\"Thor's Hammer does 32 to 128 hits at " ..
		"a range of 70 feet, and it always returns.\"",
	cost =	500
}

sageTopics["SWORD OF ZAR"] = {
	text =	"\"The Sword is rarely spoken of, but it " ..
		"is said to be throwable, up to 80 feet for 96-384 hits.\"",
	cost =	500
}

sageTopics["NOSPEN RING"] = {
	text =	"\"The Nospen...oh yes. It protects against spinning.\"",
	cost =	500
}



local sageNoAnswer = {
	"\"Such relevancies are easily overlooked,\" the Sage says.",
	"\"I have a more interesting tale than that,\" the Sage starts " ..
		"to tell you about his amazing travels abroad.",
	"\"Ah yes\" the Sage says, \"therein lies a tale of great magnitude.\"",
	"\"That is something better left unsaid,\" the Sage whispers."
}

function sagehut:talkToSage()
	local inkey
	local c
	local answer
	local amount

	text:cdprint(true, false, "Who will talk to the Sage?")
	c = party:readSlot()
	if (not c) then
		return
	end

	text:cdprint(true, false, "What will you ask him about?\n")
	answer = text:readString()
	if (not answer) then
		return
	end

	answer = string.gsub(answer, "THE ", "")
	if (not sageTopics[answer]) then
		return
	end

	text:cdprint(true, false,
		"\"Knowledge costs,\" the Sage chuckles.\n" ..
		"How much will you pay him?"
		)
	amount = text:readNumber()
	if ((not amount) or (amount == 0)) then
		text:csplash(false, true,
			"\n\n\"My memory fails me,\" he says."
			)
		return
	end

	if (amount > c.gold) then
		text:csplash(false, true, 
			"\n\n\"Get enough gold first,\" he says."
			)
		return
	end

	if (amount >= sageTopics[answer].cost) then
		text:csplash(true, true, sageTopics[answer].text)
	else
		text:csplash(true, true, sageNoAnswer[random:xdy(1,4)])
	end
end

