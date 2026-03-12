namespace Corsairs.Platform.Database.Entities;

/// <summary>Гильдия.</summary>
public class Guild
{
    public int Id { get; set; }

    public string Name { get; set; } = string.Empty;
    public string? Motto { get; set; }
    public string? Password { get; set; }

    public int LeaderCharacterId { get; set; }

    public int Level { get; set; }
    public long Experience { get; set; }
    public long Gold { get; set; }

    public short MemberCount { get; set; }
    public short MaxMembers { get; set; }

    /// <summary>Данные банка гильдии (JSON).</summary>
    public string? BankDataJson { get; set; }

    // --- PvP Challenge ---
    public short ChallengeLevel { get; set; }
    public int ChallengeTargetId { get; set; }
    public long ChallengeMoney { get; set; }
    public bool ChallengeActive { get; set; }

    public DateTimeOffset? DisbandScheduledAt { get; set; }
    public DateTimeOffset CreatedAt { get; set; }

    // Навигация
    public Character LeaderCharacter { get; set; } = null!;
    public ICollection<Character> Members { get; set; } = [];
}

/// <summary>Дружба между персонажами.</summary>
public class Friendship
{
    public long Id { get; set; }

    public int CharacterId1 { get; set; }
    public int CharacterId2 { get; set; }

    /// <summary>Тип отношений: 0=друг, 1=заблокирован.</summary>
    public byte RelationType { get; set; }

    /// <summary>Группа друзей (для UI-папок).</summary>
    public byte FriendGroup { get; set; }

    public DateTimeOffset CreatedAt { get; set; }

    public Character Character1 { get; set; } = null!;
    public Character Character2 { get; set; } = null!;
}

/// <summary>Наставничество (мастер-ученик).</summary>
public class Mentorship
{
    public long Id { get; set; }

    public int MasterCharacterId { get; set; }
    public int PrenticeCharacterId { get; set; }

    public bool IsFinished { get; set; }
    public DateTimeOffset CreatedAt { get; set; }

    public Character MasterCharacter { get; set; } = null!;
    public Character PrenticeCharacter { get; set; } = null!;
}
