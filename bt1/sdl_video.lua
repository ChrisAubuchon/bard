
----------------------------------------
-- load_image()
--
-- Interface to image loading for SDL
----------------------------------------
function load_image(name, type)
	if (type == "png") then
		return sdl.img.LoadPNG(name)
	elseif (type == "bta") then
		return sdl.img.LoadBTA(name)
	else
		error("Invalid image type: " .. type, 2)
	end
end

----------------------------------------
-- video_rectangle()
--
-- Interface to sdl.NewRect
----------------------------------------
function video_rectangle(x,y,w,h)
	return sdl.NewRect(x,y,w,h)
end

----------------------------------------
-- video_color()
--
-- Interface to sdl.NewColor
----------------------------------------
function video_color(r,g,b)
	return sdl.NewColor(r,g,b)
end

function video_new_window(w, h, d)
	return sdl.CreateRGBSurface(w,h,d)
end

local vid_conf = {
	main_window = {
		width = 640,
		height = 400,
		depth = 8
	},
	title = "Bard's Tale Engine"
}

function video_init()

	sdl.Init(sdl.SDL_INIT_TIMER, sdl.SDL_INIT_AUDIO, sdl.SDL_INIT_VIDEO)

	m_window = sdl.SetVideoMode(vid_conf["main_window"].width,
				    vid_conf["main_window"].height,
				    vid_conf["main_window"].depth,
				    sdl.SDL_HWSURFACE, sdl.SDL_HWPALETTE,
				    sdl.SDL_DOUBLEBUF)
end
