package dafyomi

import (
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the Daf Yomi schedule with the dailylearning registry under
// the name "dafYomi". Importing this package (directly or via
// github.com/hebcal/learning) makes the schedule available to
// hebcal-go's calendar generation.
func init() {
	start := hdate.FromRD(greg.ToRD(1923, time.September, 11))
	dailylearning.AddCalendar("dafYomi", func(hd hdate.HDate, il bool) event.CalEvent {
		daf, err := New(hd)
		if err != nil {
			return nil
		}
		return NewDafYomiEvent(hd, daf)
	}, start)
}
