local class = require 'lib.class'
local Animator = class:extend('Animator')

function Animator:new(tex, frame_sz)
    if not tex then error("tex cannot be nil") end
    frame_sz = frame_sz or 16

    self.tex = tex
    local width, height = self.tex:size()
    self.cols = width / frame_sz
    self.rows = height / frame_sz
    self.time = 0
    self.speed = 8
    self.frame = 1
    self.frames = {}
    for yy=1,self.rows do
	for xx=1,self.cols do
	    table.insert(self.frames, { xx-1, yy-1 })
	end
    end
    self.animation = ''
    self.animations = {}
    self.rect = cafe.Rect(0, 0, frame_sz, frame_sz)
    self.frame_sz = frame_sz
end

function Animator:update(dt)
    local anim = self.animations[self.animation]
    if anim then
	self.time = self.time + (self.speed*dt)
	if self.time >= 1 then
	    self.time = 0
	    self.frame = self.frame + 1
	end
	if self.frame > #anim then self.frame = 1 end
	local r = self.frames[anim[self.frame]]
	self.rect(r[1]*self.frame_sz, r[2]*self.frame_sz, self.frame_sz, self.frame_sz)
    end
end

function Animator:add(name, from, to)
    self.animations[name] = {}
    for i=from,to do
	table.insert(self.animations[name], i)
    end
end

function Animator:play(name)
    if self.animation ~= name then
	self.frame = 1 
    end
    self.animation = name
end

function Animator:stop()
    self.animation = ''
end

return Animator
