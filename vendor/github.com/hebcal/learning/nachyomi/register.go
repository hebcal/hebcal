package nachyomi

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Nach Yomi schedule with the dailylearning registry under
// the name "nachYomi". The cycle index is built once at init time.
func init() {
	idx := MakeIndex()
	start := hdate.FromRD(NachYomiStart)
	dailylearning.AddCalendar("nachYomi", func(hd hdate.HDate, il bool) event.CalEvent {
		chapter, err := idx.Lookup(hd)
		if err != nil {
			return nil
		}
		return NewNachYomiEvent(hd, chapter)
	}, start)
}
