sys.dataPath = sys.dataPath .. "/bt1"

package.path = package.path .. ";" .. "../bt1/?.lua"

require "btdebug"
require "sdl"
require "sdl_video"
require "globals"
require "text"
require "btio"

video_init()
