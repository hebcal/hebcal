package zmanim

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2024 Michael J. Radwin
// Additional zmanim ported from the @hebcal/core JavaScript Zmanim class.
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

	noaa "github.com/hebcal/noaa-go"
)

// SeaLevelSunrise is sunrise (0.833° above the horizon) calculated without any
// elevation adjustment, regardless of the [Zmanim.UseElevation] setting.
func (z *Zmanim) SeaLevelSunrise() time.Time {
	t := z.calc.GetUTCSunrise(z.adjustedDate(), z.geo, noaa.GeometricZenith, false)
	return z.inLoc(z.getInstantFromTime(t, eventSunrise))
}

// SeaLevelSunset is sunset (0.833° below the horizon) calculated without any
// elevation adjustment, regardless of the [Zmanim.UseElevation] setting.
func (z *Zmanim) SeaLevelSunset() time.Time {
	t := z.calc.GetUTCSunset(z.adjustedDate(), z.geo, noaa.GeometricZenith, false)
	return z.inLoc(z.getInstantFromTime(t, eventSunset))
}

// addTemporalHours returns start plus the given number of proportional (halachic)
// hours, where one proportional hour is (end - start) / 12. It returns the zero
// time if either bound is the zero time (the event does not occur, e.g. in polar
// regions). The truncation matches the @hebcal/core convention.
func (z *Zmanim) addTemporalHours(start, end time.Time, hours float64) time.Time {
	if start.IsZero() || end.IsZero() {
		return time.Time{}
	}
	temporalHour := float64(end.Unix()-start.Unix()) / 12.0 // seconds per proportional hour
	seconds := start.Unix() + int64(temporalHour*hours)
	return time.Unix(seconds, 0).In(z.TimeZone)
}

// SofZmanShmaMGA16Point1 is the latest Shema (MGA), sunrise plus 3 halachic
// hours, where the day is measured from dawn to nightfall with both being 16.1°
// below the horizon.
func (z *Zmanim) SofZmanShmaMGA16Point1() time.Time {
	return z.addTemporalHours(z.TimeAtAngle(16.1, true), z.TimeAtAngle(16.1, false), 3)
}

// SofZmanShmaMGA19Point8 is the latest Shema (MGA), sunrise plus 3 halachic
// hours, where the day is measured from dawn to nightfall with both being 19.8°
// below the horizon.
func (z *Zmanim) SofZmanShmaMGA19Point8() time.Time {
	return z.addTemporalHours(z.TimeAtAngle(19.8, true), z.TimeAtAngle(19.8, false), 3)
}

// SofZmanTfillaMGA16Point1 is the latest Shacharit (MGA), sunrise plus 4 halachic
// hours, where the day is measured from dawn to nightfall with both being 16.1°
// below the horizon.
func (z *Zmanim) SofZmanTfillaMGA16Point1() time.Time {
	return z.addTemporalHours(z.TimeAtAngle(16.1, true), z.TimeAtAngle(16.1, false), 4)
}

// SofZmanTfillaMGA19Point8 is the latest Shacharit (MGA), sunrise plus 4 halachic
// hours, where the day is measured from dawn to nightfall with both being 19.8°
// below the horizon.
func (z *Zmanim) SofZmanTfillaMGA19Point8() time.Time {
	return z.addTemporalHours(z.TimeAtAngle(19.8, true), z.TimeAtAngle(19.8, false), 4)
}

// MinchaGedolaMGA is the earliest Mincha (MGA): 6.5 halachic hours after dawn,
// where the day starts 72 minutes before sunrise and ends 72 minutes after
// sunset. If [Zmanim.UseElevation] is set, elevation is included via sunrise and
// sunset.
func (z *Zmanim) MinchaGedolaMGA() time.Time {
	return z.addTemporalHours(z.Sunrise().Add(-72*time.Minute), z.Sunset().Add(72*time.Minute), 6.5)
}

// MinchaKetanaMGA is the preferable earliest time to recite Mincha (MGA): 9.5
// halachic hours after dawn, where the day starts 72 minutes before sunrise and
// ends 72 minutes after sunset. If [Zmanim.UseElevation] is set, elevation is
// included via sunrise and sunset.
func (z *Zmanim) MinchaKetanaMGA() time.Time {
	return z.addTemporalHours(z.Sunrise().Add(-72*time.Minute), z.Sunset().Add(72*time.Minute), 9.5)
}

// ---------------------------------------------------------------------------
// Baal Hatanya zmanim
//
// The Baal Hatanya (Shneur Zalman of Liadi) calculates the halachic day from
// netz amiti (true sunrise) to shkiah amiti (true sunset), each defined as when
// the center of the sun's disk is 1.583° below the horizon.
// ---------------------------------------------------------------------------

// zenithBaalHatanya is the depression angle (below the geometric horizon) used
// for the Baal Hatanya's netz amiti (sunrise) and shkiah amiti (sunset).
const zenithBaalHatanya = 1.583

// sunriseBaalHatanya returns the Baal Hatanya's netz amiti (true sunrise), when
// the center of the sun's disk is 1.583° below the horizon in the morning.
func (z *Zmanim) sunriseBaalHatanya() time.Time {
	return z.TimeAtAngle(zenithBaalHatanya, true)
}

// sunsetBaalHatanya returns the Baal Hatanya's shkiah amiti (true sunset), when
// the center of the sun's disk is 1.583° below the horizon in the evening.
func (z *Zmanim) sunsetBaalHatanya() time.Time {
	return z.TimeAtAngle(zenithBaalHatanya, false)
}

// shaahZmanisBaalHatanya returns the given number of proportional hours after
// netz amiti, where a proportional hour is 1/12 of the time from netz amiti to
// shkiah amiti.
func (z *Zmanim) shaahZmanisBaalHatanya(hours float64) time.Time {
	return z.addTemporalHours(z.sunriseBaalHatanya(), z.sunsetBaalHatanya(), hours)
}

// AlosBaalHatanya is the Baal Hatanya's dawn, when the sun is 16.9° below the
// horizon in the morning (72 minutes before netz amiti in Jerusalem around the
// equinox).
func (z *Zmanim) AlosBaalHatanya() time.Time {
	return z.TimeAtAngle(16.9, true)
}

// SofZmanShmaBaalHatanya is the latest Shema according to the Baal Hatanya: 3
// proportional hours after netz amiti.
func (z *Zmanim) SofZmanShmaBaalHatanya() time.Time {
	return z.shaahZmanisBaalHatanya(3)
}

// SofZmanTfilaBaalHatanya is the latest Shacharit according to the Baal Hatanya:
// 4 proportional hours after netz amiti.
func (z *Zmanim) SofZmanTfilaBaalHatanya() time.Time {
	return z.shaahZmanisBaalHatanya(4)
}

// MinchaGedolaBaalHatanya is the earliest Mincha according to the Baal Hatanya:
// 6.5 proportional hours after netz amiti.
func (z *Zmanim) MinchaGedolaBaalHatanya() time.Time {
	return z.shaahZmanisBaalHatanya(6.5)
}

// MinchaKetanaBaalHatanya is the preferable earliest Mincha according to the Baal
// Hatanya: 9.5 proportional hours after netz amiti.
func (z *Zmanim) MinchaKetanaBaalHatanya() time.Time {
	return z.shaahZmanisBaalHatanya(9.5)
}

// PlagHaminchaBaalHatanya is Plag HaMincha according to the Baal Hatanya: 10.75
// proportional hours after netz amiti.
func (z *Zmanim) PlagHaminchaBaalHatanya() time.Time {
	return z.shaahZmanisBaalHatanya(10.75)
}

// TzaisBaalHatanya is nightfall according to the Baal Hatanya, when the sun is 6°
// below the western geometric horizon after sunset.
func (z *Zmanim) TzaisBaalHatanya() time.Time {
	return z.TimeAtAngle(6, false)
}
