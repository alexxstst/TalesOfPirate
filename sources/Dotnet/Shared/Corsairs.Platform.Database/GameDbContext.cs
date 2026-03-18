using System;
using System.Linq;
using Microsoft.EntityFrameworkCore;
using Microsoft.EntityFrameworkCore.Storage.ValueConversion;
using Corsairs.Platform.Database.Entities;

namespace Corsairs.Platform.Database;

/// <summary>DbContext для игровой базы (GroupServer/GameServer). Legacy-схема MSSQL.</summary>
public class GameDbContext : DbContext
{
    // --- Персонажи ---
    public DbSet<Character> Characters => Set<Character>();
    public DbSet<PersonInfo> PersonInfos => Set<PersonInfo>();

    // --- Социальные ---
    public DbSet<Guild> Guilds => Set<Guild>();
    public DbSet<Friendship> Friendships => Set<Friendship>();
    public DbSet<Mentorship> Mentorships => Set<Mentorship>();

    // --- Аккаунты ---
    public DbSet<GameAccount> GameAccounts => Set<GameAccount>();

    // --- Геймплей ---
    public DbSet<Boat> Boats => Set<Boat>();
    public DbSet<CharacterResource> CharacterResources => Set<CharacterResource>();

    // --- Параметры ---
    public DbSet<ServerParam> ServerParams => Set<ServerParam>();

    // --- Статистика ---
    public DbSet<ServerStatLog> ServerStatLogs => Set<ServerStatLog>();

    public GameDbContext(DbContextOptions<GameDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder b)
    {
        // Конвертеры DateTime ↔ DateTimeOffset для legacy datetime-колонок
        var dtoConv = new ValueConverter<DateTimeOffset, DateTime>(
            v => v.UtcDateTime,
            v => new DateTimeOffset(DateTime.SpecifyKind(v, DateTimeKind.Utc)));

        var nullableDtoConv = new ValueConverter<DateTimeOffset?, DateTime?>(
            v => v.HasValue ? v.Value.UtcDateTime : null,
            v => v.HasValue
                ? new DateTimeOffset(DateTime.SpecifyKind(v.Value, DateTimeKind.Utc))
                : null);

        // ═══════════════════════════════════════════════════════
        // GameAccount → [dbo].[account]
        // ═══════════════════════════════════════════════════════
        b.Entity<GameAccount>(e =>
        {
            e.ToTable("account");
            e.HasKey(a => a.Id);
            e.Property(a => a.Id).HasColumnName("ato_id").ValueGeneratedNever();
            e.Property(a => a.AccountName).HasColumnName("ato_nome").HasMaxLength(50);
            e.Property(a => a.GmFlag).HasColumnName("jmes");
            e.Property(a => a.CharacterIds).HasColumnName("ator_ids").HasMaxLength(80);
            e.Property(a => a.LastIp).HasColumnName("last_ip").HasMaxLength(16);
            e.Property(a => a.DisconnectReason).HasColumnName("disc_reason").HasMaxLength(128);
            e.Property(a => a.LastLeave).HasColumnName("last_leave").HasColumnType("datetime");
            e.Property(a => a.Password2).HasColumnName("password").HasMaxLength(50);
            e.Property(a => a.MergeState).HasColumnName("merge_state");
            e.Property(a => a.Imp).HasColumnName("IMP");
            e.Property(a => a.TotalVotes).HasColumnName("total_votes");
            e.Property(a => a.Credit).HasColumnName("credit");
        });

        // ═══════════════════════════════════════════════════════
        // Character → [dbo].[character]
        // ═══════════════════════════════════════════════════════
        b.Entity<Character>(e =>
        {
            e.ToTable("character");
            e.HasKey(c => c.Id);
            e.Property(c => c.Id).HasColumnName("atorID").ValueGeneratedOnAdd();
            e.Property(c => c.AccountId).HasColumnName("ato_id");
            e.Property(c => c.Name).HasColumnName("atorNome").HasMaxLength(50);
            e.Property(c => c.Motto).HasColumnName("motto").HasMaxLength(50);
            e.Property(c => c.IconId).HasColumnName("icon");
            e.Property(c => c.Job).HasColumnName("job").HasMaxLength(50);
            e.Property(c => c.Level).HasColumnName("degree");
            e.Property(c => c.Experience).HasColumnName("exp");
            e.Property(c => c.Hp).HasColumnName("hp");
            e.Property(c => c.Sp).HasColumnName("sp");
            e.Property(c => c.Ap).HasColumnName("ap");
            e.Property(c => c.Tp).HasColumnName("tp");
            e.Property(c => c.Strength).HasColumnName("str");
            e.Property(c => c.Dexterity).HasColumnName("dex");
            e.Property(c => c.Agility).HasColumnName("agi");
            e.Property(c => c.Constitution).HasColumnName("con");
            e.Property(c => c.Spirit).HasColumnName("sta");
            e.Property(c => c.Luck).HasColumnName("luk");
            e.Property(c => c.SailLevel).HasColumnName("sail_lv");
            e.Property(c => c.SailExperience).HasColumnName("sail_exp");
            e.Property(c => c.LifeLevel).HasColumnName("live_lv");
            e.Property(c => c.LifeExperience).HasColumnName("live_exp");
            e.Property(c => c.MapName).HasColumnName("map").HasMaxLength(50);
            e.Property(c => c.MapX).HasColumnName("map_x");
            e.Property(c => c.MapY).HasColumnName("map_y");
            e.Property(c => c.Angle).HasColumnName("angle");
            e.Property(c => c.MainMap).HasColumnName("main_map").HasMaxLength(50);
            // Gold (long) ↔ bomd (int): legacy хранит как int
            e.Property(c => c.Gold).HasColumnName("bomd")
                .HasConversion(v => (int)v, v => (long)v);
            e.Property(c => c.PkMode).HasColumnName("pk_ctrl");
            // GuildId (int?) ↔ guild_id (int NOT NULL DEFAULT 0): 0 = нет гильдии.
            // HasDefaultValue(0) гарантирует: при GuildId=null колонка опускается из INSERT → БД ставит 0.
            e.Property(c => c.GuildId).HasColumnName("guild_id").HasDefaultValue(0);
            e.Property(c => c.GuildRank).HasColumnName("guild_stat");
            e.Property(c => c.GuildPermissions).HasColumnName("guild_permission");
            e.Property(c => c.LookData).HasColumnName("olhe").HasMaxLength(2000);
            e.Property(c => c.ChatColor).HasColumnName("chatColour");
            // IsDeleted (bool) ↔ delflag (tinyint)
            e.Property(c => c.IsDeleted).HasColumnName("delflag")
                .HasConversion(v => (byte)(v ? 1 : 0), v => v != 0);
            // Даты: DateTimeOffset ↔ legacy datetime
            e.Property(c => c.DeletedAt).HasColumnName("deldate")
                .HasColumnType("datetime").HasConversion(nullableDtoConv);
            e.Property(c => c.CreatedAt).HasColumnName("operdate")
                .HasColumnType("datetime").HasConversion(dtoConv);
            // EstopUntil (DateTimeOffset?) ↔ estop (datetime NOT NULL):
            // legacy всегда non-null (default GETDATE()), null → запись даты в прошлом.
            // EF Core не вызывает ValueConverter для null — HasDefaultValueSql гарантирует,
            // что при null колонка опускается из INSERT и БД подставляет значение по умолчанию.
            e.Property(c => c.EstopUntil).HasColumnName("estop")
                .HasColumnType("datetime")
                .HasDefaultValueSql("'2001-01-01'")
                .HasConversion(
                    new ValueConverter<DateTimeOffset?, DateTime>(
                        v => v.HasValue ? v.Value.UtcDateTime : new DateTime(2001, 1, 1),
                        v => (DateTimeOffset?)new DateTimeOffset(
                            DateTime.SpecifyKind(v, DateTimeKind.Utc))));

            // Свойства без legacy-колонок
            e.Ignore(c => c.MaxHp);
            e.Ignore(c => c.MaxSp);
            e.Ignore(c => c.BankGold);
            e.Ignore(c => c.PkValue);
            e.Ignore(c => c.LastLoginAt);

            e.HasIndex(c => c.Name).IsUnique().HasDatabaseName("IX_character");
            e.HasIndex(c => c.AccountId);
            e.HasIndex(c => c.GuildId);
            e.HasOne(c => c.Guild).WithMany(g => g.Members)
                .HasForeignKey(c => c.GuildId).OnDelete(DeleteBehavior.SetNull);
            e.HasQueryFilter(c => !c.IsDeleted);
        });

        // ═══════════════════════════════════════════════════════
        // PersonInfo → [dbo].[personinfo]
        // ═══════════════════════════════════════════════════════
        b.Entity<PersonInfo>(e =>
        {
            e.ToTable("personinfo");
            e.HasKey(p => p.CharacterId);
            e.Property(p => p.CharacterId).HasColumnName("atorID");
            e.Property(p => p.ProfileMotto).HasColumnName("motto").HasMaxLength(40);
            e.Property(p => p.ShowMotto).HasColumnName("showmotto");
            e.Property(p => p.Sex).HasColumnName("sex").HasMaxLength(50);
            e.Property(p => p.Age).HasColumnName("age");
            e.Property(p => p.RealName).HasColumnName("name").HasMaxLength(50);
            e.Property(p => p.AnimalZodiac).HasColumnName("animal_zodiac").HasMaxLength(50);
            e.Property(p => p.BloodType).HasColumnName("blood_type").HasMaxLength(50);
            e.Property(p => p.BirthdayDayOfYear).HasColumnName("birthday");
            e.Property(p => p.Province).HasColumnName("state").HasMaxLength(50);
            e.Property(p => p.City).HasColumnName("city").HasMaxLength(50);
            e.Property(p => p.Constellation).HasColumnName("constellation").HasMaxLength(50);
            e.Property(p => p.Career).HasColumnName("career").HasMaxLength(50);
            e.Property(p => p.PreventMessages).HasColumnName("prevent");
            e.Property(p => p.SupportCount).HasColumnName("support");
            e.Property(p => p.OpposeCount).HasColumnName("oppose");
            // Avatar хранится в отдельной таблице personavatar
            e.Ignore(p => p.Avatar);
            e.HasOne(p => p.Character).WithOne(c => c.PersonInfo)
                .HasForeignKey<PersonInfo>(p => p.CharacterId);
        });

        // ═══════════════════════════════════════════════════════
        // Guild → [dbo].[guild]
        // ═══════════════════════════════════════════════════════
        b.Entity<Guild>(e =>
        {
            e.ToTable("guild");
            e.HasKey(g => g.Id);
            // guild_id не IDENTITY — назначается вручную (0..198)
            e.Property(g => g.Id).HasColumnName("guild_id").ValueGeneratedNever();
            e.Property(g => g.Name).HasColumnName("guild_name").HasMaxLength(16);
            e.Property(g => g.Motto).HasColumnName("motto").HasMaxLength(50);
            e.Property(g => g.Password).HasColumnName("passwd").HasMaxLength(20);
            e.Property(g => g.LeaderCharacterId).HasColumnName("leader_id");
            e.Property(g => g.Level).HasColumnName("level");
            e.Property(g => g.Experience).HasColumnName("exp");
            e.Property(g => g.Gold).HasColumnName("gold");
            e.Property(g => g.BankDataJson).HasColumnName("bank");
            e.Property(g => g.MemberCount).HasColumnName("member_total");
            e.Property(g => g.MaxMembers).HasColumnName("try_total");
            // DisbandScheduledAt (DateTimeOffset?) ↔ disband_date (datetime NOT NULL)
            e.Property(g => g.DisbandScheduledAt).HasColumnName("disband_date")
                .HasColumnType("datetime")
                .HasConversion(
                    new ValueConverter<DateTimeOffset?, DateTime>(
                        v => v.HasValue ? v.Value.UtcDateTime : new DateTime(2001, 1, 1),
                        v => (DateTimeOffset?)new DateTimeOffset(
                            DateTime.SpecifyKind(v, DateTimeKind.Utc))));
            e.Property(g => g.ChallengeLevel).HasColumnName("challlevel");
            e.Property(g => g.ChallengeTargetId).HasColumnName("challid");
            e.Property(g => g.ChallengeMoney).HasColumnName("challmoney");
            // ChallengeActive (bool) ↔ challstart (smallint)
            e.Property(g => g.ChallengeActive).HasColumnName("challstart")
                .HasConversion(v => (short)(v ? 1 : 0), v => v != 0);
            // Нет legacy-колонки
            e.Ignore(g => g.CreatedAt);
            e.HasIndex(g => g.Name).IsUnique().HasDatabaseName("IX_guild_name");
            // LeaderCharacter — навигация по leader_id, без обратной коллекции
            e.HasOne(g => g.LeaderCharacter).WithMany()
                .HasForeignKey(g => g.LeaderCharacterId).OnDelete(DeleteBehavior.NoAction);
        });

        // ═══════════════════════════════════════════════════════
        // Friendship → [dbo].[friends] (нет PK в legacy)
        // ═══════════════════════════════════════════════════════
        b.Entity<Friendship>(e =>
        {
            e.ToTable("friends");
            e.HasKey(f => new { f.CharacterId1, f.CharacterId2 });
            e.Ignore(f => f.Id); // Нет identity-колонки в legacy
            e.Property(f => f.CharacterId1).HasColumnName("cha_id1");
            e.Property(f => f.CharacterId2).HasColumnName("cha_id2");
            // Legacy 'relation' — varchar(50), содержит тип как строку
            e.Property(f => f.RelationType).HasColumnName("relation")
                .HasColumnType("varchar(50)")
                .HasConversion(
                    new ValueConverter<byte, string>(
                        v => v.ToString(),
                        v => string.IsNullOrEmpty(v) ? (byte)0 : byte.Parse(v)));
            e.Ignore(f => f.FriendGroup); // Нет legacy-колонки
            // CreatedAt (DateTimeOffset) ↔ createtime (datetime NULL) — несовместимо
            e.Ignore(f => f.CreatedAt);
            e.HasOne(f => f.Character1).WithMany(c => c.FriendshipsAsFirst)
                .HasForeignKey(f => f.CharacterId1).OnDelete(DeleteBehavior.Restrict);
            e.HasOne(f => f.Character2).WithMany(c => c.FriendshipsAsSecond)
                .HasForeignKey(f => f.CharacterId2).OnDelete(DeleteBehavior.Restrict);
        });

        // ═══════════════════════════════════════════════════════
        // Mentorship → [dbo].[master] (нет PK в legacy)
        // ═══════════════════════════════════════════════════════
        b.Entity<Mentorship>(e =>
        {
            e.ToTable("master");
            e.HasKey(m => new { m.MasterCharacterId, m.PrenticeCharacterId });
            e.Ignore(m => m.Id); // Нет identity-колонки в legacy
            e.Property(m => m.MasterCharacterId).HasColumnName("cha_id1");
            e.Property(m => m.PrenticeCharacterId).HasColumnName("cha_id2");
            // IsFinished (bool) ↔ finish (int)
            e.Property(m => m.IsFinished).HasColumnName("finish")
                .HasConversion(v => v ? 1 : 0, v => v != 0);
            e.Ignore(m => m.CreatedAt); // Нет legacy-колонки
            // Legacy 'relation' varchar(50) — не маппится, EF игнорирует немаппленные колонки
            e.HasOne(m => m.MasterCharacter).WithMany(c => c.MentorshipsAsMaster)
                .HasForeignKey(m => m.MasterCharacterId).OnDelete(DeleteBehavior.Restrict);
            e.HasOne(m => m.PrenticeCharacter).WithMany(c => c.MentorshipsAsPrentice)
                .HasForeignKey(m => m.PrenticeCharacterId).OnDelete(DeleteBehavior.Restrict);
        });

        // ═══════════════════════════════════════════════════════
        // Boat → [dbo].[boat]
        // ═══════════════════════════════════════════════════════
        b.Entity<Boat>(e =>
        {
            e.ToTable("boat");
            e.HasKey(bt => bt.Id);
            e.Property(bt => bt.Id).HasColumnName("boat_id").ValueGeneratedOnAdd();
            e.Property(bt => bt.OwnerCharacterId).HasColumnName("boat_ownerid");
            e.Property(bt => bt.Name).HasColumnName("boat_name").HasMaxLength(17);
            e.Property(bt => bt.BoatDataId).HasColumnName("boat_boatid");
            e.Property(bt => bt.Berth).HasColumnName("boat_berth");
            e.Property(bt => bt.HeaderPartId).HasColumnName("boat_header");
            e.Property(bt => bt.BodyPartId).HasColumnName("boat_body");
            e.Property(bt => bt.EnginePartId).HasColumnName("boat_engine");
            e.Property(bt => bt.CannonPartId).HasColumnName("boat_cannon");
            e.Property(bt => bt.EquipmentPartId).HasColumnName("boat_equipment");
            e.Property(bt => bt.CurrentEndurance).HasColumnName("cur_endure");
            e.Property(bt => bt.MaxEndurance).HasColumnName("mx_endure");
            e.Property(bt => bt.CurrentSupply).HasColumnName("cur_supply");
            e.Property(bt => bt.MaxSupply).HasColumnName("mx_supply");
            e.Property(bt => bt.BagSize).HasColumnName("boat_bagsize");
            e.Property(bt => bt.BagDataJson).HasColumnName("boat_bag");
            e.Property(bt => bt.SkillStateData).HasColumnName("skill_state");
            e.Property(bt => bt.DeathCount).HasColumnName("boat_diecount");
            // IsDead (bool) ↔ boat_isdead (char(1))
            e.Property(bt => bt.IsDead).HasColumnName("boat_isdead")
                .HasColumnType("char(1)")
                .HasConversion(v => v ? "1" : "0", v => v == "1");
            e.Property(bt => bt.IsDeleted).HasColumnName("boat_isdeleted")
                .HasColumnType("char(1)")
                .HasConversion(v => v ? "1" : "0", v => v == "1");
            e.Property(bt => bt.MapName).HasColumnName("map").HasMaxLength(50);
            e.Property(bt => bt.MapX).HasColumnName("map_x");
            e.Property(bt => bt.MapY).HasColumnName("map_y");
            e.Property(bt => bt.Angle).HasColumnName("angle");
            e.Property(bt => bt.Degree).HasColumnName("degree");
            e.Property(bt => bt.Experience).HasColumnName("exp");
            // boat_createtime — char(50), строковый формат
            e.Ignore(bt => bt.CreatedAt);
            e.HasIndex(bt => bt.OwnerCharacterId);
            e.HasOne(bt => bt.OwnerCharacter).WithMany(c => c.Boats)
                .HasForeignKey(bt => bt.OwnerCharacterId);
        });

        // ═══════════════════════════════════════════════════════
        // CharacterResource → [dbo].[Resource]
        // ═══════════════════════════════════════════════════════
        b.Entity<CharacterResource>(e =>
        {
            e.ToTable("Resource");
            e.HasKey(r => r.Id);
            e.Property(r => r.Id).HasColumnName("id").ValueGeneratedOnAdd();
            e.Property(r => r.CharacterId).HasColumnName("atorID");
            e.Property(r => r.ResourceTypeId).HasColumnName("type_id");
            e.Property(r => r.ContentJson).HasColumnName("content");
            e.HasIndex(r => new { r.CharacterId, r.ResourceTypeId });
            e.HasOne(r => r.Character).WithMany(c => c.Resources)
                .HasForeignKey(r => r.CharacterId);
        });

        // ═══════════════════════════════════════════════════════
        // ServerParam → [dbo].[param] (колонки совпадают по конвенции)
        // ═══════════════════════════════════════════════════════
        b.Entity<ServerParam>(e =>
        {
            e.ToTable("param");
        });

        // ═══════════════════════════════════════════════════════
        // ServerStatLog → [dbo].[stat_log] (PK = track_date)
        // ═══════════════════════════════════════════════════════
        b.Entity<ServerStatLog>(e =>
        {
            e.ToTable("stat_log");
            e.HasKey(s => s.TrackDate);
            e.Ignore(s => s.Id); // Legacy PK — track_date, не identity
            e.Property(s => s.TrackDate).HasColumnName("track_date");
            e.Property(s => s.LoginCount).HasColumnName("login_num");
            e.Property(s => s.PlayCount).HasColumnName("play_num");
        });

    }

    // ══════════════════════════════════════════════════════════
    // Методы запросов (вызываются из F# для обхода BlockExpression)
    // ══════════════════════════════════════════════════════════

    /// <summary>Персонажи аккаунта, отсортированные по Id.</summary>
    public Character[] GetCharactersByAccount(int accountId, int take)
        => Characters.Where(c => c.AccountId == accountId)
                     .OrderBy(c => c.Id)
                     .Take(take)
                     .ToArray();

    /// <summary>Существует ли персонаж с данным именем.</summary>
    public bool CharacterNameExists(string name)
        => Characters.Any(c => c.Name == name);

    /// <summary>Участники гильдии (все персонажи с данным GuildId).</summary>
    public Character[] GetGuildMembers(int guildId)
        => Characters.Where(c => c.GuildId == guildId).ToArray();

    /// <summary>Количество друзей персонажа.</summary>
    public int GetFriendCount(int characterId)
        => Friendships.Count(f => f.CharacterId1 == characterId && f.RelationType == 0);

    /// <summary>Проверка дружбы.</summary>
    public bool AreFriends(int chaId1, int chaId2)
        => Friendships.Any(f => f.CharacterId1 == chaId1 && f.CharacterId2 == chaId2 && f.RelationType == 0);

    /// <summary>Все дружбы персонажа (тип = 0).</summary>
    public Friendship[] GetFriendships(int characterId)
        => Friendships.Where(f => f.CharacterId1 == characterId && f.RelationType == 0).ToArray();

    /// <summary>Дружбы в обе стороны между двумя персонажами.</summary>
    public Friendship[] GetFriendshipsBothWays(int chaId1, int chaId2)
        => Friendships.Where(f =>
            (f.CharacterId1 == chaId1 && f.CharacterId2 == chaId2) ||
            (f.CharacterId1 == chaId2 && f.CharacterId2 == chaId1)).ToArray();

    /// <summary>Найти конкретную дружбу (тип = 0).</summary>
    public Friendship? FindFriendship(int chaId1, int chaId2)
        => Friendships.Where(f =>
            f.CharacterId1 == chaId1 && f.CharacterId2 == chaId2 && f.RelationType == 0).FirstOrDefault();

    /// <summary>Все активные наставничества.</summary>
    public Mentorship[] GetActiveMentorships()
        => Mentorships.Where(m => !m.IsFinished).ToArray();

    /// <summary>Активное наставничество между конкретным учеником и мастером.</summary>
    public Mentorship[] FindActiveMentorship(int prenticeId, int masterId)
        => Mentorships.Where(m =>
            m.PrenticeCharacterId == prenticeId &&
            m.MasterCharacterId == masterId &&
            !m.IsFinished).ToArray();

    /// <summary>Все активные наставничества ученика.</summary>
    public Mentorship[] GetActiveMentorshipsByPrentice(int prenticeId)
        => Mentorships.Where(m => m.PrenticeCharacterId == prenticeId && !m.IsFinished).ToArray();

    // ══════════════════════════════════════════════════════════
    // GameAccount — методы запросов
    // ══════════════════════════════════════════════════════════

    /// <summary>Получить игровой аккаунт по ID. Создаёт запись если отсутствует (аналог C++ InsertRow).</summary>
    public GameAccount GetOrCreateGameAccount(int accountId, string accountName)
    {
        var acct = GameAccounts.Find(accountId);
        if (acct != null) return acct;
        acct = new GameAccount
        {
            Id = accountId,
            AccountName = accountName,
            CharacterIds = "0",
            LastIp = "",
            DisconnectReason = "",
            LastLeave = DateTime.UtcNow
        };
        GameAccounts.Add(acct);
        SaveChanges();
        return acct;
    }

    /// <summary>Обновить Password2 аккаунта.</summary>
    public void UpdatePassword2(int accountId, string password2)
    {
        var acct = GameAccounts.Find(accountId);
        if (acct != null)
        {
            acct.Password2 = password2;
            SaveChanges();
        }
    }

    /// <summary>Записать информацию об отключении (аналог C++ SetDiscInfo).</summary>
    public void SetDisconnectInfo(int accountId, string ip, string reason)
    {
        var acct = GameAccounts.Find(accountId);
        if (acct != null)
        {
            acct.LastIp = ip;
            acct.DisconnectReason = reason;
            acct.LastLeave = DateTime.UtcNow;
            SaveChanges();
        }
    }
}
