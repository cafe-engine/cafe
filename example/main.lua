local Player = require 'objects.player'
local Lizard = require 'objects.lizard'
function cafe.load()
    player = Player(0, 0)
    liz = Lizard(32, 32)
    canv = cafe.Texture(160, 95, 3)
    a = cafe.Audio("assets/Som.wav")
    a:volume(0.2)
    a:play()
end

function cafe.update(dt)
    player:update(dt)
    liz:update(dt)
end

function cafe.draw()
    cafe.render.target(canv)
    cafe.render.clear(75, 75, 90)
    player:draw()
    liz:draw()
    cafe.render.target()

    canv:draw(0, 0, 0, 4, 4)
end

function cafe.keypressed(wid, key, rpt)
    if key == 'X' and not rpt then
	player:jump()
    end
end
