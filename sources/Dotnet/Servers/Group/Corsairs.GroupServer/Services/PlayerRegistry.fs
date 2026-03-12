namespace Corsairs.GroupServer.Services

open System.Collections.Concurrent
open Microsoft.Extensions.Logging
open Corsairs.GroupServer.Types

/// Реестр онлайн-игроков (все серверы).
type PlayerRegistry(logger: ILogger<PlayerRegistry>) =
    let _byAccount = ConcurrentDictionary<int, PlayerRecord>()
    let _byCharacter = ConcurrentDictionary<int, PlayerRecord>()
    let _byName = ConcurrentDictionary<string, PlayerRecord>()

    /// Зарегистрировать вход.
    member _.Register(record: PlayerRecord) =
        _byAccount[record.AccountId] <- record
        if record.CharacterId > 0 then
            _byCharacter[record.CharacterId] <- record
        if not (System.String.IsNullOrEmpty(record.CharacterName)) then
            _byName[record.CharacterName] <- record
        logger.LogDebug("Игрок зарегистрирован: {Record}", record)

    /// Обновить данные персонажа (после выбора).
    member _.UpdateCharacter(accountId: int, chaId: int, chaName: string) =
        match _byAccount.TryGetValue(accountId) with
        | true, record ->
            record.CharacterId <- chaId
            record.CharacterName <- chaName
            _byCharacter[chaId] <- record
            _byName[chaName] <- record
        | _ -> ()

    /// Удалить из реестра.
    member _.Unregister(accountId: int) =
        match _byAccount.TryRemove(accountId) with
        | true, record ->
            if record.CharacterId > 0 then
                _byCharacter.TryRemove(record.CharacterId) |> ignore
            if not (System.String.IsNullOrEmpty(record.CharacterName)) then
                _byName.TryRemove(record.CharacterName) |> ignore
            logger.LogDebug("Игрок удалён: {Record}", record)
        | _ -> ()

    member _.TryGetByAccount(accountId: int) =
        match _byAccount.TryGetValue(accountId) with
        | true, r -> ValueSome r | _ -> ValueNone

    member _.TryGetByCharacter(chaId: int) =
        match _byCharacter.TryGetValue(chaId) with
        | true, r -> ValueSome r | _ -> ValueNone

    member _.TryGetByName(name: string) =
        match _byName.TryGetValue(name) with
        | true, r -> ValueSome r | _ -> ValueNone

    member _.OnlineCount = _byAccount.Count

    member _.GetAllPlayers() =
        _byAccount.Values |> Seq.toArray
