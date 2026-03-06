open System
open System.IO
open System.Collections.Generic
open System.Text.RegularExpressions

let path = "D:/Projects/MMORPG/TalesOfPirate/src"

let cleanString (line:string) = Regex.Replace(line, "[^\u0000-\u007F]+", String.Empty)
    
let cleanLuaFile filePath =
    Console.WriteLine($"file: {filePath}")
    let lines = List<string>(File.ReadAllLines(filePath))
    for i in 0..lines.Count - 1 do
        lines[i] <- cleanString lines[i]
    File.WriteAllLines(filePath, lines.ToArray())

let convertCluToLua (cluPath: string) =
    let luaPath = Path.ChangeExtension(cluPath, ".lua")
    Console.WriteLine($"converting: {cluPath} -> {luaPath}")
    File.Copy(cluPath, luaPath, overwrite = true)
    File.Delete(cluPath)

let cleanCppFile filePath =
    Console.WriteLine($"cpp file: {filePath}")
    let lines = List<string>(File.ReadAllLines(filePath))
    for i in 0..lines.Count - 1 do
        lines[i] <- cleanString lines[i]
    File.WriteAllLines(filePath, lines.ToArray(), Text.Encoding.UTF8)

// // Очищаем .lua файлы
// Directory.GetFiles(path, "*.lua", SearchOption.AllDirectories)
// |> Seq.iter cleanLuaFile
//
// // Очищаем .clu файлы и конвертируем их в .lua
// Directory.GetFiles(path, "*.clu", SearchOption.AllDirectories)
// |> Seq.iter (fun cluPath ->
//     cleanLuaFile cluPath
//     convertCluToLua cluPath
// )

// Очищаем C++ файлы и сохраняем в UTF-8
[| "*.cpp"; "*.h"; "*.hpp"; "*.inl" |]
|> Seq.collect (fun ext -> Directory.GetFiles(path, ext, SearchOption.AllDirectories))
|> Seq.iter cleanCppFile

