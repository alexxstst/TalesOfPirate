namespace Corsairs.Platform.Database.Entities;

/// <summary>Игровой аккаунт (GameDB.dbo.account). Хранит привязку к персонажам, второй пароль, данные отключения.</summary>
public class GameAccount
{
    public int Id { get; set; }
    public string AccountName { get; set; } = string.Empty;
    public byte GmFlag { get; set; }
    public string CharacterIds { get; set; } = "0";
    public string LastIp { get; set; } = string.Empty;
    public string DisconnectReason { get; set; } = string.Empty;
    public DateTime LastLeave { get; set; }
    public string? Password2 { get; set; }
    public int MergeState { get; set; }
    public int Imp { get; set; }
    public int TotalVotes { get; set; }
    public int Credit { get; set; }
}

/// <summary>Корабль.</summary>
public class Boat
{
    public int Id { get; set; }
    public int OwnerCharacterId { get; set; }

    public string Name { get; set; } = string.Empty;
    public int BoatDataId { get; set; }
    public short Berth { get; set; }

    // Части корабля (ID из таблицы данных)
    public int HeaderPartId { get; set; }
    public int BodyPartId { get; set; }
    public int EnginePartId { get; set; }
    public int CannonPartId { get; set; }
    public int EquipmentPartId { get; set; }

    public int CurrentEndurance { get; set; }
    public int MaxEndurance { get; set; }
    public int CurrentSupply { get; set; }
    public int MaxSupply { get; set; }

    public short BagSize { get; set; }

    /// <summary>Сериализованные данные инвентаря корабля.</summary>
    public string? BagDataJson { get; set; }

    public string? SkillStateData { get; set; }

    public short DeathCount { get; set; }
    public bool IsDead { get; set; }
    public bool IsDeleted { get; set; }

    // Позиция
    public string? MapName { get; set; }
    public int MapX { get; set; }
    public int MapY { get; set; }
    public int Angle { get; set; }

    public short Degree { get; set; }
    public int Experience { get; set; }

    public DateTimeOffset CreatedAt { get; set; }

    public Character OwnerCharacter { get; set; } = null!;
}

/// <summary>Ресурсные данные персонажа (расширяемое хранилище).</summary>
public class CharacterResource
{
    public long Id { get; set; }
    public int CharacterId { get; set; }

    public short ResourceTypeId { get; set; }
    public string ContentJson { get; set; } = string.Empty;

    public Character Character { get; set; } = null!;
}

/// <summary>Таблица параметров сервера (рейтинг Top-5, param1..5 = ID персонажей, param6..10 = fight points).</summary>
public class ServerParam
{
    public int Id { get; set; }
    public int? Param1 { get; set; }
    public int? Param2 { get; set; }
    public int? Param3 { get; set; }
    public int? Param4 { get; set; }
    public int? Param5 { get; set; }
    public int? Param6 { get; set; }
    public int? Param7 { get; set; }
    public int? Param8 { get; set; }
    public int? Param9 { get; set; }
    public int? Param10 { get; set; }
}

/// <summary>Статистический лог сервера.</summary>
public class ServerStatLog
{
    public long Id { get; set; }
    public DateTimeOffset TrackDate { get; set; }
    public int LoginCount { get; set; }
    public int PlayCount { get; set; }
}
