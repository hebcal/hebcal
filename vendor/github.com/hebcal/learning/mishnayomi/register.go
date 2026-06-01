package mishnayomi

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Mishna Yomi schedule with the dailylearning registry under
// the name "mishnaYomi". The 6-year index is built once at init time.
func init() {
	idx := MakeIndex()
	start := hdate.FromRD(MishnaYomiStart)
	dailylearning.AddCalendar("mishnaYomi", func(hd hdate.HDate, il bool) event.CalEvent {
		pair, err := idx.Lookup(hd)
		if err != nil {
			return nil
		}
		return NewMishnaYomiEvent(hd, pair)
	}, start)
}
