local class = {}

function class:new()
end

function class:__new(...)
    local o = setmetatable({}, { __index = self })
    o:new(...)
    return o
end

function class:extend(name)
    local o = setmetatable({}, { __call = class.__new, __index = self})
    return o
end

return class
