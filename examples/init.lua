local traceback = debug.traceback

local function _error(msg)
    trace = traceback("", 1)
    print(msg, trace)
    cafe.error(msg, trace)
end

local function _step()
    if cafe.update then cafe.update() end
    if cafe.draw then cafe.draw() end
end

function cafe._load()
    if cafe.load then xpcall(cafe.load, _error) end
end

function cafe._step()
    xpcall(_step, _error)
end

function cafe.error()
    print("viji")
end

xpcall(function() require "main" end, _error)
