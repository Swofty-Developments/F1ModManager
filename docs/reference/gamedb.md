# Game database (GVDB)

`game_package/gamedatabases/gamedatabases.2025.bin` holds the game's values: tracks, teams, drivers, cars, grids, career data, AI tuning, menus and rules. The game loads it whole at startup. It is one flat list of 742,857 records in the shipping game, each a hashed key, a type tag and a text value.

| Page | Contents |
|---|---|
| [File format](/reference/gamedb/format) | Header, records, key hashing, value masking, value types, path naming, row counts, datasets |
| [How the game reads it](/reference/gamedb/reading) | The lookup API in `F1_25.exe`, row counts, label lookups, missing-key crashes |
| [Modding recipes](/reference/gamedb/recipes) | Editing values, adding rows, grid size, adding a track, checking a build |
| [Tools](/reference/gamedb/tools) | The formats crate, the launcher's database stage, the Python tools |
| [Table catalogue](/reference/gamedb/catalogue) | Every table family with row counts and scope (generated) |
| [Key schema](/reference/gamedb/schema) | Fields, types and examples for the main tables (generated) |

For mod files that change the database, see [Database edits](/modding/database-ops).

## In short

- A key is `fnv1a64(path)`, where `path` is a dotted name such as `tracks.Track_3.numberOfCorners`. The file stores only the hash. Names come from the game's code and from dictionaries.
- Tables are rows named `<container>.<Row>_<i>`. The row count is a separate key, `<container>._<Row>_instancecount_`. The game reads rows `0` to count−1 and ignores the rest.
- Every row has a `_metadata_` field holding its label (`bahrain`, `mclaren_26`, `f1_2026`). The game finds rows by label with a scan bounded by the row count.
- A key the game reads must exist. A missing key does not read as zero: the lookup returns a garbage pointer and the game crashes.
- Adding a row means adding every key a stock row of that table has, then raising the count.

## Table catalogue

`tools/gamedb/catalog.py` builds the catalogue from a game folder. It walks the database from the names `F1_25.exe` reads (8,286, stack-built ones included) plus `tools/gamedb/names.txt`, and writes:

| File | Contents |
|---|---|
| `tools/gamedb/catalog/tables.tsv` | One line per table family: path, row word, instances, rows, `_instancecount_`, field count, scope, label examples |
| `tools/gamedb/catalog/keys.tsv.gz` | One line per schema key (`Table_N.field`): type, rows holding it, whether the exe reads it, examples. Exe names with no match in the database are listed with `in_db = no` |
| `tools/gamedb/catalog/exe_names.tsv` | Every name the exe's database readers use, and whether the database has it |
| `tools/gamedb/catalog/summary.json` | The totals below |

Totals for the shipping database (game build `d873a1c8…`):

| | |
|---|---|
| Records | 742,857 |
| Records with a known name | 566,078 (76%) |
| Table families | 2,409 (823 with a full path) |
| Table instances / rows | 11,532 / 60,782 |
| Schema keys (`Table_N.field` form) | 18,649 |
| Schema keys whose dotted path a reader uses | 2,393 |
| Names the exe's database readers use | 8,286 |
| Exe names found in the database | 4,351 |
| Exe names not found among the named keys | 3,935 |
| Full key hashes held as 64-bit immediates in the exe | 1,098 (98 named) |

The 176,779 unnamed records sit under paths no known name reaches. The 3,935 unmatched exe names include format strings, prefixes, keys of other databases the same API reads (such as `.uidb` overrides) and keys that are absent from the shipping database.

A family groups the tables that share a path once row numbers are replaced by `N`: the 68 entrant lists each have an `entrant_N` table, and all 68 form `entrants.entrant_lists.entrant_list_N.entrant_N`.

Facts in the catalogue come from the data (rows, fields, types, counts) or the exe (names, key hashes). The `scope` column (per track, team, driver, vehicle or season) is inferred from row labels. The catalogue does not say what a table controls; the [recipes](/reference/gamedb/recipes) and [reading](/reference/gamedb/reading) pages cover the tables whose purpose is proven.

Regenerate it after a game update:

```sh
nix-shell -p 'python3.withPackages(ps: [ps.numpy ps.pefile ps.capstone])' --run \
  'python3 tools/gamedb/catalog.py --game "/path/to/F1 25" --docs docs/reference/gamedb'
```

A new `F1_25.exe` build needs its database API and reader addresses in `tools/track_keys.py` (`BUILDS`). `--db` and `--exe` take the two files directly; `--no-immediates` skips the scan for key hashes in the exe.

## Test

```sh
F1_25_DIR="/path/to/F1 25" cargo test -p swofty-formats
```

This reads and rewrites the real database when `F1_25_DIR` is set. An unchanged database writes back byte for byte.
