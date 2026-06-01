package perekyomi

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Perek Yomi schedule with the dailylearning registry under
// the name "perekYomi".
func init() {
	dailylearning.AddCalendar("perekYomi", func(hd hdate.HDate, il bool) event.CalEvent {
		reading, ok := New(hd)
		if !ok {
			return nil
		}
		return NewPerekYomiEvent(hd, reading)
	}, hdate.FromRD(PerekYomiStart))
}
