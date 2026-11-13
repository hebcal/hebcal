package hebcal

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
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
	"time"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/zmanim"
)

// YerushalmiEdition selects which page numbering is used for Yerushalmi
// Yomi (the Jerusalem Talmud daily learning schedule).
type YerushalmiEdition int

const (
	// Vilna Edition. The classic Vilna cycle takes ~4.25 years and skips
	// both Yom Kippur and Tisha B'Av.
	Vilna YerushalmiEdition = 1 + iota
	// Schottenstein Edition. Uses different page numbers and takes ~6 years.
	Schottenstein
)

// UserEventSpec asks for a personal event that recurs on a Hebrew date every
// year, such as a birthday or an anniversary (see Options.UserEvents).
// HebrewCalendar generates an event.UserEvent from it in each year.
type UserEventSpec struct {
	Month hdate.HMonth // Hebrew month
	Day   int          // Day in month (1-30)
	Desc  string       // Description
}

// UserYahrzeit is used for generating a yahrtzeit reminder events.
type UserYahrzeit struct {
	Date time.Time // Gregorian Date of death
	Name string    // Name of deceased
}

// Options configures which events HebrewCalendar returns. The zero value
// asks for the holidays of the current Gregorian year in the Diaspora.
//
// HebrewCalendar does not modify the Options it is given.
type Options struct {
	// Location is the place candle-lighting and other times are calculated
	// for: latitude, longitude, elevation and time zone. Its country also
	// chooses the default clock format (see Clock).
	Location *zmanim.Location
	// Year is the Gregorian or Hebrew year to generate. Zero means the
	// current year.
	Year int
	// IsHebrewYear interprets Year as a Hebrew year.
	IsHebrewYear bool
	// NoJulian disables the Julian calendar transition and uses the
	// proleptic Gregorian calendar. Only used when Year is non-zero and
	// IsHebrewYear is false.
	NoJulian bool
	// Month restricts the calendar to one Gregorian month of Year.
	Month time.Month
	// NumYears is the number of years to generate. Zero means 1.
	NumYears int
	// Start is the first day of the calendar. It requires End, and the two
	// replace Year, Month and NumYears.
	Start hdate.HDate
	// End is the last day of the calendar. It requires Start.
	End hdate.HDate
	// CandleLighting adds candle-lighting and Havdalah times, Chanukah
	// candle-lighting times, the start and end of fasts, and the chametz
	// deadlines of Erev Pesach. Requires Location.
	CandleLighting bool
	// UseElevation enables elevation-aware sunrise and sunset (and the halachic
	// times derived from them) using the Location's elevation. Degree-based
	// zmanim, such as dawn, dusk, tzeit, alot haShachar and misheyakir, estimate
	// the amount of light in the sky and are intentionally never affected by
	// elevation. Defaults to false.
	UseElevation bool
	// CandleLightingMins is the number of minutes before sundown to light
	// candles. Zero means the default: 18, or the custom of an Israeli city
	// (see IsraelCandleLightingMins).
	CandleLightingMins int
	// CandleLightingAtSunset lights candles at sunset itself, with no offset.
	// A zero CandleLightingMins cannot say this, because zero means "unset,
	// use the default" (18 minutes, or the Israeli city's custom). When set,
	// CandleLightingMins is ignored. Candle-lighting that follows Havdalah
	// rules (the second night of Yom Tov, Chanukah on Saturday night) is not
	// affected.
	CandleLightingAtSunset bool
	// HavdalahMins is the number of minutes after sundown for Havdalah
	// (typical values are 42, 50, or 72). If 0 (the default), Havdalah is
	// calculated according to Tzeit Hakochavim - Nightfall (the point when 3
	// small stars are observable in the night time sky with the naked eye).
	// Mutually exclusive with HavdalahDeg.
	HavdalahMins int
	// HavdalahDeg is the solar depression, in degrees, for Havdalah.
	// The default is 8.5 degrees for 3 small stars. Use 7.083 degrees for 3
	// medium-sized stars. Mutually exclusive with HavdalahMins.
	HavdalahDeg float64
	// SuppressHavdalah omits Havdalah events entirely, keeping candle-lighting.
	// Because HavdalahMins is a plain int, it cannot distinguish "unset, use the
	// default tzeit" (0) from "the user asked for no Havdalah" the way
	// @hebcal/core's nullable havdalahMins does (where havdalahMins===0 suppresses
	// Havdalah). Callers that need that behavior set this flag explicitly.
	SuppressHavdalah bool
	// FastStartDeg is the solar depression, in degrees, for the start of
	// minor fasts (Yom Kippur Katan included). Mutually exclusive with
	// FastStartMins. If neither is set, minor fasts begin at Alot HaShachar,
	// 16.1 degrees. Tish'a B'Av begins at sunset the evening before and is
	// not affected.
	FastStartDeg float64
	// FastStartMins is the number of minutes before sunrise for the start of
	// minor fasts (typical values are 72 or 90). Mutually exclusive with
	// FastStartDeg.
	FastStartMins int
	// FastEndDeg is the solar depression, in degrees, for the end of minor
	// fasts (Yom Kippur Katan included). Commonly-used values are 7.083 (3
	// medium-sized stars) and 6.45 (Rabbi Yechiel Michel Tucazinsky).
	// Mutually exclusive with FastEndMins. If neither is set, minor fasts end
	// at 7.083 degrees in the Diaspora, or 15 minutes after sunset in Israel
	// (Rabbi Deblitzky's practice). Tish'a B'Av is not affected; see
	// TishaBavEndDeg.
	FastEndDeg float64
	// FastEndMins is the number of minutes after sunset for the end of minor
	// fasts. Mutually exclusive with FastEndDeg. Tish'a B'Av is not affected;
	// see TishaBavEndMins.
	FastEndMins int
	// TishaBavEndDeg is the solar depression, in degrees, for the end of
	// Tish'a B'Av. Mutually exclusive with TishaBavEndMins. If neither is
	// set, Tish'a B'Av ends at 6.45 degrees (Rabbi Yechiel Michel
	// Tucazinsky), in Israel too.
	TishaBavEndDeg float64
	// TishaBavEndMins is the number of minutes after sunset for the end of
	// Tish'a B'Av. Mutually exclusive with TishaBavEndDeg.
	TishaBavEndMins int
	// Sedrot adds the weekly Torah portion (Parashat HaShavua) on Saturdays.
	Sedrot bool
	// IL selects the Israeli holiday and Torah reading schedule. A Location
	// in Israel implies it.
	IL bool
	// NoMinorFast suppresses minor fasts.
	NoMinorFast bool
	// NoModern suppresses modern holidays.
	NoModern bool
	// NoRoshChodesh suppresses Rosh Chodesh.
	NoRoshChodesh bool
	// ShabbatMevarchim adds Shabbat Mevarchim HaChodesh, on the Saturday
	// before Rosh Chodesh.
	ShabbatMevarchim bool
	// NoSpecialShabbat suppresses the special Shabbatot.
	NoSpecialShabbat bool
	// NoHolidays suppresses the regular holidays.
	NoHolidays bool
	// DafYomi adds the Babylonian Talmud Daf Yomi.
	DafYomi bool
	// MishnaYomi adds Mishna Yomi.
	MishnaYomi bool
	// YerushalmiYomi adds the Jerusalem Talmud Daf Yomi.
	YerushalmiYomi bool
	// NachYomi adds Nach Yomi.
	NachYomi bool
	// DailyLearning adds daily learning schedules by registered name (e.g.
	// "929", "rambam1", "rambam3"). Names are case-insensitive and resolved
	// through the dailylearning registry; a schedule provider such as
	// github.com/hebcal/learning must be imported to register them. Unknown
	// or unregistered names are silently ignored.
	DailyLearning []string
	// YerushalmiEdition is the edition of Yerushalmi Yomi, Vilna (the
	// default) or Schottenstein.
	YerushalmiEdition YerushalmiEdition
	// Omer adds the days of the Omer.
	Omer bool
	// Molad adds an event announcing the molad, on the Saturday before Rosh
	// Chodesh.
	Molad bool
	// AddHebrewDates adds the Hebrew date for every day of the calendar.
	AddHebrewDates bool
	// AddHebrewDatesForEvents adds the Hebrew date for days that have some
	// event.
	AddHebrewDatesForEvents bool
	// Mask filters events by flag. When non-zero it takes the place of the
	// boolean options that select event streams.
	Mask event.Flags
	// YomKippurKatan adds Yom Kippur Katan (default false).
	// יוֹם כִּפּוּר קָטָן is a minor day of atonement occurring monthly on the day preceeding each Rosh Chodesh.
	//
	// Yom Kippur Katan is omitted in Elul (on the day before Rosh Hashanah),
	// Tishrei (Yom Kippur has just passed), Kislev (due to Chanukah)
	// and Nisan (fasting not permitted during Nisan).
	//
	// When Rosh Chodesh occurs on Shabbat or Sunday, Yom Kippur Katan is observed on the preceding Thursday.
	//
	// See https://en.wikipedia.org/wiki/Yom_Kippur_Katan#Practices
	YomKippurKatan bool
	// BeHaB adds the BeHaB fast days (default false): the Monday, Thursday
	// and Monday after Pesach (in Iyyar) and after Sukkot (in Cheshvan).
	// When the final Monday of Iyyar is 14 Iyyar (Pesach Sheni), that fast
	// is moved to 17 Iyyar.
	BeHaB bool
	// KiddushLevanaMaharil adds the latest time for Kiddush Levana each
	// month, calculated as halfway between one molad and the next (the
	// Maharil's opinion). Requires Location. See KiddushLevanaEvent.
	KiddushLevanaMaharil bool
	// Yizkor adds Yizkor (default false), the memorial prayer recited on Yom
	// Kippur, Shmini Atzeret, the last day of Pesach (Pesach VIII, or
	// Pesach VII in Israel) and the last day of Shavuot (Shavuot II, or
	// Shavuot in Israel). See YizkorEvent.
	Yizkor bool
	// Clock is the clock format for TimedEvent.Render(), the molad
	// announcement and the Kiddush Levana memo. The default, event.ClockAuto,
	// follows the location's country (or Israel when IL is set and there is no
	// location, else the United States): 12-hour time with an "am"/"pm" suffix
	// (e.g. "5:19am", "7:42pm") in countries that conventionally use it,
	// 24-hour "15:04" elsewhere. event.Clock12 and event.Clock24 force one or
	// the other everywhere. See ReformatTimeStr.
	Clock event.ClockFormat

	// The options below exist mainly for the hebcal command-line program.

	// SunriseSunset adds the sunrise and sunset times of every day.
	// Requires Location.
	SunriseSunset bool
	// DailyZmanim adds the day's zmanim every day (Alot HaShachar;
	// Misheyakir; Kriat Shema, sof zeman; Tefilah, sof zeman; Chatzot hayom;
	// Mincha Gedolah; Mincha Ketanah; Plag HaMincha; Tzeit HaKochavim).
	// Requires Location.
	DailyZmanim bool
	// Yahrzeits adds Yahrzeit reminders when the anniversary falls within
	// the date range.
	Yahrzeits []UserYahrzeit
	// UserEvents adds other personal events that recur on a Hebrew date,
	// when the date falls within the date range.
	UserEvents []UserEventSpec
	// WeeklyAbbreviated is the weekly abbreviated view: the Omer, daily
	// learning and non-date-specific zmanim are shown once a week, on the
	// weekday of the first day in the range.
	WeeklyAbbreviated bool
	// DailySedra adds the weekly sedra to every day: the Torah reading of
	// the Saturday on or immediately following that date. Nothing is added
	// when that Saturday has no weekly reading.
	DailySedra bool
}
