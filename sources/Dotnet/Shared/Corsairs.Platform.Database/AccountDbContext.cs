using Microsoft.EntityFrameworkCore;
using Corsairs.Platform.Database.Entities;

namespace Corsairs.Platform.Database;

/// <summary>DbContext для базы аккаунтов (AccountServer). Legacy-схема MSSQL.</summary>
public class AccountDbContext : DbContext
{
    public DbSet<Account> Accounts => Set<Account>();
    public DbSet<AccountDetails> AccountDetails => Set<AccountDetails>();
    public DbSet<LoginLog> LoginLogs => Set<LoginLog>();

    public AccountDbContext(DbContextOptions<AccountDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder b)
    {
        // Account → [dbo].[account_login]
        b.Entity<Account>(e =>
        {
            e.ToTable("account_login");
            e.HasKey(a => a.Id);
            e.Property(a => a.Id).HasColumnName("id").ValueGeneratedOnAdd();
            e.Property(a => a.Username).HasColumnName("name").HasMaxLength(50);
            e.Property(a => a.PasswordHash).HasColumnName("password").HasMaxLength(255);
            e.Property(a => a.Salt).HasColumnName("salt").HasMaxLength(255);
            e.Property(a => a.Sid).HasColumnName("sid");
            e.Property(a => a.LoginStatus).HasColumnName("login_status");
            e.Property(a => a.EnableLoginTick).HasColumnName("enable_login_tick");
            e.Property(a => a.LoginGroup).HasColumnName("login_group").HasMaxLength(50);
            e.Property(a => a.LastLoginTime).HasColumnName("last_login_time");
            e.Property(a => a.LastLogoutTime).HasColumnName("last_logout_time");
            e.Property(a => a.LastLoginIp).HasColumnName("last_login_ip").HasMaxLength(50);
            e.Property(a => a.EnableLoginTime).HasColumnName("enable_login_time");
            e.Property(a => a.TotalLiveTime).HasColumnName("total_live_time");
            e.Property(a => a.LastLoginMac).HasColumnName("last_login_mac").HasMaxLength(50);
            e.Property(a => a.Ban).HasColumnName("ban");
            e.Property(a => a.Email).HasColumnName("email").HasMaxLength(50);

            e.HasIndex(a => a.Username).IsUnique().HasDatabaseName("IX_account_login");
        });

        // AccountDetails → [dbo].[account_details]
        b.Entity<AccountDetails>(e =>
        {
            e.ToTable("account_details");
            e.HasKey(d => d.AccountId);
            e.Property(d => d.AccountId).HasColumnName("acc_id");
            e.Property(d => d.VipStatus).HasColumnName("vipstatus");
            e.Property(d => d.SecurityQuestion).HasColumnName("squestion").HasMaxLength(50);
            e.Property(d => d.SecurityAnswer).HasColumnName("answer").HasMaxLength(50);
            e.Property(d => d.Email).HasColumnName("email").HasMaxLength(100);
            e.Property(d => d.Gender).HasColumnName("gender");
            e.Property(d => d.Credit).HasColumnName("credit");
            e.Property(d => d.TrueName).HasColumnName("truename").HasMaxLength(50);
            e.Property(d => d.Birthday).HasColumnName("birthday");
            e.Property(d => d.Contact).HasColumnName("contact").HasMaxLength(50);
            e.Property(d => d.Country).HasColumnName("country").HasMaxLength(50);
            e.Property(d => d.IpAddr).HasColumnName("ipaddr").HasMaxLength(50);
            e.Property(d => d.Ip2Country).HasColumnName("ip2country").HasMaxLength(50);
            e.Property(d => d.Activated).HasColumnName("activated");
            e.Property(d => d.ActCode).HasColumnName("actcode").HasMaxLength(20);
            e.Property(d => d.InvtCode).HasColumnName("invtcode").HasMaxLength(50);
            e.Property(d => d.Reference).HasColumnName("reference");
            e.Property(d => d.CreateTime).HasColumnName("create_time");

            e.HasOne(d => d.Account).WithOne(a => a.Details)
                .HasForeignKey<AccountDetails>(d => d.AccountId);
        });

        // LoginLog → [dbo].[user_log]
        b.Entity<LoginLog>(e =>
        {
            e.ToTable("user_log");
            e.HasKey(l => l.Id);
            e.Property(l => l.Id).HasColumnName("log_id").ValueGeneratedOnAdd();
            e.Property(l => l.AccountId).HasColumnName("user_id");
            e.Property(l => l.UserName).HasColumnName("user_name").HasMaxLength(50);
            e.Property(l => l.LoginTime).HasColumnName("login_time");
            e.Property(l => l.LogoutTime).HasColumnName("logout_time");
            e.Property(l => l.LoginIp).HasColumnName("login_ip").HasMaxLength(20);

            e.HasIndex(l => l.AccountId);
        });
    }
}
