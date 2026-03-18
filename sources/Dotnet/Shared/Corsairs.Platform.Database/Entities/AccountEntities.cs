namespace Corsairs.Platform.Database.Entities;

/// <summary>Учетная запись игрока. Таблица: [dbo].[account_login]</summary>
public class Account
{
    public int Id { get; set; }

    /// <summary>Уникальное имя для входа (логин). Колонка: name</summary>
    public string Username { get; set; } = string.Empty;

    /// <summary>Хэш пароля. Колонка: password</summary>
    public string PasswordHash { get; set; } = string.Empty;

    /// <summary>Соль для хэша пароля. Колонка: salt</summary>
    public string? Salt { get; set; }

    /// <summary>ID сессии (int, используется legacy-протоколом). Колонка: sid</summary>
    public int Sid { get; set; }

    /// <summary>Статус онлайн: 0=оффлайн, 1=на Gate, 2=в игре. Колонка: login_status</summary>
    public int LoginStatus { get; set; }

    /// <summary>Тик разрешения входа (ban-timing). Колонка: enable_login_tick</summary>
    public long EnableLoginTick { get; set; }

    /// <summary>Группа серверов, на которой залогинен. Колонка: login_group</summary>
    public string? LoginGroup { get; set; }

    /// <summary>Последний вход. Колонка: last_login_time</summary>
    public DateTime? LastLoginTime { get; set; }

    /// <summary>Последний выход. Колонка: last_logout_time</summary>
    public DateTime? LastLogoutTime { get; set; }

    /// <summary>IP последнего входа. Колонка: last_login_ip</summary>
    public string? LastLoginIp { get; set; }

    /// <summary>Время разрешения входа (дата разбана). Колонка: enable_login_time</summary>
    public DateTime? EnableLoginTime { get; set; }

    /// <summary>Общее время онлайн в секундах. Колонка: total_live_time</summary>
    public long TotalLiveTime { get; set; }

    /// <summary>MAC-адрес последнего входа. Колонка: last_login_mac</summary>
    public string? LastLoginMac { get; set; }

    /// <summary>Аккаунт заблокирован. Колонка: ban</summary>
    public bool? Ban { get; set; }

    /// <summary>Email. Колонка: email</summary>
    public string? Email { get; set; }

    // Навигация
    public AccountDetails? Details { get; set; }
    public ICollection<LoginLog> LoginLogs { get; set; } = [];
}

/// <summary>Дополнительные данные аккаунта. Таблица: [dbo].[account_details]</summary>
public class AccountDetails
{
    /// <summary>FK на Account.Id. Колонка: acc_id</summary>
    public int AccountId { get; set; }

    /// <summary>VIP-статус (дата окончания). Колонка: vipstatus</summary>
    public DateTime? VipStatus { get; set; }

    /// <summary>Секретный вопрос. Колонка: squestion</summary>
    public string SecurityQuestion { get; set; } = string.Empty;

    /// <summary>Ответ на секретный вопрос. Колонка: answer</summary>
    public string SecurityAnswer { get; set; } = string.Empty;

    /// <summary>Email (дублируется из account_login). Колонка: email</summary>
    public string Email { get; set; } = string.Empty;

    /// <summary>Пол. Колонка: gender</summary>
    public int Gender { get; set; }

    /// <summary>Кредит (донат-валюта). Колонка: credit</summary>
    public double Credit { get; set; }

    /// <summary>Настоящее имя. Колонка: truename</summary>
    public string TrueName { get; set; } = string.Empty;

    /// <summary>Дата рождения. Колонка: birthday</summary>
    public DateTime Birthday { get; set; }

    /// <summary>Контакт. Колонка: contact</summary>
    public string Contact { get; set; } = string.Empty;

    /// <summary>Страна. Колонка: country</summary>
    public string Country { get; set; } = string.Empty;

    /// <summary>IP при регистрации. Колонка: ipaddr</summary>
    public string IpAddr { get; set; } = string.Empty;

    /// <summary>Страна по IP. Колонка: ip2country</summary>
    public string Ip2Country { get; set; } = string.Empty;

    /// <summary>Активирован ли аккаунт. Колонка: activated</summary>
    public int Activated { get; set; }

    /// <summary>Код активации. Колонка: actcode</summary>
    public string? ActCode { get; set; }

    /// <summary>Код приглашения. Колонка: invtcode</summary>
    public string? InvtCode { get; set; }

    /// <summary>ID реферера. Колонка: reference</summary>
    public int? Reference { get; set; }

    /// <summary>Дата создания. Колонка: create_time</summary>
    public DateTime CreateTime { get; set; }

    // Навигация
    public Account Account { get; set; } = null!;
}

/// <summary>Лог входов/выходов. Таблица: [dbo].[user_log]</summary>
public class LoginLog
{
    /// <summary>Колонка: log_id</summary>
    public int Id { get; set; }

    /// <summary>Колонка: user_id</summary>
    public int AccountId { get; set; }

    /// <summary>Колонка: user_name</summary>
    public string UserName { get; set; } = string.Empty;

    /// <summary>Колонка: login_time</summary>
    public DateTime? LoginTime { get; set; }

    /// <summary>Колонка: logout_time</summary>
    public DateTime? LogoutTime { get; set; }

    /// <summary>Колонка: login_ip</summary>
    public string? LoginIp { get; set; }

    // Навигация
    public Account Account { get; set; } = null!;
}
