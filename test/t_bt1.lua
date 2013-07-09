package.path = package.path .. ";" .. "../bt1/?.lua"

require "sdl"

testkeys = {
	sdl.SDLK_RETURN,
	sdl.SDLK_RETURN,
	"a",
	sdl.SDLK_RETURN,
	"e",
	"d",
	"3",
	"f",
	"d",
	"d",
	"d",
	"y",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"r",
	"q"
}
sdl.inputKeys(testkeys)

require "bt1"
