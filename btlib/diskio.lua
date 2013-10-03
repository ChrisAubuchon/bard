diskio = {}

local function __false_mt(t, key)
	local val

	val = rawget(t, key)
	if (val == nil) then
		return false
	end

	return val
end

----------------------------------------
-- readTable()
----------------------------------------
function diskio:readTable(inName, inCreateFlag)
	local rval
	local name

	inCreateFlag = (inCreateFlag == true)
	name = string.format("json/%s.json", inName)

	if (inCreateFlag) then
		local path = sys.dataPath .. "/" .. name
		local tmp  = io.open(path, "r")

		if (tmp == nil) then
			tmp = io.open(path, "w")
			if (tmp == nil) then
				error("Error creating: " .. tostring(path))
			end
			tmp:write("{}")
		end

		tmp:close()
	end

	rval = jansson.json2lua(name)
	setmetatable(rval, { __index = __false_mt })
	return rval
end

----------------------------------------
-- writeTable()
----------------------------------------
function diskio:writeTable(inTable, inName)
	local name = string.format("json/%s.json", inName)

	jansson.lua2json(inTable, name)
end


----------------------------------------
-- readShared()
----------------------------------------
function diskio:readShared(inName, inCreateFlag)
	local rval
	local savedPath

	assert(sys.sharedPath, "Must define sys.sharedPath")

	savedPath = sys.dataPath
	sys.dataPath = sys.sharedPath
	
	rval = diskio:readTable(inName, inCreateFlag)

	sys.dataPath = savedPath

	return rval
end

----------------------------------------
-- writeShared()
----------------------------------------
function diskio:writeShared(inTable, inName)
	local savedPath

	savedPath = sys.dataPath
	sys.dataPath = sys.sharedPath

	diskio:writeTable(inTable, inName)

	sys.dataPath = savedPath
end
