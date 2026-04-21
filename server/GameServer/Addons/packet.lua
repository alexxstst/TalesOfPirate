--    legacy CMD  ,
--            Lua.
CMD_MT_KICKUSER         = 1505
CMD_MC_SAY              = 501
CMD_MC_ENTERMAP         = 516
CMD_MC_POPUP_NOTICE     = 503
CMD_MC_GM_MAIL          = 597
CMD_MC_GUILD_LISTCHALL  = 905
CMD_MC_KITBAG_CHECK_ASR = 553
CMD_MC_NOTIACTION       = 508
CMD_MM_QUERY_CHA        = 4003
CMD_MM_DO_STRING        = 4015

--     ( Lua    ).

function KickCha(cha)
    SendKickUser(cha)
end

--  NPC-   ( id)  help-;
--  npc==0/nil  npcId=-1   .
function HelpInfoX(role, npc, text)
    local npcdata = -1
    if npc ~= nil and type(npc) == "userdata" then
        npcdata = GetCharID(npc)
    end
    ShowTalkPageById(role, npcdata, -1, text or "")
end

function CharSay(role, target, text)
    local tid = GetCharID(target)
    ShowSay(role, tid, text or "", 0)
end

function CloseClient(role)
    SendCloseClient(role)
end

function lua_all(role, cmd)
    SendDoStringBroadcast(role, 0, cmd)
end

--      :
-- CloneX, GmMail, GuildBid, LockKitbag, UnlockKitbag, Blind, QueryCha
--          .
