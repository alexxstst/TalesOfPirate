namespace Corsairs.GroupServer.Services

open System.Collections.Concurrent
open Microsoft.Extensions.Logging

/// Участник команды.
type TeamMember =
    { CharacterId: int
      CharacterName: string
      Level: int16
      Job: string }

/// Команда (группа) игроков (in-memory).
type Team =
    { Id: int
      LeaderId: int
      mutable Members: TeamMember list }

/// Сервис управления командами (in-memory, не сохраняется в БД).
type TeamService(logger: ILogger<TeamService>) =
    let mutable _nextId = 1
    let _teams = ConcurrentDictionary<int, Team>()
    let _playerTeam = ConcurrentDictionary<int, int>() // chaId → teamId

    /// Создать команду.
    member _.CreateTeam(leader: TeamMember) =
        let id = System.Threading.Interlocked.Increment(&_nextId)
        let team = { Id = id; LeaderId = leader.CharacterId; Members = [leader] }
        _teams[id] <- team
        _playerTeam[leader.CharacterId] <- id
        logger.LogDebug("Команда создана: #{Id} лидер {Leader}", id, leader.CharacterName)
        team

    /// Добавить участника.
    member _.AddMember(teamId: int, m: TeamMember) =
        match _teams.TryGetValue(teamId) with
        | true, team when team.Members.Length < 6 ->
            team.Members <- m :: team.Members
            _playerTeam[m.CharacterId] <- teamId
            true
        | _ -> false

    /// Удалить участника.
    member _.RemoveMember(chaId: int) =
        match _playerTeam.TryRemove(chaId) with
        | true, teamId ->
            match _teams.TryGetValue(teamId) with
            | true, team ->
                team.Members <- team.Members |> List.filter (fun m -> m.CharacterId <> chaId)
                if team.Members.IsEmpty then
                    _teams.TryRemove(teamId) |> ignore
            | _ -> ()
        | _ -> ()

    /// Получить команду игрока.
    member _.GetPlayerTeam(chaId: int) =
        match _playerTeam.TryGetValue(chaId) with
        | true, teamId -> _teams.TryGetValue(teamId) |> fun (ok, t) -> if ok then ValueSome t else ValueNone
        | _ -> ValueNone

    /// Расформировать команду.
    member _.DisbandTeam(teamId: int) =
        match _teams.TryRemove(teamId) with
        | true, team ->
            for m in team.Members do
                _playerTeam.TryRemove(m.CharacterId) |> ignore
        | _ -> ()
