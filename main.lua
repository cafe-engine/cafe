local x = 0
local y = 0
local delta = 1
local tex = nil

function cafe.load()
    x = 64
    y = 32
    tex = cafe.render.newTexture('penguin.jpg')
    target = cafe.render.newTexture(160, 95)
end

function cafe.update(dt)
    if cafe.keyboard.isDown('left') then
        x = x - 2
    elseif cafe.keyboard.isDown('right') then
        x = x + 2
    end

    if cafe.mouse.isDown(1) then
        y = y + 2
    elseif cafe.mouse.isDown(3) then
        y = y - 2
    end
end

function cafe.draw()
    cafe.render.clear(0, 0, 0)
    cafe.render.setMode('line')
    cafe.render.rectangle(x, y, 64, 32)
    cafe.render.circle(32, 32, 16)
    
    cafe.render.setMode('fill')
    cafe.render.circle(64, 64, 32)
    
    cafe.render.setTarget(target)
    -- cafe.render.clear(0, 0, 0)
    -- cafe.render.rectangle(16, 16, 32, 16)
    cafe.render.texture(tex, -192, -192)
    cafe.render.setTarget()

    cafe.render.texture(target)
end