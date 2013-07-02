local mt = nil

--------------------------------
-- x <= r <= (x*y)
--------------------------------
function rnd_xdy(x, y)
	local value = 0

	for i = 1,x do
		value = value + ((mt() % y) + 1)
	end

	return value
end


--------------------------------
-- x <= r <= y
--------------------------------
function rnd_between_xy_inc(x, y)
	if (x == y) then
		return x
	end

	-- Make sure x is the lower number
	if (x > y) then
		local t = x
		x = y
		y = t
	end

	return mt:value(x, y)
end

--------------------------------
-- x < r < y
--------------------------------
function rnd_between_xy(x, y)
	return rnd_between_xy_inc(x+1, y-1)
end

--------------------------------
-- This function generates the
-- random number XdY minus a
-- number of the smallest rolls
--------------------------------
function rnd_xdy_z(x, y, z)
	local rolls = {}
	local value = 0
	local i, j
	local minimum

	if (z >= x) then
		return 0
	end

	for i = 1,x do
		rolls[i] = mt(y)
	end

	table.sort(rolls)

	for i = z+1,x do
		value = value + rolls[i]
	end

	return value
end

function rnd_and_x(x)
	return bit32.band(mt(), x)
end

----------------------------------------
-- Return a random number
----------------------------------------
function rnd()
	return mt()
end

----------------------------------------
-- rnd_hash()
--
-- Return a random hexadecimal string
-- for use as a hash key
----------------------------------------
function rnd_hash()
	return string.format("0x%08X", mt())
end

mt = mt_random.new()
