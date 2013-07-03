btapi	= {}
btapi.city	= {}
btapi.dun	= {}
btapi.spellTarget = false

----------------------------------------
-- executeString()
--
-- Function to run API code in a 
-- sandbox
----------------------------------------
function executeString(inString)
	local rval
	local mesg
	local chunk

	-- Load the chunk using the API as the global environment
	chunk, mesg = load(inString, "=(load)", "bt", btapi)
	if (chunk == nil) then
		error("Compilation error: \n" .. inString.."\n", 2)
	end

	rval, mesg = pcall(chunk)
	if (not rval) then
		error("Code execution error: \n" .. inString .."\n"..mesg, 2)
	end

	return mesg
end

function executeFunction(inFunction)
	local rval
	local mesg

	rval, mesg = pcall(inFunction)
	if (mesg ~= nil) then
		error("Code execution error: \n"..mesg, 2)
	end

	return rval
end

function compileAction(inAction)
	local mesg

	if (inAction.inFunction) then
		inAction.func, mesg = load(inAction.inFunction)
		if (inAction.func == nil) then
			error("Compilation error: \n" ..
				inAction.inFunction .. "\n" .. mesg, 2)
		end
	elseif (inAction.inCode) then
		inAction.func, mesg = load(
			"local function f(inAction) " .. inAction.inCode ..
			" end f(...)")
		if (inAction.func == nil) then
			error("Compilation error: \n" ..
				inAction.inCode .. "\n" .. mesg, 2)
		end
	end
end

----------------------------------------
-- API functions
----------------------------------------

btapi.clearText = text_clear

function btapi.doStairs(direction)
	text_print("\nThere are stairs here, going " .. direction ..". Do you wish to take them?")
	if (getYesNo()) then
		return true
	else
		text_clear()
	end
end

function btapi.inDungeon()
	return (globals.gameState == globals.STATE_DUNGEON)
end
