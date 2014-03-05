----------------------------------------
-- declare_global()
--
-- Global variables need to be declared
-- before they can be used. This is to
-- easily catch typos in code. If you
-- try to assign a value to a variable
-- that has not been declared the game
-- will error
----------------------------------------
function declare_global(name, initval)
	rawset(_G, name, initval or false)
end

----------------------------------------
-- Set up the global registry to only
-- index and newindex on a previously
-- declared value
----------------------------------------

-- Set the data path
--
package.path = package.path .. ";../btlib/?.lua"
sys.sharedPath = "../data/share"
sys.dataPath = "../data/bt2"

if false then
setmetatable(_G, {
	__newindex = function(_, n)
		error("Attempt to write to undeclared global " .. n, 2)
	end,
	__index = function(_, n)
		error("Attempt to read from undeclared global " .. n, 2)
	end,
	})
end

require "declares"
require "btlib"

log.level = log.LOG_DEBUG

gfx:Init("Bard's Tale II")
require "globals"
require "structures"
require "btdebug"

local title = gfxImage:new("images/title.png", "png")
title:Draw(nil)

require "icons"
require "bigpic"
require "b2lib"
require "random"
require "btapi"
require "linkedList"
require "btroster"
require "roster"
require "battleBonus"
require "battleMonster"
require "battlePlayer"
require "battleParty"
require "character"
require	"monsterData"
require "monster"
require "songs"
require "monsterParty"
require "monsterGroup"
require "summon"
require "durationSpell"
require "action"
require "party"
require "guild"
require "level"
require "building"
require "sagehut"
require "wild"
require "bank"
require "temple"
require "tavern"
require "review"
require "roscoes"
require "shoppe"
require "cityBuildings"
require "city"
require "dunSquares"
require "dun"
require "battle"

getkey()

title = gfxImage:new("images/screen.png", "png")
title:Draw(nil)

text:print("THIS IS A ")
text:print("test of the emergency")
getkey()
os.exit()

text:clear()
text:print("\n   (POOL GOLD)\n   (TRADE GOLD)")
getkey()

text:clear()

text:print("A Forest Giant slams MAGICY, and hits for 5 points of damage.")
getkey()

text:print("A Forest Giant slams MAGICY")
text:print(", and hits for 5 points of damage.")
getkey()

text:clear()
text:print("Thou art in the Guild of Adventurers.")
getkey()

text:cdprint(true, false, "You face west")
text:csplash(false, true, ", 23 squares north\n 2 squares east of the sages hut.")

text:csplash(true, true, "You face west, 23 squares north\n 2 squares east of the sages hut.")

os.exit()

repeat
	if (globals.gameState == globals.STATE_GUILD) then	
		globals.partyDied = false
		if (party.song.active) then
			party.song:deactivate()
		end

		party.detect:deactivate()
		party.shield:deactivate()
		party.levitate:deactivate()
		party.light:deactivate()
		party.compass:deactivate()
		guild:enter()
	elseif (globals.gameState == globals.STATE_INGAME) then
		currentLevel:main()
	end
until (globals.gameState == globals.STATE_EXIT)

