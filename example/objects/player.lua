local Object = require 'objects.object'
local Player = Object:extend('Player')
local Animator = require 'lib.animator'

function Player:new(x, y)
    Object.new(self, x, y)
    self.image = cafe.Texture("assets/player.png")
    self.animator = Animator(self.image, 16)
    self.animator:add('idle', 1, 4)
    self.animator:add('walk', 5, 10)
    self.src = cafe.Rect(0, 0, 16, 16)
    self.gravity = 80
    self.speed = 100
    self.velocity = { x = 0, y = 0 }
end

function Player:update(dt)
    self.animator:update(dt)
    if cafe.keyboard.down('left') then
        self.velocity.x = self.velocity.x - self.speed * dt
	self.animator:play('walk')
    elseif cafe.keyboard.down('right') then
        self.velocity.x = self.velocity.x + self.speed * dt
	self.animator:play('walk')
    else
        self.velocity.x = self.velocity.x * 0.8
	self.animator:play('idle')
    end

    --[[
    if cafe.keyboard.pressed('x') then
        self.velocity.y = -self.gravity
    end]]

    self.velocity.y = self.velocity.y + (self.gravity*dt)
    local gvt = self.velocity.y * dt

    if self.y + gvt + 16 >= 95 then
        self.velocity.y = 0
        self.y = 95 - 16
    end

    self.x = self.x + (self.velocity.x * dt)
    self.y = self.y + (self.velocity.y * dt)
end

function Player:draw()
    self.image:draw(self.animator.rect, self.x, self.y)
end

function Player:jump()
    self.velocity.y = -self.gravity
end

return Player
