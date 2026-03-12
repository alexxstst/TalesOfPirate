using Microsoft.EntityFrameworkCore;
using Corsairs.Platform.Database.Entities;

namespace Corsairs.Platform.Database;

/// <summary>DbContext для игровой базы (GroupServer/GameServer).</summary>
public class GameDbContext : DbContext
{
    // --- Персонажи ---
    public DbSet<Character> Characters => Set<Character>();
    public DbSet<PersonInfo> PersonInfos => Set<PersonInfo>();

    // --- Инвентарь ---
    public DbSet<InventoryItem> InventoryItems => Set<InventoryItem>();
    public DbSet<SkillSlot> SkillSlots => Set<SkillSlot>();

    // --- Социальные ---
    public DbSet<Guild> Guilds => Set<Guild>();
    public DbSet<Friendship> Friendships => Set<Friendship>();
    public DbSet<Mentorship> Mentorships => Set<Mentorship>();

    // --- Геймплей ---
    public DbSet<Boat> Boats => Set<Boat>();
    public DbSet<MissionProgress> MissionProgresses => Set<MissionProgress>();
    public DbSet<MapMask> MapMasks => Set<MapMask>();
    public DbSet<CharacterResource> CharacterResources => Set<CharacterResource>();

    // --- Статистика ---
    public DbSet<ServerStatLog> ServerStatLogs => Set<ServerStatLog>();

    public GameDbContext(DbContextOptions<GameDbContext> options) : base(options) { }

    protected override void OnModelCreating(ModelBuilder b)
    {
        // Character
        b.Entity<Character>(e =>
        {
            e.HasIndex(c => c.Name).IsUnique();
            e.HasIndex(c => c.AccountId);
            e.HasIndex(c => c.GuildId);
            e.Property(c => c.Name).HasMaxLength(50);
            e.Property(c => c.Motto).HasMaxLength(200);
            e.Property(c => c.Job).HasMaxLength(30);
            e.Property(c => c.MapName).HasMaxLength(100);
            e.Property(c => c.MainMap).HasMaxLength(100);
            e.Property(c => c.LookData).HasMaxLength(500);
            e.HasOne(c => c.Guild).WithMany(g => g.Members)
                .HasForeignKey(c => c.GuildId).OnDelete(DeleteBehavior.SetNull);
            e.HasQueryFilter(c => !c.IsDeleted);
        });

        // PersonInfo (1:1)
        b.Entity<PersonInfo>(e =>
        {
            e.HasKey(p => p.CharacterId);
            e.HasOne(p => p.Character).WithOne(c => c.PersonInfo)
                .HasForeignKey<PersonInfo>(p => p.CharacterId);
        });

        // InventoryItem
        b.Entity<InventoryItem>(e =>
        {
            e.HasIndex(i => new { i.CharacterId, i.ContainerType, i.SlotIndex }).IsUnique();
            e.HasOne(i => i.Character).WithMany(c => c.InventoryItems)
                .HasForeignKey(i => i.CharacterId);
        });

        // SkillSlot
        b.Entity<SkillSlot>(e =>
        {
            e.HasIndex(s => new { s.CharacterId, s.SlotIndex }).IsUnique();
            e.HasOne(s => s.Character).WithMany(c => c.Skills)
                .HasForeignKey(s => s.CharacterId);
        });

        // Guild
        b.Entity<Guild>(e =>
        {
            e.HasIndex(g => g.Name).IsUnique();
            e.Property(g => g.Name).HasMaxLength(50);
            e.Property(g => g.Motto).HasMaxLength(200);
        });

        // Friendship (двунаправленная)
        b.Entity<Friendship>(e =>
        {
            e.HasIndex(f => new { f.CharacterId1, f.CharacterId2 }).IsUnique();
            e.HasOne(f => f.Character1).WithMany(c => c.FriendshipsAsFirst)
                .HasForeignKey(f => f.CharacterId1).OnDelete(DeleteBehavior.Restrict);
            e.HasOne(f => f.Character2).WithMany(c => c.FriendshipsAsSecond)
                .HasForeignKey(f => f.CharacterId2).OnDelete(DeleteBehavior.Restrict);
        });

        // Mentorship
        b.Entity<Mentorship>(e =>
        {
            e.HasOne(m => m.MasterCharacter).WithMany(c => c.MentorshipsAsMaster)
                .HasForeignKey(m => m.MasterCharacterId).OnDelete(DeleteBehavior.Restrict);
            e.HasOne(m => m.PrenticeCharacter).WithMany(c => c.MentorshipsAsPrentice)
                .HasForeignKey(m => m.PrenticeCharacterId).OnDelete(DeleteBehavior.Restrict);
        });

        // Boat
        b.Entity<Boat>(e =>
        {
            e.HasIndex(bt => bt.OwnerCharacterId);
            e.Property(bt => bt.Name).HasMaxLength(50);
            e.Property(bt => bt.MapName).HasMaxLength(100);
            e.HasOne(bt => bt.OwnerCharacter).WithMany(c => c.Boats)
                .HasForeignKey(bt => bt.OwnerCharacterId);
        });

        // MissionProgress
        b.Entity<MissionProgress>(e =>
        {
            e.HasIndex(m => new { m.CharacterId, m.MissionDataId });
            e.HasOne(m => m.Character).WithMany(c => c.Missions)
                .HasForeignKey(m => m.CharacterId);
        });

        // MapMask
        b.Entity<MapMask>(e =>
        {
            e.HasIndex(m => new { m.CharacterId, m.MapName }).IsUnique();
            e.Property(m => m.MapName).HasMaxLength(100);
            e.HasOne(m => m.Character).WithMany(c => c.MapMasks)
                .HasForeignKey(m => m.CharacterId);
        });

        // CharacterResource
        b.Entity<CharacterResource>(e =>
        {
            e.HasIndex(r => new { r.CharacterId, r.ResourceTypeId });
            e.HasOne(r => r.Character).WithMany(c => c.Resources)
                .HasForeignKey(r => r.CharacterId);
        });

        // ServerStatLog
        b.Entity<ServerStatLog>(e =>
        {
            e.HasIndex(s => s.TrackDate);
        });
    }
}
