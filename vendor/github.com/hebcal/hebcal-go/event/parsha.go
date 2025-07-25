package event

import (
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/hebcal-go/sedra"
)

// Represents one of 54 weekly Torah portions, always on a Saturday
type parshaEvent struct {
	Date   hdate.HDate
	Parsha sedra.Parsha
	IL     bool
}

func NewParshaEvent(hd hdate.HDate, parsha sedra.Parsha, il bool) CalEvent {
	return parshaEvent{Date: hd, Parsha: parsha, IL: il}
}

func (ev parshaEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev parshaEvent) Render(locale string) string {
	locale = strings.ToLower(locale)
	prefix, _ := locales.LookupTranslation("Parashat", locale)
	name, _ := locales.LookupTranslation(ev.Parsha.Name[0], locale)
	if len(ev.Parsha.Name) == 2 {
		p2, _ := locales.LookupTranslation(ev.Parsha.Name[1], locale)
		delim := "-"
		if locale == "he" || locale == "he-x-nonikud" {
			delim = "־"
		}
		name = name + delim + p2
	}
	return prefix + " " + name
}

func (ev parshaEvent) GetFlags() HolidayFlags {
	return PARSHA_HASHAVUA
}

func (ev parshaEvent) GetEmoji() string {
	return ""
}

func (ev parshaEvent) Basename() string {
	return strings.Join(ev.Parsha.Name, "-")
}
