namespace Corsairs.Platform.Database.Entities;

/// <summary>Игровой персонаж.</summary>
public class Character
{
    public int Id { get; set; }

    /// <summary>ID аккаунта-владельца (связь через GameDb, accountId хранится как int).</summary>
    public int AccountId { get; set; }

    /// <summary>Уникальное имя персонажа.</summary>
    public string Name { get; set; } = string.Empty;

    public string Motto { get; set; } = string.Empty;
    public short IconId { get; set; }

    /// <summary>Версия схемы данных персонажа.</summary>
    public short Version { get; set; }

    /// <summary>Класс/профессия (код из job таблицы данных).</summary>
    public string Job { get; set; } = string.Empty;

    /// <summary>Уровень персонажа.</summary>
    public short Level { get; set; }

    /// <summary>Опыт.</summary>
    public long Experience { get; set; }

    // --- Основные атрибуты ---
    public int Hp { get; set; }
    public int MaxHp { get; set; }
    public int Sp { get; set; }
    public int MaxSp { get; set; }
    public int Ap { get; set; }
    public int Tp { get; set; }

    // --- Статы ---
    public int Strength { get; set; }
    public int Dexterity { get; set; }
    public int Agility { get; set; }
    public int Constitution { get; set; }
    public int Spirit { get; set; }
    public int Luck { get; set; }

    // --- Мореходство ---
    public int SailLevel { get; set; }
    public int SailExperience { get; set; }
    public int SailLeftExperience { get; set; }
    public int LifeLevel { get; set; }
    public int LifeExperience { get; set; }
    public int LifeTp { get; set; }

    // --- Позиция ---
    public string MapName { get; set; } = string.Empty;
    public string MainMap { get; set; } = string.Empty;
    public int MapX { get; set; }
    public int MapY { get; set; }
    public int Radius { get; set; }
    public int Angle { get; set; }
    public string MapMask { get; set; } = string.Empty;

    // --- Рождение / логин ---
    public string BirthCity { get; set; } = string.Empty;
    public string LoginCha { get; set; } = string.Empty;

    // --- Денежные средства ---
    public long Gold { get; set; }
    public int BankGold { get; set; }

    // --- PK ---
    public byte PkMode { get; set; }
    public int PkValue { get; set; }

    // --- Гильдия ---
    public int? GuildId { get; set; }
    public byte GuildRank { get; set; }
    public long GuildPermissions { get; set; }

    // --- Внешний вид ---
    public string LookData { get; set; } = string.Empty;
    public int ChatColor { get; set; }

    // --- Инвентарь ---
    public int KitbagCapacity { get; set; }
    public string Kitbag { get; set; } = string.Empty;
    public int KitbagTmp { get; set; }
    public int KitbagLocked { get; set; }

    // --- Навыки ---
    public string Skillbag { get; set; } = string.Empty;
    public string SkillState { get; set; } = string.Empty;
    public string Shortcut { get; set; } = string.Empty;

    // --- Квесты ---
    public string Mission { get; set; } = string.Empty;
    public string MissionRecord { get; set; } = string.Empty;
    public string MissionTrigger { get; set; } = string.Empty;
    public string MissionCount { get; set; } = string.Empty;

    // --- Банк / магазин ---
    public string Bank { get; set; } = string.Empty;
    public int Credit { get; set; }
    public int StoreItemId { get; set; }

    // --- Сетевой адрес (runtime) ---
    public int ServerAddress { get; set; }

    // --- Состояние ---
    public bool IsDeleted { get; set; }
    public DateTimeOffset? DeletedAt { get; set; }
    public DateTimeOffset CreatedAt { get; set; }
    public DateTimeOffset LastLoginAt { get; set; }

    // --- Блокировки ---
    public DateTimeOffset? EstopUntil { get; set; }
    public int EstopTime { get; set; }

    // --- Расширенные данные (nullable в БД) ---
    public string? ExtendData { get; set; }
    public int? Imp { get; set; }

    // --- Навигация ---
    public Guild? Guild { get; set; }
    public PersonInfo? PersonInfo { get; set; }
    public ICollection<Boat> Boats { get; set; } = [];
    public ICollection<Friendship> FriendshipsAsFirst { get; set; } = [];
    public ICollection<Friendship> FriendshipsAsSecond { get; set; } = [];
    public ICollection<Mentorship> MentorshipsAsMaster { get; set; } = [];
    public ICollection<Mentorship> MentorshipsAsPrentice { get; set; } = [];
    public ICollection<CharacterResource> Resources { get; set; } = [];
}

/// <summary>Персональная информация персонажа (профиль).</summary>
public class PersonInfo
{
    public int CharacterId { get; set; }

    public string? ProfileMotto { get; set; }
    public bool ShowMotto { get; set; }
    public string? Sex { get; set; }
    public int? Age { get; set; }
    public string? RealName { get; set; }
    public string? AnimalZodiac { get; set; }
    public string? BloodType { get; set; }
    public int? BirthdayDayOfYear { get; set; }
    public string? Province { get; set; }
    public string? City { get; set; }
    public string? Constellation { get; set; }
    public string? Career { get; set; }
    public byte[]? Avatar { get; set; }
    public bool PreventMessages { get; set; }
    public int SupportCount { get; set; }
    public int OpposeCount { get; set; }

    public Character Character { get; set; } = null!;
}
