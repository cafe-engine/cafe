print("okokok")

function cafe.load()
    x = 0
    img = cafe.graphics.Image("goblin.png")

    if not cafe.filesystem.rmdir("build/") then
        print("deu ruim")
    end
    --[[fp = cafe.filesystem.File("build/teste.txt", "w")
    fp:write("testando, serase vai?")

    fp:close()]]


    canvas = cafe.graphics.Canvas(160, 95)
end

function cafe.update(dt)
    if cafe.keyboard.isDown("a") then
        x = x - 5
    elseif cafe.keyboard.isDown("d") then
        x = x + 5
    end
end

function cafe.draw()
    cafe.graphics.setColor()
    cafe.graphics.circle(x, 16, 16)

    canvas:set()
    cafe.graphics.clear()
    img:draw(x)
    canvas:unset()

    canvas:draw()
end
