package hebcal

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2024 Michael J. Radwin
// isAssurBemlacha ported from the @hebcal/core JavaScript implementation.
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
	"time"

	"github.com/hebcal/hdate"

	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/zmanim"
)

// GetHolidaysOnDate returns the holidays observed on the given Hebrew date, for
// either the Diaspora (il=false) or Israel (il=true) schedule.
func GetHolidaysOnDate(hd hdate.HDate, il bool) []event.HolidayEvent {
	abs := hd.Abs()
	yearEvents := GetHolidaysForYear(hd.Year(), il)
	var result []event.HolidayEvent
	for _, ev := range yearEvents {
		if ev.Date.Abs() == abs {
			result = append(result, ev)
		}
	}
	return result
}

// lightCandlesMask matches @hebcal/core: an event that begins a candle-lighting
// (erev) period.
const lightCandlesMask = event.LIGHT_CANDLES | event.LIGHT_CANDLES_TZEIS

// isTomorrowShabbosOrYomTov reports whether the following day is Shabbat or a Yom
// Tov (i.e. the given day is erev Shabbat / erev Yom Tov / Yom Tov sheni).
func isTomorrowShabbosOrYomTov(dow time.Weekday, events []event.HolidayEvent) bool {
	if dow == time.Friday {
		return true
	}
	for _, ev := range events {
		if ev.GetFlags()&lightCandlesMask != 0 {
			return true
		}
	}
	return false
}

// isTodayAssurBemelacha reports whether the given day is Shabbat or a Yom Tov
// that has a melacha (work) prohibition.
func isTodayAssurBemelacha(dow time.Weekday, events []event.HolidayEvent) bool {
	if dow == time.Saturday {
		return true
	}
	for _, ev := range events {
		if ev.GetFlags()&event.CHAG != 0 {
			return true
		}
	}
	return false
}

// IsAssurBemlacha reports whether the given moment (date and time) falls within a
// period when melacha (work) is prohibited — that is, Shabbat or a Yom Tov.
//
// The Shabbat / Yom Tov window is taken to begin at sunset (shkiah) on the
// preceding day (erev Shabbat / erev Yom Tov / Yom Tov sheni) and to end at
// tzais (nightfall) on the day itself. Tzais is calculated using a solar
// depression of 8.5° for simplicity; consult a halachic authority for more
// stringent opinions.
//
// currentTime should be expressed in the location's time zone. il selects the
// Israel (true) or Diaspora (false) holiday schedule. useElevation includes the
// location's elevation when computing sunset (it has no effect on the
// degree-based tzais calculation).
//
// It returns an error if sunset (or tzais) cannot be computed for the location
// on that day, e.g. in polar regions.
func IsAssurBemlacha(currentTime time.Time, loc *zmanim.Location, il bool, useElevation bool) (bool, error) {
	z := zmanim.New(loc, currentTime)
	z.UseElevation = useElevation
	sunset := z.Sunset()
	if sunset.IsZero() {
		return false, errors.New("could not determine sunset")
	}
	hd := hdate.FromTime(currentTime)
	dow := hd.Weekday()
	events := GetHolidaysOnDate(hd, il)
	// erev Shabbat, Yom Tov or Yom Tov sheni and after shkiah
	if isTomorrowShabbosOrYomTov(dow, events) && !currentTime.Before(sunset) {
		return true, nil
	}
	// is Shabbat or Yom Tov and before tzais
	if isTodayAssurBemelacha(dow, events) {
		tzais := z.Tzeit(zmanim.Tzeit3SmallStars)
		if tzais.IsZero() {
			return false, errors.New("could not determine tzais")
		}
		return !currentTime.After(tzais), nil
	}
	return false, nil
}
