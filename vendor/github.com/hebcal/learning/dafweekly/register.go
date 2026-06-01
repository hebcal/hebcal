package dafweekly

import (
	"time"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Daf-a-Week schedule with the dailylearning registry under
// two names: "dafWeekly" (the same daf every day of the week) and
// "dafWeeklySunday" (only on Sundays).
func init() {
	start := hdate.FromRD(DafWeeklyStart)

	dailylearning.AddCalendar("dafWeekly", func(hd hdate.HDate, il bool) event.CalEvent {
		daf, ok := New(hd)
		if !ok {
			return nil
		}
		return NewDafWeeklyEvent(hd, daf)
	}, start)

	dailylearning.AddCalendar("dafWeeklySunday", func(hd hdate.HDate, il bool) event.CalEvent {
		if hd.Weekday() != time.Sunday {
			return nil
		}
		daf, ok := New(hd)
		if !ok {
			return nil
		}
		return NewDafWeeklyEvent(hd, daf)
	}, start)
}
