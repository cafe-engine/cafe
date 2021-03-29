local Player = {}
Player.__index = Player

function Player:init(x, y)
    self.x = x or 0
    self.y = y or 0
    self.speed = 100
    self.gravity = 80
    self.velocity = {x = 0, y = 0}
    self.image = cafe.graphics.Image("goblin.png")
    self.rect = cafe.Rect(0, 0, 16, 16)
    self.time = 0
    self.frame = 0
    self.scale = {x = 1, y = 1}
    self.flip = 1
end

function Player:update(dt)
    if cafe.keyboard.isDown("left") then
        self.velocity.x = -self.speed
        self.flip = -1
    elseif cafe.keyboard.isDown("right") then
        self.velocity.x = self.speed
        self.flip = 1
    else
        self.velocity.x = self.velocity.x * 0.8
    end

    if cafe.keyboard.wasPressed("x") then
        self.velocity.y = -self.gravity
    end

    local vdt = self.velocity.y * dt
    if (self.y + 16 + vdt) < floor_y then
        self.velocity.y = self.velocity.y + self.gravity * dt
    else
        self.velocity.y = 0
    end

    self.x = self.x + (self.velocity.x * dt)
    self.y = self.y + (self.velocity.y * dt)
    
    self.time = self.time + 8*dt
    if self.time >= 1 then
        self.time = 0
        self.frame = self.frame + 1
        if self.frame > 5 then self.frame = 0 end
    end
end

function Player:draw()
    self.rect:x(self.frame*16)
    -- self.image:draw(self.rect, self.x, self.y, 0, self.scale.x*self.flip, self.scale.y, 8, 8) 
    self.image:draw(self.rect, self.x, self.y) 
end

return setmetatable({}, Player)
