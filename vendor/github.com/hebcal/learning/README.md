# hebcal/learning

[![Build Status](https://github.com/hebcal/learning/actions/workflows/go.yml/badge.svg)](https://github.com/hebcal/learning/actions/workflows/go.yml)
[![GoDoc](https://pkg.go.dev/badge/github.com/hebcal/learning?status.svg)](https://pkg.go.dev/github.com/hebcal/learning)

Daily learning schedules for the [Hebcal](https://www.hebcal.com/) Jewish
calendar, implemented in Go.

This module supplies daily Torah/Talmud learning schedules and registers
them with the daily learning registry in
[github.com/hebcal/hebcal-go](https://github.com/hebcal/hebcal-go). It is
the Go counterpart of the TypeScript
[@hebcal/learning](https://github.com/hebcal/hebcal-learning) package, and
mirrors the relationship between `@hebcal/core` and `@hebcal/learning`:
`hebcal-go` contains no schedules itself, and this module plugs them in.

## Schedules

  - **dafyomi** — Daf Yomi, a daily page of the Babylonian Talmud (Bavli).
  - **mishnayomi** — Mishna Yomi, two Mishnayot per day (~6-year cycle).
  - **nachyomi** — Nach Yomi, a daily chapter of Nevi'im (Prophets) and
    Ketuvim (Writings).
  - **yerushalmi** — Yerushalmi Yomi, a daily page of the Jerusalem
    Talmud, in both the Vilna and Schottenstein editions.
  - **nine29** — 929 (Tanakh B'Yachad), a daily chapter of the Hebrew
    Bible, Sun–Thu, covering all 929 chapters in ~3.5 years.
  - **rambam** — Daily Rambam (Mishneh Torah), in both the 1-chapter-a-day
    and 3-chapters-a-day cycles.
  - **dafweekly** — Daf-a-Week, the same page of the Babylonian Talmud
    studied for an entire week (~52-year cycle).
  - **perekyomi** — Perek Yomi, one chapter of the Mishnah per day
    (~17-month cycle).
  - **psalms** — Daily Psalms (Tehillim) in the traditional 30-day cycle,
    completing the book of Psalms each Hebrew month.
  - **pirkeiavot** — Pirkei Avot ("Ethics of the Fathers"), one chapter on
    each Shabbat of the summer between Pesach and Rosh Hashana.
  - **tanakhyomi** — Tanakh Yomi, completing the Tanakh annually by the
    Masoretic division into sedarim (skips Shabbat and major festivals).

## Usage

Each schedule registers itself with the `dailylearning` registry in
`hebcal-go` via a package `init` function. Import the top-level `learning`
package for its side effects to enable every schedule:

```go
import (
	"github.com/hebcal/hebcal-go/hebcal"
	_ "github.com/hebcal/learning" // registers all learning schedules
)

opts := &hebcal.Options{
	Start:         hdate.New(5783, hdate.Cheshvan, 18),
	End:           hdate.New(5783, hdate.Cheshvan, 23),
	DailyLearning: []string{"dafYomi", "rambam3"},
}
events, err := hebcal.HebrewCalendar(opts)
```

To register only a subset of schedules, blank-import the individual
sub-packages instead:

```go
import _ "github.com/hebcal/learning/dafyomi"
```

The schedules can also be used directly without going through the
calendar:

```go
daf, _ := dafyomi.New(hdate.FromGregorian(1995, time.December, 17))
fmt.Println(daf) // Avodah Zarah 68
```

## Event URLs

Every learning event implements `URL()`, part of the `event.CalEvent`
interface from `hebcal-go`, returning a canonical link to the day's reading
(or `""` for an event that carries no page):

```go
ev := dailylearning.Lookup("dafYomi", hdate.FromGregorian(2020, time.June, 18), false)
fmt.Println(ev.URL()) // https://www.sefaria.org/Shabbat.104a?lang=bi
```

Most schedules link to [sefaria.org](https://www.sefaria.org/); Daf Yomi
and Daf-a-Week fall back to [dafyomi.org](https://www.dafyomi.org/) for the
Kinnim and Midot tractates. A few events have no page and return `""`:

  - `yerushalmi-schottenstein` — only the Vilna edition is mapped to
    Sefaria references, so the Schottenstein edition has no URL.
  - `rambam3` — a URL is returned only when the day's three chapters
    collapse to a single Mishneh Torah section; days spanning multiple
    sections return `""`; their links are in the event's memo (below).

The generated URLs match those produced by `@hebcal/learning` exactly,
verified day-by-day across all schedules over a 50-year span.

## Brief renderings and memos

Where `@hebcal/learning`'s `renderBrief()` differs from `render()`, the event
also has a `RenderBrief(locale)` method (hebcal-go's `event.BriefRenderer`),
for calendars with little room per row:

  - `929` — the chapter without its day in the cycle: `Job 11` for
    `Job 11 (759)`.
  - `yerushalmi-vilna`, `yerushalmi-schottenstein` — no "Yerushalmi" prefix:
    `Shekalim 17`.
  - `tanakhYomi` — "Seder" abbreviated: `Samuel S31 (II Samuel 18:27-19:39)`.

Every other schedule's `Render` is already the brief form. (Daf Yomi's is too:
`@hebcal/learning`'s `render()` prefixes "Daf Yomi:", this one does not.)

Two schedules carry a `Memo(locale)` with the longer description
`@hebcal/learning` puts in the event's memo, in English in every locale:

  - `tanakhYomi` — the verse range with its book: `II Samuel 18:27-19:39`.
  - `rambam3` — on days spanning more than one section, each reading and its
    sefaria.org link, separated by a blank line.

## Registered calendar names

The schedules register the following case-insensitive names with the
`dailylearning` registry, matching the names used by `@hebcal/learning`:

  - `dafYomi`
  - `mishnaYomi`
  - `nachYomi`
  - `yerushalmi-vilna`
  - `yerushalmi-schottenstein`
  - `929`
  - `rambam1`
  - `rambam3`
  - `dafWeekly`
  - `dafWeeklySunday`
  - `perekYomi`
  - `psalms`
  - `pirkeiAvotSummer`
  - `tanakhYomi`

## Roadmap

The goal is to eventually port all of the daily learning schedules from
the TypeScript [@hebcal/learning](https://github.com/hebcal/hebcal-learning)
package. Still to port: Arukh HaShulchan Yomi, Sefer HaMitzvot, Chofetz
Chaim, Shemirat HaLashon, Kitzur Shulchan Aruch, Dirshu Amud Yomi, etc.

## License

GPL-2.0-or-later. See [LICENSE](LICENSE).
