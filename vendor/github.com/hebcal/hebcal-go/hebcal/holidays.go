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
	"sort"
	"strconv"
	"sync"
	"time"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/sedra"
)

type holiday struct {
	mm     hdate.HMonth
	dd     int
	desc   string
	flags  event.Flags
	emoji  string
	chmDay int
}

const chanukahEmoji = "🕎"

// chanukahCandlesEmoji is the menorah followed by the night's candle count
// as a keycap digit ("🕎3️⃣"), the emoji of each night of Chanukah.
func chanukahCandlesEmoji(candles int) string {
	return chanukahEmoji + strconv.Itoa(candles) + "\uFE0F\u20E3"
}

const sukkotEmoji = "🌿🍋"

var staticHolidays = []holiday{
	{mm: hdate.Tishrei, dd: 2, desc: "Rosh Hashana II",
		flags: event.Chag | event.YomTovEnds,
		emoji: "🍏🍯"},
	{mm: hdate.Tishrei, dd: 9, desc: "Erev Yom Kippur",
		flags: event.Erev | event.LightCandles},
	{mm: hdate.Tishrei, dd: 10, desc: "Yom Kippur",
		flags: event.Chag | event.MajorFast | event.YomTovEnds},
	// Erev Sukkot and Erev Pesach are listed twice, once per schedule, so that
	// the event carries ChulOnly or ILOnly like the yom tov it leads into.
	// Consumers rely on that flag to tell the two schedules apart -- the
	// canonical URL appends ?i=on for an Israel observance -- and an entry
	// carrying neither flag is emitted in both calendars, so it cannot.
	{mm: hdate.Tishrei, dd: 14, desc: "Erev Sukkot",
		flags: event.ChulOnly | event.Erev | event.LightCandles,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 14, desc: "Erev Sukkot",
		flags: event.ILOnly | event.Erev | event.LightCandles,
		emoji: sukkotEmoji},

	{mm: hdate.Tishrei, dd: 15, desc: "Sukkot I",
		flags: event.ChulOnly | event.Chag | event.LightCandlesTzeis,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 16, desc: "Sukkot II",
		flags: event.ChulOnly | event.Chag | event.YomTovEnds,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 17, desc: "Sukkot III (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 1,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 18, desc: "Sukkot IV (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 2,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 19, desc: "Sukkot V (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 3,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 20, desc: "Sukkot VI (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 4,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 22, desc: "Shmini Atzeret",
		flags: event.ChulOnly | event.Chag | event.LightCandlesTzeis},
	{mm: hdate.Tishrei, dd: 23, desc: "Simchat Torah",
		flags: event.ChulOnly | event.Chag | event.YomTovEnds},

	{mm: hdate.Tishrei, dd: 15, desc: "Sukkot I",
		flags: event.ILOnly | event.Chag | event.YomTovEnds,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 16, desc: "Sukkot II (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 1,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 17, desc: "Sukkot III (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 2,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 18, desc: "Sukkot IV (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 3,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 19, desc: "Sukkot V (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 4,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 20, desc: "Sukkot VI (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 5,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 22, desc: "Shmini Atzeret",
		flags: event.ILOnly | event.Chag | event.YomTovEnds},

	{mm: hdate.Tishrei, dd: 21, desc: "Sukkot VII (Hoshana Raba)",
		flags: event.LightCandles | event.CholHamoed, chmDay: -1,
		emoji: sukkotEmoji},
	{mm: hdate.Kislev, dd: 24, desc: "Chanukah: 1 Candle",
		flags: event.Erev | event.MinorHoliday | event.ChanukahCandles,
		emoji: chanukahEmoji + "1\uFE0F\u20E3"},
	{mm: hdate.Tevet, dd: 10, desc: "Asara B'Tevet",
		flags: event.MinorFast},
	{mm: hdate.Shvat, dd: 15, desc: "Tu BiShvat",
		flags: event.MinorHoliday,
		emoji: "🌳"},
	{mm: hdate.Adar2, dd: 13, desc: "Erev Purim",
		flags: event.Erev | event.MinorHoliday,
		emoji: "🎭️📜"},
	{mm: hdate.Adar2, dd: 14, desc: "Purim",
		flags: event.MinorHoliday,
		emoji: "🎭️📜"},
	{mm: hdate.Adar2, dd: 15, desc: "Shushan Purim",
		flags: event.MinorHoliday,
		emoji: "🎭️📜"},
	{mm: hdate.Nisan, dd: 14, desc: "Erev Pesach",
		flags: event.ChulOnly | event.Erev | event.LightCandles,
		emoji: "🫓🍷"},
	{mm: hdate.Nisan, dd: 14, desc: "Erev Pesach",
		flags: event.ILOnly | event.Erev | event.LightCandles,
		emoji: "🫓🍷"},
	// Pesach Israel
	{mm: hdate.Nisan, dd: 15, desc: "Pesach I",
		flags: event.ILOnly | event.Chag | event.YomTovEnds,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 16, desc: "Pesach II (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 1,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 17, desc: "Pesach III (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 2,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 18, desc: "Pesach IV (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 3,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 19, desc: "Pesach V (CH''M)",
		flags: event.ILOnly | event.CholHamoed, chmDay: 4,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 20, desc: "Pesach VI (CH''M)",
		flags: event.ILOnly | event.CholHamoed | event.LightCandles, chmDay: 5,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 21, desc: "Pesach VII",
		flags: event.ILOnly | event.Chag | event.YomTovEnds,
		emoji: "🫓"},
	// Pesach chutz l'aretz
	{mm: hdate.Nisan, dd: 15, desc: "Pesach I",
		flags: event.ChulOnly | event.Chag | event.LightCandlesTzeis,
		emoji: "🫓🍷"},
	{mm: hdate.Nisan, dd: 16, desc: "Pesach II",
		flags: event.ChulOnly | event.Chag | event.YomTovEnds,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 17, desc: "Pesach III (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 1,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 18, desc: "Pesach IV (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 2,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 19, desc: "Pesach V (CH''M)",
		flags: event.ChulOnly | event.CholHamoed, chmDay: 3,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 20, desc: "Pesach VI (CH''M)",
		flags: event.ChulOnly | event.CholHamoed | event.LightCandles, chmDay: 4,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 21, desc: "Pesach VII",
		flags: event.ChulOnly | event.Chag | event.LightCandlesTzeis,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 22, desc: "Pesach VIII",
		flags: event.ChulOnly | event.Chag | event.YomTovEnds,
		emoji: "🫓"},

	{mm: hdate.Iyyar, dd: 14, desc: "Pesach Sheni",
		flags: event.MinorHoliday},
	{mm: hdate.Iyyar, dd: 18, desc: "Lag BaOmer",
		flags: event.MinorHoliday,
		emoji: "🔥"},
	{mm: hdate.Sivan, dd: 5, desc: "Erev Shavuot",
		flags: event.Erev | event.LightCandles,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 6, desc: "Shavuot",
		flags: event.ILOnly | event.Chag | event.YomTovEnds,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 6, desc: "Shavuot I",
		flags: event.ChulOnly | event.Chag | event.LightCandlesTzeis,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 7, desc: "Shavuot II",
		flags: event.ChulOnly | event.Chag | event.YomTovEnds,
		emoji: "⛰️🌸"},
	{mm: hdate.Av, dd: 15, desc: "Tu B'Av",
		flags: event.MinorHoliday,
		emoji: "❤️"},
	{mm: hdate.Elul, dd: 1, desc: "Rosh Hashana LaBehemot",
		flags: event.MinorHoliday,
		emoji: "🐑"},
	{mm: hdate.Elul, dd: 29, desc: "Erev Rosh Hashana",
		flags: event.Erev | event.LightCandles,
		emoji: "🍏🍯"},
}

var staticModernHolidays = []struct {
	firstYear        int // First observed in Hebrew year
	mm               hdate.HMonth
	dd               int
	desc             string
	chul             bool // also display in Chutz L'Aretz
	suppressEmoji    bool
	satPostponeToSun bool
	friPostponeToSun bool
	friSatMovetoThu  bool
}{
	{firstYear: 5727, mm: hdate.Iyyar, dd: 28, desc: "Yom Yerushalayim",
		chul: true},
	{firstYear: 5737, mm: hdate.Kislev, dd: 6, desc: "Ben-Gurion Day",
		satPostponeToSun: true, friPostponeToSun: true},
	{firstYear: 5750, mm: hdate.Shvat, dd: 30, desc: "Family Day"},
	{firstYear: 5758, mm: hdate.Cheshvan, dd: 12, desc: "Yitzhak Rabin Memorial Day",
		friSatMovetoThu: true},
	{firstYear: 5764, mm: hdate.Iyyar, dd: 10, desc: "Herzl Day",
		satPostponeToSun: true},
	{firstYear: 5765, mm: hdate.Tamuz, dd: 29, desc: "Jabotinsky Day",
		satPostponeToSun: true},
	{firstYear: 5769, mm: hdate.Cheshvan, dd: 29, desc: "Sigd",
		chul: true, suppressEmoji: true, friSatMovetoThu: true},
	{firstYear: 5777, mm: hdate.Nisan, dd: 10, desc: "Yom HaAliyah",
		chul: true},
	{firstYear: 5777, mm: hdate.Cheshvan, dd: 7, desc: "Yom HaAliyah School Observance"},
	// https://www.gov.il/he/departments/policies/2012_des5234
	{firstYear: 5773, mm: hdate.Tevet, dd: 21, desc: "Hebrew Language Day",
		friSatMovetoThu: true},
	// https://fs.knesset.gov.il/25/law/25_lsr_14184773.pdf
	// (Published in Sefer HaChukim No. 3567 on 8 Av 5786 / July 22, 2026)
	{firstYear: 5786, mm: hdate.Tishrei, dd: 24, desc: "Swords of Iron War Memorial Day",
		chul: true, satPostponeToSun: true},
}

func tzomGedaliahDate(rh hdate.HDate) hdate.HDate {
	offset := 0
	if rh.Weekday() == time.Thursday {
		offset = 1
	}
	return hdate.New(rh.Year(), hdate.Tishrei, 3+offset)
}

func taanitEstherDate(pesach hdate.HDate) hdate.HDate {
	var offset int64 = 31
	if pesach.Weekday() == time.Tuesday {
		offset = 33
	}
	return hdate.FromRD(pesach.Abs() - offset)
}

func taanitBechorotDate(pesach hdate.HDate) hdate.HDate {
	if pesach.Prev().Weekday() == time.Saturday {
		return hdate.FromRD(hdate.DayOnOrBefore(time.Thursday, pesach.Abs()))
	}
	return hdate.New(pesach.Year(), hdate.Nisan, 14)
}

// DayPart is the part of the day in which an event falls, as far as that
// decides the order of the events on one date. DayPartOf returns it, and the
// values compare in the order events appear within a day.
type DayPart int

const (
	// DayPartDawn: timed events before noon, such as "Fast begins".
	DayPartDawn DayPart = iota
	// DayPartMorning: the parsha, special Shabbatot, minor fasts.
	DayPartMorning
	// DayPartMevarchim: the Molad and Shabbat Mevarchim.
	DayPartMevarchim
	// DayPartDay: everything else, including the Omer and user events.
	DayPartDay
	// DayPartMidday: the deadlines for eating and burning chametz.
	DayPartMidday
	// DayPartLearning: daily learning.
	DayPartLearning
	// DayPartEvening: Erev holidays, Chanukah candle-lighting, Leil Selichot.
	DayPartEvening
	// DayPartNight: other timed events, such as candle-lighting and Havdalah.
	DayPartNight
)

var dayPartNames = [...]string{"Dawn", "Morning", "Mevarchim", "Day", "Midday", "Learning", "Evening", "Night"}

// String returns the name of the part of the day, e.g. "Morning".
func (p DayPart) String() string {
	if p < 0 || int(p) >= len(dayPartNames) {
		return "DayPart(" + strconv.Itoa(int(p)) + ")"
	}
	return dayPartNames[p]
}

const learningFlags = event.DailyLearning | event.DafYomi | event.MishnaYomi |
	event.NachYomi | event.YerushalmiYomi

// holidayDayPart returns the approximate time of day of a holiday, used to order
// holidays on the same date. See DayPartOf for the full list.
func holidayDayPart(ev event.HolidayEvent) DayPart {
	return untimedDayPart(ev.Flags(), ev.Desc())
}

// untimedTimeOfDay ranks an event that has no clock time by its flags and
// untranslated description.
func untimedDayPart(f event.Flags, desc string) DayPart {
	switch {
	case f.HasAny(event.Erev|event.ChanukahCandles) || desc == "Leil Selichot":
		return DayPartEvening
	case f.HasAny(event.ParshaHashavua|event.SpecialShabbat|event.MinorFast) ||
		desc == "Birkat Hachamah":
		return DayPartMorning
	case f.HasAny(event.ShabbatMevarchim | event.Molad):
		return DayPartMevarchim
	case f.HasAny(learningFlags):
		return DayPartLearning
	}
	return DayPartDay
}

// timedTimeOfDay ranks an event that occurs at clock time t.
func timedDayPart(f event.Flags, kind TimedKind, t time.Time) DayPart {
	switch {
	case f.Has(event.ChanukahCandles):
		// Chanukah candle-lighting sits with the evening holidays
		return DayPartEvening
	case kind == FinishChametz || kind == BiurChametz:
		return DayPartMidday
	case kind == FastBegins || kind == KiddushLevana:
		// A minor fast begins at dawn, but Tish'a B'Av at sunset the day
		// before. Latest Kiddush Levana is either before dawn or after
		// nightfall.
		if t.Hour() < 12 {
			return DayPartDawn
		}
	}
	return DayPartNight
}

// DayPartOf returns the approximate time of day of an event, used to order
// the events on one date. HebrewCalendar sorts each day's events by it:
// events of equal rank keep the order in which they were created, and timed
// events of equal rank are ordered by time. A caller merging events of its
// own into a calendar can use it to find their place.
//   - dawn: Fast begins for a minor fast, and a Latest Kiddush Levana before
//     noon (which is never later than alot hashachar)
//   - morning: the parsha, special Shabbatot (tied to the Torah or Haftarah
//     reading), minor fasts (which begin at dawn) and Birkat Hachamah
//     (recited at sunrise)
//   - the Molad and Shabbat Mevarchim, announced in synagogue after the Torah
//     reading
//   - day: everything else, including the Omer count and user events
//   - midday: the deadlines for eating and burning chametz
//   - daily learning
//   - evening: Erev holidays, Chanukah candle-lighting and Leil Selichot
//   - night: other timed events such as candle-lighting, Havdalah, Fast ends
//     and a Latest Kiddush Levana after noon
func DayPartOf(ev event.CalEvent) DayPart {
	switch e := ev.(type) {
	case TimedEvent:
		return timedDayPart(e.flags, e.kind, e.eventTime)
	case KiddushLevanaEvent:
		return timedDayPart(e.Flags(), KiddushLevana, e.Time())
	case event.HolidayEvent:
		return holidayDayPart(e)
	case riseSetEvent:
		return DayPartDawn
	}
	return untimedDayPart(ev.Flags(), "")
}

// sortHolidays orders events by date, then by holidayDayPart. Events on the same
// date and at the same time of day keep the order in which they were added.
func sortHolidays(events []event.HolidayEvent) {
	sort.SliceStable(events, func(i, j int) bool {
		abs1, abs2 := events[i].Date().Abs(), events[j].Date().Abs()
		if abs1 != abs2 {
			return abs1 < abs2
		}
		return holidayDayPart(events[i]) < holidayDayPart(events[j])
	})
}

func nextMonthName(year int, month hdate.HMonth) (string, hdate.HMonth) {
	nextMonth := month + 1
	monthsInYear := hdate.MonthsInYear(year)
	if month == hdate.HMonth(monthsInYear) {
		nextMonth = hdate.Nisan
	}
	nextMonthName := hdate.New(year, nextMonth, 1).MonthName("en")
	return nextMonthName, nextMonth
}

// chanukahEvent constructs the holiday event of a day of Chanukah.
func chanukahEvent(date hdate.HDate, desc string, flags event.Flags, emoji string, chanukahDay int) event.HolidayEvent {
	ev := event.NewHolidayEvent(date, desc, flags, emoji)
	ev.ChanukahDay = chanukahDay
	return ev
}

func getAllHolidaysForYear(year int) []event.HolidayEvent {
	events := make([]event.HolidayEvent, 0, 120)
	// standard holidays that don't shift based on year
	for _, h := range staticHolidays {
		ev := event.NewHolidayEvent(hdate.New(year, h.mm, h.dd), h.desc, h.flags, h.emoji)
		ev.CholHaMoedDay = h.chmDay
		events = append(events, ev)
	}
	// variable holidays
	roshHashana := hdate.New(year, hdate.Tishrei, 1)
	nextRh := hdate.New(year+1, hdate.Tishrei, 1)
	pesach := hdate.New(year, hdate.Nisan, 15)
	pesachAbs := pesach.Abs()
	events = append(events,
		event.NewHolidayEvent(
			roshHashana,
			"Rosh Hashana "+strconv.Itoa(year),
			event.Chag|event.LightCandlesTzeis,
			"🍏🍯"),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, 7+roshHashana.Abs())),
			"Shabbat Shuva",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			tzomGedaliahDate(roshHashana),
			"Tzom Gedaliah",
			event.MinorFast,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-43)),
			"Shabbat Shekalim",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-30)),
			"Shabbat Zachor",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			taanitEstherDate(pesach),
			"Ta'anit Esther",
			event.MinorFast,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-14)-7),
			"Shabbat Parah",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-14)),
			"Shabbat HaChodesh",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-1)),
			"Shabbat HaGadol",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			taanitBechorotDate(pesach),
			"Ta'anit Bechorot",
			event.MinorFast,
			""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, nextRh.Abs()-4)),
			"Leil Selichot",
			event.MinorHoliday,
			"🕍"),
	)
	if pesach.Weekday() == time.Sunday {
		events = append(events, event.NewHolidayEvent(
			hdate.New(year, hdate.Adar2, 16),
			"Purim Meshulash",
			event.MinorHoliday,
			""),
		)
	}
	if hdate.IsLeapYear(year) {
		events = append(events, event.NewHolidayEvent(
			hdate.New(year, hdate.Adar1, 14),
			"Purim Katan",
			event.MinorHoliday,
			"🎭️"))
		events = append(events, event.NewHolidayEvent(
			hdate.New(year, hdate.Adar1, 15),
			"Shushan Purim Katan",
			event.MinorHoliday,
			"🎭️"))
	}
	var chanukah7 hdate.HDate
	if hdate.ShortKislev(year) {
		chanukah7 = hdate.New(year, hdate.Tevet, 1)
	} else {
		chanukah7 = hdate.New(year, hdate.Kislev, 30)
	}
	// On the 1st day of Rosh Chodesh Tevet
	events = append(events,
		event.NewHolidayEvent(
			chanukah7,
			"Chag HaBanot",
			event.MinorHoliday,
			""))
	for candles := 2; candles <= 6; candles++ {
		events = append(events, chanukahEvent(
			hdate.New(year, hdate.Kislev, 23+candles),
			"Chanukah: "+strconv.Itoa(candles)+" Candles",
			event.MinorHoliday|event.ChanukahCandles,
			chanukahCandlesEmoji(candles), candles-1))
	}
	chanukah8 := chanukah7.Next()
	events = append(events,
		chanukahEvent(
			chanukah7,
			"Chanukah: 7 Candles",
			event.MinorHoliday|event.ChanukahCandles,
			chanukahCandlesEmoji(7), 6),
		chanukahEvent(
			chanukah8,
			"Chanukah: 8 Candles",
			event.MinorHoliday|event.ChanukahCandles,
			chanukahCandlesEmoji(8), 7),
		chanukahEvent(
			chanukah8.Next(),
			"Chanukah: 8th Day",
			event.MinorHoliday,
			chanukahEmoji, 8))

	// Tisha BAv and the 3 weeks
	var tamuz17 = hdate.New(year, hdate.Tamuz, 17)
	if tamuz17.Weekday() == time.Saturday {
		tamuz17 = tamuz17.Next()
	}
	events = append(events,
		event.NewHolidayEvent(tamuz17, "Tzom Tammuz", event.MinorFast, ""))

	var av9dt = hdate.New(year, hdate.Av, 9)
	var av9title = "Tish'a B'Av"
	if av9dt.Weekday() == time.Saturday {
		av9dt = av9dt.Next()
		av9title += " (observed)"
	}
	var av9abs = av9dt.Abs()
	events = append(events,
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, av9abs)),
			"Shabbat Chazon",
			event.SpecialShabbat,
			""),
		event.NewHolidayEvent(
			av9dt.Prev(),
			"Erev Tish'a B'Av",
			event.Erev|event.MajorFast,
			""),
		event.NewHolidayEvent(av9dt, av9title, event.MajorFast, ""),
		event.NewHolidayEvent(
			hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, av9abs+7)),
			"Shabbat Nachamu",
			event.SpecialShabbat,
			""))

	// modern holidays
	if year >= 5708 {
		// Yom HaAtzma'ut only celebrated after 1948
		var day int
		if pesach.Weekday() == time.Sunday {
			day = 2
		} else if pesach.Weekday() == time.Saturday {
			day = 3
		} else if year < 5764 {
			day = 4
		} else if pesach.Weekday() == time.Tuesday {
			day = 5
		} else {
			day = 4
		}
		var tmpDate = hdate.New(year, hdate.Iyyar, day)
		events = append(events,
			event.NewHolidayEvent(
				tmpDate,
				"Yom HaZikaron",
				event.ModernHoliday,
				"🇮🇱"),
			event.NewHolidayEvent(
				tmpDate.Next(),
				"Yom HaAtzma'ut",
				event.ModernHoliday,
				"🇮🇱"),
		)
	}

	if year >= 5711 {
		// Yom HaShoah first observed in 1951
		var nisan27dt = hdate.New(year, hdate.Nisan, 27)
		/* When the actual date of Yom Hashoah falls on a Friday, the
		 * state of Israel observes Yom Hashoah on the preceding
		 * Thursday. When it falls on a Sunday, Yom Hashoah is observed
		 * on the following Monday.
		 * https://www.ushmm.org/remember/days-of-remembrance/resources/calendar
		 */
		if nisan27dt.Weekday() == time.Friday {
			nisan27dt = nisan27dt.Prev()
		} else if nisan27dt.Weekday() == time.Sunday {
			nisan27dt = nisan27dt.Next()
		}
		events = append(events,
			event.NewHolidayEvent(
				nisan27dt,
				"Yom HaShoah",
				event.ModernHoliday,
				""))
	}

	for _, h := range staticModernHolidays {
		if year >= h.firstYear {
			emoji := "🇮🇱"
			if h.suppressEmoji {
				emoji = ""
			}
			hd := hdate.New(year, h.mm, h.dd)
			dow := hd.Weekday()
			if h.friSatMovetoThu && (dow == time.Friday || dow == time.Saturday) {
				hd = hd.OnOrBefore(time.Thursday)
			} else if h.friPostponeToSun && dow == time.Friday {
				hd = hd.Next().Next()
			} else if h.satPostponeToSun && dow == time.Saturday {
				hd = hd.Next()
			}
			flags := event.ModernHoliday
			if !h.chul {
				flags |= event.ILOnly
			}
			events = append(events, event.NewHolidayEvent(
				hd,
				h.desc,
				flags,
				emoji))
		}
	}

	// Rosh Chodesh
	monthsInYear := hdate.MonthsInYear(year)
	for i := 1; i <= monthsInYear; i++ {
		isNisan := i == 1
		prevMonthNum := i - 1
		prevMonthYear := year
		if isNisan {
			prevMonthYear = year - 1
			prevMonthNum = hdate.MonthsInYear(prevMonthYear)
		}
		prevMonth := hdate.HMonth(prevMonthNum)
		prevMonthNumDays := hdate.DaysInMonth(prevMonth, prevMonthYear)
		month := hdate.HMonth(i)
		monthName := hdate.New(year, month, 1).MonthName("en")
		desc := "Rosh Chodesh " + monthName
		if prevMonthNumDays == 30 {
			events = append(events,
				event.NewHolidayEvent(
					hdate.New(year, prevMonth, 30),
					desc,
					event.RoshChodesh,
					""),
				event.NewHolidayEvent(
					hdate.New(year, month, 1),
					desc,
					event.RoshChodesh,
					""),
			)
		} else if month != hdate.Tishrei {
			events = append(events,
				event.NewHolidayEvent(
					hdate.New(year, month, 1),
					desc,
					event.RoshChodesh,
					""))
		}

		// Shabbat Mevarchim Chodesh
		if month == hdate.Elul {
			continue
		}
		nextMonthName, _ := nextMonthName(year, month)
		events = append(events,
			event.NewHolidayEvent(
				hdate.New(year, month, 29).OnOrBefore(time.Saturday),
				"Shabbat Mevarchim Chodesh "+nextMonthName,
				event.ShabbatMevarchim,
				""))
	}

	// Begin: Yom Kippur Katan
	// start at hdate.Iyyar because one may not fast during hdate.Nisan
	for month := hdate.Iyyar; month <= hdate.HMonth(monthsInYear); month++ {
		nextMonthName, nextMonth := nextMonthName(year, month)
		// Yom Kippur Katan is not observed on the day before Rosh Hashanah.
		// Not observed prior to Rosh Chodesh hdate.Cheshvan because Yom Kippur has just passed.
		// Not observed before Rosh Chodesh hdate.Tevet, because that day is Hanukkah.
		if nextMonth == hdate.Tishrei || nextMonth == hdate.Cheshvan || nextMonth == hdate.Tevet {
			continue
		}
		ykk := hdate.New(year, month, 29)
		dow := ykk.Weekday()
		if dow == time.Friday || dow == time.Saturday {
			ykk = ykk.OnOrBefore(time.Thursday)
		}

		events = append(events,
			event.NewHolidayEvent(
				ykk,
				"Yom Kippur Katan "+nextMonthName,
				event.MinorFast|event.YomKippurKatan,
				""))
	}

	// Ta'anit BeHaB: Monday, Thursday, Monday after Rosh Chodesh Cheshvan and Iyyar
	for _, month := range []hdate.HMonth{hdate.Cheshvan, hdate.Iyyar} {
		rc := hdate.New(year, month, 1)
		shabbos := hdate.DayOnOrBefore(time.Saturday, rc.Abs()+6)
		if shabbos == rc.Abs() {
			shabbos += 7
		}
		fastDays := []hdate.HDate{
			hdate.FromRD(shabbos + 2),
			hdate.FromRD(shabbos + 5),
			hdate.FromRD(shabbos + 9),
		}
		// Pesach Sheni: move the last fast to 17 Iyyar
		if month == hdate.Iyyar && fastDays[2].Day() == 14 {
			fastDays[2] = hdate.New(year, hdate.Iyyar, 17)
		}
		for _, hd := range fastDays {
			events = append(events, event.NewHolidayEvent(
				hd,
				"Ta'anit BeHaB",
				event.MinorFast|event.BeHaB,
				""))
		}
	}

	sedra := sedra.New(year, false)
	beshalachHd, _ := sedra.FindParshaNum(16)
	events = append(events,
		event.NewHolidayEvent(
			beshalachHd,
			"Shabbat Shirah",
			event.SpecialShabbat,
			""))

	// Birkat Hachamah appears only once every 28 years.
	birkatHaChamaRD := getBirkatHaChama(year)
	if birkatHaChamaRD != 0 {
		events = append(events,
			event.NewHolidayEvent(
				hdate.FromRD(birkatHaChamaRD),
				"Birkat Hachamah",
				event.MinorHoliday,
				"☀️"))
	}

	sortHolidays(events)
	return events
}

// Birkat Hachamah appears only once every 28 years.
// Although almost always in Nisan, it can occur in Adar II.
//   - 27 Adar II 5461 (Gregorian year 1701)
//   - 29 Adar II 5993 (Gregorian year 2233)
//
// Due to drift, this will eventually slip into Iyyar
//   - 2 Iyyar 7141 (Gregorian year 3381)
func getBirkatHaChama(year int) int64 {
	startMonth := hdate.Nisan
	startDay := 1
	if hdate.IsLeapYear(year) {
		startMonth = hdate.Adar2
		startDay = 20
	}
	baseRD := hdate.ToRD(year, startMonth, startDay)
	for day := 0; day <= 40; day++ {
		rataDie := baseRD + int64(day)
		elapsed := rataDie + 1373429
		if elapsed%10227 == 172 {
			return rataDie
		}
	}
	return 0
}

// holidaysYearKey identifies a memoized HolidaysForYear result.
type holidaysYearKey struct {
	year int
	il   bool
}

// holidaysForYearCache memoizes HolidaysForYear. The map is unbounded (the
// set of (year, il) pairs a process sees is small in practice) and guarded by
// holidaysForYearMu.
var (
	holidaysForYearMu    sync.RWMutex
	holidaysForYearCache = make(map[holidaysYearKey][]event.HolidayEvent)
)

// HolidaysForYear returns all holidays in the given Hebrew year, for either
// the Diaspora (il=false) or Israel (il=true) schedule.
//
// year must be a valid Hebrew year, 1 or later; an earlier year panics, as
// in github.com/hebcal/hdate.
//
// The result is memoized and shared between callers, so it must be treated as
// read-only: do not modify the returned slice or its elements. (The slice is
// returned with its capacity clamped to its length, so appending to it safely
// allocates a new backing array rather than corrupting the cache.)
func HolidaysForYear(year int, il bool) []event.HolidayEvent {
	key := holidaysYearKey{year: year, il: il}
	holidaysForYearMu.RLock()
	cached, ok := holidaysForYearCache[key]
	holidaysForYearMu.RUnlock()
	if ok {
		return cached
	}
	events := getAllHolidaysForYear(year)
	result := make([]event.HolidayEvent, 0, len(events))
	for _, ev := range events {
		if (il && !ev.Flags().Has(event.ChulOnly)) ||
			(!il && !ev.Flags().Has(event.ILOnly)) {
			result = append(result, ev)
		}
	}
	result = result[:len(result):len(result)] // clamp cap so callers' appends don't alias the cache
	holidaysForYearMu.Lock()
	holidaysForYearCache[key] = result
	holidaysForYearMu.Unlock()
	return result
}
