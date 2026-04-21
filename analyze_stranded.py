#!/usr/bin/env python3
"""Find stranded .cpp/.h files not included in vcxproj files."""
import os
import re
import sys
from pathlib import Path

ROOT = Path(r"D:\Projects\MMORPG\TalesOfPirate")

PROJECTS = [
    {
        "name": "AudioSDL",
        "vcxproj": r"sources\Libraries\AudioSDL\proj\AudioSDL.vcxproj",
        "folders": [r"sources\Libraries\AudioSDL\src", r"sources\Libraries\AudioSDL\inc"],
        "recursive": True,
    },
    {
        "name": "Common",
        "vcxproj": r"sources\Libraries\common\Proj\Common.vcxproj",
        "folders": [r"sources\Libraries\common\src", r"sources\Libraries\common\include"],
        "recursive": True,
    },
    {
        "name": "Utils",
        "vcxproj": r"sources\Libraries\Util\src\Utils.vcxproj",
        "folders": [r"sources\Libraries\Util\src", r"sources\Libraries\Util\include"],
        "recursive": True,
    },
    {
        "name": "CorsairsNet",
        "vcxproj": r"sources\Libraries\CorsairsNet\proj\CorsairsNet.vcxproj",
        "folders": [r"sources\Libraries\CorsairsNet\src", r"sources\Libraries\CorsairsNet\include"],
        "recursive": True,
    },
    {
        "name": "CorsairsNet.Tests",
        "vcxproj": r"sources\Libraries\CorsairsNet\tests\CorsairsNet.Tests.vcxproj",
        "folders": [r"sources\Libraries\CorsairsNet\tests"],
        "recursive": True,
    },
    {
        "name": "cryptlib",
        "vcxproj": r"sources\Libraries\Cryptopp\cryptlib.vcxproj",
        "folders": [r"sources\Libraries\Cryptopp"],
        "recursive": False,  # top-level only
    },
    {
        "name": "MindPower3D",
        "vcxproj": r"sources\Engine\proj\MindPower3D.vcxproj",
        "folders": [r"sources\Engine\src", r"sources\Engine\include"],
        "recursive": True,
    },
    {
        "name": "Game (kop)",
        "vcxproj": r"sources\Client\proj\kop.vcxproj",
        "folders": [r"sources\Client\src", r"sources\Client\include"],
        "recursive": True,
    },
    {
        "name": "GameServer",
        "vcxproj": r"sources\Server\GameServer\Proj\GameServer.vcxproj",
        "folders": [r"sources\Server\GameServer\src", r"sources\Server\GameServer\include"],
        "recursive": True,
    },
    {
        "name": "LookDataTests",
        "vcxproj": r"sources\Tests\LookDataTests\LookDataTests.vcxproj",
        "folders": [r"sources\Tests\LookDataTests"],
        "recursive": True,
    },
    {
        "name": "DatabaseTests",
        "vcxproj": r"sources\Tests\DatabaseTests\DatabaseTests.vcxproj",
        "folders": [r"sources\Tests\DatabaseTests"],
        "recursive": True,
    },
]

IGNORE_DIR_NAMES = {"build", ".vs", "obj", "bin"}
IGNORE_EXTS = {".tlog", ".obj", ".pch", ".ipdb", ".iobj", ".pdb"}
SOURCE_EXTS = {".cpp", ".h", ".hpp", ".hxx", ".cxx", ".c", ".cc"}


def norm(p: Path) -> str:
    """Normalize path for comparison: absolute, lowercase, forward slashes."""
    return str(p.resolve()).replace("\\", "/").lower()


def parse_vcxproj(vcxproj_path: Path):
    """Parse vcxproj, return (included_paths_set, excluded_paths_set).

    included = files in ClCompile/ClInclude that are NOT excluded from build
               (but per-config ExcludedFromBuild=true still counts as included per spec).
    We treat anything listed in ClCompile/ClInclude as included, since
    ExcludedFromBuild just marks it as "явно исключено, но присутствует".
    """
    content = vcxproj_path.read_text(encoding="utf-8-sig", errors="replace")
    proj_dir = vcxproj_path.parent

    # Find ClCompile and ClInclude items with Include="..."
    # Match both self-closing and block forms.
    pattern = re.compile(
        r'<(ClCompile|ClInclude)\s+Include\s*=\s*"([^"]+)"(?:\s*/>|\s*>(.*?)</\1>)',
        re.DOTALL,
    )
    included = set()
    for m in pattern.finditer(content):
        tag = m.group(1)
        raw_path = m.group(2)
        # Resolve relative to vcxproj directory
        try:
            abs_path = (proj_dir / raw_path).resolve()
        except Exception:
            continue
        included.add(norm(abs_path))
    return included


def iter_source_files(folder: Path, recursive: bool):
    if not folder.exists():
        return
    if recursive:
        for root, dirs, files in os.walk(folder):
            # Prune ignored dirs (case-insensitive)
            dirs[:] = [d for d in dirs if d.lower() not in IGNORE_DIR_NAMES]
            for f in files:
                p = Path(root) / f
                ext = p.suffix.lower()
                if ext in SOURCE_EXTS:
                    yield p
    else:
        for f in folder.iterdir():
            if f.is_file():
                ext = f.suffix.lower()
                if ext in SOURCE_EXTS:
                    yield f


def analyze_project(proj):
    name = proj["name"]
    vcxproj = ROOT / proj["vcxproj"]
    if not vcxproj.exists():
        return name, None, f"vcxproj not found: {vcxproj}"
    included = parse_vcxproj(vcxproj)

    # Collect all disk source files from each folder
    all_disk_files = set()
    disk_file_list = []
    for rel_folder in proj["folders"]:
        folder = ROOT / rel_folder
        for f in iter_source_files(folder, proj["recursive"]):
            normed = norm(f)
            if normed not in all_disk_files:
                all_disk_files.add(normed)
                disk_file_list.append(f)

    # Determine project root for relative display — use common ancestor of folders (or vcxproj parent)
    project_root = ROOT / proj["folders"][0]
    # Use highest-level common root among all folders
    roots = [ROOT / f for f in proj["folders"]]
    # Find lowest common ancestor
    try:
        common = Path(os.path.commonpath([str(r.resolve()) for r in roots]))
    except ValueError:
        common = project_root.resolve()

    stranded = []
    for f in disk_file_list:
        if norm(f) not in included:
            try:
                rel = f.resolve().relative_to(common)
            except ValueError:
                rel = f
            stranded.append(str(rel).replace("\\", "/"))

    stranded.sort()
    return name, stranded, None


def main():
    total = 0
    summary = []
    for proj in PROJECTS:
        name, stranded, err = analyze_project(proj)
        print(f"\n=== {name} ===")
        if err:
            print(f"  ERROR: {err}")
            summary.append((name, "ERROR"))
            continue
        if not stranded:
            print("  (no stranded files)")
            summary.append((name, 0))
            continue
        for s in stranded:
            print(f"  {s}")
        summary.append((name, len(stranded)))
        total += len(stranded)

    print("\n\n========== SUMMARY ==========")
    for name, count in summary:
        print(f"  {name:25s} : {count}")
    print(f"  {'TOTAL':25s} : {total}")


if __name__ == "__main__":
    main()
