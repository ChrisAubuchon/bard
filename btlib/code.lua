local codeEnvironment	= nil

code = {}

----------------------------------------
-- code:execute()
----------------------------------------
function code:execute(inCode, inArgs)
	local func
	local mesg
	local rval

	if (not codeEnvironment) then
		codeEnvironment = table.shallowCopy(_G)
	end

	codeEnvironment.args = inArgs

	func,mesg = load(inCode, "=(load)", "bt", codeEnvironment)
	if (not func) then
		error("Compilation error: \n", inCode .. "\n" .. mesg, 2)
	end

	rval,mesg = pcall(func)
	if (not rval) then
		error("Execution error: " .. mesg, 2)
	end

	codeEnvironment.args = nil

	return mesg
end
