local Player = require("player")

function cafe.load()
    canvas = cafe.graphics.Canvas(160, 95)
    Player:init()
    floor_y = 95-16
end

function cafe.update(dt)
    Player:update(dt)
end

function cafe.draw()
    canvas:set()
    cafe.graphics.clear()
    cafe.graphics.setColor(255, 0, 0)
    cafe.graphics.setMode(1)
    cafe.graphics.rectangle(0, floor_y, 160, 16)
    cafe.graphics.setMode(0)
    Player:draw()
    canvas:unset()

    canvas:draw(0, 0, 0, 4, 4)
end
