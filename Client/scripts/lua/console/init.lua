-- Каркас Lua-команд игровой консоли.
--
-- Архитектура:
--   console.commands  — реестр: имя → { fn, help }
--   console.state     — стейтфул хранилище, переживает hot-reload
--   console.register  — регистрация команды
--   console.dispatch  — вызывается из C++ ConsoleBridge::Dispatch
--
-- Контракт console.dispatch(line):
--   nil     → Lua не знает команду; C++ сделает fallback в legacy ConsoleCallback
--   string  → команда обработана; output (может быть пустой строкой) попадёт
--             в консоль через MPConsole::_AddText
--
-- Примечание по регистрации API:
--   C-функции зарегистрированы как глобалы `console_print`/`console_print_err`/
--   `console_clear` (без LuaBridge-namespace, чтобы не конфликтовать с этой
--   Lua-таблицей `console`). Мы оборачиваем их ниже в console.print/print_err/clear.

console = console or {}
console.commands = {}
console.state = console.state or {}

-- Обёртки над C-функциями -------------------------------------------------

function console.print(text)
    if console_print then
        console_print(tostring(text or ""))
    end
end

function console.print_err(text)
    if console_print_err then
        console_print_err(tostring(text or ""))
    end
end

function console.clear()
    if console_clear then
        console_clear()
    end
end

function console.set_size(width_pct, lines)
    if console_set_size then
        console_set_size(width_pct, lines)
    end
end

function console.set_backdrop(argb)
    if console_set_backdrop then
        console_set_backdrop(argb)
    end
end

function console.set_font_encoding(codepage)
    if console_set_font_encoding then
        console_set_font_encoding(codepage)
    end
end

-- API регистрации -----------------------------------------------------------

function console.register(name, fn, help)
    if type(name) ~= "string" or name == "" then
        return false
    end
    if type(fn) ~= "function" then
        return false
    end
    console.commands[name] = { fn = fn, help = help or "" }
    return true
end

function console.unregister(name)
    console.commands[name] = nil
end

-- Диспатчер -----------------------------------------------------------------

function console.dispatch(line)
    if type(line) ~= "string" then
        return nil
    end
    line = console.trim(line)
    if line == "" then
        return ""
    end

    local tokens = console.split(line)
    local name = tokens[1]
    table.remove(tokens, 1)

    local entry = console.commands[name]
    if not entry then
        return nil  -- unhandled → C++ откатится на legacy ConsoleCallback
    end

    local ok, result = pcall(entry.fn, tokens, line)
    if not ok then
        return "[error] " .. tostring(result)
    end
    if type(result) == "string" then
        return result
    end
    return ""
end

-- Встроенные команды --------------------------------------------------------

console.register("?", function()
    local names = {}
    for k in pairs(console.commands) do
        table.insert(names, k)
    end
    table.sort(names)
    console.print("Lua commands (" .. #names .. "):")
    for _, n in ipairs(names) do
        local help = console.commands[n].help
        if help and help ~= "" then
            console.print("  " .. n .. "  -  " .. help)
        else
            console.print("  " .. n)
        end
    end
    return ""
end, "список Lua-команд консоли")

console.register("help", function(args)
    if #args == 0 then
        return console.commands["?"].fn(args)
    end
    local name = args[1]
    local entry = console.commands[name]
    if not entry then
        return "нет такой команды: " .. name
    end
    return name .. "  -  " .. (entry.help ~= "" and entry.help or "(без описания)")
end, "help <команда> — справка по команде")

console.register("clear", function()
    console.clear()
    return ""
end, "очистить консоль")

console.register("echo", function(_, line)
    local rest = line:sub(6)  -- "echo " → 5 символов
    return rest
end, "echo <text> — вывести текст")

if ToDebugLog then
    ToDebugLog("[console] init.lua loaded, " .. console.table_count(console.commands) .. " built-in commands")
end
