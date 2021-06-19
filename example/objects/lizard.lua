local Enemy = require 'objects.enemy'
local Lizard = Enemy:extend('Lizard')

function Lizard:new(x, y)
    Enemy.new(self, x, y)
    self.tex = cafe.Texture("assets/lizard.png")
end

function Lizard:draw()
    self.tex:draw()
end

return Lizard
