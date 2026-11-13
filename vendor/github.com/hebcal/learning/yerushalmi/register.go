package yerushalmi

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
)

// Register the two Yerushalmi Yomi (Jerusalem Talmud) schedules with the
// dailylearning registry: "yerushalmi-vilna" (the classic Vilna Edition,
// which skips Yom Kippur and Tisha B'Av) and "yerushalmi-schottenstein".
func init() {
	dailylearning.AddCalendarFrom("yerushalmi-vilna", func(hd hdate.HDate, il bool) event.CalEvent {
		daf, err := New(hd, Vilna)
		// daf.Blatt is 0 to signal no Yerushalmi Yomi on Yom Kippur and 9 Av.
		if err != nil || daf.Blatt == 0 {
			return nil
		}
		return NewYerushalmiYomiEvent(hd, daf, Vilna)
	}, hdate.FromRD(VilnaStartRD))

	dailylearning.AddCalendarFrom("yerushalmi-schottenstein", func(hd hdate.HDate, il bool) event.CalEvent {
		daf, err := New(hd, Schottenstein)
		if err != nil || daf.Blatt == 0 {
			return nil
		}
		return NewYerushalmiYomiEvent(hd, daf, Schottenstein)
	}, hdate.FromRD(SchottensteinStartRD))
}
