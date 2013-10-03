----------------------------------------
-- executeString()
--
-- Function to run API code in a 
-- sandbox
----------------------------------------
function executeString(inString)
	local chunk
	local rval, mesg

	chunk = compileString(inString)
	if (chunk == nil) then
		error("Compilation error: \n" .. inString .. "\n", 2)
	end

	rval, mesg = pcall(chunk)
	if (not rval) then
		error("Code execution error: \n" .. inString.."\n"..mesg, 2)
	end

	return mesg
end

----------------------------------------
-- compileString()
----------------------------------------
function compileString(inString)
	local rval, mesg

	if (type(inString) == "string") then
		rval, mesg = load(inString)
		if (rval == nil) then
			error("Compilation error: \n" ..
				inString .. "\n" .. mesg, 2)
		end
		return rval
	end

	return nil
end

----------------------------------------
-- compileAction()
----------------------------------------
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

