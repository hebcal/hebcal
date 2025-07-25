package event

import (
	"fmt"
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/hebcal-go/molad"
)

type moladEvent struct {
	Date      hdate.HDate
	Molad     molad.Molad
	MonthName string
}

func NewMoladEvent(date hdate.HDate, m molad.Molad, monthName string) CalEvent {
	return moladEvent{
		Date:      date,
		Molad:     m,
		MonthName: monthName,
	}
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
			"בְּשָׁעָה %d %s, ו-%d דַּקּוֹת "+"ו-%d חֲלָקִים",
			monthStr, dow,
			ev.Molad.Hours, ampm, ev.Molad.Minutes, ev.Molad.Chalakim)
		if locale == "he-x-nonikud" {
			str = locales.HebrewStripNikkud(str)
		}
		return str
	}
	return fmt.Sprintf("Molad %s: %s, %d minutes and %d chalakim after %d:00",
		monthStr, ev.Molad.Date.Weekday().String()[0:3],
		ev.Molad.Minutes, ev.Molad.Chalakim, ev.Molad.Hours)
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
