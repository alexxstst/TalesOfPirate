module Corsairs.GroupServer.LookDataParser

open System
open Corsairs.Platform.Network.Protocol.CommandMessages

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
