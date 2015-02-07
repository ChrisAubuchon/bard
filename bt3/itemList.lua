local disk_itemList	= diskio:readTable("items")


itemList	= {}

----------------------------------------
-- itemList:getItem()
----------------------------------------
function itemList:getItem(inName)
	local self	= object:new()

	self:addParent(disk_itemList[inName])
	self:addParent(itemList)

	return self
end
