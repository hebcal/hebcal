package hebcal

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original C version, Copyright (C) 1994-2004 Danny Sadinoff
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

import (
	"math"
	"sort"
	"strings"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/molad"
	"github.com/hebcal/hebcal-go/omer"
	"github.com/hebcal/hebcal-go/sedra"
	"github.com/hebcal/hebcal-go/zmanim"
)

/*
Calculates holidays and other Hebrew calendar events based on Options.

Each holiday is represented by event.CalEvent object which includes a date,
a description, flags and optional attributes.
If given no options, returns holidays for the Diaspora for the current Gregorian year.

HebrewCalendar does not modify opts, and the events it returns keep no
reference to it, so one Options value may be shared by concurrent calls and
changed freely afterwards. A nil opts is the same as a zero Options.

The date range returned by this function can be controlled by:
  - opts.Year - Gregorian (e.g. 1993) or Hebrew year (e.g. 5749)
  - opts.IsHebrewYear - to interpret year as Hebrew year
  - opts.NumYears - generate calendar for multiple years (default 1)

Alternatively, specify start and end days HDate instances:
  - opts.Start - use specific start date (requires end date)
  - opts.End - use specific end date (requires start date)

Unless opts.NoHolidays == true, default holidays include:
  - Major holidays - Rosh Hashana, Yom Kippur, Pesach, Sukkot, etc.
  - Minor holidays - Purim, Chanukah, Tu BiShvat, Lag BaOmer, etc.
  - Minor fasts - Ta'anit Esther, Tzom Gedaliah, etc. (unless opts.NoMinorFast)
  - Special Shabbatot - Shabbat Shekalim, Zachor, etc. (unless opts.NoSpecialShabbat)
  - Modern Holidays - Yom HaShoah, Yom HaAtzma'ut, etc. (unless opts.NoModern)
  - Rosh Chodesh (unless opts.NoRoshChodesh)

Holiday and Torah reading schedules differ between Israel and the Disapora.
Set opts.IL=true to use the Israeli schedule.

Additional non-default event types can be specified:
  - Parashat HaShavua - weekly Torah Reading on Saturdays (opts.Sedrot)
  - Counting of the Omer (opts.Omer)
  - Babylonian Talmud Daf Yomi (opts.DafYomi)
  - Jerusalem Talmud (Yerushalmi) Yomi (opts.YerushalmiYomi)
  - Mishna Yomi (opts.MishnaYomi)
  - Nach Yomi (opts.NachYomi)
  - Shabbat Mevarchim HaChodesh on Saturday before Rosh Chodesh (opts.ShabbatMevarchim)
  - Molad announcement on Saturday before Rosh Chodesh (opts.Molad)
  - Yom Kippur Katan (opts.YomKippurKatan)
  - BeHaB fast days (opts.BeHaB)
  - Yizkor (opts.Yizkor; see YizkorEvent)
  - Latest time for Kiddush Levana, according to the Maharil
    (opts.KiddushLevanaMaharil, requires opts.Location; see KiddushLevanaEvent)

Candle-lighting and Havdalah times are approximated using latitude and longitude
specified by the Location class. The Location class contains a small
database of cities with their associated geographic information and time-zone information.
If you ever have any doubts about Hebcal's times, consult your local halachic authority.
If you enter geographic coordinates above the arctic circle or antarctic circle,
the times are guaranteed to be wrong.

To add candle-lighting options, set opts.CandleLighting=true and set
opts.Location to an instance of Location. By default, candle lighting
time is 18 minutes before sundown (40 minutes for Jerusalem) and Havdalah is
calculated according to Tzeit Hakochavim - Nightfall (the point when 3 small stars
are observable in the night time sky with the naked eye). The default Havdalah
option (Tzeit Hakochavim) is calculated when the sun is 8.5° below the horizon.
These defaults can be changed using these options:
  - opts.CandleLightingMins - minutes before sundown to light candles
  - opts.CandleLightingAtSunset - light candles at sundown itself
  - opts.HavdalahMins - minutes after sundown for Havdalah (typical values are 42, 50, or 72).
    Havdalah times are supressed when opts.HavdalahMins=0.
  - opts.HavdalahDeg - degrees for solar depression for Havdalah.
    Default is 8.5 degrees for 3 small stars. Use 7.083 degress for 3 medium-sized stars.
    Havdalah times are supressed when opts.HavdalahDeg=0.
  - opts.UseElevation - use the location's elevation for sunrise/sunset-based times
    (candle-lighting, Havdalah, fast start/end, and the sunset-based daily zmanim).
    Degree-based zmanim (dawn, tzeit, alot, misheyakir) are never affected by elevation.

If both opts.CandleLighting=true and opts.Location is specified,
Chanukah candle-lighting times and minor fast start/end times will also be generated.
Chanukah candle-lighting is at dusk (when the sun is 6.0° below the horizon in the evening)
on weekdays, at regular candle-lighting time on Fridays, and at regular Havdalah time on
Saturday night (see above).

Minor fasts begin at Alot HaShachar (sun is 16.1° below the horizon in the morning) and
end when 3 medium-sized stars are observable in the night sky (sun is 7.083° below the horizon
in the evening), or in Israel 15 minutes after sunset. Tish'a B'Av begins at sunset and
ends when the sun is 6.45° below the horizon. These defaults can be changed using these
options, each pair mutually exclusive:
  - opts.FastStartDeg / opts.FastStartMins - start of minor fasts, in degrees of solar
    depression or minutes before sunrise
  - opts.FastEndDeg / opts.FastEndMins - end of minor fasts, in degrees of solar
    depression or minutes after sunset
  - opts.TishaBavEndDeg / opts.TishaBavEndMins - end of Tish'a B'Av, likewise

Two options also exist for generating an Event with the Hebrew date:
  - opts.AddHebrewDates - print the Hebrew date for the entire date range
  - opts.AddHebrewDatesForEvents - print the Hebrew date for dates with some events
*/
func HebrewCalendar(options *Options) ([]event.CalEvent, error) {
	opts, startAbs, endAbs, err := newCalConfig(options)
	if err != nil {
		return nil, err
	}
	yerushalmiCalendar := opts.yerushalmiCalendar
	var (
		il           = opts.IL
		currentYear  = -1
		holidaysYear []event.HolidayEvent
		sedraYear    sedra.Sedra
		beginOmer    int64
		endOmer      int64
		userEvents   []event.UserEvent
		klMonthKey   = -1
		klEv         KiddushLevanaEvent
		klOk         bool
	)
	firstWeekday := time.Weekday(startAbs % 7)
	events := make([]event.CalEvent, 0, 20)
	for abs := startAbs; abs <= endAbs; abs++ {
		hd := hdate.FromRD(abs)
		hyear := hd.Year()
		if hyear != currentYear {
			currentYear = hyear
			holidaysYear = HolidaysForYear(hyear, il)
			if opts.Sedrot || opts.DailySedra {
				sedraYear = sedra.New(hyear, il)
			}
			if opts.Omer {
				beginOmer = hdate.ToRD(hyear, hdate.Nisan, 16)
				endOmer = hdate.ToRD(hyear, hdate.Sivan, 5)
			}
			numUserEvents := len(opts.Yahrzeits) + len(opts.UserEvents)
			if numUserEvents != 0 {
				userEvents = make([]event.UserEvent, 0, numUserEvents)
				for _, yahrzeit := range opts.Yahrzeits {
					origDate := hdate.FromTime(yahrzeit.Date)
					observedDate, err := hdate.GetYahrzeit(currentYear, origDate)
					if err == nil {
						userEvents = append(userEvents, event.NewUserEvent(
							observedDate,
							yahrzeit.Name))
					}
				}
				for _, userEv := range opts.UserEvents {
					// Watch for ShortKislev and LongCheshvan
					if userEv.Day <= hdate.DaysInMonth(userEv.Month, hyear) {
						userEvents = append(userEvents, event.NewUserEvent(
							hdate.New(hyear, userEv.Month, userEv.Day),
							userEv.Desc))
					}
				}
			}
		}
		dow := hd.Weekday()
		prevEventsLength := len(events)
		sortByTime := false
		if opts.SunriseSunset && (!opts.WeeklyAbbreviated || dow == firstWeekday) {
			events = append(events, newRiseSetEvent(hd, opts))
		}
		if opts.DailySedra || (opts.Sedrot && dow == time.Saturday) {
			parsha := sedraYear.LookupByRD(abs)
			if !parsha.Chag {
				events = append(events, event.NewParshaEvent(hd, parsha, il))
			}
		}
		var candlesEv TimedEvent
		if opts.Molad && dow == time.Saturday && hd.Month() != hdate.Elul && hd.Day() >= 23 && hd.Day() <= 29 {
			nextMonthName, nextMonth := nextMonthName(hd.Year(), hd.Month())
			molad := molad.New(hd.Year(), nextMonth)
			events = append(events, event.NewMoladEvent(hd, molad, nextMonthName,
				opts.clock.cc, opts.clock.format))
		}
		for _, holidayEv := range holidaysYear {
			if hd == holidayEv.Date() {
				events, candlesEv = appendHolidayAndRelated(events, candlesEv, holidayEv, opts)
			}
		}
		// When Erev Pesach falls on Shabbat, burning chametz is moved to the
		// Friday before (13 Nisan), since chametz cannot be burned on Shabbat.
		if opts.CandleLighting && dow == time.Friday && hd.Month() == hdate.Nisan && hd.Day() == 13 {
			if biurEv := makeBiurChametz(hd, opts); (biurEv != TimedEvent{}) {
				events = append(events, biurEv)
			}
		}
		if opts.Yizkor && isYizkorDay(hd, il) {
			events = append(events, newYizkorEvent(hd, holidaysYear))
		}
		for _, userEv := range userEvents {
			if abs == userEv.Date().Abs() {
				events = append(events, userEv)
			}
		}
		numDailyLearning := 0
		if !opts.WeeklyAbbreviated || dow == firstWeekday {
			if opts.Omer && abs >= beginOmer && abs <= endOmer {
				omerDay := int(abs - beginOmer + 1)
				// omerDay is between 1 and 49 by the bounds just tested
				events = append(events, omer.MustNewOmerEvent(hd, omerDay))
			}
			// Daily learning schedules (Daf Yomi, Mishna Yomi, Yerushalmi
			// Yomi) are supplied by schedule providers that register
			// themselves with the dailylearning package (e.g. by importing
			// github.com/hebcal/learning). When no provider is registered,
			// these lookups return nil and the events are simply omitted.
			if opts.DafYomi {
				if ev := dailylearning.Lookup("dafYomi", hd, il); ev != nil {
					events = append(events, ev)
					numDailyLearning++
				}
			}
			if opts.YerushalmiYomi {
				if ev := dailylearning.Lookup(yerushalmiCalendar, hd, il); ev != nil {
					events = append(events, ev)
					numDailyLearning++
				}
			}
			if opts.MishnaYomi {
				if ev := dailylearning.Lookup("mishnaYomi", hd, il); ev != nil {
					events = append(events, ev)
					numDailyLearning++
				}
			}
			if opts.NachYomi {
				if ev := dailylearning.Lookup("nachYomi", hd, il); ev != nil {
					events = append(events, ev)
					numDailyLearning++
				}
			}
			// Any additional daily learning schedules requested by name
			// (e.g. "929", "rambam1"). Names are case-insensitive and
			// resolved through the dailylearning registry.
			for _, name := range opts.DailyLearning {
				if ev := dailylearning.Lookup(name, hd, il); ev != nil {
					events = append(events, ev)
					numDailyLearning++
				}
			}
			if opts.DailyZmanim {
				zmanEvents := dailyZemanim(hd, opts)
				events = append(events, zmanEvents...)
				sortByTime = true
			}
		}
		if (candlesEv == TimedEvent{}) && opts.CandleLighting && (dow == time.Friday || dow == time.Saturday) {
			candlesEv = makeCandleEvent(hd, opts, nil)
			// Link erev-Shabbat candle-lighting to this week's parsha (matching
			// @hebcal/core), so consumers can render "Parashat X" as its memo.
			if dow == time.Friday && opts.Sedrot && (candlesEv != TimedEvent{}) {
				if parsha := sedraYear.LookupByRD(abs); !parsha.Chag {
					candlesEv.linked = event.NewParshaEvent(hd.OnOrAfter(time.Saturday), parsha, il)
				} else {
					candlesEv.chagReading = parsha.Name[0]
				}
			}
		}
		// Suppress Havdalah when asked, matching @hebcal/core's behavior for
		// havdalahMins===0 (which drops any HavdalahEvent). Candle-lighting on
		// Fridays and yom-tov evenings is unaffected; only the "Havdalah"-titled
		// events (Saturday night and the end of yom tov) are dropped.
		if opts.SuppressHavdalah && candlesEv.kind == Havdalah {
			candlesEv = TimedEvent{}
		}
		if (candlesEv != TimedEvent{}) {
			events = append(events, candlesEv)
		}
		if opts.KiddushLevanaMaharil && hd.Day() >= 9 && hd.Day() <= 17 {
			// The event usually falls between the 11th and the 16th, but may be
			// moved back to candle-lighting before a Shabbat or Yom Tov
			monthKey := hyear*100 + int(hd.Month())
			if monthKey != klMonthKey {
				klMonthKey = monthKey
				klEv, klOk = makeKiddushLevanaEvent(hyear, hd.Month(), opts)
			}
			if klOk && klEv.Date() == hd {
				events = append(events, klEv)
			}
		}
		sortByDayPart(events[prevEventsLength:])
		if sortByTime {
			sortTimedEvents(events[prevEventsLength:])
		}
		if (opts.AddHebrewDates && (!opts.WeeklyAbbreviated || dow == firstWeekday)) ||
			(opts.AddHebrewDates && prevEventsLength != len(events)) ||
			// Every day has a daily-learning reading, so a day whose only
			// events are learning does not count as a day with events.
			(opts.AddHebrewDatesForEvents && prevEventsLength != len(events)-numDailyLearning) {
			events = append(events, nil)
			copy(events[prevEventsLength+1:], events[prevEventsLength:])
			events[prevEventsLength] = event.NewHebrewDateEvent(hd)
		}
	}
	return events, nil
}

// hebrewEpoch is the R.D. day of 1 Tishrei of Hebrew year 1, the first day
// that has a Hebrew date.
var hebrewEpoch = hdate.ToRD(1, hdate.Tishrei, 1)

// calConfig is what HebrewCalendar works from: a private copy of the caller's
// Options, validated and normalized (defaults filled in, the Israel
// schedule implied by an Israeli location, the event mask resolved), plus
// the values derived from it. The caller's Options is never written to.
type calConfig struct {
	Options
	// clock is the clock format events render their times in
	clock clock
	// yerushalmiCalendar is the registered name of the Yerushalmi Yomi edition
	yerushalmiCalendar string
}

// newCalConfig copies and normalizes options, and returns the range of R.D.
// days the calendar covers.
func newCalConfig(options *Options) (cfg *calConfig, startAbs, endAbs int64, err error) {
	cfg = &calConfig{}
	if options != nil {
		cfg.Options = *options
	}
	opts := &cfg.Options
	// before checkCandleOptions, because the mask can enable this option
	if opts.Mask.Has(event.KiddushLevana) {
		opts.KiddushLevanaMaharil = true
	}
	if err = checkCandleOptions(opts); err != nil {
		return nil, 0, 0, err
	}
	if opts.SunriseSunset && opts.Location == nil {
		return nil, 0, 0, ErrSunriseSunsetLocation
	}
	if opts.DailyZmanim && opts.Location == nil {
		return nil, 0, 0, ErrDailyZmanimLocation
	}
	startAbs, endAbs, err = getStartAndEnd(opts)
	if err != nil {
		return nil, 0, 0, err
	}
	if startAbs < hebrewEpoch {
		return nil, 0, 0, ErrBeforeHebrewEpoch
	}
	if opts.CandleLighting || opts.KiddushLevanaMaharil || opts.SunriseSunset || opts.DailyZmanim {
		// The location is about to be used for times: check it once, here,
		// rather than fail on the first day of the calendar.
		if _, err = zmanim.New(opts.Location, time.Time{}); err != nil {
			return nil, 0, 0, err
		}
	}
	if opts.Location != nil && opts.Location.CountryCode == "IL" {
		opts.IL = true
	}
	opts.Mask = getMaskFromOptions(opts)
	if opts.YerushalmiYomi && opts.YerushalmiEdition == 0 {
		opts.YerushalmiEdition = Vilna
	}
	cfg.yerushalmiCalendar = "yerushalmi-vilna"
	if opts.YerushalmiEdition == Schottenstein {
		cfg.yerushalmiCalendar = "yerushalmi-schottenstein"
	}
	cfg.clock = clockFor(opts)
	return cfg, startAbs, endAbs, nil
}

// getStartAndEnd returns the first and last R.D. day of the calendar. It
// turns opts.CandleLighting off for years too early for candle-lighting
// times; opts is HebrewCalendar's private copy.
func getStartAndEnd(opts *Options) (int64, int64, error) {
	hasStart := opts.Start != hdate.HDate{}
	hasEnd := opts.End != hdate.HDate{}
	if hasStart != hasEnd {
		return 0, 0, ErrStartRequiresEnd
	}
	if hasStart && hasEnd {
		return opts.Start.Abs(), opts.End.Abs(), nil
	}
	year := opts.Year
	if year == 0 {
		t := time.Now()
		gy, gm, gd := t.Date()
		if opts.IsHebrewYear {
			today := hdate.FromGregorian(gy, gm, gd)
			year = today.Year()
		} else {
			year = gy
		}
	} else if opts.IsHebrewYear && year < 1 {
		return 0, 0, ErrInvalidHebrewYear
	}
	numYears := opts.NumYears
	if numYears == 0 {
		numYears = 1
	}
	if opts.IsHebrewYear {
		// disable candle-lighting times for very early dates
		if year < 5514 {
			opts.CandleLighting = false
		}
		startDate := hdate.New(year, hdate.Tishrei, 1)
		// for full Hebrew year, start on Erev Rosh Hashana which
		// is technically in the previous Hebrew year
		// (but conveniently lets us get candle-lighting time for Erev)
		startAbs := startDate.Abs()
		if year > 1 {
			startAbs--
		}
		endDate := hdate.New(year+numYears, hdate.Tishrei, 1)
		return startAbs, endDate.Abs() - 1, nil
	}
	// disable candle-lighting times for very early dates
	if year < 1753 {
		opts.CandleLighting = false
	}
	if opts.Month < 0 || opts.Month > time.December {
		return 0, 0, ErrInvalidMonth
	}
	month := time.January
	if opts.Month != 0 {
		month = opts.Month
	}
	var startAbs int64
	if opts.NoJulian {
		startAbs = greg.ProlepticToRD(year, month, 1)
	} else {
		startAbs = greg.ToRD(year, month, 1)
	}
	if opts.Month != 0 {
		endAbs := startAbs + int64(greg.DaysIn(opts.Month, year))
		return startAbs, endAbs - 1, nil
	}
	var endAbs int64
	if opts.NoJulian {
		endAbs = greg.ProlepticToRD(year+numYears, time.January, 1)
	} else {
		endYear := year + numYears
		// historical Gregorian calendar has no year 0; 1 BC (-1) is
		// followed directly by 1 AD (+1), so skip over 0
		if year < 0 && endYear >= 0 {
			endYear++
		}
		endAbs = greg.ToRD(endYear, time.January, 1)
	}
	return startAbs, endAbs - 1, nil
}

func intAbs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

var israelCityOffset = map[string]int{
	"Jerusalem":       40,
	"Haifa":           30,
	"Zichron Ya'akov": 30,
	"Zichron Ya‘akov": 30,
	"Zichron Ya’akov": 30,
	"Zichron Yaakov":  30,
	"Zikhron Ya'akov": 30,
	"Zikhron Ya'aqov": 30,
	"Zikhron Ya‘akov": 30,
	"Zikhron Ya‘aqov": 30,
	"Zikhron Ya’akov": 30,
	"Zikhron Ya’aqov": 30,
	"Zikhron Yaakov":  30,
}

// geoNameIDCandleOffset is the customary candle-lighting offset of the Israeli
// cities that light earlier than the usual 20 minutes, by GeoNames id.
var geoNameIDCandleOffset = map[int]int{
	281184: 40, // Jerusalem
	294801: 30, // Haifa
	293067: 30, // Zikhron Yaakov
}

// IsraelCandleLightingMins returns the customary candle-lighting offset, in
// minutes before sunset, of a location in Israel: 40 for Jerusalem, 30 for
// Haifa and Zikhron Ya'akov, otherwise 20. The city is recognized by its
// GeoNames id, or else by its name up to the first comma ("Jerusalem,
// Israel" is Jerusalem). HebrewCalendar applies it when an Israeli location
// asks for the default 18 minutes.
func IsraelCandleLightingMins(loc *zmanim.Location) int {
	if offset, ok := geoNameIDCandleOffset[loc.GeoNameID]; ok {
		return offset
	}
	name, _, _ := strings.Cut(loc.Name, ",")
	if offset, ok := israelCityOffset[strings.TrimSpace(name)]; ok {
		return offset
	}
	return 20
}

func checkCandleOptions(opts *Options) error {
	if !opts.CandleLighting && !opts.KiddushLevanaMaharil {
		return nil
	}
	if opts.Location == nil {
		if opts.CandleLighting {
			return ErrCandleLightingLocation
		}
		return ErrKiddushLevanaLocation
	}
	// Kiddush Levana needs candle-lighting time for Shabbat and Yom Tov
	setCandleLightingMins(opts)
	if !opts.CandleLighting {
		// Havdalah and fast options only affect candle-lighting events
		return nil
	}
	if opts.HavdalahMins != 0 && opts.HavdalahDeg != 0.0 {
		return ErrHavdalahMinsAndDeg
	}
	if opts.FastStartMins != 0 && opts.FastStartDeg != 0.0 {
		return ErrFastStartMinsAndDeg
	}
	if opts.FastEndMins != 0 && opts.FastEndDeg != 0.0 {
		return ErrFastEndMinsAndDeg
	}
	if opts.TishaBavEndMins != 0 && opts.TishaBavEndDeg != 0.0 {
		return ErrTishaBavEndMinsAndDeg
	}
	if opts.HavdalahMins != 0 {
		opts.HavdalahMins = intAbs(opts.HavdalahMins)
	} else if opts.HavdalahDeg != 0.0 {
		opts.HavdalahDeg = math.Abs(opts.HavdalahDeg)
	} else {
		opts.HavdalahDeg = zmanim.Tzeit3SmallStars
	}
	opts.FastStartMins = intAbs(opts.FastStartMins)
	opts.FastStartDeg = math.Abs(opts.FastStartDeg)
	opts.FastEndMins = intAbs(opts.FastEndMins)
	opts.FastEndDeg = math.Abs(opts.FastEndDeg)
	opts.TishaBavEndMins = intAbs(opts.TishaBavEndMins)
	opts.TishaBavEndDeg = math.Abs(opts.TishaBavEndDeg)
	return nil
}

// setCandleLightingMins normalizes opts.CandleLightingMins to a negative
// offset from sunset, applying the default and Israeli city customs.
func setCandleLightingMins(opts *Options) {
	if opts.CandleLightingAtSunset {
		opts.CandleLightingMins = 0
		return
	}
	mins := 18
	if opts.CandleLightingMins != 0 {
		mins = opts.CandleLightingMins
	}
	loc := opts.Location
	if loc.CountryCode == "IL" && intAbs(mins) == 18 {
		mins = IsraelCandleLightingMins(loc)
	}
	opts.CandleLightingMins = -1 * intAbs(mins)
}

const maskLightCandles = event.LightCandles |
	event.LightCandlesTzeis |
	event.ChanukahCandles |
	event.YomTovEnds

func getMaskFromOptions(opts *Options) event.Flags {
	if opts.Mask != 0 {
		m := opts.Mask
		if m.Has(event.RoshChodesh) {
			opts.NoRoshChodesh = false
		}
		if m.Has(event.ModernHoliday) {
			opts.NoModern = false
		}
		if m.Has(event.MinorFast) {
			opts.NoMinorFast = false
		}
		if m.Has(event.SpecialShabbat) {
			opts.NoSpecialShabbat = false
		}
		if m.Has(event.ParshaHashavua) {
			opts.Sedrot = true
		}
		if m.Has(event.DafYomi) {
			opts.DafYomi = true
		}
		if m.Has(event.OmerCount) {
			opts.Omer = true
		}
		if m.Has(event.ShabbatMevarchim) {
			opts.ShabbatMevarchim = true
		}
		if m.Has(event.MishnaYomi) {
			opts.MishnaYomi = true
		}
		if m.Has(event.NachYomi) {
			opts.NachYomi = true
		}
		if m.Has(event.YomKippurKatan) {
			opts.YomKippurKatan = true
		}
		if m.Has(event.BeHaB) {
			opts.BeHaB = true
		}
		if m.Has(event.YerushalmiYomi) {
			opts.YerushalmiYomi = true
		}
		if m.Has(event.KiddushLevana) {
			opts.KiddushLevanaMaharil = true
		}
		if m.Has(event.Yizkor) {
			opts.Yizkor = true
		}
		return m
	}
	var mask event.Flags
	// default opts
	if !opts.NoHolidays {
		mask |= event.RoshChodesh | event.YomTovEnds | event.MinorFast |
			event.SpecialShabbat | event.ModernHoliday | event.MajorFast |
			event.MinorHoliday | event.Erev | event.CholHamoed |
			event.LightCandles | event.LightCandlesTzeis | event.ChanukahCandles
	}
	if opts.CandleLighting {
		mask |= event.LightCandles | event.LightCandlesTzeis | event.YomTovEnds
	}
	// suppression of defaults
	if opts.NoRoshChodesh {
		mask &^= event.RoshChodesh
	}
	if opts.NoModern {
		mask &^= event.ModernHoliday
	}
	if opts.NoMinorFast {
		mask &^= event.MinorFast
	}
	if opts.NoSpecialShabbat {
		mask &^= event.SpecialShabbat | event.ShabbatMevarchim
	}
	if opts.IL {
		mask |= event.ILOnly
	} else {
		mask |= event.ChulOnly
	}
	// non-default opts
	if opts.Sedrot {
		mask |= event.ParshaHashavua
	}
	if opts.DafYomi {
		mask |= event.DafYomi
	}
	if opts.MishnaYomi {
		mask |= event.MishnaYomi
	}
	if opts.YerushalmiYomi {
		mask |= event.YerushalmiYomi
	}
	if opts.NachYomi {
		mask |= event.NachYomi
	}
	if opts.Omer {
		mask |= event.OmerCount
	}
	if opts.ShabbatMevarchim {
		mask |= event.ShabbatMevarchim
	}
	if opts.YomKippurKatan {
		mask |= event.YomKippurKatan
	}
	if opts.BeHaB {
		mask |= event.BeHaB
	}
	if opts.KiddushLevanaMaharil {
		mask |= event.KiddushLevana
	}
	if opts.Yizkor {
		mask |= event.Yizkor
	}
	return mask
}

// makeMevarchimEvent upgrades a bare "Shabbat Mevarchim Chodesh" holiday event
// to a MevarchimChodeshEvent carrying the molad of the upcoming month.
func makeMevarchimEvent(ev event.HolidayEvent, opts *calConfig) event.MevarchimChodeshEvent {
	hd := ev.Date()
	nextName, nextMonth := nextMonthName(hd.Year(), hd.Month())
	m := molad.New(hd.Year(), nextMonth)
	mev := event.NewMevarchimChodeshEvent(hd, nextName, m)
	mev.CountryCode = opts.clock.cc
	mev.Clock = opts.clock.format
	return mev
}

// eventTime returns the time of a timed event, or false for other events.
func eventTime(ev event.CalEvent) (time.Time, bool) {
	if e, ok := ev.(event.Timed); ok {
		return e.Time(), true
	}
	return time.Time{}, false
}

// sortByDayPart orders one day's events by DayPartOf. The sort is
// stable, so events of equal rank keep the order in which they were added
// (e.g. the parsha before a special Shabbat); timed events of equal rank are
// ordered by time.
func sortByDayPart(events []event.CalEvent) {
	if len(events) < 2 {
		return
	}
	sort.SliceStable(events, func(i, j int) bool {
		ri, rj := DayPartOf(events[i]), DayPartOf(events[j])
		if ri != rj {
			return ri < rj
		}
		ti, okI := eventTime(events[i])
		tj, okJ := eventTime(events[j])
		return okI && okJ && ti.Before(tj)
	})
}

// sortTimedEvents orders one day's events with the untimed ones first, in
// their existing order, followed by the timed ones in chronological order.
// Used with opts.DailyZmanim, whose list of zmanim would otherwise separate
// candle-lighting, fast start/end and so on from their place in the day.
func sortTimedEvents(events []event.CalEvent) {
	sort.SliceStable(events, func(i, j int) bool {
		ti, okI := eventTime(events[i])
		tj, okJ := eventTime(events[j])
		if okI != okJ {
			return okJ
		}
		return okI && ti.Before(tj)
	})
}

func appendHolidayAndRelated(events []event.CalEvent, candlesEv TimedEvent, ev event.CalEvent, opts *calConfig) ([]event.CalEvent, TimedEvent) {
	mask := ev.Flags()
	if (!opts.YomKippurKatan && mask.Has(event.YomKippurKatan)) ||
		(!opts.BeHaB && mask.Has(event.BeHaB)) ||
		(opts.NoModern && mask.Has(event.ModernHoliday)) {
		return events, candlesEv // bail out early
	}
	if opts.CandleLighting && ev.Desc() == "Erev Pesach" {
		events = append(events, makeErevPesachChametz(ev, opts)...)
	}
	isMajorFast := mask.Has(event.MajorFast)
	isMinorFast := mask.Has(event.MinorFast)
	var startEvent, endEvent TimedEvent
	if opts.CandleLighting && (isMajorFast || isMinorFast) && ev.Desc() != "Yom Kippur" {
		startEvent, endEvent = makeFastStartEnd(ev, opts)
		if (startEvent != TimedEvent{}) && (isMajorFast || (isMinorFast && !opts.NoMinorFast)) {
			events = append(events, startEvent)
		}
	}
	if mask.HasAny(opts.Mask) {
		if opts.CandleLighting && mask.HasAny(maskLightCandles) {
			if mask.Has(event.ChanukahCandles) && !opts.NoHolidays {
				// Replace Chanukah event with a clone that includes candle lighting time.
				// For clarity, allow a "duplicate" candle lighting event to remain for Shabbat
				tmp := makeChanukahCandleLighting(ev.(event.HolidayEvent), opts)
				if (tmp != TimedEvent{}) {
					ev = tmp
				}
			} else {
				hd := ev.Date()
				candlesEv = makeCandleEvent(hd, opts, ev)
			}
		}
		isMevarchim := mask.Has(event.ShabbatMevarchim)
		if isMevarchim {
			// Upgrade the bare holiday event to a MevarchimChodeshEvent that
			// carries the molad of the upcoming month.
			if he, ok := ev.(event.HolidayEvent); ok {
				ev = makeMevarchimEvent(he, opts)
			}
		}
		// Yom Kippur Katan, BeHaB and Shabbat Mevarchim are opt-in streams of
		// their own, emitted whenever requested even with NoHolidays.
		if (opts.YomKippurKatan && mask.Has(event.YomKippurKatan)) ||
			(opts.BeHaB && mask.Has(event.BeHaB)) ||
			(opts.ShabbatMevarchim && isMevarchim) {
			events = append(events, ev)
		} else if !opts.NoHolidays {
			events = append(events, ev)
		}
	}
	if (endEvent != TimedEvent{}) && (isMajorFast || (isMinorFast && !opts.NoMinorFast)) {
		events = append(events, endEvent)
	}
	return events, candlesEv
}
