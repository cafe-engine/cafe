local traceback = debug.traceback
local path = cafe.filesystem.basepath()
package.path = package.path .. ';' .. path .. '/?.lua;' .. path .. '/?/init.lua'


local function customload(modulename)
    local errmsg = ""
    local modulepath = string.gsub(modulename, "%.", "/")
    local filename = modulepath .. ".lua"
    local file = cafe.filesystem.File(filename)

    if file then
        return assert(load(assert(file:read()), filename)) 
    end
    errmsg = errmsg .. "\n\t[cafe] no file '" .. filename .. "'"
    return errmsg
end
table.insert(package.searchers, 1, customload)


local function _error(msg)
    trace = traceback("", 1)
    print(msg, trace)
    cafe.error(msg, trace)
end

local function _step()
    local dt = cafe.timer.delta()
    if cafe.update then cafe.update(dt) end
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

function cafe.run()
    local dt = cafe.delta()
    if cafe.load then cafe.load() end
    while ~cafe.running() do
        if cafe.update then cafe.update(dt) end
        if cafe.draw then cafe.draw() end
    end
    return 0;
end

xpcall(function() require "main" end, _error)
-- xpcall(cafe.run)
