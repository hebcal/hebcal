// Hebcal's yerushalmi package calculates the Yerushalmi Yomi, a
// daily regimen of learning the Jerusalem Talmud.
//
// https://en.wikipedia.org/wiki/Jerusalem_Talmud
package yerushalmi

import (
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dafyomi"
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

	shas := vilnaShas
	prevCycle := VilnaStartRD
	nextCycle := VilnaStartRD
	if edition == Schottenstein {
		if cday < SchottensteinStartRD {
			panic(hd.String() + " is before Schottenstein Edition Yomi Yerushalmi cycle began")
		}
		shas = schottensteinShas
		prevCycle = SchottensteinStartRD
		nextCycle = SchottensteinStartRD
	}

	numDapim := 0
	for _, masechet := range shas {
		numDapim += masechet.Blatt
	}

	for cday >= nextCycle {
		prevCycle = nextCycle
		nextCycle += int64(numDapim)
		n := numSpecialDays(edition, prevCycle, nextCycle)
		// recalculate for any additional special days at the end
		n2 := numSpecialDays(edition, nextCycle, nextCycle+int64(n))
		nextCycle += int64(n + n2)
	}

	total := int(cday - prevCycle - int64(numSpecialDays(edition, prevCycle, cday)))

	for j := 0; j < len(shas); j++ {
		masechet := shas[j]
		if total < masechet.Blatt {
			return dafyomi.Daf{Name: masechet.Name, Blatt: total + 1}
		}
		total -= masechet.Blatt
	}

	panic("Interal error, this code should be unreachable")
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
	startYear := hdate.FromRD(startAbs).Year()
	endYear := hdate.FromRD(endAbs).Year()

	specialDays := 0
	for year := startYear; year <= endYear; year++ {
		yk := hdate.New(year, hdate.Tishrei, 10)
		ykAbs := yk.Abs()
		if ykAbs >= startAbs && ykAbs <= endAbs {
			specialDays++
		}
		av9dt := hdate.New(year, hdate.Av, 9)
		if av9dt.Weekday() == time.Saturday {
			av9dt = av9dt.Next()
		}
		av9abs := av9dt.Abs()
		if av9abs >= startAbs && av9abs <= endAbs {
			specialDays++
		}
	}
	return specialDays
}
