[<AutoOpen>]
module Corsairs.GateServer.DomainSerialization

let parseRecoveryMode =
    function
    | "DisconnectOnly" -> DisconnectOnly
    | "All" -> All
    | _ -> Disabled
