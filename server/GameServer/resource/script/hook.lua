Hook = {hookList = {}, enabled = true}

function Hook:AddReplacementHook(originalFctName, newFct)
    local hookDef = self.hookList[originalFctName]
    if (hookDef == nil) then
        hookDef = Hook:InitHook(originalFctName)
    end
    hookDef.replacement = newFct
end

function Hook:RemoveReplacementHook(originalFct)
    local hookDef = self.hookList[originalFctName]
    if (hookDef ~= nil) then
        hookDef.replacement = nil
    end
end

function Hook:GenericHook(originalFctName, ...)
    local arg = {...}
    local hookDef = self.hookList[originalFctName]
    if (hookDef ~= nil) then
        if (self.enabled == true and hookDef.enabled == true) then
            local pre_filter_count = #hookDef.pre
            for i = 1, pre_filter_count do
                local returnedValue = hookDef.pre[i].fct(unpack(arg))
                if (returnedValue == false) then
                    return
                end
            end

            local returnedValues = nil
            if (hookDef.replacement ~= nil) then
                returnedValues = {hookDef.replacement(unpack(arg))}
            else
                returnedValues = {hookDef.original(unpack(arg))}
            end

            local post_filter_count = #hookDef.post
            for i = 1, post_filter_count do
                newReturnedValues = {hookDef.post[i].fct(returnedValues, unpack(arg))}
                if (newReturnedValues ~= nil and #newReturnedValues > 0) then
                    returnedValues = newReturnedValues
                end
            end

            local a, b, c = unpack(returnedValues)
            return unpack(returnedValues)
        else
            return hookDef.original(unpack(arg))
        end
    end
end

function Hook:InitHook(originalFctName)
    local hookDef = {pre = {}, post = {}, original = nil, replacement = nil, enabled = true}
    self.hookList[originalFctName] = hookDef
    hookDef.original = _G[originalFctName]
    _G[originalFctName] = function(...)
        local arg = {...}
        return Hook:GenericHook(originalFctName, unpack(arg or {}))
    end
    return hookDef
end

function Hook:AddPreHook(originalFctName, hookFct, priorityOrder)
    local hookDef = self.hookList[originalFctName]
    if (hookDef == nil) then
        hookDef = Hook:InitHook(originalFctName)
    end

    if (priorityOrder == nil) then
        priorityOrder = 999999
    end

    local hookCount = #hookDef.pre
    local found = false
    for i = 1, hookCount do
        if (hookDef.pre[i].fct == hookFct) then
            found = true
            hookDef.pre[i].order = priorityOrder
        end
    end
    if (found == false) then
        table.insert(hookDef.pre, {fct = hookFct, order = priorityOrder})
    end

    Hook:SortHooks(hookDef.pre)
end

function Hook:RemovePreHook(originalFctName, hookFct, priorityOrder)
    print("ERROR >> Hook:RemovePreHook() is not yet implemented !!")
end

function Hook:AddPostHook(originalFctName, hookFct, priorityOrder)
    local hookDef = self.hookList[originalFctName]
    if (hookDef == nil) then
        hookDef = Hook:InitHook(originalFctName)
    end

    if (priorityOrder == nil) then
        priorityOrder = 999999
    end

    local hookCount = #hookDef.post
    local found = false
    for i = 1, hookCount do
        if (hookDef.post[i].fct == hookFct) then
            found = true
            hookDef.post[i].order = priorityOrder
        end
    end
    if (found == false) then
        table.insert(hookDef.post, {fct = hookFct, order = priorityOrder})
    end

    Hook:SortHooks(hookDef.post)
end

function Hook:RemovePostHook(originalFctName, hookFct, priorityOrder)
    print("ERROR >> Hook:RemovePostHook() is not yet implemented !!")
end

function Hook:DisableHooks(originalFctName)
    local hookDef = self.hookList[originalFctName]
    if (hookDef ~= nil) then
        hookDef.enabled = false
    end
end

function Hook:EnableHooks(originalFctName)
    local hookDef = self.hookList[originalFctName]
    if (hookDef ~= nil) then
        hookDef.enabled = true
    end
end

function Hook:ClearHooks(originalFctName)
    local hookDef = self.hookList[originalFctName]
    if (hookDef ~= nil) then
        hookDef.enabled = true
    end
    _G[originalFctName] = hookDef.original
    self.hookList[originalFctName] = nil
end

function Hook:ClearAllHooks()
    print("ERROR >> Hook:ClearAllHooks() is not yet implemented !!")
end

function Hook:DisableAllHooks()
    self.enabled = false
end

function Hook:EnableAllHooks()
    self.enabled = true
end

function Hook:LoadHookFile(filename)
    local split = function(str, delim, maxNb)
        if string.find(str, delim) == nil then
            return {str}
        end
        if maxNb == nil or maxNb < 1 then
            maxNb = 0
        end
        local result = {}
        local pat = "(.-)" .. delim .. "()"
        local nb = 0
        local lastPos
        for part, pos in string.gmatch(str, pat) do
            nb = nb + 1
            result[nb] = part
            lastPos = pos
            if nb == maxNb then
                break
            end
        end

        if nb ~= maxNb then
            result[nb + 1] = string.sub(str, lastPos)
        end
        return result
    end

    file = assert(io.open(filename, "r"))
    for line in file:lines() do
        lineData = split(line, "|", 3)

        if (_G[lineData[2]] ~= nil) then
            if (_G[lineData[3]] ~= nil) then
                if (lineData[1] == "REPLACE") then
                    Hook:AddReplacementHook(lineData[2], _G[lineData[3]])
                elseif (lineData[1] == "PRE") then
                    Hook:AddPreHook(lineData[2], _G[lineData[3]], lineData[4])
                elseif (lineData[1] == "POST") then
                    Hook:AddPostHook(lineData[2], _G[lineData[3]], lineData[4])
                else
                    print("ERROR >> Hook:LoadHookFile() : Unrecongized type of hook [" .. lineData[1] .. "] !!")
                end
            else
                print("ERROR >> Hook:LoadHookFile() : Function [" .. lineData[3] .. "] is not defined !!")
            end
        else
            print("ERROR >> Hook:LoadHookFile() : Function [" .. lineData[2] .. "] is not defined !!")
        end
    end
    io.close(file)
end

function Hook:SetHookPattern(pattern, hookType, hookFct, priority)
    if (hookFct == nil) then
        print("ERROR >> Hook:SetHookPattern() : Hook function is not defined [" .. hookFct .. "] !!")
        return
    end

    for key, value in pairs(_G) do
        if (type(value) == "function") then
            if (string.find(key, pattern) ~= nil) then
                if (hookType == "REPLACE") then
                    Hook:AddReplacementHook(key, hookFct)
                elseif (hookType == "PRE") then
                    Hook:AddPreHook(key, hookFct, priority)
                elseif (hookType == "POST") then
                    Hook:AddPostHook(key, hookFct, priority)
                else
                    print("ERROR >> Hook:SetHookPattern() : Unrecongized type of hook [" .. hookType .. "] !!")
                end
            end
        end
    end
end

function Hook:SortHooks(hookList)
    local compareFct = function(a, b)
        return a.order < b.order
    end
    table.sort(hookList, compareFct)
end
