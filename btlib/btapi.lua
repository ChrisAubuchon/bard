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
		inAction.func, mesg = load(
			"local self = select(1, ...)" ..
			inAction.inFunction
			)
if false then
		inAction.func, mesg = load(
			"local function f(...) " 
			.. "local inAction = select(1, ...) " 
			.. inAction.inFunction 
			.. " end f(...)"
			)
end
		if (inAction.func == nil) then
			error("Compilation error: \n" ..
				inAction.inFunction .. "\n" .. mesg, 2)
		end
	end
end

