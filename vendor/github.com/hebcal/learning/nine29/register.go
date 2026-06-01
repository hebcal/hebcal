package nine29

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the 929 schedule with the dailylearning registry under the
// name "929".
func init() {
	dailylearning.AddCalendar("929", func(hd hdate.HDate, il bool) event.CalEvent {
		reading, ok := New(hd)
		if !ok {
			return nil
		}
		return NewNine29Event(hd, reading)
	}, hdate.FromRD(Nine29Start))
}
