package psalms

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Daily Psalms schedule with the dailylearning registry under
// the name "psalms". Unlike most schedules it has no cycle start date —
// every Hebrew date has a reading.
func init() {
	dailylearning.AddCalendar("psalms", func(hd hdate.HDate, il bool) event.CalEvent {
		return NewPsalmsEvent(hd, New(hd))
	})
}
