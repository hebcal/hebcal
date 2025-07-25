package event

import (
	"strconv"
	"strings"

	"github.com/dustin/go-humanize"
	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/locales"
)

type hebrewDateEvent struct {
	Date hdate.HDate
}

func (ev hebrewDateEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev hebrewDateEvent) Render(locale string) string {
	hd := ev.Date
	enMonthName := hd.MonthName("en")
	locale = strings.ToLower(locale)
	switch locale {
	case "he", "he-x-nonikud":
		return gematriya.Gematriya(hd.Day()) + " " + hd.MonthName(locale) + " " + gematriya.Gematriya(hd.Year())
	case "", "en", "sephardic", "ashkenazi",
		"ashkenazi_litvish", "ashkenazi_poylish", "ashkenazi_standard":
		return humanize.Ordinal(hd.Day()) + " of " + enMonthName +
			", " + strconv.Itoa(hd.Year())
	case "es":
		monthName, _ := locales.LookupTranslation(enMonthName, locale)
		return strconv.Itoa(hd.Day()) + "º " + monthName + " " + strconv.Itoa(hd.Year())

	}
	monthName, _ := locales.LookupTranslation(enMonthName, locale)
	return strconv.Itoa(hd.Day()) + " " + monthName + " " + strconv.Itoa(hd.Year())
}

func (ev hebrewDateEvent) GetFlags() HolidayFlags {
	return HEBREW_DATE
}

func (ev hebrewDateEvent) GetEmoji() string {
	return ""
}

func (ev hebrewDateEvent) Basename() string {
	return ev.Date.String()
}

func NewHebrewDateEvent(hd hdate.HDate) CalEvent {
	return hebrewDateEvent{Date: hd}
}
