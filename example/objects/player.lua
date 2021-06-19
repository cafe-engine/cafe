local Object = require 'objects.object'
local Player = Object:extend('Player')

function Player:new(x, y)
    Object.new(self, x, y)
    self.image = cafe.Texture("assets/player.png")
    self.dest = cafe.Rect(0, 0, 16, 16)
    self.src = cafe.Rect(0, 0, 16, 16)
    self.gravity = 80
    self.speed = 100
    self.time = 0
    self.velocity = { x = 0, y = 0 }
    self.frame = 1
    self.animation = 'idle'
    self.animations = {
        idle = {
            {0, 0, 16, 16},
            {16, 0, 16, 16},
            {32, 0, 16, 16},
            {48, 0, 16, 16}
        },
        walk = {
            {0, 16, 16, 16},
            {16, 16, 16, 16},
            {32, 16, 16, 16},
            {48, 16, 16, 16},
            {0, 32, 16, 16},
            {16, 32, 16, 16}
        }
    }
end

function Player:update(dt)
    self.time = self.time + (8*dt)
    if cafe.keyboard.down('left') then
        self.velocity.x = self.velocity.x - self.speed * dt
        self.animation = 'walk'
    elseif cafe.keyboard.down('right') then
        self.velocity.x = self.velocity.x + self.speed * dt
        self.animation = 'walk'
    else
        self.animation = 'idle'
        self.velocity.x = self.velocity.x * 0.8
    end

    if cafe.keyboard.pressed('x') then
        self.velocity.y = -self.gravity
    end

    self.velocity.y = self.velocity.y + (self.gravity*dt)
    local gvt = self.velocity.y * dt


    if self.y + gvt + 16 >= 95 then
        self.velocity.y = 0
        self.y = 95 - 16
    end

    if self.time >= 1 then
        self.frame = self.frame + 1
        self.time = 0
    end
    if self.frame > #self.animations[self.animation] then
        self.frame = 1
    end

    self.x = self.x + (self.velocity.x * dt)
    self.y = self.y + (self.velocity.y * dt)
    self.dest:x(self.x)
    self.dest:y(self.y)
end

function Player:draw()
    self.image()
    local frame = self.animations[self.animation][self.frame]
    self.src(frame[1], frame[2], frame[3], frame[4])
end

return Player
