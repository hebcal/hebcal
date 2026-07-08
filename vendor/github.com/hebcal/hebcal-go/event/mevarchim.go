package event

import (
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/molad"
	"github.com/hebcal/locales"
)

// mevarchimChodeshPrefix is the untranslated title prefix shared by all Shabbat
// Mevarchim events.
const mevarchimChodeshPrefix = "Shabbat Mevarchim Chodesh"

// MevarchimChodeshEvent represents Shabbat Mevarchim HaChodesh, the announcement
// on the Shabbat before Rosh Chodesh of the upcoming Hebrew month. It carries
// the molad (new-moon calculation) of that month, which consumers can render
// as an announcement (the time-of-day format is locale/location dependent, so
// it is intentionally not baked into a single string here).
type MevarchimChodeshEvent struct {
	Date      hdate.HDate // the Shabbat on which the month is blessed
	MonthName string      // untranslated English name of the upcoming month
	Molad     molad.Molad // molad of the upcoming month
}

// NewMevarchimChodeshEvent constructs a MevarchimChodeshEvent for the given
// Shabbat, upcoming (untranslated) month name, and molad.
func NewMevarchimChodeshEvent(date hdate.HDate, monthName string, m molad.Molad) MevarchimChodeshEvent {
	return MevarchimChodeshEvent{Date: date, MonthName: monthName, Molad: m}
}

func (ev MevarchimChodeshEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev MevarchimChodeshEvent) GetFlags() HolidayFlags {
	return SHABBAT_MEVARCHIM
}

func (ev MevarchimChodeshEvent) GetEmoji() string {
	return ""
}

func (ev MevarchimChodeshEvent) GetCategories() []string {
	return []string{"mevarchim"}
}

// Basename returns the untranslated title, e.g. "Shabbat Mevarchim Chodesh Sivan".
func (ev MevarchimChodeshEvent) Basename() string {
	return mevarchimChodeshPrefix + " " + ev.MonthName
}

// Render returns the translated title, e.g. "Shabbat Mevarchim Chodesh Sivan".
func (ev MevarchimChodeshEvent) Render(locale string) string {
	prefix, _ := locales.LookupTranslation(mevarchimChodeshPrefix, locale)
	month, _ := locales.LookupTranslation(ev.MonthName, locale)
	return prefix + " " + month
}

// RenderBrief returns the title without the leading "Shabbat" word, e.g.
// "Mevarchim Chodesh Sivan", matching @hebcal/core MevarchimChodeshEvent.
func (ev MevarchimChodeshEvent) RenderBrief(locale string) string {
	s := ev.Render(locale)
	if i := strings.Index(s, " "); i >= 0 {
		return s[i+1:]
	}
	return s
}
