package event

import (
	"fmt"

	"strconv"
	"strings"
	"time"

	"github.com/hebcal/hdate"
)

// slugOverrides maps a generated slug onto the spelling hebcal.com actually
// uses, where the two differ. The website predates these tables and its URLs
// are permanent, so the event descriptions cannot simply be renamed.
// Note that only Rosh Chodesh is affected: the fast keeps the two-m spelling
// as tzom-tammuz.
var slugOverrides = map[string]string{
	"rosh-chodesh-tammuz": "rosh-chodesh-tamuz",
}

// urlFriendly converts a holiday or parsha name into the slug used in
// hebcal.com paths: lower case, apostrophes dropped, spaces hyphenated.
func urlFriendly(s string) string {
	s = strings.ToLower(s)
	s = strings.ReplaceAll(s, "'", "")
	s = strings.ReplaceAll(s, "’", "")
	s = strings.ReplaceAll(s, " ", "-")
	return s
}

// URL returns the holiday's page on hebcal.com, e.g.
// https://www.hebcal.com/holidays/sukkot-2026 .
//
// The year suffix is not always the event's own Gregorian year:
//
//   - Asara B'Tevet can fall twice in one Gregorian year, so it is
//     disambiguated with a full YYYYMMDD suffix.
//   - Chanukah can start in December and end in January, and the whole
//     festival is filed under the year it began.
//
// Israel-only observances carry ?i=on so the page shows the Israel schedule.
func (ev HolidayEvent) URL() string {
	hy := ev.date.Year()
	if hy < 5513 || hy > 6260 {
		return ""
	}
	// Yom Kippur Katan and Ta'anit BeHaB have no page on the website.
	if strings.HasPrefix(ev.desc, "Yom Kippur Katan") || ev.flags.Has(BeHaB) {
		return ""
	}
	gy, gm, gd := ev.date.ProlepticGreg()
	var suffix string
	switch {
	case ev.desc == "Asara B'Tevet":
		suffix = fmt.Sprintf("%04d%02d%02d", gy, int(gm), gd)
	case strings.HasPrefix(ev.desc, "Chanukah"):
		year := gy
		if gm == time.January {
			year--
		}
		suffix = strconv.Itoa(year)
	default:
		suffix = strconv.Itoa(gy)
	}
	slug := urlFriendly(ev.Basename())
	if alt, ok := slugOverrides[slug]; ok {
		slug = alt
	}
	// Generated Rosh Chodesh events in a leap year are already described as
	// "Rosh Chodesh Adar I" or "Adar II", and Basename keeps the numeral.
	// A caller-built event may say just "Rosh Chodesh Adar", so infer which
	// Adar from the date. Rosh Chodesh can span two days, and on the first
	// of them the date is the 30th of the *previous* month, so the month has
	// to be advanced before deciding which Adar this is.
	//
	// A common year has a single Adar and keeps the undecorated slug, even
	// though hdate numbers that month Adar1.
	if slug == "rosh-chodesh-adar" && hdate.IsLeapYear(ev.date.Year()) {
		m := ev.date.Month()
		if ev.date.Day() == 30 {
			m++
		}
		switch m {
		case hdate.Adar1:
			slug = "rosh-chodesh-adar-i"
		case hdate.Adar2:
			slug = "rosh-chodesh-adar-ii"
		}
	}
	url := "https://www.hebcal.com/holidays/" + slug + "-" + suffix
	if ev.flags.Has(ILOnly) {
		url += "?i=on"
	}
	return url
}

// URL returns the parsha's page on hebcal.com, e.g.
// https://www.hebcal.com/sedrot/bereshit-20261010 . The date is the Saturday
// on which the portion is read, which is what makes the URL unique across the
// triennial and doubled-parsha variations.
func (ev ParshaEvent) URL() string {
	hy := ev.date.Year()
	if hy < 5513 || hy > 6260 {
		return ""
	}
	gy, gm, gd := ev.date.ProlepticGreg()
	url := "https://www.hebcal.com/sedrot/" +
		urlFriendly(strings.Join(ev.Parsha.Name, "-")) +
		fmt.Sprintf("-%04d%02d%02d", gy, int(gm), gd)
	if ev.IL {
		url += "?i=on"
	}
	return url
}

// URL returns the Hebrew date's converter page, e.g.
// https://www.hebcal.com/converter?hd=15&hm=Nisan&hy=5786&h2g=1 .
func (ev HebrewDateEvent) URL() string {
	hd := ev.date
	if hd.Year() < 1 {
		return ""
	}
	return fmt.Sprintf("https://www.hebcal.com/converter?hd=%d&hm=%s&hy=%d&h2g=1",
		hd.Day(), hd.MonthName("en"), hd.Year())
}
