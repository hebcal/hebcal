package tanakhyomi

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Tanakh Yomi schedule with the dailylearning registry under
// the name "tanakhYomi".
func init() {
	dailylearning.AddCalendar("tanakhYomi", func(hd hdate.HDate, il bool) event.CalEvent {
		reading, ok := New(hd)
		if !ok {
			return nil
		}
		return NewTanakhYomiEvent(hd, reading)
	}, hdate.FromRD(TanakhYomiStart))
}
