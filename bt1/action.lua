require "bttable"

btAction = {}
function btAction.new()
	local self = {
		next		= false,
		prev		= false,
		source		= false,
		target		= false,
		func		= false,
		inBattle	= false,
		inData		= {},
		outData		= {}
	}

	return self
end

