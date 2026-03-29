-- Debug utilities — loaded first, available to all scripts

-- Printf-style formatted logging to game log
-- Usage: to_console("Player %s has %d HP", name, hp)
function to_console(fmt, ...)
    ToDebugLog(string.format(fmt, ...))
end
