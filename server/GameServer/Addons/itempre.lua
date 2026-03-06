function SetItemPrefix(item, pre)
    local baoshilv = GetItemAttr(item, 53)
    local oldpre = GetItemPrefix(item)
    if oldpre == pre then
        return
    end
    SetItemAttr(item, 53, ((math.floor(baoshilv / 1000) * 1000) + (pre * 10) + (math.fmod(math.abs(baoshilv), 10))))
end

function GetItemPrefix(item)
    local prefix = math.floor(math.fmod(GetItemAttr(item, 53), 1000) / 10)
    return prefix
end

function GetItemColour(item)
    local baoshilv = GetItemAttr(item, 53)
    local colourid = math.floor(baoshilv / 1000)
    if colourid >= 00 and colourid < 01 then
        return "gray"
    elseif colourid >= 01 and colourid < 03 then
        return "white"
    elseif colourid >= 03 and colourid < 05 then
        return "green"
    elseif colourid >= 05 and colourid < 07 then
        return "purple"
    elseif colourid >= 07 and colourid < 09 then
        return "red"
    elseif colourid >= 09 and colourid < 11 then
        return "gold"
    else
        return nil
    end
end

function SetItemColour(item, colour)
    if colour == GetItemColour(item) then
        return
    end
    local colourid
    if colour == "gray" then
        colourid = 00
    end
    if colour == "white" then
        colourid = 01
    end
    if colour == "green" then
        colourid = 03
    end
    if colour == "purple" then
        colourid = 05
    end
    if colour == "red" then
        colourid = 07
    end
    if colour == "gold" then
        colourid = 09
    end
    if colour ~= nil then
        SetItemAttr(item, 53, ((colour * 1000) + (pre * 10) + (math.fmod(math.abs(baoshilv), 10))))
    end
end

function SetBaoshiLv(item, colour, prefix)
    SetItemColour(item, colour)
    SetItemPrefix(item, prefix)
end

function CalculateBaoshiLv(base, pre)
    return math.floor(base / 1000) * 1000 + pre * 10 + math.fmod(math.abs(base), 10)
end
