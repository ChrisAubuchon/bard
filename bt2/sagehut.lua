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
sageTopics["TOMBS"] =		"\"Hidden beneath the city of Ephesus, the " ..
				"Tombs have been closed for centuries. Seek " ..
				"the entrance at the Temple of Darkness, in " ..
				"the city's center.\""
sageTopics["FANSKAR"] =		"\"Fanskar is a deadly warrior-mage who " ..
				"tried once to claim the title of Destiny " ..
				"Knight and failed. His castle is a " ..
				"well-guarded fortress, located south-west " ..
				"of Colosse.\""
sageTopics["DARGOTH"] =		"\"Dargoth is a creature more animal than " ..
				"man, whose tower is hidden in the city of " ..
				"Philippi. Take care, friends, for he is " ..
				"death itself.\""
sageTopics["MAZE OF DREAD"] =	"\"The Maze is said to be the home of the " ..
				"Graphnar Fist, a group of elite " ..
				"mercenaries who kill for both gold and " ..
				"pleasure. The Maze itself exists from " ..
				"ancient times. Seek it in Thessalonica.\""
sageTopics["OSCON"] =		"\"In the city of Corinth is the Fortress " ..
				"of the mage Oscon, whose strength is " ..
				"legendary and whose skill surpasses even " ..
				"Mangar's. Only an item of Kazdek will gain " ..
				"you entrance.\""
sageTopics["KAZDEK"] =		"\"Ah, yes, Kazdek. The immortal one, as he " ..
				"is called, he is in fact a living statue. " ..
				"Seek him between Thessalonica and Colosse, " ..
				"with the word of his name.\""
sageTopics["GREY CRYPT"] =	"\"Take care, my friends. None has left " ..
				"the Grey Crypt alive. Know you, however, " ..
				"that no magical spells will work therein.\""
sageTopics["DESTINY STONE"] =	"\"A rock it is, in fact, yet much more. " ..
				"Ponder this, my friends:\nNever find and " ..
				"never fool, the last one drinks the poison " ..
				"pool, the first one reads the ancient " ..
				"tome, the third one breaks the double dome.\""
sageTopics["LAGOTH ZANTA"] =	"\"An evil genius is Lagoth, else he'd not " ..
				"have posed the threat to the Land that he " ..
				"has. Yet, an enemy to be respected, I " ..
				"think. His whereabouts are unknown, but I'd "..
				"not be surprised to find him near.\""
sageTopics["LUCKSHIELD"] =	"\"Ah, that one makes you lucky at " ..
				"avoiding bad spells, thrown weapons, " ..
				"and dragon breath.\"",
sageTopics["DEATH STARS"] =	"\"A ninja weapon, they do 16-64 damage " ..
				"and have a range of 60 feet.\""
sageTopics["MAGE STAFF"] =	"The Mage Staff regenerates spell points " ..
				"and boosts the AC by 2.\""
sageTopics["THIEF DAGGER"] =	"\"A thief with that has an increased " ..
				"chance of hiding in shadows.\""
sageTopics["KATOS BRACER"] =	"\"An ingenious device! If given to a " ..
				"monster ally, he is not likely to turn " ..
				"on you.\"",
sageTopics["AGS ARROWS"] =	"\"Ag's arrows do 24 to 96 in damage and " ..
				"have a range of 90 feet.\""
sageTopics["ELF BOOTS"] =	"\"Ah, that one makes you lucky at " ..
				"avoiding bad spells, thrown weapons, " ..
				"and dragon breath.\"",
sageTopics["RING OF RETURN"] =	"\"That ring does a Safety Spell when you " ..
				"use it. Careful of that!\""
sageTopics["BREATHRING"] =	"\"That ring protects the wearer from " ..
				"breath attacks. Most useful!\""
sageTopics["WIZHELM"] =		"\"Ah, that one casts a Wizard War when " ..
				"you use it.\""
sageTopics["SPEEDBOOTS"] =	"\"Those boots aid you in running from " ..
				"fights. Not a manly item...\""
sageTopics["ZEN ARROWS"] =	"\"Zen Arrows...oh yes. They do 40-160 " ..
				"in damage, to a range of 70 feet.\""
sageTopics["DRUMS OF DEATH"] =	"\"The Drums of Death cast a Deathstrike " ..
				"spell on an opponent.\""
sageTopics["SONG AXE"] =	"\"The Song Axe is a wonderous item which " ..
				"always returns to the thrower, and does " ..
				"64-256 hits at a range of 80 feet.\""
sageTopics["TRICK BRICK"] =	"\"That one makes a Wizardwall appear.\""
sageTopics["ARAMS KNIFE"] =	"\"Aram's Knife? Oh yes. 128-512 points " ..
				"of damage, thrown at any range. Not too " ..
				"shabby.\""
sageTopics["ANGRAS EYE"] =	"\"The Eye does Batchspells, as needed.\""
sageTopics["THORS HAMMER"] =	"\"Thor's Hammer does 32 to 128 hits at " ..
				"a range of 70 feet, and it always returns.\"",
sageTopics["SWORD OF ZAR"] =	"\"The Sword is rarely spoken of, but it " ..
				"is said to be throwable, up to 80 feet " ..
				"for 96-384 hits.\""
sageTopics["NOSPEN RING"] =	"\"The Nospen...oh yes. It protects against " ..
				"spinning.\""


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
end

