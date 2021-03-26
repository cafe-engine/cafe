function cafe.load()
    img = cafe.graphics.Image("goblin.png")
    x = 0
    r = cafe.Rect(0, 0, 16, 16)

    canvas = cafe.graphics.Canvas(160, 95)
end

function cafe.update(dt)
    if cafe.keyboard.isDown("a") then
        x = x - 10
    elseif cafe.keyboard.isDown("d") then
        x = x + 10
    end
end

function cafe.draw()
    canvas:set()
    cafe.graphics.clear()
    img:draw(r, x)
    canvas:unset()

    canvas:draw(0, 0, 0, 4, 4)
end
