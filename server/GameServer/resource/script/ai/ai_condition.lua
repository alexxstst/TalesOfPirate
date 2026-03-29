print("-- [Loading] Ai_Condition.lua")

function aic_at_spawnpos(c)
    local x, y = GetChaSpawnPos(c)

    if is_near_pos(c, x, y, 100) == 1 then
        return 1
    end

    return 0
end

function aic_at_spawn_toofar(c)
    local chase_r = GetChaChaseRange(c)
    local x, y = GetChaSpawnPos(c)
    local now_x, now_y = GetChaPos(c)
    local dis = (now_x - x) * (now_x - x) + (now_y - y) * (now_y - y)
    if dis > chase_r * chase_r then
        return 1
    end
    return 0
end

function aic_target_outofsight(c)
    local t = GetChaTarget(c)
    local vision = GetChaVision(c)
    if is_near(c, t, vision) == 0 then
        return 1
    end
    return 0
end

function aic_seek_target(c)
    local t = find_target(c, 0)
    if t ~= nil then
        return 1
    end

    return 0
end

function aic_rand_5_1(c)
    if Rand(5) == 1 then
        return 1
    end
end

function aic_patrol_begin(c)
    local patrol_state = GetChaPatrolState(c)
    if patrol_state == 0 then
        return 1
    end
    return 0
end

function aic_patrol_arrive_end(c)
    local px, py = GetChaPatrolPos(c)
    if is_near_pos(c, px, py, 40) == 1 then
        return 1
    end
    return 0
end

function aic_patrol_return(c)
    local patrol_state = GetChaPatrolState(c)
    if patrol_state == 1 then
        return 1
    end
    return 0
end

function aic_update_target(c)
    if Rand(100) < CHANGE_TARGET_RATIO then
        return 1
    end
    return 0
end
