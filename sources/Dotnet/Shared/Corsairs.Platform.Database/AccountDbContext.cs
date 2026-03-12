using Microsoft.EntityFrameworkCore;
using Corsairs.Platform.Database.Entities;

namespace Corsairs.Platform.Database;

/// <summary>DbContext для базы аккаунтов (AccountServer).</summary>
public class AccountDbContext : DbContext
{
    public DbSet<Account> Accounts => Set<Account>();
    public DbSet<AccountDetails> AccountDetails => Set<AccountDetails>();
    public DbSet<LoginLog> LoginLogs => Set<LoginLog>();

    public AccountDbContext(DbContextOptions<AccountDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder b)
    {
        // Account
        b.Entity<Account>(e =>
        {
            e.HasIndex(a => a.Username).IsUnique();
            e.HasIndex(a => a.Email);
            e.Property(a => a.Username).HasMaxLength(50);
            e.Property(a => a.PasswordHash).HasMaxLength(255);
            e.Property(a => a.BanReason).HasMaxLength(500);
            e.Property(a => a.Credit).HasPrecision(18, 2);
            e.Property(a => a.LastLoginIp).HasMaxLength(45);
            e.Property(a => a.LastLoginMac).HasMaxLength(20);
        });

        // AccountDetails (1:1)
        b.Entity<AccountDetails>(e =>
        {
            e.HasKey(d => d.AccountId);
            e.HasOne(d => d.Account).WithOne(a => a.Details)
                .HasForeignKey<AccountDetails>(d => d.AccountId);
        });

        // LoginLog
        b.Entity<LoginLog>(e =>
        {
            e.HasIndex(l => l.AccountId);
            e.HasIndex(l => l.LoginAt);
            e.Property(l => l.IpAddress).HasMaxLength(45);
        });
    }
}
