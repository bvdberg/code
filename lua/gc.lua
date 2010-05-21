t = {}
-- set as weak table
setmetatable(t, { __mode = 'v' })

do
    local someval = {}
    t['foo'] = someval
end

collectgarbage()

-- should be empty after gc
for k, v in pairs(t) do
    print(k, v)
end

