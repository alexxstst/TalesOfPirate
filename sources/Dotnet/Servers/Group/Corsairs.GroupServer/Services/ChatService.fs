namespace Corsairs.GroupServer.Services

open Microsoft.Extensions.Logging

/// Тип чат-сообщения.
[<Struct>]
type ChatType =
    | Say
    | Whisper
    | Trade
    | Guild
    | Team
    | System

/// Сервис маршрутизации чата.
type ChatService(
    registry: PlayerRegistry,
    logger: ILogger<ChatService>) =

    /// Отправить сообщение (маршрутизация по типу).
    member _.SendMessage(senderChaId: int, chatType: ChatType, message: string, targetName: string) =
        match chatType with
        | Whisper ->
            match registry.TryGetByName(targetName) with
            | ValueSome _target ->
                logger.LogDebug("Whisper от {Sender} к {Target}: {Msg}", senderChaId, targetName, message)
                // TODO: Переслать через Gate на клиент
            | ValueNone ->
                logger.LogDebug("Whisper: игрок {Target} не в сети", targetName)
        | Trade | Say ->
            logger.LogDebug("Chat [{Type}] от {Sender}: {Msg}", chatType, senderChaId, message)
            // TODO: Broadcast через все Gate
        | Guild ->
            logger.LogDebug("GuildChat от {Sender}: {Msg}", senderChaId, message)
            // TODO: Отправить всем членам гильдии
        | Team ->
            logger.LogDebug("TeamChat от {Sender}: {Msg}", senderChaId, message)
            // TODO: Отправить всем членам команды
        | System ->
            logger.LogInformation("SystemMessage: {Msg}", message)
            // TODO: Broadcast всем
