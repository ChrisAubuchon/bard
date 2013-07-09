require "bt1test"
require "guild"

testkeys = {
	"c",
	"1",
	"r",
	"1",
	"1",
	"a",
	"a",
	sdl.SDLK_RETURN,
	"d",
	sdl.SDLK_RETURN,
	"y",
	"q"
}

sdl.inputKeys(testkeys)
guild.enter()
