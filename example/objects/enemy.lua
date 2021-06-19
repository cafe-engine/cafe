local Object = require 'objects.object'
local Enemy = Object:extend('Enemy')

function Enemy:new(x, y)
    Object.new(self, x, y) 
    self.life = 5
end

return Enemy
