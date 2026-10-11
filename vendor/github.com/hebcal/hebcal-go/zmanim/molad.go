package zmanim

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2026 Michael J. Radwin
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
	"github.com/hebcal/hebcal-go/molad"
)

// hebrewDate returns the Hebrew date of the civil date this Zmanim describes.
// The Gregorian fields are proleptic, as in [time.Time].
func (z *Zmanim) hebrewDate() hdate.HDate {
	return hdate.FromProlepticGregorian(z.Year, z.Month, z.Day)
}

// moladOf returns the molad of the Hebrew month containing hd.
func moladOf(hd hdate.HDate) molad.Molad {
	return molad.New(hd.Year(), hd.Month())
}

// moladBasedTime returns a molad-based time if it falls on this civil date
// (between the midnight that starts it and the one that ends it, inclusive),
// and the zero time otherwise.
//
// Kiddush Levana is said only at night. When both alos and tzais are non-zero
// and the time falls between them, it is moved to the adjacent night: tzais
// for the start of the period (techila), alos for its end. When either is the
// zero time no adjustment is made.
func (z *Zmanim) moladBasedTime(t, alos, tzais time.Time, techila bool) time.Time {
	lastMidnight := time.Date(z.Year, z.Month, z.Day, 0, 0, 0, 0, z.TimeZone)
	midnightTonight := time.Date(z.Year, z.Month, z.Day+1, 0, 0, 0, 0, z.TimeZone)
	if t.Before(lastMidnight) || t.After(midnightTonight) {
		return time.Time{}
	}
	if alos.IsZero() || tzais.IsZero() {
		return t.In(z.TimeZone)
	}
	if t.After(alos) && t.Before(tzais) {
		if techila {
			return tzais.In(z.TimeZone)
		}
		return alos.In(z.TimeZone)
	}
	return t.In(z.TimeZone)
}

// ZmanMolad returns the moment of the molad if it occurs on this date, and the
// zero time otherwise. A molad usually falls on the 29th or 30th of the
// preceding month, occasionally on the 1st, and very rarely on the 27th; in
// the extreme cases of Rapa Iti (Dec 2027) and Anadyr (Jan 2071) it falls on
// the 27th of Kislev and the 2nd of Shevat (Rabbi Dovid Heber, Shaarei
// Zmanim, chapter 4).
//
// For the traditional day of week, hour, minutes and chalakim, see
// [molad.Molad].
func (z *Zmanim) ZmanMolad() time.Time {
	hd := z.hebrewDate()
	if hd.Day() > 2 && hd.Day() < 27 {
		return time.Time{}
	}
	t := z.moladBasedTime(moladOf(hd).Instant(), time.Time{}, time.Time{}, true)
	// a molad at the end of the month belongs to the following month
	if t.IsZero() && hd.Day() > 26 {
		next := hdate.FromRD(hd.Abs() + 7)
		t = z.moladBasedTime(moladOf(next).Instant(), time.Time{}, time.Time{}, true)
	}
	return t
}

// TchilasZmanKidushLevana3Days returns the earliest time of Kiddush Levana
// according to Rabbeinu Yonah, 3 days after the molad, if it occurs on this
// date, and the zero time otherwise.
//
// If alos and tzais are both non-zero and the time falls between them, the
// following tzais is returned instead. Pass the zero time for either to get
// the unadjusted time.
func (z *Zmanim) TchilasZmanKidushLevana3Days(alos, tzais time.Time) time.Time {
	hd := z.hebrewDate()
	// In the extreme case of Rapa Iti in Dec 2027 this is the evening of the
	// 30th; in Anadyr in Jan 2071, the 5th.
	if hd.Day() > 5 && hd.Day() < 30 {
		return time.Time{}
	}
	t := z.moladBasedTime(moladOf(hd).TchilasZmanKidushLevana3Days(), alos, tzais, true)
	// the following month's time can fall on the evening of the 30th
	if t.IsZero() && hd.Day() == 30 {
		next := hdate.FromRD(hd.Abs() + 7)
		t = z.moladBasedTime(moladOf(next).TchilasZmanKidushLevana3Days(), time.Time{}, time.Time{}, true)
	}
	return t
}

// TchilasZmanKidushLevana7Days returns the earliest time of Kiddush Levana
// according to the opinions that it is not said until 7 days after the molad,
// if it occurs on this date, and the zero time otherwise.
//
// If alos and tzais are both non-zero and the time falls between them, the
// following tzais is returned instead.
func (z *Zmanim) TchilasZmanKidushLevana7Days(alos, tzais time.Time) time.Time {
	hd := z.hebrewDate()
	if hd.Day() < 4 || hd.Day() > 9 {
		return time.Time{}
	}
	return z.moladBasedTime(moladOf(hd).TchilasZmanKidushLevana7Days(), alos, tzais, true)
}

// SofZmanKidushLevanaBetweenMoldos returns the latest time of Kiddush Levana
// according to the Maharil, halfway between molad and molad, if it occurs on
// this date, and the zero time otherwise.
//
// If alos and tzais are both non-zero and the time falls between them, that
// morning's alos (the end of the last night) is returned instead.
func (z *Zmanim) SofZmanKidushLevanaBetweenMoldos(alos, tzais time.Time) time.Time {
	hd := z.hebrewDate()
	if hd.Day() < 11 || hd.Day() > 16 {
		return time.Time{}
	}
	return z.moladBasedTime(moladOf(hd).SofZmanKidushLevanaBetweenMoldos(), alos, tzais, false)
}

// SofZmanKidushLevana15Days returns the latest time of Kiddush Levana
// according to the Shulchan Aruch (Orach Chaim 426), 15 days after the molad,
// if it occurs on this date, and the zero time otherwise.
//
// If alos and tzais are both non-zero and the time falls between them, that
// morning's alos is returned instead.
func (z *Zmanim) SofZmanKidushLevana15Days(alos, tzais time.Time) time.Time {
	hd := z.hebrewDate()
	if hd.Day() < 11 || hd.Day() > 17 {
		return time.Time{}
	}
	return z.moladBasedTime(moladOf(hd).SofZmanKidushLevana15Days(), alos, tzais, false)
}
