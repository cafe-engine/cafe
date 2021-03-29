local Player = require("objects.player")

function cafe.load()
    canvas = cafe.graphics.Canvas(160, 95)
    player = Player()
    floor_y = 95-16
end

function cafe.update(dt)
    player:update(dt)
end

function cafe.draw()
    canvas:set()
    cafe.graphics.clear()
    cafe.graphics.setMode(1)
    cafe.graphics.rectangle(0, floor_y, 160, 16)
    player:draw()
    canvas:unset()

    canvas:draw(0, 0, 0, 4, 4)
end
