# hebcal/learning

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

## Usage

Each schedule registers itself with the `dailylearning` registry in
`hebcal-go` via a package `init` function. Import the top-level `learning`
package for its side effects to enable every schedule:

```go
import (
	"github.com/hebcal/hebcal-go/hebcal"
	_ "github.com/hebcal/learning" // registers all learning schedules
)

opts := &hebcal.CalOptions{
	Start:    hdate.New(5783, hdate.Cheshvan, 18),
	End:      hdate.New(5783, hdate.Cheshvan, 23),
	DafYomi:  true,
	NachYomi: true,
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

## Roadmap

The goal is to eventually port all of the daily learning schedules from
the TypeScript [@hebcal/learning](https://github.com/hebcal/hebcal-learning)
package. Still to port: Daily Psalms, Arukh HaShulchan Yomi, Sefer
HaMitzvot, Chofetz Chaim, Shemirat HaLashon, Pirkei Avot, Daf-a-Week,
Tanakh Yomi, Perek Yomi, Kitzur Shulchan Aruch, Dirshu Amud Yomi, etc.

## License

GPL-2.0-or-later. See [LICENSE](LICENSE).
