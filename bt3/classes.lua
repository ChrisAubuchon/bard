classes = object:new()

local classData	= diskio:readTable("classes")

----------------------------------------
-- classes:getPic()
----------------------------------------
function classes:getPic(inClass)
	if (not classData[inClass]) then
		return nil
	end

	return classData[inClass].pic
end

----------------------------------------
-- classes:getStartingAbilities()
----------------------------------------
function classes:getStartingAbilities(inCharTable)
	if (inCharTable.class == "Conjurer") then
		inCharTable.spells.MAFL		= true
		inCharTable.spells.ARFI		= true
		inCharTable.spells.TRZP		= true
	elseif (inCharTable.class == "Magician") then
		inCharTable.spells.VOPL		= true
		inCharTable.spells.QUFI		= true
		inCharTable.spells.SCSI		= true
	elseif (inCharTable.class == "Rogue") then
		inCharTable.disarmTraps		= 20
		inCharTable.identify		= 20
		inCharTable.hideInShadows	= 20
	elseif (inCharTable.class == "Bard") then
		inCharTable.tunes		= 1
		inCharTable.songs.SIRROBIN	= true
		inCharTable.songs.SAFETY	= true
		inCharTable.songs.SANCTUARY	= true
		inCharTable.songs.BRING		= true
		inCharTable.songs.RHYME		= true
		inCharTable.songs.MELODY	= true
	elseif (inCharTable.class == "Hunter") then
		inCharTable.critHit		= 5
	end
end

----------------------------------------
-- classes:getStartingInventory()
----------------------------------------
function classes:getStartingInventory(inCharTable)
	local cclass	= inCharTable.class
	local inv

	inv = {}

	if ((cclass == "Warrior") or (cclass == "Paladin") or
	    (cclass == "Hunter")  or (cclass == "Bard")) then
		if (cclass == "Bard") then 
			table.insert(inv, {
				name		= "Mandolin",
				isEquipped	= true,
				isIdentified	= true
				})
			table.insert(inv, {
				name		= "Wineskin",
				isEquipped	= true,
				isIdentified	= true,
				contains	= "Spirits",
				count		= 10
				})
		end
		table.insert(inv, {
			name		= "Broadsword",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Scale Armor",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Buckler",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Helm",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Leather Gloves",
			isEquipped	= true,
			isIdentified	= true
			})
	elseif (cclass == "Monk") then
		table.insert(inv, {
			name		= "Short Sword",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Leather Armor",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Canteen",
			isEquipped	= true,
			isIdentified	= true,
			contains	= "Water",
			count		= 10
			})
	elseif (cclass == "Rogue") then
		table.insert(inv, {
			name		= "Short Sword",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Leather Armor",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Buckler",
			isEquipped	= true,
			isIdentified	= true
			})
	elseif ((cclass == "Conjurer") or (cclass == "Magician")) then
		table.insert(inv, {
			name		= "Robes",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Staff",
			isEquipped	= true,
			isIdentified	= true
			})
		table.insert(inv, {
			name		= "Lamp",
			isEquipped	= true,
			isIdentified	= true,
			count		= 1
			})
	end

	return inv

end

----------------------------------------
-- classes:isSpellCaster()
----------------------------------------
function classes:isSpellCaster(inClass)
	if ((inClass == "Conjurer") or (inClass == "Magician") or
	    (inClass == "Sorcerer") or (inClass == "Wizard") or
	    (inClass == "Archage")  or (inClass == "Chronomancer") or
	    (inClass == "Geomancer")) then
		return true
	else
		return false
	end
end
