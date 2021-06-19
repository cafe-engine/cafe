local class = require 'lib.class'
local Object = class:extend('Object')

function Object:new(x, y)
    self.x = x or 0
    self.y = y or 0
end

function Object:update(dt)
end

function Object:draw()
end

return Object
