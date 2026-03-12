namespace Corsairs.Platform.Database.Entities;

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

/// <summary>Прогресс миссии/квеста.</summary>
public class MissionProgress
{
    public long Id { get; set; }
    public int CharacterId { get; set; }

    /// <summary>ID миссии из таблицы данных.</summary>
    public int MissionDataId { get; set; }

    /// <summary>Статус: 0=в процессе, 1=завершена, 2=провалена.</summary>
    public byte Status { get; set; }

    /// <summary>Текущий этап/шаг.</summary>
    public int CurrentStep { get; set; }

    /// <summary>Сериализованный прогресс (JSON).</summary>
    public string? ProgressJson { get; set; }

    public DateTimeOffset AcceptedAt { get; set; }
    public DateTimeOffset? CompletedAt { get; set; }

    public Character Character { get; set; } = null!;
}

/// <summary>Маска карты (туман войны) для персонажа.</summary>
public class MapMask
{
    public long Id { get; set; }
    public int CharacterId { get; set; }

    /// <summary>Имя карты.</summary>
    public string MapName { get; set; } = string.Empty;

    /// <summary>Бинарные данные маски.</summary>
    public byte[] MaskData { get; set; } = [];

    public Character Character { get; set; } = null!;
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

/// <summary>Статистический лог сервера.</summary>
public class ServerStatLog
{
    public long Id { get; set; }
    public DateTimeOffset TrackDate { get; set; }
    public int LoginCount { get; set; }
    public int PlayCount { get; set; }
}
