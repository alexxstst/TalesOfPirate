using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.DependencyInjection;

namespace Corsairs.Platform.Database;

/// <summary>Поддерживаемые провайдеры БД.</summary>
public enum DatabaseProviderType
{
    SqlServer,
    PostgreSql,
    MySql,
    Sqlite
}

/// <summary>Расширения для регистрации DbContext с выбранным провайдером.</summary>
public static class DatabaseServiceExtensions
{
    /// <summary>Зарегистрировать AccountDbContext с указанным провайдером.</summary>
    public static IServiceCollection AddAccountDatabase(
        this IServiceCollection services,
        DatabaseProviderType provider,
        string connectionString)
    {
        services.AddDbContext<AccountDbContext>(options =>
            ConfigureProvider(options, provider, connectionString));
        return services;
    }

    /// <summary>Зарегистрировать GameDbContext с указанным провайдером.</summary>
    public static IServiceCollection AddGameDatabase(
        this IServiceCollection services,
        DatabaseProviderType provider,
        string connectionString)
    {
        services.AddDbContext<GameDbContext>(options =>
            ConfigureProvider(options, provider, connectionString));
        return services;
    }

    /// <summary>Распарсить тип провайдера из строки конфигурации.</summary>
    public static DatabaseProviderType ParseProvider(string providerName)
    {
        return providerName.ToLowerInvariant() switch
        {
            "sqlserver" or "mssql" => DatabaseProviderType.SqlServer,
            "postgresql" or "postgres" or "npgsql" => DatabaseProviderType.PostgreSql,
            "mysql" or "mariadb" => DatabaseProviderType.MySql,
            "sqlite" => DatabaseProviderType.Sqlite,
            _ => throw new ArgumentException($"Неизвестный провайдер БД: {providerName}")
        };
    }

    private static void ConfigureProvider(
        DbContextOptionsBuilder options,
        DatabaseProviderType provider,
        string connectionString)
    {
        switch (provider)
        {
            case DatabaseProviderType.SqlServer:
                options.UseSqlServer(connectionString);
                break;
            case DatabaseProviderType.PostgreSql:
                options.UseNpgsql(connectionString);
                break;
            case DatabaseProviderType.MySql:
                var serverVersion = ServerVersion.AutoDetect(connectionString);
                options.UseMySql(connectionString, serverVersion);
                break;
            case DatabaseProviderType.Sqlite:
                options.UseSqlite(connectionString);
                break;
        }
    }
}
