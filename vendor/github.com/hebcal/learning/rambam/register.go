package rambam

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the two Daily Rambam schedules with the dailylearning registry:
// "rambam1" (1 chapter a day) and "rambam3" (3 chapters a day). Both cycles
// began on 29 April 1984.
func init() {
	start := hdate.FromRD(Rambam1Start)

	dailylearning.AddCalendar("rambam1", func(hd hdate.HDate, il bool) event.CalEvent {
		reading, ok := New1(hd)
		if !ok {
			return nil
		}
		return NewDailyRambam1Event(hd, reading)
	}, start)

	dailylearning.AddCalendar("rambam3", func(hd hdate.HDate, il bool) event.CalEvent {
		readings, ok := New3(hd)
		if !ok {
			return nil
		}
		return NewDailyRambam3Event(hd, readings)
	}, start)
}
