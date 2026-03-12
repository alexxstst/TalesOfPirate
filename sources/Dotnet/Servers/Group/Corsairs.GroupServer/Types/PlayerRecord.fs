namespace Corsairs.GroupServer.Types

/// Запись онлайн-игрока в GroupServer.
[<AllowNullLiteral>]
type PlayerRecord() =
    member val AccountId = 0 with get, set
    member val CharacterId = 0 with get, set
    member val CharacterName = "" with get, set
    member val Level = 0s with get, set
    member val Job = "" with get, set
    member val MapName = "" with get, set
    member val GuildId = 0 with get, set
    member val GuildName = "" with get, set

    /// ID канала на GateServer.
    member val GateChannelId = 0u with get, set
    /// ID канала на GameServer.
    member val GameChannelId = 0u with get, set

    override this.ToString() =
        $"Player acc:{this.AccountId} cha:{this.CharacterId} [{this.CharacterName}]"
