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
	"github.com/hebcal/hebcal-go/yerushalmi"
	"github.com/hebcal/hebcal-go/zmanim"
)

// UserEvent is used for generating a non-yahrtzeit user event.
type UserEvent struct {
	Month hdate.HMonth // Hebrew month
	Day   int          // Day in month (1-30)
	Desc  string       // Description
}

// UserYahrzeit is used for generating a yahrtzeit reminder events.
type UserYahrzeit struct {
	Date time.Time // Gregorian Date of death
	Name string    // Name of deceased
}

// CalOptions are used by HebrewCalendar() to configure which events are returned
type CalOptions struct {
	/* latitude/longitude/tzid used for candle-lighting */
	Location *zmanim.Location
	/* Gregorian or Hebrew year */
	Year int
	/* to interpret year as Hebrew year */
	IsHebrewYear bool
	// disable Julian calendar transition and use the proleptic Gregorian calendar.
	// only used when specifying Year !=0 and IsHebrewYear == false
	NoJulian bool
	/* Gregorian month (to filter results to a single month) */
	Month time.Month
	/* generate calendar for multiple years (default 1) */
	NumYears int
	/* use specific start date (requires end date) */
	Start hdate.HDate
	/* use specific end date (requires start date) */
	End hdate.HDate
	/* calculate candle-lighting and havdalah times */
	CandleLighting bool
	/* minutes before sundown to light candles (default 18) */
	CandleLightingMins int
	// minutes after sundown for Havdalah (typical values are 42, 50, or 72).
	// If 0 (the default), calculate Havdalah according to Tzeit Hakochavim -
	// Nightfall (the point when 3 small stars are observable in the night time sky with
	// the naked eye).
	HavdalahMins int
	// degrees for solar depression for Havdalah.
	// Default is 8.5 degrees for 3 small stars.
	// Use 7.083 degress for 3 medium-sized stars.
	HavdalahDeg float64
	/* calculate parashah hashavua on Saturdays */
	Sedrot bool
	/* Israeli holiday and sedra schedule */
	IL bool
	/* suppress minor fasts */
	NoMinorFast bool
	/* suppress modern holidays */
	NoModern bool
	/* suppress Rosh Chodesh & Shabbat Mevarchim */
	NoRoshChodesh    bool
	ShabbatMevarchim bool
	/* suppress Special Shabbat */
	NoSpecialShabbat bool
	/* suppress regular holidays */
	NoHolidays bool
	/* include Babylonian Talmud Daf Yomi */
	DafYomi bool
	/* include Mishna Yomi */
	MishnaYomi bool
	/* include Jerusalem Talmud Daf Yomi */
	YerushalmiYomi bool
	/* include Nach Yomi */
	NachYomi bool
	/* Either the Vilna or Schottenstein edition of Yerushalmi Yomi */
	YerushalmiEdition yerushalmi.Edition
	/* include Days of the Omer */
	Omer bool
	/* include event announcing the molad */
	Molad bool
	/* print the Hebrew date for the entire date range */
	AddHebrewDates bool
	/* print the Hebrew date for dates with some events */
	AddHebrewDatesForEvents bool
	/* use bitmask from flags to filter events */
	Mask event.HolidayFlags
	// include Yom Kippur Katan (default false).
	// יוֹם כִּפּוּר קָטָן is a minor day of atonement occurring monthly on the day preceeding each Rosh Chodesh.
	//
	// Yom Kippur Katan is omitted in Elul (on the day before Rosh Hashanah),
	// Tishrei (Yom Kippur has just passed), Kislev (due to Chanukah)
	// and Nisan (fasting not permitted during Nisan).
	//
	// When Rosh Chodesh occurs on Shabbat or Sunday, Yom Kippur Katan is observed on the preceding Thursday.
	//
	// See https://en.wikipedia.org/wiki/Yom_Kippur_Katan#Practices
	YomKippurKatan bool
	// Whether to use 24-hour time (as opposed to 12-hour time) for
	// TimedEvent.Render().
	Hour24 bool

	//	-------- Begin: CLI legacy compatibility options  --------
	//  These options are primarily here for the command-line interface.
	//
	// Output sunrise and sunset times every day.
	SunriseSunset bool
	// Add zemanim daily (Alot HaShachar; Misheyakir; Kriat Shema, sof zeman;
	// Tefilah, sof zeman;  Chatzot hayom; Mincha Gedolah; Mincha Ketanah;
	// Plag HaMincha; Tzait HaKochavim).
	DailyZmanim bool
	// Add Yahrzeit reminders when the anniversary falls within the date range.
	Yahrzeits []UserYahrzeit
	// Add non-yahrtzeit Hebrew user event reminders when the anniversary falls within the date range.
	UserEvents []UserEvent
	// Weekly abbreviated view. Omer, dafyomi, and non-date-specific zemanim are shown once a week,
	// on the day which corresponds to the first day in the range.
	WeeklyAbbreviated bool
	// Add the weekly sedra to the output every day.  When this option is
	// invoked, every time a day is printed, the torah reading for the
	// Saturday on or immediately following that date is printed.  If
	// there is no reading for the next Saturday, then nothing is printed.
	DailySedra bool
	//
	//	-------- End: CLI legacy compatibility options  --------
}
