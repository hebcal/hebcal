package event

import (
	"github.com/hebcal/hdate"
)

// UserEvent is for Yahrzeit or Birthday
type UserEvent struct {
	Date          hdate.HDate  // Date of occurrence
	Desc          string       // Description
}

func (ev UserEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev UserEvent) Render(locale string) string {
	return ev.Desc
}

func (ev UserEvent) GetFlags() HolidayFlags {
	return USER_EVENT
}

func (ev UserEvent) GetEmoji() string {
	return ""
}

func (ev UserEvent) Basename() string {
	return ev.Desc
}
