// Hebcal's yerushalmi package calculates the Yerushalmi Yomi, a
// daily regimen of learning the Jerusalem Talmud.
//
// https://en.wikipedia.org/wiki/Jerusalem_Talmud
package yerushalmi

import (
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/learning/dafyomi"
)

// Using the Vilna edition, the Yerushalmi Daf Yomi program takes
// ~4.25 years or 51 months.
// Unlike the Daf Yomi Bavli cycle, this Yerushalmi cycle skips both
// Yom Kippur and Tisha B'Av. The page numbers are according to the Vilna
// Edition which is used since 1900.
//
// The Schottenstein edition uses different page numbers and takes
// ~6 years to complete.
type Edition int

const (
	Vilna Edition = 1 + iota
	Schottenstein
)

// Vilna Edition
var vilnaShas = []dafyomi.Daf{
	{Name: "Berakhot", Blatt: 68},
	{Name: "Peah", Blatt: 37},
	{Name: "Demai", Blatt: 34},
	{Name: "Kilayim", Blatt: 44},
	{Name: "Sheviit", Blatt: 31},
	{Name: "Terumot", Blatt: 59},
	{Name: "Maasrot", Blatt: 26},
	{Name: "Maaser Sheni", Blatt: 33},
	{Name: "Challah", Blatt: 28},
	{Name: "Orlah", Blatt: 20},
	{Name: "Bikkurim", Blatt: 13},
	{Name: "Shabbat", Blatt: 92},
	{Name: "Eruvin", Blatt: 65},
	{Name: "Pesachim", Blatt: 71},
	{Name: "Beitzah", Blatt: 22},
	{Name: "Rosh Hashanah", Blatt: 22},
	{Name: "Yoma", Blatt: 42},
	{Name: "Sukkah", Blatt: 26},
	{Name: "Taanit", Blatt: 26},
	{Name: "Shekalim", Blatt: 33},
	{Name: "Megillah", Blatt: 34},
	{Name: "Chagigah", Blatt: 22},
	{Name: "Moed Katan", Blatt: 19},
	{Name: "Yevamot", Blatt: 85},
	{Name: "Ketubot", Blatt: 72},
	{Name: "Sotah", Blatt: 47},
	{Name: "Nedarim", Blatt: 40},
	{Name: "Nazir", Blatt: 47},
	{Name: "Gittin", Blatt: 54},
	{Name: "Kiddushin", Blatt: 48},
	{Name: "Bava Kamma", Blatt: 44},
	{Name: "Bava Metzia", Blatt: 37},
	{Name: "Bava Batra", Blatt: 34},
	{Name: "Shevuot", Blatt: 44},
	{Name: "Makkot", Blatt: 9},
	{Name: "Sanhedrin", Blatt: 57},
	{Name: "Avodah Zarah", Blatt: 37},
	{Name: "Horayot", Blatt: 19},
	{Name: "Niddah", Blatt: 13},
}

// Schottenstein Edition
var schottensteinShas = []dafyomi.Daf{
	{Name: "Berakhot", Blatt: 94},
	{Name: "Peah", Blatt: 73},
	{Name: "Demai", Blatt: 77},
	{Name: "Kilayim", Blatt: 84},
	{Name: "Sheviit", Blatt: 87},
	{Name: "Terumot", Blatt: 107},
	{Name: "Maasrot", Blatt: 46},
	{Name: "Maaser Sheni", Blatt: 59},
	{Name: "Challah", Blatt: 49},
	{Name: "Orlah", Blatt: 42},
	{Name: "Bikkurim", Blatt: 26},
	{Name: "Shabbat", Blatt: 113},
	{Name: "Eruvin", Blatt: 71},
	{Name: "Pesachim", Blatt: 86},
	{Name: "Shekalim", Blatt: 61},
	{Name: "Yoma", Blatt: 57},
	{Name: "Sukkah", Blatt: 33},
	{Name: "Beitzah", Blatt: 49},
	{Name: "Rosh Hashanah", Blatt: 27},
	{Name: "Taanit", Blatt: 31},
	{Name: "Megillah", Blatt: 41},
	{Name: "Chagigah", Blatt: 28},
	{Name: "Moed Katan", Blatt: 23},
	{Name: "Yevamot", Blatt: 88},
	{Name: "Ketubot", Blatt: 77},
	{Name: "Nedarim", Blatt: 42},
	{Name: "Nazir", Blatt: 53},
	{Name: "Sotah", Blatt: 52},
	{Name: "Gittin", Blatt: 53},
	{Name: "Kiddushin", Blatt: 53},
	{Name: "Bava Kamma", Blatt: 40},
	{Name: "Bava Metzia", Blatt: 35},
	{Name: "Bava Batra", Blatt: 39},
	{Name: "Sanhedrin", Blatt: 75},
	{Name: "Shevuot", Blatt: 49},
	{Name: "Avodah Zarah", Blatt: 34},
	{Name: "Makkot", Blatt: 11},
	{Name: "Horayot", Blatt: 18},
	{Name: "Niddah", Blatt: 11},
}

// VilnaStartRD is the R.D. date of the first cycle of
// Yerushalmi Yomi, using the Vilna Edition page numbering.
var VilnaStartRD = greg.ToRD(1980, time.February, 2)

// SchottensteinStartRD is the R.D. date of the first cycle of
// Yerushalmi Yomi using the Schottenstein Edition page numbering.
var SchottensteinStartRD = greg.ToRD(2022, time.November, 14)

// New calculates the Daf Yomi Yerushalmi for given date.
//
// Returns an empty Daf for Yom Kippur and Tisha B'Av.
//
// Panics if the date is before Daf Yomi Yerushalmi cycle began
// (2 February 1980).
func New(hd hdate.HDate, edition Edition) dafyomi.Daf {
	cday := hd.Abs()
	if cday < VilnaStartRD {
		panic(hd.String() + " is before Daf Yomi Yerushalmi cycle began")
	}

	if edition == Vilna && skipDay(hd) {
		return dafyomi.Daf{}
	}

	if edition == Schottenstein && cday < SchottensteinStartRD {
		panic(hd.String() + " is before Schottenstein Edition Yomi Yerushalmi cycle began")
	}
	shas := shasFor(edition)

	// Each cycle consumes exactly numDapim reading days, so the position
	// within the current cycle is just the running reading count modulo the
	// cycle length -- no need to walk cycle by cycle from the start of the
	// epoch.
	total := readingsBefore(edition, cday) % countDapim(edition)

	for j := 0; j < len(shas); j++ {
		masechet := shas[j]
		if total < masechet.Blatt {
			return dafyomi.Daf{Name: masechet.Name, Blatt: total + 1}
		}
		total -= masechet.Blatt
	}

	// A modulo can't reach numDapim, so this is genuinely unreachable.
	panic("Internal error, this code should be unreachable")
}

// shasFor returns the masechtot and page counts for the given edition.
func shasFor(edition Edition) []dafyomi.Daf {
	if edition == Schottenstein {
		return schottensteinShas
	}
	return vilnaShas
}

// startAbsFor returns the R.D. day on which the given edition's first cycle
// began.
func startAbsFor(edition Edition) int64 {
	if edition == Schottenstein {
		return SchottensteinStartRD
	}
	return VilnaStartRD
}

// countDapim returns the total number of pages in the given edition, i.e. the
// number of reading days in a full cycle.
func countDapim(edition Edition) int {
	numDapim := 0
	for _, masechet := range shasFor(edition) {
		numDapim += masechet.Blatt
	}
	return numDapim
}

func yomKippurAbs(year int) int64 {
	return hdate.ToRD(year, hdate.Tishrei, 10)
}

func tishaBavObservedAbs(year int) int64 {
	av9dt := hdate.New(year, hdate.Av, 9)
	if av9dt.Weekday() == time.Saturday {
		av9dt = av9dt.Next()
	}
	return av9dt.Abs()
}

// skippedDaysBefore returns the number of skipped days (Yom Kippur and the
// observed Tisha B'Av) falling strictly before abs, counted from an arbitrary
// fixed origin.
//
// Every Hebrew year contains exactly one of each, with Yom Kippur (10 Tishrei)
// near the start of the year and Tisha B'Av (9 Av, deferred to the 10th when
// the 9th is Shabbat) near the end. So every skipped day of every preceding
// year is before abs, and only the current year needs to be examined. The
// arbitrary origin cancels whenever two of these counts are subtracted.
func skippedDaysBefore(abs int64) int {
	year := hdate.FromRD(abs).Year()
	n := 2 * year
	if yomKippurAbs(year) < abs {
		n++
	}
	if tishaBavObservedAbs(year) < abs {
		n++
	}
	return n
}

// readingsBefore returns the number of reading days in the half-open range
// [startAbs, cday), i.e. how many dapim have been studied before cday.
func readingsBefore(edition Edition, cday int64) int {
	startAbs := startAbsFor(edition)
	elapsed := int(cday - startAbs)
	if edition == Schottenstein {
		return elapsed
	}
	return elapsed - (skippedDaysBefore(cday) - skippedDaysBefore(startAbs))
}

// cycleStart returns the R.D. day on which the cycle containing cday began.
func cycleStart(edition Edition, cday int64) int64 {
	numDapim := countDapim(edition)
	startAbs := startAbsFor(edition)
	target := readingsBefore(edition, cday) / numDapim * numDapim
	if edition == Schottenstein {
		return startAbs + int64(target)
	}
	// Invert readingsBefore(): begin at the lower bound that assumes no
	// skipped days, then add back however many were found, repeating until the
	// count settles. Skipped days are sparse (two per Hebrew year), so adding
	// them can only uncover a handful more and this converges after a couple
	// of passes.
	abs := startAbs + int64(target)
	deficit := target - readingsBefore(edition, abs)
	for deficit > 0 {
		abs += int64(deficit)
		deficit = target - readingsBefore(edition, abs)
	}
	// A cycle boundary can land on a skipped day; advance to the first day
	// that actually has a reading.
	for skipDay(hdate.FromRD(abs)) {
		abs++
	}
	return abs
}

// No Daf for Yom Kippur and Tisha B'Av when following
// the classic Vilna Edition
func skipDay(hd hdate.HDate) bool {
	day := hd.Day()
	switch hd.Month() {
	case hdate.Tishrei:
		if day == 10 {
			return true
		}
	case hdate.Av:
		dow := hd.Weekday()
		if (day == 9 && dow != time.Saturday) ||
			(day == 10 && dow == time.Sunday) {
			return true
		}
	}
	return false
}

// Calculates the number of YK and 9Av that occur during between
// startAbs and endAbs.
func numSpecialDays(edition Edition, startAbs, endAbs int64) int {
	if edition == Schottenstein {
		return 0
	}
	// Both bounds are inclusive, so count everything before endAbs+1 and
	// discount everything before startAbs.
	return skippedDaysBefore(endAbs+1) - skippedDaysBefore(startAbs)
}
