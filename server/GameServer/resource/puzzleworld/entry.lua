function config_entry(entry)
    SetMapEntryEntiID(entry, 193, 1)
end

function after_create_entry(entry)
    local CopyMgr = GetMapEntryCopyObj(entry, 0)
    SetMapEntryEventName(entry, "Demonic World")
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    Notice("Announcement: The portal of Demonic World has appeared on Ascaron [" .. posx .. "," .. posy .. "].")
end

function after_destroy_entry_puzzleworld(entry)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    Notice(
        "Announcement: The portal of Demonic World has already disappeared. Please always check the announcement for the next portal appearance."
    )
end

function after_player_login_puzzleworld(entry, player_name)
    map_name, posx, posy, tmap_name = GetMapEntryPosInfo(entry)
    ChaNotice(
        player_name,
        "Announcement: The portal of Demonic World has appeared on Ascaron [" .. posx .. "," .. posy .. "]."
    )
end

function check_can_enter_puzzleworld(Player, CopyMgr)
    if Lv(Player) >= 40 then
        return 1
    else
        SystemNotice(Player, "Characters need to be level 40 and above to enter Demonic World")
        return 0
    end
end

function begin_enter_puzzleworld(Player, CopyMgr)
    SystemNotice(Player, "Entering Demonic World")
    MoveCity(Player, "Demonic World")
    Notice("Beware! " .. GetChaDefaultName(Player) .. " has entered Demonic World!")
end
