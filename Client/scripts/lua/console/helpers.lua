-- Вспомогательные функции для Lua-команд консоли.
-- Загружается до init.lua — init.lua может пользоваться этими утилитами.

console = console or {}

-- Разбить строку по пробелам в массив токенов.
function console.split(line)
    local parts = {}
    if type(line) ~= "string" then
        return parts
    end
    for token in string.gmatch(line, "%S+") do
        table.insert(parts, token)
    end
    return parts
end

-- Убрать пробелы по краям.
function console.trim(s)
    if type(s) ~= "string" then
        return ""
    end
    return (s:gsub("^%s+", ""):gsub("%s+$", ""))
end

-- Распознать булево: on/off/1/0/true/false/yes/no. Возвращает nil если не распознано.
function console.parse_bool(s)
    if s == nil then
        return nil
    end
    s = tostring(s):lower()
    if s == "1" or s == "on" or s == "true" or s == "yes" then
        return true
    end
    if s == "0" or s == "off" or s == "false" or s == "no" then
        return false
    end
    return nil
end

-- tonumber-обёртка для int (округление вниз).
function console.parse_int(s)
    local n = tonumber(s)
    if n == nil then
        return nil
    end
    return math.floor(n)
end

-- tonumber-обёртка для float.
function console.parse_float(s)
    return tonumber(s)
end

-- Подсчёт элементов в таблице-словаре (для служебных целей).
function console.table_count(t)
    local n = 0
    for _ in pairs(t) do
        n = n + 1
    end
    return n
end
