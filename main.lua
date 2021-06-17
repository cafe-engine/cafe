function cafe.load()
    x = 0
    y = 0
    img = cafe.Texture("goblin.png")
    canvas = cafe.Texture(160, 95, 3)
    d = cafe.Rect(0, 0, 160*4, 95*4)
    r = cafe.Rect(0, 0, 16, 16)
    s = cafe.Rect(0, 0, 16, 16)
    time = 0
    anim = 'walk'
    frame = 1
    frames = {
        walk = {
            {0, 0, 16, 16},
            {16, 0, 16, 16},
            {32, 0, 16, 16},
            {48, 0, 16, 16},
            {64, 0, 16, 16},
            {80, 0, 16, 16}
        }
    }
end

function cafe.update(dt)
    if cafe.keyboard.down("left") then x = x - 8
    elseif cafe.keyboard.down("right") then x = x + 8 end
    r:x(x)
    time = time + dt*8
    if time >= 1 then
        frame = frame + 1
        time = 0
        if frame + 1 > #frames[anim] then frame = 1 end
    end
end

function cafe.draw()
    cafe.render.target(canvas)
    cafe.render.clear()
    cafe.render.color(255, 255, 255)
    local ss = frames[anim][frame]
    s(ss[1], ss[2], ss[3], ss[4])
    img:draw(r, s)
    cafe.render.target()

    canvas:draw(d)
end
