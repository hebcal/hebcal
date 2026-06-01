# CLAUDE.md — github.com/hebcal/learning

Go port of the daily-learning schedules from the TypeScript
[@hebcal/learning](https://github.com/hebcal/hebcal-learning) package. Each
schedule registers itself with the `dailylearning` registry in
[github.com/hebcal/hebcal-go](https://github.com/hebcal/hebcal-go), mirroring
how `@hebcal/learning` plugs into `@hebcal/core`.

## Architecture

- **hebcal-go** owns the generic plumbing and contains **no schedules**:
  - `dailylearning` — registry: `AddCalendar(name, fn, startDate...)`,
    `Lookup(name, hd, il) event.CalEvent`, `Has`, `GetCalendars`, `GetStartDate`.
    Names are case-insensitive.
  - `event` — the `CalEvent` interface (`GetDate`, `Render(locale)`, `GetFlags`,
    `GetEmoji`, `Basename`) and the `HolidayFlags` bits, including the generic
    `DAILY_LEARNING` flag used by plugin schedules.
  - `hebcal` — `HebrewCalendar` reads `CalOptions.DailyLearning []string` and
    looks up each name per day. (`DafYomi`/`MishnaYomi`/`NachYomi`/`YerushalmiYomi`
    bools also resolve through the registry.)
- **This module** (`learning`) depends on hebcal-go and supplies the schedules.
  Dependency direction is one-way: `learning → hebcal-go`. Never make hebcal-go
  import this module.
- Top-level package `learning` (`learning.go`) blank-imports every sub-package so
  `import _ "github.com/hebcal/learning"` registers all schedules. Consumers can
  also blank-import a single sub-package.

## Layout: one package per schedule family

Each sub-package (`dafyomi`, `mishnayomi`, `nachyomi`, `yerushalmi`, `nine29`,
`rambam`, `dafweekly`) typically has:
- `<name>.go` — algorithm + a `Reading`/`Daf` value type with a `String()` method
  (the untranslated English description, used as `Basename`).
- `event.go` — an unexported event struct implementing `event.CalEvent`.
  Constructor is exported (`NewXxxEvent`). `Render("en")` uses raw English;
  Hebrew (`he`, `he-x-nonikud`) uses `locales.LookupTranslation` + gematriya.
- `register.go` — `func init()` calling `dailylearning.AddCalendar(...)`. The
  registered name matches the TS calendar name exactly (e.g. `dafYomi`,
  `yerushalmi-vilna`, `929`, `rambam1`, `dafWeekly`). The wrapper returns `nil`
  when there's no reading (before start date, skip day, etc.).
- `data.go` — generated data tables (mark `// Code generated ... DO NOT EDIT`).
- `<name>_test.go` — `package x_test`, asserting against reference values copied
  from the TS spec files.

## Recipe to port a new schedule from TypeScript

1. Fetch the TS source from `https://raw.githubusercontent.com/hebcal/hebcal-learning/main/src/`:
   the schedule file (calls `DailyLearning.addCalendar('<name>', ...)`), its
   `*Base.ts` (algorithm + start date), its `*Event.ts`, and any `*.json` data.
   Get reference values from `test/<name>.spec.ts`.
2. Note the exact registered name(s) and start date (TS `Date(y, m0, d)` months
   are 0-indexed; Go `greg.ToRD(y, time.Month, d)` is 1-indexed).
3. R.D. convention matches TS `abs`: `abs % 7` is `0=Sun … 5=Fri, 6=Sat`;
   `hdate.FromRD(abs).Weekday()` and `hd.Abs()` (int64) are the tools.
4. Reuse existing data/logic instead of duplicating — e.g. `dafweekly` calls the
   exported `dafyomi.FindDaf(idx)`. Generate big tables from JSON with a script.
5. Plugin schedules return `event.DAILY_LEARNING` from `GetFlags()`. (The
   original four keep their dedicated flags: `DAF_YOMI`, etc.)
6. Add a blank import to `learning.go` and extend the expected list in
   `TestRegistry` (in `integration_test.go`) — it asserts `GetCalendars()` exactly
   and that lookups render correctly end-to-end through `HebrewCalendar`.
7. Update `README.md` (Schedules list + registered names + roadmap).

## Conventions

- GPL-2.0-or-later header on each non-generated `.go` file (copy from a sibling).
- `go 1.17` in `go.mod`. Run `gofmt -l .`, `go vet ./...`, `go test ./...` before
  committing — keep them all clean.
- Hebrew section/book names already exist in hebcal-go `locales` (Tanakh books,
  Mishneh Torah sections). `LookupTranslation` returns the key unchanged for
  `en`/`sephardic`/`""`, and `(\"\", false)` for a missing key in other locales —
  fall back to the English key in that case.
- Reference originating GitHub issues in commit/release notes when a schedule
  fulfills a feature request (e.g. 929 → hebcal/hebcal#300).

## Release process

This module is versioned with git tags. To cut a release after committing:
```
git tag -a vX.Y.Z -m "vX.Y.Z: ..." main && git push origin vX.Y.Z
gh release create vX.Y.Z --repo hebcal/learning --title "vX.Y.Z" --notes "..."
```
When changes require a new hebcal-go API, release hebcal-go first
(`go get github.com/hebcal/hebcal-go@vX.Y.Z`), then this module. The hebcal CLI
(github.com/hebcal/hebcal) vendors this module — bump + `go mod vendor` there to
expose new schedules via `--daily-learning <name>`.

Latest releases at time of writing: hebcal-go v0.14.0, learning v0.2.0 (HEAD adds
`dafweekly`, untagged), hebcal CLI v5.11.0.
