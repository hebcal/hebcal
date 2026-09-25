// Hebcal's event package provides an interface for calendar events.
package event

import "github.com/hebcal/hdate"

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

type CalEvent interface {
	GetDate() hdate.HDate        // Holiday date of occurrence
	Render(locale string) string // Description (e.g. "Pesach III (CH''M)")
	GetFlags() HolidayFlags      // Event flag bitmask
	GetEmoji() string            // Holiday-specific emoji
	// Returns a simplified (untranslated) description for this event.
	// For example, HolidayEvent supports "Erev Pesach" => "Pesach",
	// and "Sukkot III (CH''M)" => "Sukkot".
	// For many holidays the basename and the event description are
	// the same.
	Basename() string
	// GetCategories returns the category and optional sub-categories for this
	// event, e.g. ["holiday", "major", "fast"] or ["roshchodesh"]. The first
	// element is the primary category.
	GetCategories() []string
}

// flagToCategory maps an event flag to its category and sub-categories, in
// priority order (the first matching flag wins). It mirrors the flagToCategory
// table in @hebcal/core.
var flagToCategory = []struct {
	flag HolidayFlags
	cats []string
}{
	{MAJOR_FAST, []string{"holiday", "major", "fast"}},
	{CHANUKAH_CANDLES, []string{"holiday", "minor"}},
	{HEBREW_DATE, []string{"hebdate"}},
	{MINOR_FAST, []string{"holiday", "fast"}},
	{MINOR_HOLIDAY, []string{"holiday", "minor"}},
	{MODERN_HOLIDAY, []string{"holiday", "modern"}},
	{MOLAD, []string{"molad"}},
	{OMER_COUNT, []string{"omer"}},
	{PARSHA_HASHAVUA, []string{"parashat"}},
	{ROSH_CHODESH, []string{"roshchodesh"}},
	{SHABBAT_MEVARCHIM, []string{"mevarchim"}},
	{SPECIAL_SHABBAT, []string{"holiday", "shabbat"}},
	{USER_EVENT, []string{"user"}},
}

// CategoriesFromFlags returns the category and sub-categories implied by an
// event's flag bitmask, matching the base Event.getCategories() in
// @hebcal/core. It returns ["unknown"] when no flag matches. The returned
// slice is a fresh copy that the caller may modify.
func CategoriesFromFlags(mask HolidayFlags) []string {
	for _, fc := range flagToCategory {
		if mask.HasAny(fc.flag) {
			return append([]string(nil), fc.cats...)
		}
	}
	return []string{"unknown"}
}
