btroster = {}


----------------------------------------
-- btroster class
----------------------------------------
function btroster:new(inFileName, inVersion)
	local self = {
		fileName	= inFileName,
		version		= inVersion,
		diskTable	= {},
		characters	= {},
		parties		= {}
	}

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


	btTable.addParent(self, btroster)
	btTable.setClassMetatable(self)

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


