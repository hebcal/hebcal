package event

import (
	"fmt"
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/molad"
	"github.com/hebcal/locales"
)

// MoladEvent is the announcement of the molad (new moon) of the coming Hebrew
// month, made on the Shabbat before Rosh Chodesh.
type MoladEvent struct {
	date        hdate.HDate // the Shabbat on which the molad is announced
	Molad       molad.Molad // the molad of the coming month
	MonthName   string      // untranslated name of the coming month
	CountryCode string      // used to pick a 12- or 24-hour clock for the announcement
	Clock       ClockFormat // overrides CountryCode unless ClockAuto; see ReformatTimeStr
}

// NewMoladEvent constructs a molad announcement event. With ClockAuto,
// countryCode selects the wall-clock format used in non-Hebrew locales
// (12-hour with am/pm for the United States and other 12-hour countries,
// 24-hour elsewhere); pass "" for the default (12-hour), matching
// @hebcal/core. Clock12 and Clock24 override the country's convention.
func NewMoladEvent(date hdate.HDate, m molad.Molad, monthName string, countryCode string, clock ClockFormat) MoladEvent {
	return MoladEvent{
		date:        date,
		Molad:       m,
		MonthName:   monthName,
		CountryCode: countryCode,
		Clock:       clock,
	}
}

// Date returns the Shabbat on which the molad is announced.
func (ev MoladEvent) Date() hdate.HDate {
	return ev.date
}

var heDayNames = []string{
	"רִאשׁוֹן", "שֵׁנִי", "שְׁלִישִׁי", "רְבִיעִי", "חֲמִישִׁי", "שִׁישִּׁי", "שַׁבָּת",
}

var frDayNames = []string{
	"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi",
}

const night = "בַּלַּ֥יְלָה"
const morning = "בַּבֹּקֶר"
const afternoon = "בַּצׇּהֳרַיִים"
const evening = "בָּעֶרֶב"

func gettext(key, locale string) string {
	s, _ := locales.LookupTranslation(key, locale)
	return s
}

// Render returns the announcement, e.g. "Molad Cheshvan: Sunday, 9:43am and 2
// chalakim", or the Hebrew sentence "מוֹלָד הָלְּבָנָה חֶשְׁוָן יִהְיֶה בַּיּוֹם
// רִאשׁוֹן בשָׁבוּעַ, …".
func (ev MoladEvent) Render(locale string) string {
	return RenderMolad(ev.Molad, ev.MonthName, locale, ev.CountryCode, ev.Clock)
}

// RenderMolad renders the announcement of molad m of the month monthName
// (untranslated) in locale. In non-Hebrew locales the time is 12- or 24-hour
// as countryCode and clock choose (see ReformatTimeStr); the weekday is
// French for "fr" and English otherwise, as in @hebcal/core.
func RenderMolad(m molad.Molad, monthName, locale, countryCode string, clock ClockFormat) string {
	monthStr := gettext(monthName, locale)
	moladStr := gettext("Molad", locale)
	chalakimStr := gettext("chalakim", locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		dow := heDayNames[m.Date.Weekday()]
		var ampm string
		if m.Hours < 5 {
			ampm = night
		} else if m.Hours < 12 {
			ampm = morning
		} else if m.Hours < 17 {
			ampm = afternoon
		} else if m.Hours < 21 {
			ampm = evening
		} else {
			ampm = night
		}
		str := fmt.Sprintf("%s %s יִהְיֶה בַּיּוֹם %s בשָׁבוּעַ, "+
			"בְּשָׁעָה %d %s, ו-%d %s",
			moladStr, monthStr, dow,
			m.Hours, ampm, m.Minutes, gettext("min", locale))
		if m.Chalakim != 0 {
			str += fmt.Sprintf(" ו-%d %s", m.Chalakim, chalakimStr)
		}
		if locale == "he-x-nonikud" {
			str = locales.HebrewStripNikkud(str)
		}
		return str
	}
	dow := m.Date.Weekday().String()
	if locale == "fr" {
		dow = frDayNames[m.Date.Weekday()]
	}
	timeStr := ReformatTimeStr(fmt.Sprintf("%d:%02d", m.Hours, m.Minutes), "pm", countryCode, clock)
	result := fmt.Sprintf("%s %s: %s, %s", moladStr, SmartApostrophe(monthStr), dow, timeStr)
	if m.Chalakim != 0 {
		result += fmt.Sprintf(" %s %d %s", gettext("and", locale), m.Chalakim, chalakimStr)
	}
	return result
}

func (ev MoladEvent) Flags() Flags {
	return Molad
}

func (ev MoladEvent) Emoji() string {
	return ""
}

// Basename returns the untranslated description with the Hebrew year, e.g.
// "Molad Cheshvan 5787".
func (ev MoladEvent) Basename() string {
	return ev.Desc()
}

// Desc returns the untranslated description with the Hebrew year, e.g.
// "Molad Cheshvan 5787".
func (ev MoladEvent) Desc() string {
	return fmt.Sprintf("Molad %s %d", ev.MonthName, ev.date.Year())
}

// URL returns "": a molad has no page on hebcal.com.
func (ev MoladEvent) URL() string {
	return ""
}

func (ev MoladEvent) Categories() []string {
	return CategoriesFromFlags(ev.Flags())
}
