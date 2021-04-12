local Class = require "libs.class"
local Rabbit = Class:extend("Rabbit")

function Rabbit:constructor(x, y)
  self.x = x or 0
  self.y = y or 0
  self.speed = 80
  self.gravity = 80
  self.jump_force = 80
  self.velocity = {x = 0, y = 0}
  self.image = cafe.graphics.Image("assets/rabbit.png")
  self.size = 32
  self.coll = {
    left = 10,
    right = 12,
    top = 12,
    bottom = 7
  }
  self.rect = cafe.Rect(0, 0, self.size, self.size)
  self.time = 0
  self.frame = 1
  self.scale = {x = 1, y = 1}
  self.flip = 1
  self.anim = "idle"
  self.animations = {
    ["idle"] = {
      {0, 0}, {self.size, 0}, {self.size*2, 0}, {self.size*3, 0}
    }
  }
  self.rect = cafe.Rect(0, 0, self.size, self.size)
end

function Rabbit:update(dt)
  local next_step = (self.velocity.y+self.gravity*dt) * dt
  if (self.y+self.coll.top+self.coll.bottom+next_step) < floor_y then
    self.velocity.y = self.velocity.y + self.gravity*dt
  else
    self.velocity.y = self.velocity.y * 0.2
  end

  -- if self.velocity.y > 0 then self.velocity.y = self.velocity.y + 20*dt end

  self.x = self.x + (self.velocity.x * dt)
  self.y = self.y + (self.velocity.y * dt)

  self.time = self.time + 8*dt
  if self.time >= 1 then
    self.time = 0
    self.frame = self.frame + 1
  end
  if self.frame > #self.animations[self.anim] then
    self.frame = 1
  end
end

function Rabbit:draw()
  local r = self.animations[self.anim][self.frame]
  self.rect:set(r[1], r[2])
  self.image:draw(self.rect, self.x, self.y, 0, self.scale.x*self.flip, self.scale.y, 16, 16)
  cafe.graphics.setMode(0)
  cafe.graphics.rectangle(self.x+self.coll.left, self.y+self.coll.top, self.coll.right, self.coll.bottom)
  cafe.graphics.setMode(1)
end

return Rabbit
