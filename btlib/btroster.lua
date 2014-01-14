btroster = {}


----------------------------------------
-- btroster class
----------------------------------------
function btroster:new(inFileName, inVersion)
	local self = object:new()

	self:addParent(btroster)

	self.fileName	= inFileName
	self.version	= inVersion
	self.diskTable	= {}
	self.characters	= {}
	self.parties	= {}

	local function init()
		local k
		local v

		self.diskTable = diskio:readShared(inFileName, true)

		if (not self.diskTable.parties) then
			print("No parties table.")
			self.diskTable.parties = {}
			diskio:writeShared(self.diskTable, inFileName)
		end

		if (not self.diskTable[inVersion]) then
			self.diskTable[inVersion] = {}
			return
		end

		for k,v in pairs(self.diskTable[inVersion]) do
			self.characters[k] = v
		end

		for k,v in pairs(self.diskTable.parties) do
			self.parties[k] = v
		end
	end

	init()

	return self
end

----------------------------------------
-- write()
----------------------------------------
function btroster:write()
	local k
	local v

	self.diskTable[self.version] = self.characters
	self.diskTable.parties = self.parties

	diskio:writeShared(self.diskTable, self.fileName)
end


----------------------------------------
-- toArray()
----------------------------------------
function btroster:toArray()
	local	t = {}
	local	k

	for k,_ in pairs(self.characters) do
		table.insert(t, k)
	end

	for k,_ in pairs(self.parties) do
		table.insert(t, k)
	end

	table.sort(t)

	return t
end

----------------------------------------
-- nameExists()
----------------------------------------
function btroster:nameExists(inName)
	if ((self.characters[inName] ~= nil) or 
	    (self.parties[inName] ~= nil)) then
		return true
	end

	return false
end

----------------------------------------
-- isParty()
----------------------------------------
function btroster:isParty(inName)
	if (self.parties[inName] ~= nil) then
		return true
	end

	return false
end

----------------------------------------
-- remove()
----------------------------------------
function btroster:remove(inName)
	if (self.characters[inName] ~= nil) then
		self.characters[inName] = nil
	elseif (self.parties[inName] ~= nil) then
		self.parties[inName] = nil
	else
		return
	end

	self:write()
end
