package hebcal

import (
	"time"

	"github.com/hebcal/hebcal-go/event"
)

// ReformatTimeStr formats a 24-hour (00:00-23:59) time string such as "20:13"
// either as 12-hour time with a suffix (e.g. "8:13pm") or leaves it in 24-hour
// format, according to opts.
//
// suffix is the afternoon marker, such as "p", "pm" or " P.M."; a morning time
// gets "a", "am" or " A.M." instead.
//
// opts.Clock forces 12-hour time (event.Clock12) or 24-hour time
// (event.Clock24). Otherwise the country of opts.Location decides (see event.Uses12HourClock),
// falling back to Israel when opts.IL is set and to the United States when it
// is not. opts may be nil, which means the United States.
func ReformatTimeStr(timeStr, suffix string, opts *Options) string {
	c := clockFor(opts)
	return event.ReformatTimeStr(timeStr, suffix, c.cc, c.format)
}

// clock is the resolved clock format of a calendar: all that an event keeps
// of the options it was generated with.
type clock struct {
	cc     string            // ISO 3166 country code whose convention applies
	format event.ClockFormat // overrides the country unless ClockAuto
}

// clockFor resolves the clock format opts asks for. A nil opts means the
// United States.
func clockFor(opts *Options) clock {
	if opts == nil {
		return clock{cc: "US"}
	}
	return clock{cc: clockCountry(opts), format: opts.Clock}
}

// formatTime renders t as 24-hour "15:04" or 12-hour "3:04pm".
func (c clock) formatTime(t time.Time) string {
	return event.ReformatTimeStr(t.Format("15:04"), "pm", c.cc, c.format)
}

// clockCountry is the country code whose clock convention applies to opts.
func clockCountry(opts *Options) string {
	if opts.Location != nil && opts.Location.CountryCode != "" {
		return opts.Location.CountryCode
	}
	if opts.IL {
		return "IL"
	}
	return "US"
}
