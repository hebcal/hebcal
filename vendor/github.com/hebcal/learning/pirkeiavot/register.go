package pirkeiavot

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Pirkei Avot summer schedule with the dailylearning registry
// under the name "pirkeiAvotSummer". It is purely computed from the Hebrew
// calendar with no fixed start date, so no startDate is supplied.
func init() {
	dailylearning.AddCalendar("pirkeiAvotSummer", func(hd hdate.HDate, il bool) event.CalEvent {
		chapters, ok := New(hd, il)
		if !ok {
			return nil
		}
		return NewPirkeiAvotSummerEvent(hd, chapters)
	})
}
