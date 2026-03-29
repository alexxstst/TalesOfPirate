print("-- [Loading] Ai_Event.lua")

function ai_event_find_target(c, t)
    SetChaTarget(c, t)
end

function ai_event_rand_move(c)
    rand_move(c, 400)
end

function ai_event_patrol_begin(c)
    local px, py = GetChaPatrolPos(c)
    ChaMove(c, px, py)
    SetChaPatrolState(c, 2)
end

function ai_event_patrol_end_idle(c)
    SetChaPatrolState(c, 1)
end

function ai_event_patrol_return(c)
    local ox, oy = GetChaSpawnPos(c)
    ChaMove(c, ox, oy)
    SetChaPatrolState(c, 3)
end

function ai_event_patrol_start_idle(c)
    SetChaPatrolState(c, 0)
end

function ai_event_go_home(c)
    clear_target(c)
    local x, y = GetChaSpawnPos(c)
    ChaMoveToSleep(c, x, y)
    set_moving_back(c, 1)
end

function ai_event_clear_target(c)
    clear_target(c)
end

function ai_event_update_target(c)
    local t = GetChaTarget(c)
    local tNew = GetChaFirstTarget(c)
    if tNew ~= nil and tNew ~= t then
        clear_target(c)
        SetChaTarget(c, tNew)
        return 1
    end
    return 0
end
