require "dataio"
require "bttable"
require "entity"
require "random"
require "objectHash"
require "battleMonster"

monsterParty = {}

function monsterParty:new()
	local self = {
	}
	btTable.addParent(self, monsterParty, btArray, battleBonus)
	btTable.setClassMetatable(self)

	self.__index = self

	return self
end

function monsterParty:addMonsterGroup(inGroup)
	self:__add(inGroup)
	self:adjustMeleeGroups()
end

function monsterParty:removeMonsterGroup(inGroup)
	local mgroup
	local i
	local groupIndex

	for i,mgroup in self:ipairs() do
		if (mgroup == inGroup) then
			mgroup:deleteKey()
			groupIndex = i
			break
		end
	end

	dprint("Group index to delete: %d", groupIndex)
	self:__remove(groupIndex)
end

function monsterParty:adjustMeleeGroups()
	local i

	for i = 1,2 do
		if (self[i]) then self[i].inMeleeRange = true end
	end

	for i = 3,self.size do
		self[i].inMeleeRange = false
	end
end

function monsterParty:isGroup(inIndex)
	return self[inIndex]
end

function monsterParty:getLeadGroup()
	return self[1]
end

function monsterParty:disbelieve(inBattle)
	if (party.summon and party.summon.isIllusion and self.doDisbelieve) then
		local action = btAction:new()
		action.source = party[1]
		action.target = self[1]
		if (not action:savingThrow()) then
			text_cdprint(false, true, "Your foes see through your illusion!\n\n")
		end
	end
end

function monsterParty:advance()
	local i

	for i = #self,2,-1 do
		if (self[i].advanceSpeed > self[i-1].advanceSpeed) then
			local tmp

			text:print("The %s", monster.pluralizeName(self[i].name, self[i].size))
			if (self[i].size == 1) then
				text:cdprint(false, true, " advances!\n\n")
			else
				text:cdprint(false, true, " advance!\n\n")
			end
			tmp = self[i - 1]
			self[i - 1] = self[i]
			self[i] = tmp
		end
	end
end

