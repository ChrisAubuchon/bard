local function __false_mt(t, key)
	local val

	val = rawget(t, key)
	if (val == nil) then
		return false
	end

	return val
end

----------------------------------------
-- Convert a table to a file.
--
-- If create is true, create the file if
-- it doesn't already exist
----------------------------------------
function read_table(tname, create)
	create = (create == true)
	local rval

	local name = string.format("json/%s.json", tname)

	if (create) then
		local path = sys.dataPath .. "/" .. name
		local tmp = io.open(path, "r")

		if (tmp == nil) then
			tmp = io.open(path, "w")

			tmp:write("{}")
		end

		tmp:close()
	end

	rval = jansson.json2lua(name)
	setmetatable(rval, { __index = __false_mt })
	return rval
end

----------------------------------------
-- Write a table to disk
----------------------------------------
function write_table(t, tname)
	local name = string.format("json/%s.json", tname)

	jansson.lua2json(t, name)
end

function false_table(t)
	setmetatable(t, { __index = __false_mt })
end
