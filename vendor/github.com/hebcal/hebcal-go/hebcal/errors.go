package hebcal

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

import "errors"

// The two kinds of mistake in a Options that HebrewCalendar reports most
// often. Each specific error below matches one of them with errors.Is, so a
// caller can test for the kind or for the exact option.
var (
	// ErrLocationRequired means an option that needs a place was set without
	// Options.Location.
	ErrLocationRequired = errors.New("option requires opts.Location")
	// ErrMutuallyExclusive means two options that cannot be combined were
	// both set: an offset in minutes and one in degrees for the same time.
	ErrMutuallyExclusive = errors.New("options are mutually exclusive")
)

// Errors returned by HebrewCalendar for invalid options. Compare with
// errors.Is. A location that cannot be used is reported with the errors of
// package zmanim instead (zmanim.ErrLatitude, zmanim.ErrLongitude,
// zmanim.ErrTimeZone).
var (
	// ErrCandleLightingLocation: CandleLighting without Location.
	// Matches ErrLocationRequired.
	ErrCandleLightingLocation error = &optionError{"opts.CandleLighting requires opts.Location", ErrLocationRequired}
	// ErrKiddushLevanaLocation: KiddushLevanaMaharil without Location.
	// Matches ErrLocationRequired.
	ErrKiddushLevanaLocation error = &optionError{"opts.KiddushLevanaMaharil requires opts.Location", ErrLocationRequired}
	// ErrSunriseSunsetLocation: SunriseSunset without Location.
	// Matches ErrLocationRequired.
	ErrSunriseSunsetLocation error = &optionError{"opts.SunriseSunset requires opts.Location", ErrLocationRequired}
	// ErrDailyZmanimLocation: DailyZmanim without Location.
	// Matches ErrLocationRequired.
	ErrDailyZmanimLocation error = &optionError{"opts.DailyZmanim requires opts.Location", ErrLocationRequired}

	// ErrHavdalahMinsAndDeg: HavdalahMins and HavdalahDeg both set.
	// Matches ErrMutuallyExclusive.
	ErrHavdalahMinsAndDeg error = &optionError{"opts.HavdalahMins and opts.HavdalahDeg are mutually exclusive", ErrMutuallyExclusive}
	// ErrFastStartMinsAndDeg: FastStartMins and FastStartDeg both set.
	// Matches ErrMutuallyExclusive.
	ErrFastStartMinsAndDeg error = &optionError{"opts.FastStartMins and opts.FastStartDeg are mutually exclusive", ErrMutuallyExclusive}
	// ErrFastEndMinsAndDeg: FastEndMins and FastEndDeg both set.
	// Matches ErrMutuallyExclusive.
	ErrFastEndMinsAndDeg error = &optionError{"opts.FastEndMins and opts.FastEndDeg are mutually exclusive", ErrMutuallyExclusive}
	// ErrTishaBavEndMinsAndDeg: TishaBavEndMins and TishaBavEndDeg both set.
	// Matches ErrMutuallyExclusive.
	ErrTishaBavEndMinsAndDeg error = &optionError{"opts.TishaBavEndMins and opts.TishaBavEndDeg are mutually exclusive", ErrMutuallyExclusive}

	// ErrStartRequiresEnd: only one of Start and End was set.
	ErrStartRequiresEnd = errors.New("opts.Start requires opts.End")
	// ErrInvalidHebrewYear: IsHebrewYear with a Year below 1.
	ErrInvalidHebrewYear = errors.New("invalid Hebrew year")
	// ErrInvalidMonth: Month is not zero and not a month from January to
	// December.
	ErrInvalidMonth = errors.New("opts.Month must be between 1 and 12")
	// ErrBeforeHebrewEpoch: the calendar would begin before 1 Tishrei of
	// Hebrew year 1 (in the Gregorian year -3761), where no Hebrew date
	// exists.
	ErrBeforeHebrewEpoch = errors.New("calendar begins before Hebrew year 1")
)

// optionError is an error about one option that also matches the kind of
// mistake it is.
type optionError struct {
	msg  string
	kind error
}

func (e *optionError) Error() string { return e.msg }

func (e *optionError) Is(target error) bool { return target == e.kind }
