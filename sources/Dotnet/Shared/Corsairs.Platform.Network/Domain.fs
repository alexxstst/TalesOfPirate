[<AutoOpen>]
module Corsairs.Platform.Network.Domain

open System

[<Struct>]
type ChannelId = ChannelId_ of uint32

/// Extension для вывода byte array в hex-формат (lowercase, без разделителей).
type ByteArrayExtensions =
    [<System.Runtime.CompilerServices.Extension>]
    static member ToHexString(data: byte[]) : string = Convert.ToHexStringLower(data)

    [<System.Runtime.CompilerServices.Extension>]
    static member ToHexString(data: Memory<byte>) : string =
        Convert.ToHexStringLower(data.ToArray())

    [<System.Runtime.CompilerServices.Extension>]
    static member ToHexString(data: Span<byte>) : string = Convert.ToHexStringLower(data)

    [<System.Runtime.CompilerServices.Extension>]
    static member ToHexString(data: ReadOnlySpan<byte>) : string = Convert.ToHexStringLower(data)

    [<System.Runtime.CompilerServices.Extension>]
    static member ToHexString(data: ReadOnlyMemory<byte>) : string =
        Convert.ToHexStringLower(data.ToArray())
