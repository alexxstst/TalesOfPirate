namespace Corsairs.Platform.Database.Entities;

/// <summary>Предмет в инвентаре персонажа.</summary>
public class InventoryItem
{
    public long Id { get; set; }
    public int CharacterId { get; set; }

    /// <summary>Тип контейнера: 0=инвентарь, 1=экипировка, 2=банк, 3=временный.</summary>
    public byte ContainerType { get; set; }

    /// <summary>Позиция слота в контейнере.</summary>
    public short SlotIndex { get; set; }

    /// <summary>ID предмета из таблицы данных.</summary>
    public int ItemDataId { get; set; }

    /// <summary>Количество (для стакаемых).</summary>
    public int Quantity { get; set; }

    /// <summary>Прочность текущая.</summary>
    public int Endurance { get; set; }

    /// <summary>Прочность максимальная.</summary>
    public int MaxEndurance { get; set; }

    /// <summary>Уровень улучшения (forge).</summary>
    public byte ForgeLevel { get; set; }

    /// <summary>Заблокирован от продажи/передачи.</summary>
    public bool IsLocked { get; set; }

    /// <summary>Сериализованные атрибуты (instattr, dbparam).</summary>
    public string? AttributesJson { get; set; }

    public Character Character { get; set; } = null!;
}

/// <summary>Слот навыка.</summary>
public class SkillSlot
{
    public long Id { get; set; }
    public int CharacterId { get; set; }

    /// <summary>ID навыка из таблицы данных.</summary>
    public int SkillDataId { get; set; }

    /// <summary>Уровень навыка.</summary>
    public byte Level { get; set; }

    /// <summary>Позиция в панели навыков.</summary>
    public short SlotIndex { get; set; }

    /// <summary>Тип: 0=активный, 1=пассивный, 2=шорткат.</summary>
    public byte SkillType { get; set; }

    public Character Character { get; set; } = null!;
}
