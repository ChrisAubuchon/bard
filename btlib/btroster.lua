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
-- btroster:rename()
----------------------------------------
function btroster:rename(inOld, inNew)
	if ((not inOld) or (not inNew)) then
		return
	end

	if ((type(inOld) ~= "string") or (type(inNew) ~= "string")) then
		return
	end

	if (self.characters[inOld]) then
		self.characters[inNew] = self.characters[inOld]
		self.characters[inOld] = nil
	elseif (self.parties[inOld]) then
		self.parties[inNew] = self.parties[inOld]
		self.parties[inOld] = nil
	end

	self:write()
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

----------------------------------------
-- btroster:countCharacters()
----------------------------------------
function btroster:countCharacters()
	local count = 0

	for _,_ in pairs(self.characters) do
		count = count + 1
	end

	return count
end


----------------------------------------
-- btroster:countParties()
----------------------------------------
function btroster:countParties()
	local count = 0

	for _,_ in pairs(self.parties) do
		count = count + 1
	end

	return count
end

----------------------------------------
-- btroster:readParty()
----------------------------------------
function btroster:readParty(inName)
	return self.parties[inName]
end

----------------------------------------
-- btroster:writeParty()
----------------------------------------
function btroster:writeParty(inName, inParty)
	self.parties[inName] = inParty:toTable()
	self:write()
end

