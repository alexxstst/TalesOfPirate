namespace Corsairs.Platform.Database.Entities;

/// <summary>Учетная запись игрока.</summary>
public class Account
{
    public int Id { get; set; }

    /// <summary>Уникальное имя для входа (логин).</summary>
    public string Username { get; set; } = string.Empty;

    /// <summary>Хэш пароля (Argon2id).</summary>
    public string PasswordHash { get; set; } = string.Empty;

    /// <summary>Email для восстановления.</summary>
    public string? Email { get; set; }

    /// <summary>Аккаунт заблокирован.</summary>
    public bool IsBanned { get; set; }

    /// <summary>Дата разблокировки (null = бессрочный бан).</summary>
    public DateTimeOffset? BanExpiresAt { get; set; }

    /// <summary>Причина бана.</summary>
    public string? BanReason { get; set; }

    /// <summary>GM-уровень: 0=обычный, 1+=GM.</summary>
    public byte GmLevel { get; set; }

    /// <summary>Статус онлайн: 0=оффлайн, 1=на Gate, 2=в игре.</summary>
    public byte LoginStatus { get; set; }

    /// <summary>ID текущей сессии (для предотвращения повторного входа).</summary>
    public Guid? SessionId { get; set; }

    /// <summary>VIP до этой даты.</summary>
    public DateTimeOffset? VipExpiresAt { get; set; }

    /// <summary>Кредит (донат-валюта).</summary>
    public decimal Credit { get; set; }

    public DateTimeOffset CreatedAt { get; set; }
    public DateTimeOffset? LastLoginAt { get; set; }
    public DateTimeOffset? LastLogoutAt { get; set; }
    public string? LastLoginIp { get; set; }
    public string? LastLoginMac { get; set; }
    public long TotalPlaytimeSeconds { get; set; }

    // Навигация
    public AccountDetails? Details { get; set; }
    public ICollection<LoginLog> LoginLogs { get; set; } = [];
}

/// <summary>Дополнительные данные аккаунта (персональные).</summary>
public class AccountDetails
{
    public int AccountId { get; set; }

    public string? SecurityQuestion { get; set; }
    public string? SecurityAnswer { get; set; }
    public string? RealName { get; set; }
    public DateOnly? Birthday { get; set; }
    public byte Gender { get; set; }
    public string? Country { get; set; }
    public string? ContactInfo { get; set; }

    // Навигация
    public Account Account { get; set; } = null!;
}

/// <summary>Лог входов/выходов.</summary>
public class LoginLog
{
    public long Id { get; set; }
    public int AccountId { get; set; }
    public string? IpAddress { get; set; }
    public DateTimeOffset LoginAt { get; set; }
    public DateTimeOffset? LogoutAt { get; set; }

    public Account Account { get; set; } = null!;
}
