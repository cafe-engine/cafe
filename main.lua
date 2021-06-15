function cafe.load()
    x = 0
    y = 0
    img = cafe.Texture("goblin.png")
    canvas = cafe.Texture(160, 95, 3)
    d = cafe.Rect(0, 0, 160*4, 95*4)
end

function cafe.update(dt)
    if cafe.keyboard.down("left") then x = x - 16
    elseif cafe.keyboard.down("right") then x = x + 16 end
end

function cafe.draw()
    cafe.render.target(canvas)
    cafe.render.clear()
    cafe.render.color(255, 255, 255)
    img:draw()
    cafe.render.target()

    canvas:draw(d)
end
