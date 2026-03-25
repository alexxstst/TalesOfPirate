module Corsairs.GroupServer.LookDataParser

open System
open Corsairs.Platform.Network.Protocol.CommandMessages
open Corsairs.Platform.Database.Entities

/// Парсинг строки LookData из БД → (typeId, equipIds[34]).
/// Формат C++ (с версией):  "version#typeID,hairID;equip0;equip1;...;equip33;checksum"
/// Формат C++ (без версии): "typeID,hairID;equip0;equip1;...;equip33;checksum"
/// Формат F# (простой):     "typeID;hairID;faceID"
/// В каждой equip-секции sID находится:
///   - при version=112: на позиции 5 (после expiration, bItemTradable, bIsLock, sNeedLv, dwDBID)
///   - иначе: на позиции 2 (после expiration, bItemTradable)
let parseLookMinimal (lookData: string) : struct (int16 * int64[]) =
    if String.IsNullOrEmpty(lookData) then
        struct (0s, Array.zeroCreate EQUIP_NUM)
    else

    // Определяем версию и основные данные
    let version, dataStr =
        match lookData.IndexOf('#') with
        | -1 -> 0, lookData
        | idx ->
            let verStr = lookData.Substring(0, idx)
            match Int32.TryParse(verStr) with
            | true, v -> v, lookData.Substring(idx + 1)
            | _ -> 0, lookData

    let sections = dataStr.Split(';')

    // Section 0: "typeID,hairID,..." → typeID — первое значение через запятую
    let typeId =
        if sections.Length = 0 then 0s
        else
            let fields = sections[0].Split(',')
            match Int16.TryParse(fields[0]) with
            | true, v -> v
            | _ -> 0s

    // Sections 1..34: экипировка
    let equipIds = Array.zeroCreate<int64> EQUIP_NUM
    let sIdIndex = if version = 112 then 5 else 2

    for i in 0 .. EQUIP_NUM - 1 do
        let sectionIdx = i + 1
        if sectionIdx < sections.Length && sections[sectionIdx].Length > 0 then
            let fields = sections[sectionIdx].Split(',')
            if fields.Length > sIdIndex then
                match Int64.TryParse(fields[sIdIndex]) with
                | true, v -> equipIds[i] <- v
                | _ -> ()

    struct (typeId, equipIds)

/// Константы слотов экипировки (из CompCommand.h).
[<Literal>]
let private EQUIP_FACE = 1

/// Количество DB-параметров (enumITEMDBP_MAXNUM).
[<Literal>]
let private ITEMDBP_MAXNUM = 2

/// Количество instance-атрибутов (defITEM_INSTANCE_ATTR_NUM).
[<Literal>]
let private ITEM_INSTANCE_ATTR_NUM = 5

/// Генерация LookData в формате C++ v112: "112#typeID,hairID;slot0;...;slot33;checksum"
/// Аналог C++ LookData2String — для нового персонажа только hairID и faceID заполнены.
let buildLookData (typeId: int) (hairId: int) (faceId: int) =
    let sb = System.Text.StringBuilder(1024)
    let mutable checkSum = 0L

    // Версия + header
    sb.Append("112#").Append(typeId).Append(',').Append(hairId) |> ignore
    checkSum <- checkSum + int64 typeId + int64 hairId

    // 34 слота: expiration,tradable,lock,needLv,dbId,sID,num,endure0,endure1,energy0,energy1,forgeLv,dbParam0,dbParam1,instAttrFlag
    let emptySlot = ";0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"
    for i in 0 .. EQUIP_NUM - 1 do
        if i = EQUIP_FACE then
            // faceID в слоте enumEQUIP_FACE (позиция sID = index 5)
            sb.Append(";0,0,0,0,0,").Append(faceId).Append(",0,0,0,0,0,0,0,0,0") |> ignore
            checkSum <- checkSum + int64 faceId
        else
            sb.Append(emptySlot) |> ignore

    // Контрольная сумма
    sb.Append(';').Append(checkSum) |> ignore
    sb.ToString()

/// Маппинг Character entity из БД → CharacterSlot (in-memory).
let toCharacterSlot (c: Character) =
    let struct (typeId, equipIds) = parseLookMinimal c.LookData
    { ChaId = c.Id
      ChaName = c.Name
      Job = defaultArg (Option.ofObj c.Job) ""
      Level = c.Level
      TypeId = typeId
      EquipIds = equipIds
      Motto = defaultArg (Option.ofObj c.Motto) ""
      Icon = c.IconId
      GuildId = if c.GuildId.HasValue then c.GuildId.Value else 0
      GuildPermission = uint32 c.GuildPermissions
      ChatColour = uint32 c.ChatColor
      Estop = c.EstopUntil.HasValue && c.EstopUntil.Value > DateTimeOffset.UtcNow }
