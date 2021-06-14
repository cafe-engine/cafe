function cafe.load()
        x, y = 0, 32
        img = cafe.Texture("goblin.png")
        canv = cafe.Texture(160, 95, 3)
        r = cafe.Rect(0, 0, 160*4, 95*4)
        s = cafe.Rect(0, 0, 16, 16)
        j = cafe.Joystick(0)
        j_name = j:name()
        j:vibration(0xffff, 0xffff, 2000)
end

function cafe.update(dt)
    local axes = {j:axes()}
    local spd = axes[1]
    if spd < 0 then spd = -16
    elseif spd > 0 then spd = 16 end

    local hat = j:hat(0)
    if hat == 'l' then spd = -16
    elseif hat == 'r' then spd = 16 end

    x = x + spd
end

function cafe.draw()
    cafe.render.target(canv)
    cafe.render.clear()
    cafe.render.color(255, 255, 255)
    cafe.render.mode(1)
    cafe.render.rectangle(x, y, 48, 32)
    cafe.render.circle(64, 64, 16)
    cafe.render.color(255, 0, 0)
    cafe.render.text("deu certo? kkkkkkkk")
    img:draw(s, s)
    cafe.render.target()
    canv:draw(r)

    cafe.render.color(0)
    cafe.render.text('controller: "' .. j_name .. '"', 0, 128)
end
