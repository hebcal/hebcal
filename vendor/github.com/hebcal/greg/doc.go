// Package greg converts between Gregorian/Julian dates and R.D. (Rata Die)
// day numbers.
//
// An R.D. number is an absolute number equal to the number of days that
// elapsed from a fixed point in time. Negative numbers represent days before
// that fixed point. This package uses the Gregorian date 12/31/1 BCE (or the
// equivalent Julian date 1/2/1 CE) as its fixed point.
//
// The Julian calendar is used before September 1752 and the Gregorian calendar
// thereafter. The days between 9/2/1752 and 9/14/1752 (exclusive) never
// occurred and are rejected as invalid.
//
// For conversions that use the Proleptic Gregorian calendar for all dates
// (extending the Gregorian rules backwards, with no Julian period and no
// missing days), see [ProlepticToRD] and [ProlepticFromRD].
//
// Note: comments in this package write dates in American format (mm/dd/yy).
package greg

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
