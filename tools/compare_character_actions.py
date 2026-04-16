"""
Сравнение таблиц pose_data и character_actions в gamedata.sqlite.

Обе таблицы хранят анимации персонажей по составному ключу (character_type, action_no/action_id).
Колонки: start_frame, end_frame, keyframes (CSV из short).

Скрипт выводит:
  - количество строк в каждой таблице
  - PK, которые есть только в одной из таблиц
  - PK, общие для обеих, но с различающимися данными (с diff по полям)
  - итог: идентичны ли таблицы
"""

from __future__ import annotations

import argparse
import sqlite3
import sys
from pathlib import Path


DEFAULT_DB = Path(__file__).resolve().parents[1] / "server" / "GameServer" / "gamedata.sqlite"


def normalize_keyframes(text: str | None) -> tuple[int, ...]:
    if not text:
        return ()
    parts = (p.strip() for p in text.split(","))
    return tuple(int(p) for p in parts if p)


def fetch_table(cur: sqlite3.Cursor, table: str, action_col: str, kf_col: str) -> dict[tuple[int, int], tuple[int, int, tuple[int, ...]]]:
    sql = f"SELECT character_type, {action_col}, start_frame, end_frame, {kf_col} FROM {table}"
    rows: dict[tuple[int, int], tuple[int, int, tuple[int, ...]]] = {}
    for ct, act, sf, ef, kf in cur.execute(sql):
        rows[(ct, act)] = (sf, ef, normalize_keyframes(kf))
    return rows


def main() -> int:
    ap = argparse.ArgumentParser(description="Сравнить pose_data vs character_actions")
    ap.add_argument("--db", type=Path, default=DEFAULT_DB, help=f"путь к SQLite БД (по умолчанию: {DEFAULT_DB})")
    ap.add_argument("--show", type=int, default=20, help="сколько примеров различий выводить (0 = все)")
    args = ap.parse_args()

    if not args.db.exists():
        print(f"[error] БД не найдена: {args.db}", file=sys.stderr)
        return 2

    with sqlite3.connect(args.db) as con:
        cur = con.cursor()
        pose = fetch_table(cur, "pose_data", "action_id", "keyframes")
        char = fetch_table(cur, "character_actions", "action_no", "key_frames")

    print(f"БД:              {args.db}")
    print(f"pose_data:        {len(pose)} строк")
    print(f"character_actions: {len(char)} строк")

    keys_pose = set(pose)
    keys_char = set(char)
    only_in_pose = keys_pose - keys_char
    only_in_char = keys_char - keys_pose
    common = keys_pose & keys_char

    print(f"\nТолько в pose_data:         {len(only_in_pose)}")
    print(f"Только в character_actions: {len(only_in_char)}")
    print(f"Общих ключей:               {len(common)}")

    def preview(items, title):
        if not items:
            return
        print(f"\n{title} (первые {min(args.show or len(items), len(items))}):")
        for k in list(sorted(items))[: args.show or len(items)]:
            print(f"  (character_type={k[0]}, action={k[1]})")

    preview(only_in_pose, "Ключи только в pose_data")
    preview(only_in_char, "Ключи только в character_actions")

    diffs = []
    for k in common:
        if pose[k] != char[k]:
            diffs.append((k, pose[k], char[k]))

    print(f"\nРасхождений в общих ключах: {len(diffs)}")
    if diffs:
        print(f"\nПримеры расхождений (первые {min(args.show or len(diffs), len(diffs))}):")
        for (ct, act), p, c in sorted(diffs)[: args.show or len(diffs)]:
            ps, pe, pkf = p
            cs, ce, ckf = c
            print(f"  (type={ct}, action={act}):")
            if ps != cs or pe != ce:
                print(f"    frames: pose=({ps},{pe}) vs char=({cs},{ce})")
            if pkf != ckf:
                print(f"    keyframes: pose={list(pkf)} vs char={list(ckf)}")

    identical = not only_in_pose and not only_in_char and not diffs
    print("\n" + ("ИТОГ: таблицы идентичны" if identical else "ИТОГ: таблицы РАЗЛИЧАЮТСЯ"))
    return 0 if identical else 1


if __name__ == "__main__":
    sys.exit(main())
