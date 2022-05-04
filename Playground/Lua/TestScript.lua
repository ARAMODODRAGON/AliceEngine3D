
local ENEMY_STATE_IDLE = 0
local ENEMY_STATE_SEEKING = 1
local ENEMY_STATE_HUNTING = 2

local FACING_RIGHT = 0
local FACING_LEFT = 1

EnemyTable = { }
AddEnemy = function()
	local enemy = {
		x = 0.0,
		y = 0.0,
		state = ENEMY_STATE_IDLE,
		facing = FACING_RIGHT
	}
	table.insert(EnemyTable, enemy)
end

OnCreate = function(self)
	
end

OnUpdate = function(self, delta)
	
end


