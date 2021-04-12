local Player = require("objects.player")
local Rabbit = require("objects.rabbit")

function cafe.load()
    canvas = cafe.graphics.Canvas(160, 95)
    player = Player()
    rabbit = Rabbit(96, 0)
    floor_y = 95-16
    audio = cafe.audio.Music("assets/music.wav")

    time = 0
end

function cafe.update(dt)
    player:update(dt)
    rabbit:update(dt)
end

function cafe.draw()
    canvas:set()
    cafe.graphics.clear()
    cafe.graphics.print("press X to jump")
    cafe.graphics.setMode(1)
    cafe.graphics.rectangle(0, floor_y, 160, 16)
    player:draw()
    rabbit:draw()
    canvas:unset()

    canvas:draw(0, 0, 0, 4, 4)
end
