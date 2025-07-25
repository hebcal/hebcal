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

type HolidayFlags uint32

const (
	// Chag, yontiff, yom tov
	CHAG HolidayFlags = 1 << iota
	// Light candles 18 minutes before sundown
	LIGHT_CANDLES
	// End of holiday (end of Yom Tov)
	YOM_TOV_ENDS
	// Observed only in the Diaspora (chutz l'aretz)
	CHUL_ONLY
	// Observed only in Israel
	IL_ONLY
	// Light candles in the evening at Tzeit time (3 small stars)
	LIGHT_CANDLES_TZEIS
	// Candle-lighting for Chanukah
	CHANUKAH_CANDLES
	// Rosh Chodesh, beginning of a new Hebrew month
	ROSH_CHODESH
	// Minor fasts like Tzom Tammuz, Ta'anit Esther, ...
	MINOR_FAST
	// Shabbat Shekalim, Zachor, ...
	SPECIAL_SHABBAT
	// Weekly sedrot on Saturdays
	PARSHA_HASHAVUA
	// Daily page of Talmud (Bavli)
	DAF_YOMI
	// Days of the Omer
	OMER_COUNT
	// Yom HaShoah, Yom HaAtzma'ut, ...
	MODERN_HOLIDAY
	// Yom Kippur and Tish'a B'Av
	MAJOR_FAST
	// On the Saturday before Rosh Chodesh
	SHABBAT_MEVARCHIM
	// Molad
	MOLAD
	// Yahrzeit or Hebrew Anniversary
	USER_EVENT
	// Daily Hebrew date ("11th of Sivan, 5780")
	HEBREW_DATE
	// A holiday that's not major, modern, rosh chodesh, or a fast day
	MINOR_HOLIDAY
	// Evening before a major or minor holiday
	EREV
	// Chol haMoed, intermediate days of Pesach or Sukkot
	CHOL_HAMOED
	// Mishna Yomi
	MISHNA_YOMI
	// Yom Kippur Katan, minor day of atonement on the day preceding each Rosh Chodesh
	YOM_KIPPUR_KATAN
	// Zemanim, halachic times of day
	ZMANIM
	// Daily page of Jerusalem Talmud (Yerushalmi)
	YERUSHALMI_YOMI
	// Daily page of Nach (Nevi'im + Ketuvim)
	NACH_YOMI
)

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
}
