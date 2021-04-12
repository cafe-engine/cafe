local Class = require "libs.class"
local Animation = Class:extend("Animation")

function Animation:constructor(name, frameSize, frames)
	self.image = nil
	self.rect = nil
	self.name = name or ""
	self.frameSize = frameSize or {16, 16}
	self.frames = frames or {1, 1}
	self.time = 0
	self.speed = 8
end

function Animation:update(dt)
	self.time = self.time + (self.speed * dt)

	if self.time >= 1 then
		self.time = 0
		self.frame = self.frame + 1
	end
end

return Animation