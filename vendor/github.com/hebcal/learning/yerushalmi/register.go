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
	dailylearning.AddCalendar("yerushalmi-vilna", func(hd hdate.HDate, il bool) event.CalEvent {
		if hd.Abs() < VilnaStartRD {
			return nil
		}
		daf := New(hd, Vilna)
		// daf.Blatt is 0 to signal no Yerushalmi Yomi on Yom Kippur and 9 Av.
		if daf.Blatt == 0 {
			return nil
		}
		return NewYerushalmiYomiEvent(hd, daf)
	}, hdate.FromRD(VilnaStartRD))

	dailylearning.AddCalendar("yerushalmi-schottenstein", func(hd hdate.HDate, il bool) event.CalEvent {
		if hd.Abs() < SchottensteinStartRD {
			return nil
		}
		daf := New(hd, Schottenstein)
		if daf.Blatt == 0 {
			return nil
		}
		return NewYerushalmiYomiEvent(hd, daf)
	}, hdate.FromRD(SchottensteinStartRD))
}
