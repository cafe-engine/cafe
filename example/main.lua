local Player = require 'objects.player'
local Lizard = require 'objects.lizard'
function cafe.load()
    player = Player(0, 0)
    canv = cafe.Texture(160, 95, 3)
    dest = cafe.Rect(0, 0, 640, 380)
    liz = Lizard(0, 32)
end

function cafe.update(dt)
    player:update(dt)
end

function cafe.draw()
    cafe.render.target(canv)
    cafe.render.clear(75, 75, 90)
    player:draw()
    liz:draw()
    cafe.render.target()

    canv:draw(dest)
end
