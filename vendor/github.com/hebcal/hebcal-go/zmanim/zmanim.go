package zmanim

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original JavaScript version, Copyright (C) 2014 Eyal Schachter
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

	"github.com/nathan-osman/go-sunrise"
)

// Tzais (nightfall) based on the opinion of the Geonim calculated at
// the sun's position at 8.5° below the western horizon.
// https://kosherjava.com/zmanim/docs/api/com/kosherjava/zmanim/ComplexZmanimCalendar.html#getTzaisGeonim8Point5Degrees()
const Tzeit3SmallStars = 8.5

// Tzais (nightfall) based on the opinion of the
// Geonim calculated as 30 minutes after sunset during the equinox
// (on March 16, about 4 days before the astronomical equinox, the day that
// a solar hour is 60 minutes) in Yerushalayim.
// https://kosherjava.com/zmanim/docs/api/com/kosherjava/zmanim/ComplexZmanimCalendar.html#getTzaisGeonim7Point083Degrees()
const Tzeit3MediumStars = 7.083

// Zmanim are used to calculate halachic times
type Zmanim struct {
	Location *Location
	Year     int        // Gregorian year
	Month    time.Month // Gregorian month
	Day      int        // Gregorian day
	TimeZone *time.Location
}

// New makes an instance used for calculating various halachic times during this day.
//
// tzid should be a timezone identifier such as "America/Los_Angeles" or "Asia/Jerusalem".
//
// This function panics if the latitude or longitude are out of range, or if
// the timezone cannot be loaded.
func New(location *Location, date time.Time) Zmanim {
	year, month, day := date.Date()
	loc, err := time.LoadLocation(location.TimeZoneId)
	if err != nil {
		panic(err)
	}
	return Zmanim{Location: location, Year: year, Month: month, Day: day, TimeZone: loc}
}

func (z *Zmanim) inLoc(dt time.Time) time.Time {
	if dt.IsZero() {
		return dt
	}
	return dt.In(z.TimeZone)
}

// Sunset ("shkiah") calculates when the sun will set on the given day
// at the specified location.
//
// Sunset is defined as when the upper edge of the Sun disappears below
// the horizon (0.833° below horizon)
//
// Returns time.Time{} if there sun does not rise or set
func (z *Zmanim) Sunset() time.Time {
	_, set := sunrise.SunriseSunset(z.Location.Latitude, z.Location.Longitude, z.Year, z.Month, z.Day)
	return z.inLoc(set)
}

// Sunrise ("neitz haChama") is defined as when the upper edge of the
// Sun appears over the eastern horizon in the morning
// (0.833° above horizon).
func (z *Zmanim) Sunrise() time.Time {
	rise, _ := sunrise.SunriseSunset(z.Location.Latitude, z.Location.Longitude, z.Year, z.Month, z.Day)
	return z.inLoc(rise)
}

// TimeAtAngle returns when the center of the sun will be some angle
// below the horizon.
// The rising parameter chooses between the AM or PM time,
// as normally there are two such times.
func (z *Zmanim) TimeAtAngle(angle float64, rising bool) time.Time {
	morning, evening := sunrise.TimeOfElevation(z.Location.Latitude, z.Location.Longitude, -angle, z.Year, z.Month, z.Day)
	if rising {
		return z.inLoc(morning)
	} else {
		return z.inLoc(evening)
	}
}

// Civil dawn; Sun is 6° below the horizon in the morning
func (z *Zmanim) Dawn() time.Time {
	return z.TimeAtAngle(6.0, true)
}

// Civil dusk; Sun is 6° below the horizon in the evening
func (z *Zmanim) Dusk() time.Time {
	return z.TimeAtAngle(6.0, false)
}

// Hour returns the number of seconds in a halachic Hour.
// Calculated by taking the total time of daylight of a particular day,
// from sunrise until sunset and dividing it into twelve equal parts.
// A halachic Hour is thus known as a sha'ah zemanit,
// or proportional Hour, and varies by the season and even by the day.
func (z *Zmanim) Hour() float64 {
	rise, set := sunrise.SunriseSunset(z.Location.Latitude, z.Location.Longitude, z.Year, z.Month, z.Day)
	seconds := set.Unix() - rise.Unix()
	return float64(seconds) / 12.0
}

// GregEve returns the time of the previous day's sunset,
// which is the beginning of the Hebrew calendar day.
func (z *Zmanim) GregEve() time.Time {
	prev := time.Date(z.Year, z.Month, z.Day-1, 0, 0, 0, 0, z.TimeZone)
	year, month, day := prev.Date()
	zman := Zmanim{
		Location: z.Location,
		Year:     year,
		Month:    month,
		Day:      day,
		TimeZone: z.TimeZone,
	}
	return zman.Sunset()
}

// NightHour returns the number of seconds in a proportional night hour,
// where there are 12 night hours from yesterday's sunset to today's sunrise.
func (z *Zmanim) NightHour() float64 {
	set := z.GregEve()
	rise := z.Sunrise()
	seconds := rise.Unix() - set.Unix()
	return float64(seconds) / 12.0
}

// HourOffset returns sunrise plus some number of halachic hours.
func (z *Zmanim) HourOffset(hours float64) time.Time {
	rise := z.Sunrise()
	seconds := rise.Unix() + int64(z.Hour()*hours)
	return time.Unix(seconds, 0).In(z.TimeZone)
}

// NightHourOffset returns yesterday's sunset plus
// some number of proportional night hours.
func (z *Zmanim) NightHourOffset(hours float64) time.Time {
	set := z.GregEve()
	seconds := set.Unix() + int64(z.NightHour()*hours)
	return time.Unix(seconds, 0).In(z.TimeZone)
}

// Midday – Chatzot; Sunrise plus 6 halachic hours
func (z *Zmanim) Chatzot() time.Time {
	return z.HourOffset(6)
}

// ChatzotNight returns astronomical midnight,
// which is 6 proportional night hours before sunrise.
func (z *Zmanim) ChatzotNight() time.Time {
	rise := z.Sunrise()
	seconds := rise.Unix() - int64(z.NightHour()*6.0)
	return time.Unix(seconds, 0).In(z.TimeZone)
}

// Dawn – Alot haShachar; Sun is 16.1° below the horizon in the morning
func (z *Zmanim) AlotHaShachar() time.Time {
	return z.TimeAtAngle(16.1, true)
}

// Earliest talis & tefillin – Misheyakir; Sun is 11.5° below the horizon in the morning
func (z *Zmanim) Misheyakir() time.Time {
	return z.TimeAtAngle(11.5, true)
}

// Earliest talis & tefillin – Misheyakir Machmir; Sun is 10.2° below the horizon in the morning
func (z *Zmanim) MisheyakirMachmir() time.Time {
	return z.TimeAtAngle(10.2, true)
}

// Latest Shema (Gra); Sunrise plus 3 halachic hours, according to the Gra
func (z *Zmanim) SofZmanShma() time.Time {
	return z.HourOffset(3)
}

// Latest Shacharit (Gra); Sunrise plus 4 halachic hours, according to the Gra
func (z *Zmanim) SofZmanTfilla() time.Time {
	return z.HourOffset(4)
}

func (z *Zmanim) sofZmanMGA(hours float64) time.Time {
	alot72 := z.SunriseOffset(-72, false)
	tzeit72 := z.SunsetOffset(72, false)
	alot72sec := alot72.Unix()
	temporalHour := float64(tzeit72.Unix()-alot72sec) / 12.0 // sec in hour
	seconds := alot72sec + int64(hours*temporalHour)
	return time.Unix(seconds, 0).In(z.TimeZone)
}

// Latest Shema (MGA); Sunrise plus 3 halachic hours, according to Magen Avraham
func (z *Zmanim) SofZmanShmaMGA() time.Time {
	return z.sofZmanMGA(3)
}

// Latest Shacharit (MGA); Sunrise plus 4 halachic hours, according to Magen Avraham
func (z *Zmanim) SofZmanTfillaMGA() time.Time {
	return z.sofZmanMGA(4)
}

// Earliest Mincha – Mincha Gedola; Sunrise plus 6.5 halachic hours
func (z *Zmanim) MinchaGedola() time.Time {
	return z.HourOffset(6.5)
}

// Preferable earliest time to recite Minchah – Mincha Ketana; Sunrise plus 9.5 halachic hours
func (z *Zmanim) MinchaKetana() time.Time {
	return z.HourOffset(9.5)
}

// Plag haMincha; Sunrise plus 10.75 halachic hours
func (z *Zmanim) PlagHaMincha() time.Time {
	return z.HourOffset(10.75)
}

// Tzeit is defined as nightfall, when 3 stars are observable in the night sky with the naked eye.
//
// For 3 small stars use 8.5°
//
// For 3 medium stars use 7.083°
func (z *Zmanim) Tzeit(angle float64) time.Time {
	if angle == 0 {
		angle = Tzeit3SmallStars
	}
	return z.TimeAtAngle(angle, false)
}

const ThirteenFive time.Duration = -1 * time.Duration(13.5*float64(time.Minute))

// Rabbeinu Tam holds that bein hashmashos is a specific time between sunset and tzeis hakochavim
// One opinion on how to calculate this time is that it is 13.5 minutes before tzies 7.083
func (z *Zmanim) BeinHashmashos() time.Time {
	tzeis := z.Tzeit(Tzeit3MediumStars)
	if tzeis.IsZero() {
		return tzeis
	}
	return tzeis.Add(ThirteenFive)
}

func (z *Zmanim) riseSetOffset(t time.Time, offset int, roundTime bool) time.Time {
	if t.IsZero() {
		return t
	}
	year, month, day := t.Date()
	hour, min, sec := t.Clock()
	if roundTime {
		// For positive offsets only, round up to next minute if needed
		if offset > 0 && sec >= 30 {
			offset++
		}
		sec = 0
	}
	return time.Date(year, month, day, hour, min+offset, sec, 0, z.TimeZone)
}

// Returns sunrise + offset minutes (either positive or negative).
//
// If roundTime is true, rounds to the nearest minute (setting seconds to zero).
func (z *Zmanim) SunriseOffset(offset int, roundTime bool) time.Time {
	return z.riseSetOffset(z.Sunrise(), offset, roundTime)
}

// Returns sunset + offset minutes (either positive or negative).
//
// This function is used with a negative offset to calculate candle-lighting times,
// typically -18 minutes before sundown (or -40 in Jerusalem).
//
// This function can be used with a positive offset to calculate Tzeit (nightfall).
//
// For Havdalah according to Rabbeinu Tam, use 72, which approximates
// when 3 small stars are observable in the night sky with the naked eye.
// Other typical values include 50 minutes (3 small stars) or 42 minutes
// (3 medium stars).
//
// If roundTime is true, rounds to the nearest minute (setting seconds to zero).
func (z *Zmanim) SunsetOffset(offset int, roundTime bool) time.Time {
	return z.riseSetOffset(z.Sunset(), offset, roundTime)
}
