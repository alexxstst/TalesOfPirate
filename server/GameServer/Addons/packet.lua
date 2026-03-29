CMD_MT_KICKUSER = 1505
CMD_MC_SAY = 501
CMD_MC_ENTERMAP = 516
CMD_MC_POPUP_NOTICE = 503
CMD_MC_GM_MAIL = 597
CMD_MC_GUILD_LISTCHALL = 905
CMD_MC_KITBAG_CHECK_ASR = 553
CMD_MC_NOTIACTION = 508
CMD_MM_QUERY_CHA = 4003
CMD_MM_DO_STRING = 4015

function KickCha(cha)
    local pkt = GetPacket()
    WriteCmd(pkt, CMD_MT_KICKUSER)
    SendPacket(cha, pkt)
end

function HelpInfoX(role, npc, text)
    local npcdata = -1
    if npc ~= nil and type(npc) == "userdata" then
        npcdata = GetCharID(npc)
    end
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_TALKPAGE)
    WriteDword(packet, npcdata)
    WriteByte(packet, -1)
    WriteString(packet, text)
    SendPacket(role, packet)
end

function CharSay(role, target, text)
    local pkt = GetPacket()
    local tid = GetCharID(target)
    WriteCmd(pkt, CMD_MC_SAY)
    WriteDword(pkt, tid)
    WriteString(pkt, text)
    SendPacket(role, pkt)
end

function CloneX(cha)
    local orginal_cha = cha
    if (ChaIsBoat(cha) == 1) then
        ship = TurnToShip(cha)
        cha = TurnToCha(cha)
    end
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_ENTERMAP)

    if (ChaIsBoat(orginal_cha) == 1) then
        SendPacket(ship, packet)
    else
        SendPacket(cha, packet)
    end
end

function GmMail(role, str1, str2)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_GM_MAIL)
    WriteString(packet, str1)
    if str2 ~= nil then
        WriteString(packet, str2)
    else
        WriteString(packet, "")
    end
    SendPacket(role, packet)
end

function CloseClient(role)
    local ID = GetRoleID(role)
    local packet = GetPacket()
    WriteCmd(packet, 935)
    SendPacket(role, packet)
end

function GuildBid(role)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_GUILD_LISTCHALL)
    SendPacket(role, packet)
end

function LockKitbag(cha)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_KITBAG_CHECK_ASR)
    WriteByte(packet, 1)
    SendPacket(cha, packet)
end

function UnlockKitbag(cha)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_KITBAG_CHECK_ASR)
    WriteByte(packet, 0)
    SendPacket(cha, packet)
end

function Blind(cha, toself)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MC_NOTIACTION)
    WriteDword(packet, GetCharID(cha))
    WriteDword(packet, 0)
    WriteByte(packet, 5)
    for i = 1, 9 do
        local id = GetItemID(GetChaItem(cha, 1, i))
        if i == 2 then
            id = 825
        end
        WriteWord(packet, id)
    end
    NoticeChangeToEye(cha, packet, toself)
end

function QueryCha(role, targetname)
    local ID = GetRoleID(role)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MM_QUERY_CHA)
    WriteDword(packet, ID)
    WriteString(packet, target)
    WriteDword(packet, ID)
    SendPacket(role, packet)
end

function lua_all(role, cmd)
    local packet = GetPacket()
    WriteCmd(packet, CMD_MM_DO_STRING)
    WriteDword(packet, 0)
    WriteString(packet, cmd)
    SendPacket(role, packet)
end
