package event

import (
	"fmt"

	"github.com/hebcal/hdate"
	"strconv"
	"strings"
	"time"
)

// URLer is implemented by events that have a canonical page on hebcal.com.
//
// It is deliberately a separate interface rather than a method on CalEvent:
// CalEvent is implemented outside this package, and widening it would break
// those implementations. Callers should use the URL helper below, which
// returns "" for events that do not carry a URL.
type URLer interface {
	// URL returns the absolute hebcal.com URL for this event, or "" if the
	// event has no page (for example a Molad, or a date outside the range the
	// website covers).
	URL() string
}

// URL returns the canonical hebcal.com URL for ev, or "" if it has none.
func URL(ev CalEvent) string {
	if u, ok := ev.(URLer); ok {
		return u.URL()
	}
	return ""
}

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

// gregYearInRange reports whether a Gregorian year has a page on the website.
func gregYearInRange(gy int) bool {
	return gy >= 100 && gy <= 2999
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
	gy, gm, gd := ev.Date.ProlepticGreg()
	if !gregYearInRange(gy) {
		return ""
	}
	// Yom Kippur Katan has no page on the website.
	if strings.HasPrefix(ev.Desc, "Yom Kippur Katan") {
		return ""
	}
	var suffix string
	switch {
	case ev.Desc == "Asara B'Tevet":
		suffix = fmt.Sprintf("%04d%02d%02d", gy, int(gm), gd)
	case strings.HasPrefix(ev.Desc, "Chanukah"):
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
	// In a leap year the two Adars share one description, but the website
	// files them under separate pages. Rosh Chodesh can span two days, and on
	// the first of them the date is the 30th of the *previous* month, so the
	// month has to be advanced before deciding which Adar this is.
	//
	// A common year has a single Adar and keeps the undecorated slug, even
	// though hdate numbers that month Adar1.
	if slug == "rosh-chodesh-adar" && hdate.IsLeapYear(ev.Date.Year()) {
		m := ev.Date.Month()
		if ev.Date.Day() == 30 {
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
	if ev.Flags&IL_ONLY != 0 {
		url += "?i=on"
	}
	return url
}

// URL returns the parsha's page on hebcal.com, e.g.
// https://www.hebcal.com/sedrot/bereshit-20261010 . The date is the Saturday
// on which the portion is read, which is what makes the URL unique across the
// triennial and doubled-parsha variations.
func (ev parshaEvent) URL() string {
	gy, gm, gd := ev.Date.ProlepticGreg()
	if !gregYearInRange(gy) {
		return ""
	}
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
func (ev hebrewDateEvent) URL() string {
	hd := ev.Date
	if hd.Year() < 1 {
		return ""
	}
	return fmt.Sprintf("https://www.hebcal.com/converter?hd=%d&hm=%s&hy=%d&h2g=1",
		hd.Day(), hd.MonthName("en"), hd.Year())
}
