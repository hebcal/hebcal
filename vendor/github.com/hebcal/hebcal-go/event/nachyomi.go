package event

import (
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dafyomi"
	"github.com/hebcal/hebcal-go/locales"
)

type nachYomiEvent struct {
	Date    hdate.HDate
	Chapter dafyomi.Daf
}

func NewNachYomiEvent(hd hdate.HDate, chapter dafyomi.Daf) CalEvent {
	return nachYomiEvent{Date: hd, Chapter: chapter}
}

func (ev nachYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev nachYomiEvent) Render(locale string) string {
	name, _ := locales.LookupTranslation(ev.Chapter.Name, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		return name + " " + gematriya.Gematriya(ev.Chapter.Blatt)
	}
	return name + " " + strconv.Itoa(ev.Chapter.Blatt)
}

func (ev nachYomiEvent) GetFlags() HolidayFlags {
	return NACH_YOMI
}

func (ev nachYomiEvent) GetEmoji() string {
	return ""
}

func (ev nachYomiEvent) Basename() string {
	return ev.Chapter.String()
}
