package event

import (
	"fmt"
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/molad"
	"github.com/hebcal/locales"
)

type moladEvent struct {
	Date        hdate.HDate
	Molad       molad.Molad
	MonthName   string
	CountryCode string // used to pick a 12- or 24-hour clock for the announcement
}

// NewMoladEvent constructs a molad announcement event. countryCode selects the
// wall-clock format used in non-Hebrew locales (12-hour with am/pm for the
// United States and other 12-hour countries, 24-hour elsewhere); pass "" for
// the default (12-hour), matching @hebcal/core.
func NewMoladEvent(date hdate.HDate, m molad.Molad, monthName string, countryCode string) CalEvent {
	return moladEvent{
		Date:        date,
		Molad:       m,
		MonthName:   monthName,
		CountryCode: countryCode,
	}
}

// hour12Countries lists the ISO country codes that use a 12-hour clock, matching
// the hour12cc table in @hebcal/core reformatTimeStr.
var hour12Countries = map[string]bool{
	"US": true, "CA": true, "BR": true, "AU": true, "NZ": true, "DO": true,
	"PR": true, "GR": true, "IN": true, "KR": true, "NP": true, "ZA": true,
}

// moladTimeStr formats the molad's wall-clock time, matching @hebcal/core
// reformatTimeStr(`${hour}:${pad2(min)}`, "pm", options): 12-hour with an
// am/pm suffix in 12-hour countries, 24-hour otherwise.
func moladTimeStr(hour, minute int, cc string) string {
	if cc == "" {
		cc = "US"
	}
	if !hour12Countries[cc] {
		return fmt.Sprintf("%d:%02d", hour, minute)
	}
	suffix := "am"
	h := hour
	if hour >= 12 {
		suffix = "pm"
		if hour > 12 {
			h = hour - 12
		}
	} else if hour == 0 {
		h = 12
	}
	return fmt.Sprintf("%d:%02d%s", h, minute, suffix)
}

// smartApostrophe converts a straight apostrophe to a typographic one (e.g.
// "Sh'vat" => "Sh’vat"), matching @hebcal/core.
func smartApostrophe(s string) string {
	return strings.ReplaceAll(s, "'", "’")
}

func (ev moladEvent) GetDate() hdate.HDate {
	return ev.Date
}

var heDayNames = []string{
	"רִאשׁוֹן", "שֵׁנִי", "שְׁלִישִׁי", "רְבִיעִי", "חֲמִישִׁי", "שִׁישִּׁי", "שַׁבָּת",
}

const night = "בַּלַּ֥יְלָה"
const morning = "בַּבֹּקֶר"
const afternoon = "בַּצׇּהֳרַיִים"
const evening = "בָּעֶרֶב"

func (ev moladEvent) Render(locale string) string {
	monthStr, _ := locales.LookupTranslation(ev.MonthName, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		dow := heDayNames[ev.Molad.Date.Weekday()]
		var ampm string
		if ev.Molad.Hours < 5 {
			ampm = night
		} else if ev.Molad.Hours < 12 {
			ampm = morning
		} else if ev.Molad.Hours < 17 {
			ampm = afternoon
		} else if ev.Molad.Hours < 21 {
			ampm = evening
		} else {
			ampm = night
		}
		str := fmt.Sprintf("מוֹלָד הָלְּבָנָה %s יִהְיֶה בַּיּוֹם %s בשָׁבוּעַ, "+
			"בְּשָׁעָה %d %s, ו-%d דַּקּוֹת",
			monthStr, dow,
			ev.Molad.Hours, ampm, ev.Molad.Minutes)
		if ev.Molad.Chalakim != 0 {
			str += fmt.Sprintf(" ו-%d חֲלָקִים", ev.Molad.Chalakim)
		}
		if locale == "he-x-nonikud" {
			str = locales.HebrewStripNikkud(str)
		}
		return str
	}
	month := smartApostrophe(monthStr)
	dow := ev.Molad.Date.Weekday().String()
	timeStr := moladTimeStr(ev.Molad.Hours, ev.Molad.Minutes, ev.CountryCode)
	result := fmt.Sprintf("Molad %s: %s, %s", month, dow, timeStr)
	if ev.Molad.Chalakim != 0 {
		result += fmt.Sprintf(" and %d chalakim", ev.Molad.Chalakim)
	}
	return result
}

func (ev moladEvent) GetFlags() HolidayFlags {
	return MOLAD
}

func (ev moladEvent) GetEmoji() string {
	return ""
}

func (ev moladEvent) Basename() string {
	return "Molad " + ev.MonthName
}

func (ev moladEvent) GetCategories() []string {
	return CategoriesFromFlags(ev.GetFlags())
}
