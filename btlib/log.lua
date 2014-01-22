----------------------------------------
-- log class
--
-- Class for logging messages
----------------------------------------
log = {
	-- Constants
	LOG_EMERG	= 1,
	LOG_ALERT	= 2,
	LOG_CRIT	= 3,
	LOG_ERR		= 4,
	LOG_WARNING	= 5,
	LOG_NOTICE	= 6,
	LOG_INFO	= 7,
	LOG_DEBUG	= 8,

	level	= 0,
	file	= io.stdout
}

local message = {
	"EMERG",
	"ALERT",
	"CRIT",
	"ERR",
	"WARNING",
	"NOTICE",
	"INFO",
	"DEBUG"
}

function log:getLocation(inDepth)
	local dinfo

	dinfo = debug.getinfo(inDepth, "Sl")
	return string.format("%s:%s",
		string.match(dinfo.short_src, "[^\\/]*$"),
		dinfo.currentline
		)
end

function log:print(level, format, ...)
	if (level == self.level) then
		local location
		local dstring

		location = string.format("%s %s", message[level],
			self:getLocation(3))
		if (select("#", ...) == 0) then
			self.file:write(location.." "..tostring(format).."\n")
		else
			self.file:write(location.." "..
				string.format(format, ...) .. "\n")
		end
		self.file:flush()
	end
end
