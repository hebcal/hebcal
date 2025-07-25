// Hebcal's omer package calculates the Sefirat HaOmer.
package omer

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
	"strconv"
	"strings"

	"github.com/dustin/go-humanize"
	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/locales"
)

type OmerEvent struct {
	Date            hdate.HDate
	OmerDay         int
	WeekNumber      int
	DaysWithinWeeks int
}

func NewOmerEvent(hd hdate.HDate, omerDay int) OmerEvent {
	if omerDay < 1 || omerDay > 49 {
		panic("invalid omerDay")
	}
	week := ((omerDay - 1) / 7) + 1
	days := (omerDay % 7)
	if days == 0 {
		days = 7
	}
	return OmerEvent{Date: hd, OmerDay: omerDay, WeekNumber: week, DaysWithinWeeks: days}
}

func (ev OmerEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev OmerEvent) Render(locale string) string {
	locale = strings.ToLower(locale)
	dayOfTheOmer, _ := locales.LookupTranslation("day of the Omer", locale)
	switch locale {
	case "he", "he-x-nonikud":
		return gematriya.Gematriya(ev.OmerDay) + " " + dayOfTheOmer
	case "", "en", "sephardic", "ashkenazi",
		"ashkenazi_litvish", "ashkenazi_poylish", "ashkenazi_standard":
		return humanize.Ordinal(ev.OmerDay) + " " + dayOfTheOmer
	case "es":
		return strconv.Itoa(ev.OmerDay) + "º " + dayOfTheOmer
	}
	return strconv.Itoa(ev.OmerDay) + ". " + dayOfTheOmer
}

func (ev OmerEvent) GetFlags() event.HolidayFlags {
	return event.OMER_COUNT
}

func (ev OmerEvent) GetEmoji() string {
	number := ev.OmerDay
	var r rune
	if number <= 20 {
		r = rune(9312 + number - 1)
	} else if number <= 35 {
		// between 21 and 35 inclusive
		r = rune(12881 + number - 21)
	} else {
		// between 36 and 49 inclusive
		r = rune(12977 + number - 36)
	}
	return string(r)
}

func (ev OmerEvent) Basename() string {
	return "Omer"
}

func (ev OmerEvent) GetWeeks() int {
	if ev.DaysWithinWeeks == 7 {
		return ev.WeekNumber
	} else {
		return ev.WeekNumber - 1
	}
}

// adapted from pip hdate package (GPL)
// https://github.com/py-libhdate/py-libhdate/blob/master/hdate/date.py

var tens = []string{"", "עֲשָׂרָה", "עֶשְׂרִים", "שְׁלוֹשִׁים", "אַרְבָּעִים"}
var ones = []string{
	"",
	"אֶחָד",
	"שְׁנַיִם",
	"שְׁלוֹשָׁה",
	"אַרְבָּעָה",
	"חֲמִשָׁה",
	"שִׁשָׁה",
	"שִׁבְעָה",
	"שְׁמוֹנָה",
	"תִּשְׁעָה",
}

const shnei = "שְׁנֵי"
const yamim = "יָמִים"
const shneiYamim = shnei + " " + yamim
const shavuot = "שָׁבוּעוֹת"
const yom = "יוֹם"

var yomEchad = yom + " " + ones[1]

func todayIsHe(omer int) string {
	var ten = (omer / 10)
	var one = omer % 10
	var str = "הַיוֹם "
	if 10 < omer && omer < 20 {
		str += ones[one] + " עָשָׂר"
	} else if omer > 9 {
		str += ones[one]
		if one != 0 {
			str += " וְ"
		}
	}
	if omer > 2 {
		if (omer > 20) || (omer == 10) || (omer == 20) {
			str += tens[ten]
		}
		if omer < 11 {
			str += ones[one] + " " + yamim + " "
		} else {
			str += " " + yom + " "
		}
	} else if omer == 1 {
		str += yomEchad + " "
	} else { // omer == 2
		str += shneiYamim + " "
	}
	if omer > 6 {
		str = strings.TrimSpace(str) // remove trailing space before comma
		str += ", שְׁהֵם "
		var weeks = (omer / 7)
		var days = omer % 7
		if weeks > 2 {
			str += ones[weeks] + " " + shavuot + " "
		} else if weeks == 1 {
			str += "שָׁבוּעַ" + " " + ones[1] + " "
		} else { // weeks == 2
			str += shnei + " " + shavuot + " "
		}
		if days != 0 {
			str += "וְ"
			if days > 2 {
				str += ones[days] + " " + yamim + " "
			} else if days == 1 {
				str += yomEchad + " "
			} else { // days == 2
				str += shneiYamim + " "
			}
		}
	}
	str += "לָעוֹמֶר"
	return str
}

func (ev OmerEvent) TodayIs(locale string) string {
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		str := todayIsHe(ev.OmerDay)
		if locale == "he-x-nonikud" {
			str = locales.HebrewStripNikkud(str)
		}
		return str
	}
	totalDaysStr := "days"
	if ev.OmerDay == 1 {
		totalDaysStr = "day"
	}
	str := "Today is " + strconv.Itoa(ev.OmerDay) + " " + totalDaysStr
	if ev.WeekNumber > 1 || ev.OmerDay == 7 {
		day7 := ev.DaysWithinWeeks == 7
		numWeeks := ev.WeekNumber - 1
		if day7 {
			numWeeks = ev.WeekNumber
		}
		weeksStr := "weeks"
		if numWeeks == 1 {
			weeksStr = "week"
		}
		str += ", which is " + strconv.Itoa(numWeeks) + " " + weeksStr
		if !day7 {
			dayStr := "days"
			if ev.DaysWithinWeeks == 1 {
				dayStr = "day"
			}
			str += " and " + strconv.Itoa(ev.DaysWithinWeeks) + " " + dayStr
		}
	}
	return str + " of the Omer"
}

var sefirot = []string{
	"",
	"Lovingkindness",
	"Might",
	"Beauty",
	"Eternity",
	"Splendor",
	"Foundation",
	"Majesty",
}

var sefirotTranslit = []string{
	"",
	"Chesed",
	"Gevurah",
	"Tiferet",
	"Netzach",
	"Hod",
	"Yesod",
	"Malkhut",
}

func (ev OmerEvent) Sefira(locale string) string {
	weekStr := sefirot[ev.WeekNumber]
	dayWithinWeekStr := sefirot[ev.DaysWithinWeeks]
	weekNum2or6 := ev.WeekNumber == 2 || ev.WeekNumber == 6
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		week, _ := locales.LookupTranslation(weekStr, locale)
		dayWithinWeek, _ := locales.LookupTranslation(dayWithinWeekStr, locale)
		prefix := "שֶׁבְּ"
		if weekNum2or6 {
			prefix = "שֶׁבִּ"
		}
		if locale == "he-x-nonikud" {
			prefix = locales.HebrewStripNikkud(prefix)
		}
		return dayWithinWeek + " " + prefix + week
	} else if locale == "translit" {
		week := sefirotTranslit[ev.WeekNumber]
		dayWithinWeek := sefirotTranslit[ev.DaysWithinWeeks]
		prefix := "sheb'"
		if weekNum2or6 {
			prefix = "shebi"
		}
		return dayWithinWeek + " " + prefix + week
	} else {
		return dayWithinWeekStr + " within " + weekStr
	}
}
