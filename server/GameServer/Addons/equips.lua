EQUIPMENT_LV = 30

function randomBiased(min, max, bias)
    local v = math.pow(math.random(), bias)
    return math.floor(v * max) + min
end

function WeightedRandomIndex(table)
    local total = 0
    for i, v in ipairs(table) do
        total = total + v.Rate
    end
    local result = math.random(1, total)

    total = 0
    for i, v in ipairs(table) do
        total = total + v.Rate
        if total >= result then
            return i
        end
    end
end

ITEMSTAT = {
    [enumItemTypeClothing] = {
        [ITEMATTR_VAL_STR] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_AGI] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_CON] = {MaxValue = 15, Rate = 150},
        [ITEMATTR_VAL_STA] = {MaxValue = 15, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 60, Rate = 50},
        [ITEMATTR_VAL_HIT] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 10, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 10, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 15, Rate = 1},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 15, Rate = 1},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 6, Rate = 1}
    },
    [enumItemTypeGlove] = {
        [ITEMATTR_VAL_STR] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 15, Rate = 170},
        [ITEMATTR_VAL_AGI] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_CON] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_DEF] = {MaxValue = 20, Rate = 50},
        [ITEMATTR_VAL_HIT] = {MaxValue = 50, Rate = 80},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 20, Rate = 15},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 20, Rate = 15},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 10, Rate = 10},
        [ITEMATTR_VAL_SREC] = {MaxValue = 10, Rate = 10},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 40, Rate = 5},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 15, Rate = 1},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeBoot] = {
        [ITEMATTR_VAL_STR] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_AGI] = {MaxValue = 15, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_DEF] = {MaxValue = 20, Rate = 50},
        [ITEMATTR_VAL_HIT] = {MaxValue = 20, Rate = 40},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 50, Rate = 80},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 20, Rate = 15},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 10, Rate = 10},
        [ITEMATTR_VAL_SREC] = {MaxValue = 10, Rate = 10},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 10, Rate = 1},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 40, Rate = 5},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeHair] = {
        [ITEMATTR_VAL_STR] = {MaxValue = 5, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 5, Rate = 70},
        [ITEMATTR_VAL_AGI] = {MaxValue = 5, Rate = 70},
        [ITEMATTR_VAL_CON] = {MaxValue = 8, Rate = 150},
        [ITEMATTR_VAL_STA] = {MaxValue = 15, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 40, Rate = 50},
        [ITEMATTR_VAL_HIT] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 25, Rate = 15},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 10, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 10, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 15, Rate = 1},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 15, Rate = 1},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeSword] = {
        -- sword
        [ITEMATTR_VAL_STR] = {MaxValue = 10, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 7, Rate = 70},
        [ITEMATTR_VAL_AGI] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 5, Rate = 100},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 20, Rate = 10},
        [ITEMATTR_VAL_HIT] = {MaxValue = 40, Rate = 40},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 40, Rate = 40},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 100, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 100, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 10, Rate = 1},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 10, Rate = 1},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeGlave] = {
        -- gr8sword
        [ITEMATTR_VAL_STR] = {MaxValue = 15, Rate = 70},
        [ITEMATTR_VAL_DEX] = {MaxValue = 7, Rate = 120},
        [ITEMATTR_VAL_AGI] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 9, Rate = 90},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 50, Rate = 40},
        [ITEMATTR_VAL_HIT] = {MaxValue = 20, Rate = 30},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 20, Rate = 30},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 300, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 300, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 20, Rate = 1},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 10, Rate = 1},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeBow] = {
        -- bow
        [ITEMATTR_VAL_STR] = {MaxValue = 5, Rate = 130},
        [ITEMATTR_VAL_DEX] = {MaxValue = 15, Rate = 110},
        [ITEMATTR_VAL_AGI] = {MaxValue = 10, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 9, Rate = 90},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 20, Rate = 30},
        [ITEMATTR_VAL_HIT] = {MaxValue = 30, Rate = 60},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 20, Rate = 40},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeHarquebus] = {
        -- gun
        [ITEMATTR_VAL_STR] = {MaxValue = 5, Rate = 130},
        [ITEMATTR_VAL_DEX] = {MaxValue = 15, Rate = 110},
        [ITEMATTR_VAL_AGI] = {MaxValue = 10, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 9, Rate = 90},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 20, Rate = 30},
        [ITEMATTR_VAL_HIT] = {MaxValue = 30, Rate = 60},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 20, Rate = 40},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeStylet] = {
        -- dagger
        [ITEMATTR_VAL_STR] = {MaxValue = 10, Rate = 150},
        [ITEMATTR_VAL_DEX] = {MaxValue = 5, Rate = 110},
        [ITEMATTR_VAL_AGI] = {MaxValue = 5, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 13, Rate = 100},
        [ITEMATTR_VAL_STA] = {MaxValue = 15, Rate = 80},
        [ITEMATTR_VAL_DEF] = {MaxValue = 40, Rate = 60},
        [ITEMATTR_VAL_HIT] = {MaxValue = 30, Rate = 20},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 40, Rate = 40},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 10, Rate = 2},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeCosh] = {
        -- staff
        [ITEMATTR_VAL_STR] = {MaxValue = 12, Rate = 150},
        [ITEMATTR_VAL_DEX] = {MaxValue = 5, Rate = 110},
        [ITEMATTR_VAL_AGI] = {MaxValue = 5, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 12, Rate = 90},
        [ITEMATTR_VAL_STA] = {MaxValue = 15, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 40, Rate = 60},
        [ITEMATTR_VAL_HIT] = {MaxValue = 30, Rate = 20},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 40, Rate = 40},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 60},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 200, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 500, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 20, Rate = 3},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 10, Rate = 2},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 1, Rate = 1}
    },
    [enumItemTypeShield] = {
        -- shield
        [ITEMATTR_VAL_STR] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEX] = {MaxValue = 7, Rate = 110},
        [ITEMATTR_VAL_AGI] = {MaxValue = 7, Rate = 100},
        [ITEMATTR_VAL_CON] = {MaxValue = 15, Rate = 90},
        [ITEMATTR_VAL_STA] = {MaxValue = 7, Rate = 150},
        [ITEMATTR_VAL_DEF] = {MaxValue = 40, Rate = 50},
        [ITEMATTR_VAL_HIT] = {MaxValue = 30, Rate = 50},
        [ITEMATTR_VAL_FLEE] = {MaxValue = 40, Rate = 50},
        [ITEMATTR_VAL_MXATK] = {MaxValue = 60, Rate = 50},
        [ITEMATTR_VAL_MXHP] = {MaxValue = 400, Rate = 50},
        [ITEMATTR_VAL_MXSP] = {MaxValue = 400, Rate = 50},
        [ITEMATTR_VAL_HREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_SREC] = {MaxValue = 5, Rate = 30},
        [ITEMATTR_VAL_ASPD] = {MaxValue = 10, Rate = 3},
        [ITEMATTR_VAL_MSPD] = {MaxValue = 10, Rate = 2},
        [ITEMATTR_VAL_PDEF] = {MaxValue = 3, Rate = 1}
    }
}

BADCHANCE = 0.3
BIAS = 3.4

function CalculateExtraStats(Level, ItemStat, StatRate)
    local Percent = 100
    local Title = 0
    local ItemRates = {}
    local StatVar = {}
    for i, v in pairs(ItemStat) do
        ItemRates[#ItemRates + 1] = {Value = i, Rate = v.Rate}
    end
    for i = 1, 5, 1 do
        -- Check if we'll add a stat.
        if Percentage_Random(StatRate[i]) == 1 then
            -- Let's get the stat to be added.
            local Index = WeightedRandomIndex(ItemRates)
            StatVar[#StatVar + 1] = {ID = ItemRates[Index].Value, Num = 0, Pos = 1}
            table.remove(ItemRates, Index)
            -- Check if it will be a negative stat.
            if Percentage_Random(BADCHANCE) == 1 then
                StatVar[#StatVar].Pos = -1
                Title = Title - 1
            else
                Title = Title + 1
            end
        else
            -- If no stat is added, then exit loop.
            break
        end
    end
    local X = 2
    local A = (Title + 6)
    for i, v in pairs(StatVar) do
        local MinValue = math.max(A - X, 1) / 11
        local MaxValue = math.min(A, 11) / 11
        if v.Pos == -1 then
            MinValue = math.max(A - X - 3, 1) / 11
            MaxValue = math.min(math.max(A - 3, 1), 11) / 11
        end
        MinValue = math.floor(ItemStat[v.ID].MaxValue * MinValue)
        MaxValue = math.floor(ItemStat[v.ID].MaxValue * MaxValue)
        local Value = math.random(MinValue, MaxValue)
        v.Num = Value * v.Pos
        if v.Pos == -1 and v.ID == ITEMATTR_VAL_MXATK then
            v.ID = ITEMATTR_VAL_MNATK
        end
    end
    return StatVar, Title
end

ITEMRATE = {
    [MONSTER_BAOLIAO] = {
        -- Monster Drops
        1,
        0.5,
        0.3,
        0.15,
        0.1
    },
    [QUEST_AWARD_3] = {
        -- Equipment Chests
        0.2,
        0.15,
        0.1,
        0.06,
        0.03
    },
    [QUEST_AWARD_5] = {
        -- Mystic & Beautiful Chests
        0.5,
        0.3,
        0.15,
        0.1,
        0.05
    },
    [PLAYER_HSSRA] = {
        -- Black Market
        0.2,
        0.15,
        0.1,
        0.05,
        0.03
    },
    [PLAYER_ZSITEM] = {
        -- Star of Unity
        0.3,
        0.2,
        0.15,
        0.1,
        0.05
    }
}

ITEMCOLOUR = {
    [-4] = 0,
    [-3] = 0,
    [-2] = 0,
    [-1] = 0,
    [0] = 0,
    [1] = 1,
    [2] = 3,
    [3] = 7,
    [4] = 5,
    [5] = 9
}

Creat_ItemOriginal = Creat_ItemOriginal or Creat_Item
function Creat_Item(item, item_type, item_lv, item_event)
    if item_lv >= EQUIPMENT_LV and ITEMSTAT[item_type] and ITEMRATE[item_event] then
        Reset_item_add()
        local attr, title = CalculateExtraStats(item_lv, ITEMSTAT[item_type], ITEMRATE[item_event])
        for i, v in pairs(attr) do
            SetAttributeEditable(item, i - 1, v.ID)
            SetItemAttr(item, v.ID, GetItemAttr(item, v.ID) + v.Num)
        end
        local prefix = (35 + title) * 10 + (ITEMCOLOUR[title] * 1000)
        SetItemAttr(item, ITEMATTR_MAXENERGY, prefix)
        return 0
    else
        return Creat_ItemOriginal(item, item_type, item_lv, item_event)
    end
end
