local Enemy = require 'objects.enemy'
local Lizard = Enemy:extend('Lizard')
local Animator = require 'lib.animator'

function Lizard:new(x, y)
    Enemy.new(self, x, y)
    self.tex = cafe.Texture("assets/lizard.png")
    self.animator = Animator(self.tex, 16)
    self.animator:add('idle', 1, 4)
    self.animator:play('idle')
end

function Lizard:update(dt)
    self.animator:update(dt)
end

function Lizard:draw()
    self.tex:draw(self.animator.rect)
end

return Lizard
