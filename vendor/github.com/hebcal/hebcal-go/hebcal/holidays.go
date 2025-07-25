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
	"time"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/sedra"
)

type holiday struct {
	mm     hdate.HMonth
	dd     int
	desc   string
	flags  event.HolidayFlags
	emoji  string
	chmDay int
}

const chanukahEmoji = "🕎"
const sukkotEmoji = "🌿🍋"

var staticHolidays = []holiday{
	{mm: hdate.Tishrei, dd: 2, desc: "Rosh Hashana II",
		flags: event.CHAG | event.YOM_TOV_ENDS,
		emoji: "🍏🍯"},
	{mm: hdate.Tishrei, dd: 9, desc: "Erev Yom Kippur",
		flags: event.EREV | event.LIGHT_CANDLES},
	{mm: hdate.Tishrei, dd: 10, desc: "Yom Kippur",
		flags: event.CHAG | event.MAJOR_FAST | event.YOM_TOV_ENDS},
	{mm: hdate.Tishrei, dd: 14, desc: "Erev Sukkot",
		flags: event.EREV | event.LIGHT_CANDLES,
		emoji: sukkotEmoji},

	{mm: hdate.Tishrei, dd: 15, desc: "Sukkot I",
		flags: event.CHUL_ONLY | event.CHAG | event.LIGHT_CANDLES_TZEIS,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 16, desc: "Sukkot II",
		flags: event.CHUL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 17, desc: "Sukkot III (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 1,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 18, desc: "Sukkot IV (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 2,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 19, desc: "Sukkot V (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 3,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 20, desc: "Sukkot VI (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 4,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 22, desc: "Shmini Atzeret",
		flags: event.CHUL_ONLY | event.CHAG | event.LIGHT_CANDLES_TZEIS},
	{mm: hdate.Tishrei, dd: 23, desc: "Simchat Torah",
		flags: event.CHUL_ONLY | event.CHAG | event.YOM_TOV_ENDS},

	{mm: hdate.Tishrei, dd: 15, desc: "Sukkot I",
		flags: event.IL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 16, desc: "Sukkot II (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 1,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 17, desc: "Sukkot III (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 2,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 18, desc: "Sukkot IV (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 3,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 19, desc: "Sukkot V (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 4,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 20, desc: "Sukkot VI (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 5,
		emoji: sukkotEmoji},
	{mm: hdate.Tishrei, dd: 22, desc: "Shmini Atzeret",
		flags: event.IL_ONLY | event.CHAG | event.YOM_TOV_ENDS},

	{mm: hdate.Tishrei, dd: 21, desc: "Sukkot VII (Hoshana Raba)",
		flags: event.LIGHT_CANDLES | event.CHOL_HAMOED, chmDay: -1,
		emoji: sukkotEmoji},
	{mm: hdate.Kislev, dd: 24, desc: "Chanukah: 1 Candle",
		flags: event.EREV | event.MINOR_HOLIDAY | event.CHANUKAH_CANDLES,
		emoji: chanukahEmoji},
	{mm: hdate.Tevet, dd: 10, desc: "Asara B'Tevet",
		flags: event.MINOR_FAST},
	{mm: hdate.Shvat, dd: 15, desc: "Tu BiShvat",
		flags: event.MINOR_HOLIDAY,
		emoji: "🌳"},
	{mm: hdate.Adar2, dd: 13, desc: "Erev Purim",
		flags: event.EREV | event.MINOR_HOLIDAY,
		emoji: "🎭️📜"},
	{mm: hdate.Adar2, dd: 14, desc: "Purim",
		flags: event.MINOR_HOLIDAY,
		emoji: "🎭️📜"},
	{mm: hdate.Adar2, dd: 15, desc: "Shushan Purim",
		flags: event.MINOR_HOLIDAY,
		emoji: "🎭️📜"},
	{mm: hdate.Nisan, dd: 14, desc: "Erev Pesach",
		flags: event.EREV | event.LIGHT_CANDLES,
		emoji: "🫓🍷"},
	// Pesach Israel
	{mm: hdate.Nisan, dd: 15, desc: "Pesach I",
		flags: event.IL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 16, desc: "Pesach II (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 1,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 17, desc: "Pesach III (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 2,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 18, desc: "Pesach IV (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 3,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 19, desc: "Pesach V (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED, chmDay: 4,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 20, desc: "Pesach VI (CH''M)",
		flags: event.IL_ONLY | event.CHOL_HAMOED | event.LIGHT_CANDLES, chmDay: 5,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 21, desc: "Pesach VII",
		flags: event.IL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "🫓"},
	// Pesach chutz l'aretz
	{mm: hdate.Nisan, dd: 15, desc: "Pesach I",
		flags: event.CHUL_ONLY | event.CHAG | event.LIGHT_CANDLES_TZEIS,
		emoji: "🫓🍷"},
	{mm: hdate.Nisan, dd: 16, desc: "Pesach II",
		flags: event.CHUL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 17, desc: "Pesach III (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 1,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 18, desc: "Pesach IV (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 2,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 19, desc: "Pesach V (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED, chmDay: 3,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 20, desc: "Pesach VI (CH''M)",
		flags: event.CHUL_ONLY | event.CHOL_HAMOED | event.LIGHT_CANDLES, chmDay: 4,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 21, desc: "Pesach VII",
		flags: event.CHUL_ONLY | event.CHAG | event.LIGHT_CANDLES_TZEIS,
		emoji: "🫓"},
	{mm: hdate.Nisan, dd: 22, desc: "Pesach VIII",
		flags: event.CHUL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "🫓"},

	{mm: hdate.Iyyar, dd: 14, desc: "Pesach Sheni",
		flags: event.MINOR_HOLIDAY},
	{mm: hdate.Iyyar, dd: 18, desc: "Lag BaOmer",
		flags: event.MINOR_HOLIDAY,
		emoji: "🔥"},
	{mm: hdate.Sivan, dd: 5, desc: "Erev Shavuot",
		flags: event.EREV | event.LIGHT_CANDLES,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 6, desc: "Shavuot",
		flags: event.IL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 6, desc: "Shavuot I",
		flags: event.CHUL_ONLY | event.CHAG | event.LIGHT_CANDLES_TZEIS,
		emoji: "⛰️🌸"},
	{mm: hdate.Sivan, dd: 7, desc: "Shavuot II",
		flags: event.CHUL_ONLY | event.CHAG | event.YOM_TOV_ENDS,
		emoji: "⛰️🌸"},
	{mm: hdate.Av, dd: 15, desc: "Tu B'Av",
		flags: event.MINOR_HOLIDAY,
		emoji: "❤️"},
	{mm: hdate.Elul, dd: 1, desc: "Rosh Hashana LaBehemot",
		flags: event.MINOR_HOLIDAY,
		emoji: "🐑"},
	{mm: hdate.Elul, dd: 29, desc: "Erev Rosh Hashana",
		flags: event.EREV | event.LIGHT_CANDLES,
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
	{firstYear: 5750, mm: hdate.Shvat, dd: 30, desc: "Family Day",
		suppressEmoji: true},
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
	} else {
		return hdate.New(pesach.Year(), hdate.Nisan, 14)
	}
}

type byDate []event.HolidayEvent

func (s byDate) Len() int {
	return len(s)
}
func (s byDate) Swap(i, j int) {
	s[i], s[j] = s[j], s[i]
}
func (s byDate) Less(i, j int) bool {
	abs1 := s[i].Date.Abs()
	abs2 := s[j].Date.Abs()
	if abs1 == abs2 {
		return s[i].Desc < s[j].Desc
	}
	return abs1 < abs2
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

func getAllHolidaysForYear(year int) []event.HolidayEvent {
	events := make([]event.HolidayEvent, 0, 120)
	// standard holidays that don't shift based on year
	for _, h := range staticHolidays {
		events = append(events, event.HolidayEvent{Date: hdate.New(year, h.mm, h.dd),
			Desc: h.desc, Flags: h.flags, Emoji: h.emoji, CholHaMoedDay: h.chmDay})
	}
	// variable holidays
	roshHashana := hdate.New(year, hdate.Tishrei, 1)
	nextRh := hdate.New(year+1, hdate.Tishrei, 1)
	pesach := hdate.New(year, hdate.Nisan, 15)
	pesachAbs := pesach.Abs()
	events = append(events,
		event.HolidayEvent{
			Date:  roshHashana,
			Desc:  "Rosh Hashana " + strconv.Itoa(year),
			Flags: event.CHAG | event.LIGHT_CANDLES_TZEIS,
			Emoji: "🍏🍯"},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, 7+roshHashana.Abs())),
			Desc:  "Shabbat Shuva",
			Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date:  tzomGedaliahDate(roshHashana),
			Desc:  "Tzom Gedaliah",
			Flags: event.MINOR_FAST},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-43)),
			Desc:  "Shabbat Shekalim",
			Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-30)),
			Desc:  "Shabbat Zachor",
			Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date:  taanitEstherDate(pesach),
			Desc:  "Ta'anit Esther",
			Flags: event.MINOR_FAST},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-14) - 7),
			Desc:  "Shabbat Parah",
			Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date: hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-14)),
			Desc: "Shabbat HaChodesh", Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date: hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, pesachAbs-1)),
			Desc: "Shabbat HaGadol", Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date:  taanitBechorotDate(pesach),
			Desc:  "Ta'anit Bechorot",
			Flags: event.MINOR_FAST},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, nextRh.Abs()-4)),
			Desc:  "Leil Selichot",
			Flags: event.MINOR_HOLIDAY,
			Emoji: "🕍"},
	)
	if pesach.Weekday() == time.Sunday {
		events = append(events, event.HolidayEvent{
			Date:  hdate.New(year, hdate.Adar2, 16),
			Desc:  "Purim Meshulash",
			Flags: event.MINOR_HOLIDAY},
		)
	}
	if hdate.IsLeapYear(year) {
		events = append(events, event.HolidayEvent{
			Date:  hdate.New(year, hdate.Adar1, 14),
			Desc:  "Purim Katan",
			Flags: event.MINOR_HOLIDAY,
			Emoji: "🎭️"})
		events = append(events, event.HolidayEvent{
			Date:  hdate.New(year, hdate.Adar1, 15),
			Desc:  "Shushan Purim Katan",
			Flags: event.MINOR_HOLIDAY,
			Emoji: "🎭️"})
	}
	for candles := 2; candles <= 6; candles++ {
		events = append(events, event.HolidayEvent{
			Date:        hdate.New(year, hdate.Kislev, 23+candles),
			Desc:        "Chanukah: " + strconv.Itoa(candles) + " Candles",
			Flags:       event.MINOR_HOLIDAY | event.CHANUKAH_CANDLES,
			Emoji:       chanukahEmoji,
			ChanukahDay: candles - 1})
	}
	var chanukah7 hdate.HDate
	if hdate.ShortKislev(year) {
		chanukah7 = hdate.New(year, hdate.Tevet, 1)
	} else {
		chanukah7 = hdate.New(year, hdate.Kislev, 30)
	}
	chanukah8 := chanukah7.Next()
	events = append(events,
		event.HolidayEvent{
			Date:        chanukah7,
			Desc:        "Chanukah: 7 Candles",
			Flags:       event.MINOR_HOLIDAY | event.CHANUKAH_CANDLES,
			Emoji:       chanukahEmoji,
			ChanukahDay: 6},
		event.HolidayEvent{
			Date:        chanukah8,
			Desc:        "Chanukah: 8 Candles",
			Flags:       event.MINOR_HOLIDAY | event.CHANUKAH_CANDLES,
			Emoji:       chanukahEmoji,
			ChanukahDay: 7},
		event.HolidayEvent{
			Date:        chanukah8.Next(),
			Desc:        "Chanukah: 8th Day",
			Flags:       event.MINOR_HOLIDAY,
			Emoji:       chanukahEmoji,
			ChanukahDay: 8})

	// On the 1st day of Rosh Chodesh Tevet
	events = append(events,
		event.HolidayEvent{
			Date:  chanukah7,
			Desc:  "Chag HaBanot",
			Flags: event.MINOR_HOLIDAY})

	// Tisha BAv and the 3 weeks
	var tamuz17 = hdate.New(year, hdate.Tamuz, 17)
	if tamuz17.Weekday() == time.Saturday {
		tamuz17 = tamuz17.Next()
	}
	events = append(events,
		event.HolidayEvent{Date: tamuz17, Desc: "Tzom Tammuz", Flags: event.MINOR_FAST})

	var av9dt = hdate.New(year, hdate.Av, 9)
	var av9title = "Tish'a B'Av"
	if av9dt.Weekday() == time.Saturday {
		av9dt = av9dt.Next()
		av9title += " (observed)"
	}
	var av9abs = av9dt.Abs()
	events = append(events,
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, av9abs)),
			Desc:  "Shabbat Chazon",
			Flags: event.SPECIAL_SHABBAT},
		event.HolidayEvent{
			Date:  av9dt.Prev(),
			Desc:  "Erev Tish'a B'Av",
			Flags: event.EREV | event.MAJOR_FAST},
		event.HolidayEvent{Date: av9dt, Desc: av9title, Flags: event.MAJOR_FAST},
		event.HolidayEvent{
			Date:  hdate.FromRD(hdate.DayOnOrBefore(time.Saturday, av9abs+7)),
			Desc:  "Shabbat Nachamu",
			Flags: event.SPECIAL_SHABBAT})

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
			event.HolidayEvent{
				Date:  tmpDate,
				Desc:  "Yom HaZikaron",
				Flags: event.MODERN_HOLIDAY,
				Emoji: "🇮🇱"},
			event.HolidayEvent{
				Date:  tmpDate.Next(),
				Desc:  "Yom HaAtzma'ut",
				Flags: event.MODERN_HOLIDAY,
				Emoji: "🇮🇱"},
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
			event.HolidayEvent{
				Date:  nisan27dt,
				Desc:  "Yom HaShoah",
				Flags: event.MODERN_HOLIDAY})
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
			flags := event.MODERN_HOLIDAY
			if !h.chul {
				flags |= event.IL_ONLY
			}
			events = append(events, event.HolidayEvent{
				Date:  hd,
				Desc:  h.desc,
				Flags: flags,
				Emoji: emoji})
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
				event.HolidayEvent{
					Date:  hdate.New(year, prevMonth, 30),
					Desc:  desc,
					Flags: event.ROSH_CHODESH},
				event.HolidayEvent{
					Date:  hdate.New(year, month, 1),
					Desc:  desc,
					Flags: event.ROSH_CHODESH},
			)
		} else if month != hdate.Tishrei {
			events = append(events,
				event.HolidayEvent{
					Date:  hdate.New(year, month, 1),
					Desc:  desc,
					Flags: event.ROSH_CHODESH})
		}

		// Shabbat Mevarchim Chodesh
		if month == hdate.Elul {
			continue
		}
		nextMonthName, _ := nextMonthName(year, month)
		events = append(events,
			event.HolidayEvent{
				Date:  hdate.New(year, month, 29).OnOrBefore(time.Saturday),
				Desc:  "Shabbat Mevarchim Chodesh " + nextMonthName,
				Flags: event.SHABBAT_MEVARCHIM,
			})
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
			event.HolidayEvent{
				Date:  ykk,
				Desc:  "Yom Kippur Katan " + nextMonthName,
				Flags: event.MINOR_FAST | event.YOM_KIPPUR_KATAN})
	}

	sedra := sedra.New(year, false)
	beshalachHd, _ := sedra.FindParshaNum(16)
	events = append(events,
		event.HolidayEvent{
			Date:  beshalachHd,
			Desc:  "Shabbat Shirah",
			Flags: event.SPECIAL_SHABBAT})

	// Birkat Hachamah appears only once every 28 years.
	birkatHaChamaRD := getBirkatHaChama(year)
	if birkatHaChamaRD != 0 {
		events = append(events,
			event.HolidayEvent{
				Date:  hdate.FromRD(birkatHaChamaRD),
				Desc:  "Birkat Hachamah",
				Flags: event.MINOR_HOLIDAY,
				Emoji: "☀️"})
	}

	sort.Sort(byDate(events))
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

// Returns a slice of holidays for the year.
// For Israel holiday schedule, specify il=true.
func GetHolidaysForYear(year int, il bool) []event.HolidayEvent {
	events := getAllHolidaysForYear(year)
	result := make([]event.HolidayEvent, 0, len(events))
	for _, ev := range events {
		if (il && (ev.Flags&event.CHUL_ONLY) == 0) ||
			(!il && (ev.Flags&event.IL_ONLY) == 0) {
			result = append(result, ev)
		}
	}
	return result
}
