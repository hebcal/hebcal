// Package hdate converts between Hebrew and Gregorian dates.
//
// It also includes functions for calculating personal anniversaries
// (Yahrzeit, Birthday) according to the Hebrew calendar.
//
// See also https://en.wikipedia.org/wiki/Rata_Die
package hdate

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
	"encoding/json"
	"errors"
	"fmt"
	"regexp"
	"strconv"
	"strings"
	"sync/atomic"
	"time"

	"github.com/hebcal/greg"
)

// An HMonth specifies a Hebrew month of the year (Nisan = 1, Tishrei = 7, ...).
type HMonth int

const (
	// Nissan / ניסן
	Nisan HMonth = 1 + iota
	// Iyyar / אייר
	Iyyar
	// Sivan / סיון
	Sivan
	// Tammuz (sometimes Tamuz) / תמוז
	Tamuz
	// Av / אב
	Av
	// Elul / אלול
	Elul
	// Tishrei / תִשְׁרֵי
	Tishrei
	// Cheshvan / חשון
	Cheshvan
	// Kislev / כסלו
	Kislev
	// Tevet / טבת
	Tevet
	// Sh'vat / שבט
	Shvat
	// Adar or Adar Rishon / אדר
	Adar1
	// Adar Sheini (only on leap years) / אדר ב׳
	Adar2
)

var longMonthNames = []string{
	"",
	"Nisan",
	"Iyyar",
	"Sivan",
	"Tammuz",
	"Av",
	"Elul",
	"Tishrei",
	"Cheshvan",
	"Kislev",
	"Tevet",
	"Sh'vat",
	"Adar I",
	"Adar II",
}

var heAdar = [2]string{"אַדָר", "אדר"}
var heMonthNames = map[HMonth][2]string{
	Adar1:    {"אַדָר א׳", "אדר א׳"},
	Adar2:    {"אַדָר ב׳", "אדר ב׳"},
	Av:       {"אָב", "אב"},
	Cheshvan: {"חֶשְׁוָן", "חשון"},
	Elul:     {"אֱלוּל", "אלול"},
	Iyyar:    {"אִיָיר", "אייר"},
	Kislev:   {"כִּסְלֵו", "כסלו"},
	Nisan:    {"נִיסָן", "ניסן"},
	Shvat:    {"שְׁבָט", "שבט"},
	Sivan:    {"סִיוָן", "סיון"},
	Tamuz:    {"תַּמּוּז", "תמוז"},
	Tevet:    {"טֵבֵת", "טבת"},
	Tishrei:  {"תִשְׁרֵי", "תשרי"},
}

// String returns the English name of the month ("Nisan", "Iyyar", ...).
func (m HMonth) String() string {
	if Nisan <= m && m <= Adar2 {
		return longMonthNames[m]
	}
	return "%!HMonth(" + strconv.Itoa(int(m)) + ")"
}

// HDate represents a Hebrew date.
//
// To keep things simple, HDate represents just a day, month, and year.
//
// Although Hebrew dates begin at sunset, HDate does not attempt
// to represent any concept of the time of day. For halachic times,
// see the Zmanim interface.
type HDate struct {
	year  int    // Hebrew year
	month HMonth // Hebrew month (1-13)
	day   int    // Hebrew day of month (1-30)
	abs   int64  // R.D. absolute date
}

// Epoch is the Hebrew Calendar epoch using R.D. Rata Die numbers
const Epoch int64 = -1373428

// Avg year length in the cycle (19 solar years with 235 lunar months)
const avgHebrewYearDays float64 = 365.24682220597794

// IsLeapYear returns true if Hebrew year is a leap year.
func IsLeapYear(year int) bool {
	return (1+year*7)%19 < 7
}

// MonthsInYear returns the number of months in this Hebrew year
// (either 12 or 13 depending on leap year).
func MonthsInYear(year int) int {
	if IsLeapYear(year) {
		return 13
	}
	return 12
}

// DaysInYear computes the number of days in the Hebrew year.
//
// The year can be 353, 354, 355, 383, 384 or 385 days long.
func DaysInYear(year int) int {
	return int(elapsedDays(year+1) - elapsedDays(year))
}

// LongCheshvan returns true if Cheshvan is long (30 days) in Hebrew year.
func LongCheshvan(year int) bool {
	return DaysInYear(year)%10 == 5
}

// ShortKislev returns true if Kislev is short (29 days) in Hebrew year.
func ShortKislev(year int) bool {
	return DaysInYear(year)%10 == 3
}

// DaysInMonth returns the number of days in Hebrew month in a given year (29 or 30).
func DaysInMonth(month HMonth, year int) int {
	switch month {
	case Iyyar, Tamuz, Elul, Tevet, Adar2:
		return 29
	}
	if (month == Adar1 && !IsLeapYear(year)) ||
		(month == Cheshvan && !LongCheshvan(year)) ||
		(month == Kislev && ShortKislev(year)) {
		return 29
	}
	return 30
}

// edCache memoizes elapsedDays for the practical Hebrew year range.
// Each slot is written at most once with a deterministic value; atomic
// access keeps it race-free at the same cost as a plain load on x86/arm.
// Years outside [edCacheMin, edCacheMax] fall through to recompute.
const (
	edCacheMin = 5000
	edCacheMax = 6999
)

var edCache [edCacheMax - edCacheMin + 1]int32

// elapsedDays returns the number of days from the sunday prior to the start
// of the Hebrew calendar to the mean conjunction of Tishrei in Hebrew year.
func elapsedDays(year int) int64 {
	if year >= edCacheMin && year <= edCacheMax {
		idx := year - edCacheMin
		if n := atomic.LoadInt32(&edCache[idx]); n != 0 {
			return int64(n)
		}
		v := elapsedDays0(year)
		atomic.StoreInt32(&edCache[idx], int32(v))
		return v
	}
	return elapsedDays0(year)
}

func elapsedDays0(year int) int64 {
	prevYear := int64(year) - 1
	mElapsed := 235*(prevYear/19) + // Months in complete 19 year lunar (Metonic) cycles so far
		12*(prevYear%19) + // Regular months in this cycle
		(((prevYear%19)*7 + 1) / 19) // Leap months this cycle

	pElapsed := 204 + 793*(mElapsed%1080)

	hElapsed := 5 +
		12*mElapsed +
		793*(mElapsed/1080) +
		(pElapsed / 1080)

	parts := (pElapsed % 1080) + 1080*(hElapsed%24)

	day := 1 + 29*mElapsed + (hElapsed / 24)

	altDay := day

	if (parts >= 19440) ||
		(((day % 7) == 2) && (parts >= 9924) && !IsLeapYear(year)) ||
		(((day % 7) == 1) && (parts >= 16789) && IsLeapYear(int(prevYear))) {
		altDay = day + 1
	}

	if altDay%7 == 0 || altDay%7 == 3 || altDay%7 == 5 {
		return altDay + 1
	}
	return altDay
}

// ToRD converts Hebrew date to R.D. (Rata Die) fixed days.
// R.D. 1 is the imaginary date Monday, January 1, 1 on the Gregorian Calendar.
//
// Note also that R.D. = Julian Date − 1,721,424.5
//
// https://en.wikipedia.org/wiki/Rata_Die
func ToRD(year int, month HMonth, day int) int64 {
	tempabs := int64(day)
	if month < Tishrei {
		monthsInYear := HMonth(MonthsInYear(year))
		for m := Tishrei; m <= monthsInYear; m++ {
			tempabs += int64(DaysInMonth(m, year))
		}
		for m := Nisan; m < month; m++ {
			tempabs += int64(DaysInMonth(m, year))
		}
	} else {
		for m := Tishrei; m < month; m++ {
			tempabs += int64(DaysInMonth(m, year))
		}
	}
	return Epoch + elapsedDays(year) + tempabs - 1
}

// New creates a new HDate from year, Hebrew month, and day of month.
//
// Panics if Hebrew year is less than 1, if Hebrew month
// is not in the range [Tishrei..Adar2], or if Hebrew day
// is not in the range [1..30]
func New(year int, month HMonth, day int) HDate {
	if year < 1 {
		panic(fmt.Sprintf("invalid Hebrew year %d", year))
	}
	if month == Adar2 && !IsLeapYear(year) {
		month = Adar1
	}
	if month == Adar2+1 {
		month = Nisan
	}
	if month < Nisan || month > Adar2 {
		panic("invalid Hebrew Month " + month.String())
	}
	daysInMonth := DaysInMonth(month, year)
	if day < 1 || day > daysInMonth {
		panic(fmt.Sprintf("invalid Hebrew day %d", day))
	}
	return HDate{year: year, month: month, day: day}
}

func newYear(year int) int64 {
	return Epoch + elapsedDays(year)
}

// FromRD converts absolute Rata Die days to Hebrew date.
//
// Panics if rataDie is before the Hebrew epoch.
func FromRD(rataDie int64) HDate {
	if rataDie <= Epoch {
		panic(fmt.Sprintf("invalid R.D. date %d", rataDie))
	}
	approx := float64(rataDie-Epoch) / avgHebrewYearDays
	year := int(approx)
	for newYear(year) <= rataDie {
		year++
	}
	year--
	var month HMonth
	if rataDie < ToRD(year, Nisan, 1) {
		month = Tishrei
	} else {
		month = Nisan
	}
	for rataDie > ToRD(year, month, DaysInMonth(month, year)) {
		month++
	}
	day := 1 + rataDie - ToRD(year, month, 1)
	return HDate{year: year, month: month, day: int(day), abs: rataDie}
}

// FromGregorian creates an HDate from Gregorian year, month and day.
func FromGregorian(year int, month time.Month, day int) HDate {
	rataDie := greg.ToRD(year, month, day)
	return FromRD(rataDie)
}

// FromProlepticGregorian creates an HDate from Gregorian year, month and day,
// using the Proleptic Gregorian calendar.
func FromProlepticGregorian(year int, month time.Month, day int) HDate {
	rataDie := greg.ProlepticToRD(year, month, day)
	return FromRD(rataDie)
}

// FromTime creates an HDate from a Time object. Hours, minutes and seconds are ignored.
func FromTime(t time.Time) HDate {
	year, month, day := t.Date()
	rataDie := greg.ToRD(year, month, day)
	return FromRD(rataDie)
}

// Abs converts Hebrew date to R.D. (Rata Die) fixed days.
//
// R.D. 1 is the imaginary date Monday, January 1, 1 on the Gregorian Calendar.
func (hd *HDate) Abs() int64 {
	if hd.abs == 0 {
		hd.abs = ToRD(hd.Year(), hd.Month(), hd.Day())
	}
	return hd.abs
}

// Day returns the day of month (1-30) of hd.
func (hd HDate) Day() int {
	return hd.day
}

// Month returns the Hebrew month of hd.
func (hd HDate) Month() HMonth {
	return hd.month
}

// Year returns the Hebrew year of hd.
func (hd HDate) Year() int {
	return hd.year
}

// DaysInMonth returns the number of days in this date's month (29 or 30).
func (hd HDate) DaysInMonth() int {
	return DaysInMonth(hd.Month(), hd.Year())
}

// Greg converts this Hebrew Date to Gregorian year, month and day.
func (hd HDate) Greg() (int, time.Month, int) {
	return greg.FromRD(hd.Abs())
}

// ProlepticGreg converts this Hebrew Date to Proleptic Gregorian year, month and day.
func (hd HDate) ProlepticGreg() (int, time.Month, int) {
	return greg.ProlepticFromRD(hd.Abs())
}

// Gregorian converts this Hebrew Date to a Gregorian time object.
//
// Hours, minutes and seconds are set to 0, and timezone is set to UTC.
func (hd HDate) Gregorian() time.Time {
	year, month, day := hd.Greg()
	return time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
}

// Weekday returns the day of the week specified by hd.
func (hd HDate) Weekday() time.Weekday {
	abs := hd.Abs()
	return time.Weekday(((abs % 7) + 7) % 7)
}

// Prev returns the previous Hebrew date.
func (hd HDate) Prev() HDate {
	return FromRD(hd.Abs() - 1)
}

// Next returns the next Hebrew date.
func (hd HDate) Next() HDate {
	return FromRD(hd.Abs() + 1)
}

// IsLeapYear returns true if this HDate occurs during a Hebrew leap year.
func (hd HDate) IsLeapYear() bool {
	return IsLeapYear(hd.Year())
}

// MonthName returns a string representation of the month name.
//
// If locale is "he", returns Hebrew (e.g. "תִשְׁרֵי", "שְׁבָט", "אַדָר ב׳")
//
// Otherwise returns an English transliteration
// (e.g. "Tishrei", "Sh'vat", "Adar II").
func (hd HDate) MonthName(locale string) string {
	month := hd.Month()
	isAdar := month == Adar1 && !hd.IsLeapYear()
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		idx := 0
		if locale == "he-x-nonikud" {
			idx = 1
		}
		if isAdar {
			return heAdar[idx]
		}
		return heMonthNames[month][idx]
	}
	if isAdar {
		return "Adar"
	}
	return month.String()
}

// String returns a string representation of the Hebrew date
// in English transliteration (e.g. "15 Cheshvan 5769").
func (hd HDate) String() string {
	return strconv.Itoa(hd.Day()) + " " + hd.MonthName("en") + " " + strconv.Itoa(hd.Year())
}

// adarRegex matches the suffix that disambiguates "Adar I" / "Adar Rishon"
// from "Adar II" / "Adar Sheini" in MonthFromName.
var adarRegex = regexp.MustCompile("(?i)(1|[^i]i|a|א)(׳?)$")

// MonthFromName parses a Hebrew month string name to determine the month number.
//
// With the exception of Adar 1/Adar 2, Hebrew months are unique to their second letter.
//
//	N         Nisan  (November?)
//	I         Iyyar
//	E        Elul
//	C        Cheshvan
//	K        Kislev
//	Si Sh     Sivan, Shvat
//	Ta Ti Te Tamuz, Tishrei, Tevet
//	Av Ad    Av, Adar
//
//	אב אד אי אל   אב אדר אייר אלול
//	ח            חשון
//	ט            טבת
//	כ            כסלו
//	נ            ניסן
//	ס            סיון
//	ש            שבט
//	תמ תש        תמוז תשרי
//
//	Adar1, Adar 1, Adar I, אדר א׳
//	Adar2, Adar 2, Adar II, אדר ב׳
func MonthFromName(monthName string) (HMonth, error) {
	str := strings.ToLower(monthName)
	runes := []rune(str)
	strlen := len(runes)
	if strlen == 0 {
		return 0, errors.New("unable to parse month name")
	}
	var r1 rune
	if strlen > 1 {
		r1 = runes[1]
	}
	switch runes[0] {
	case 'n', 'נ':
		if r1 == 'o' {
			break // this catches "november"
		}
		return Nisan, nil
	case 'i':
		return Iyyar, nil
	case 'e':
		return Elul, nil
	case 'c', 'ח':
		return Cheshvan, nil
	case 'k', 'כ':
		return Kislev, nil
	case 's':
		switch r1 {
		case 'i':
			return Sivan, nil
		case 'h':
			return Shvat, nil
		}
	case 't':
		switch r1 {
		case 'a':
			return Tamuz, nil
		case 'i':
			return Tishrei, nil
		case 'e':
			return Tevet, nil
		}
	case 'a':
		switch r1 {
		case 'v':
			return Av, nil
		case 'd':
			if adarRegex.MatchString(monthName) {
				return Adar1, nil
			}
			return Adar2, nil // else assume sheini
		}
	case 'ס':
		return Sivan, nil
	case 'ט':
		return Tevet, nil
	case 'ש':
		return Shvat, nil
	case 'א':
		switch r1 {
		case 'ב':
			return Av, nil
		case 'ד':
			if adarRegex.MatchString(monthName) {
				return Adar1, nil
			}
			return Adar2, nil // else assume sheini
		case 'י':
			return Iyyar, nil
		case 'ל':
			return Elul, nil
		}
	case 'ת':
		switch r1 {
		case 'מ':
			return Tamuz, nil
		case 'ש':
			return Tishrei, nil
		}
	}
	return 0, errors.New("unable to parse month name")
}

// DayOnOrBefore returns the R.D. of the given day of the week on or
// before rataDie.
//
// Applying DayOnOrBefore to d+6 gives us the dayOfWeek on or after an
// absolute day rataDie.
//
// Similarly, applying it to d+3 gives the dayOfWeek nearest to
// rataDie, applying it to d-1 gives the dayOfWeek previous to
// rataDie, and applying it to d+7 gives the dayOfWeek following rataDie.
func DayOnOrBefore(dayOfWeek time.Weekday, rataDie int64) int64 {
	return rataDie - ((rataDie - int64(dayOfWeek)) % 7)
}

func onOrBefore(dayOfWeek time.Weekday, rataDie int64) HDate {
	return FromRD(DayOnOrBefore(dayOfWeek, rataDie))
}

// Before returns an HDate representing the dayOfWeek before
// the Hebrew date specified by hd.
func (hd HDate) Before(dayOfWeek time.Weekday) HDate {
	return onOrBefore(dayOfWeek, hd.Abs()-1)
}

// OnOrBefore returns an HDate corresponding to the dayOfWeek on or before
// the Hebrew date specified by hd.
func (hd HDate) OnOrBefore(dayOfWeek time.Weekday) HDate {
	return onOrBefore(dayOfWeek, hd.Abs())
}

// Nearest returns an HDate representing the nearest dayOfWeek to
// the Hebrew date specified by hd.
func (hd HDate) Nearest(dayOfWeek time.Weekday) HDate {
	return onOrBefore(dayOfWeek, hd.Abs()+3)
}

// OnOrAfter returns an HDate corresponding to the dayOfWeek on or after
// the Hebrew date specified by hd.
func (hd HDate) OnOrAfter(dayOfWeek time.Weekday) HDate {
	return onOrBefore(dayOfWeek, hd.Abs()+6)
}

// After returns an HDate corresponding to the dayOfWeek after
// the Hebrew date specified by hd.
func (hd HDate) After(dayOfWeek time.Weekday) HDate {
	return onOrBefore(dayOfWeek, hd.Abs()+7)
}

type hdJSON struct {
	Year  int    `json:"hy"` // Hebrew year
	Month string `json:"hm"` // Hebrew month ("Kislev", "Adar I", ...)
	Day   int    `json:"hd"` // Hebrew day of month (1-30)
}

// MarshalJSON implements the json.Marshaler interface.
func (hd HDate) MarshalJSON() ([]byte, error) {
	tmp := hdJSON{
		Year:  hd.year,
		Month: hd.MonthName("en"),
		Day:   hd.day,
	}
	return json.Marshal(tmp)
}

// UnmarshalJSON implements the json.Unmarshaler interface.
func (hd *HDate) UnmarshalJSON(b []byte) error {
	var tmp hdJSON
	if err := json.Unmarshal(b, &tmp); err != nil {
		return err
	}
	month, err := MonthFromName(tmp.Month)
	if err != nil {
		return err
	}
	hd.year = tmp.Year
	hd.day = tmp.Day
	hd.month = month
	return nil
}
