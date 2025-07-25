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
	"errors"
	"strconv"
)

/*
GetYahrzeit calculates yahrzeit.
hyear must be after original date of death.
Returns an error when requested year precedes or is same as original year.

Hebcal uses the algorithm defined in "Calendrical Calculations"
by Edward M. Reingold and Nachum Dershowitz.

The customary anniversary date of a death is more complicated and depends
also on the character of the year in which the first anniversary occurs.
There are several cases:
  - If the date of death is Marcheshvan 30, the anniversary in general depends
    on the first anniversary; if that first anniversary was not Marcheshvan 30,
    use the day before Kislev 1.
  - If the date of death is Kislev 30, the anniversary in general again depends
    on the first anniversary — if that was not Kislev 30, use the day before
    Tevet 1.
  - If the date of death is Adar II, the anniversary is the same day in the
    last month of the Hebrew year (Adar or Adar II).
  - If the date of death is Adar I 30, the anniversary in a Hebrew year that
    is not a leap year (in which Adar only has 29 days) is the last day in
    Shevat.
  - In all other cases, use the normal (that is, same month number) anniversary
    of the date of death. [Calendrical Calculations p. 113]
*/
func GetYahrzeit(hyear int, date HDate) (HDate, error) {
	if hyear <= date.Year() {
		return HDate{}, errors.New("year " + strconv.Itoa(hyear) + " occurs on or before original date")
	}

	if date.Month() == Cheshvan && date.Day() == 30 && !LongCheshvan(date.Year()+1) {
		// If it's Heshvan 30 it depends on the first anniversary;
		// if that was not Heshvan 30, use the day before Kislev 1.
		date = FromRD(ToRD(hyear, Kislev, 1) - 1)
	} else if date.Month() == Kislev && date.Day() == 30 && ShortKislev(date.Year()+1) {
		// If it's Kislev 30 it depends on the first anniversary;
		// if that was not Kislev 30, use the day before Teveth 1.
		date = FromRD(ToRD(hyear, Tevet, 1) - 1)
	} else if date.Month() == Adar2 {
		// If it's Adar II, use the same day in last month of year (Adar or Adar II).
		date.month = HMonth(MonthsInYear(hyear))
	} else if date.Month() == Adar1 && date.Day() == 30 && !IsLeapYear(hyear) {
		// If it's the 30th in Adar I and year is not a leap year
		// (so Adar has only 29 days), use the last day in Shevat.
		date.day = 30
		date.month = Shvat
	}
	// In all other cases, use the normal anniversary of the date of death.

	// advance day to rosh chodesh if needed
	if date.Month() == Cheshvan && date.Day() == 30 && !LongCheshvan(hyear) {
		date.month = Kislev
		date.day = 1
	} else if date.Month() == Kislev && date.Day() == 30 && ShortKislev(hyear) {
		date.month = Tevet
		date.day = 1
	}

	return New(hyear, date.Month(), date.Day()), nil
}

/*
GetBirthdayOrAnniversary calculates a birthday or anniversary (non-yahrzeit).
hyear must be after original date of anniversary.
Returns an error when requested year precedes or is same as original year.

Hebcal uses the algorithm defined in "Calendrical Calculations"
by Edward M. Reingold and Nachum Dershowitz.

The birthday of someone born in Adar of an ordinary year or Adar II of
a leap year is also always in the last month of the year, be that Adar
or Adar II. The birthday in an ordinary year of someone born during the
first 29 days of Adar I in a leap year is on the corresponding day of Adar;
in a leap year, the birthday occurs in Adar I, as expected.

Someone born on the thirtieth day of Marcheshvan, Kislev, or Adar I
has his birthday postponed until the first of the following month in
years where that day does not occur. [Calendrical Calculations p. 111]
*/
func GetBirthdayOrAnniversary(hyear int, date HDate) (HDate, error) {
	origYear := date.Year()
	if hyear == origYear {
		return date, nil
	} else if hyear < origYear {
		return HDate{}, errors.New("year " + strconv.Itoa(hyear) + " occurs before original date")
	}
	isOrigLeap := IsLeapYear(origYear)
	month := date.Month()
	day := date.Day()
	if (month == Adar1 && !isOrigLeap) || (month == Adar2 && isOrigLeap) {
		month = HMonth(MonthsInYear(hyear))
	} else if month == Cheshvan && day == 30 && !LongCheshvan(hyear) {
		month = Kislev
		day = 1
	} else if month == Kislev && day == 30 && ShortKislev(hyear) {
		month = Tevet
		day = 1
	} else if month == Adar1 && day == 30 && isOrigLeap && !IsLeapYear(hyear) {
		month = Nisan
		day = 1
	}
	return New(hyear, month, day), nil
}
