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

import (
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
)

// YizkorEvent is the Yizkor memorial prayer, recited on Yom Kippur, Shmini
// Atzeret, the last day of Pesach and the last day of Shavuot. Generated when
// Options.Yizkor is set.
//
// It satisfies event.Linked: LinkedEvent is the holiday on which Yizkor is
// recited, e.g. "Yom Kippur".
type YizkorEvent struct {
	date   hdate.HDate
	linked event.CalEvent
}

var _ event.Linked = YizkorEvent{}

// isYizkorDay reports whether Yizkor is recited on hd: Yom Kippur, Shmini
// Atzeret, Pesach VIII (Pesach VII in Israel) and Shavuot II (Shavuot in
// Israel).
func isYizkorDay(hd hdate.HDate, il bool) bool {
	pesach, shavuot := 22, 7
	if il {
		pesach, shavuot = 21, 6
	}
	switch hd.Month() {
	case hdate.Tishrei:
		return hd.Day() == 10 || hd.Day() == 22
	case hdate.Nisan:
		return hd.Day() == pesach
	case hdate.Sivan:
		return hd.Day() == shavuot
	}
	return false
}

// newYizkorEvent builds the Yizkor event for hd, linked to the first of that
// day's holidays.
func newYizkorEvent(hd hdate.HDate, holidays []event.HolidayEvent) YizkorEvent {
	ev := YizkorEvent{date: hd}
	for _, h := range holidays {
		if h.Date() == hd {
			ev.linked = h
			break
		}
	}
	return ev
}

func (ev YizkorEvent) Date() hdate.HDate { return ev.date }

// LinkedEvent returns the holiday on which Yizkor is recited, e.g. "Yom
// Kippur".
func (ev YizkorEvent) LinkedEvent() event.CalEvent { return ev.linked }

// Render returns "Yizkor", translated to locale.
func (ev YizkorEvent) Render(locale string) string {
	if s, ok := locales.LookupTranslation("Yizkor", locale); ok && s != "" {
		return s
	}
	return "Yizkor"
}

func (ev YizkorEvent) Flags() event.Flags { return event.Yizkor }
func (ev YizkorEvent) Emoji() string      { return "🕯️" }
func (ev YizkorEvent) Basename() string   { return "Yizkor" }

// Desc returns the untranslated description, "Yizkor".
func (ev YizkorEvent) Desc() string { return "Yizkor" }

// URL returns "": Yizkor has no page of its own on hebcal.com.
func (ev YizkorEvent) URL() string { return "" }

// Categories returns ["yizkor"].
func (ev YizkorEvent) Categories() []string { return event.CategoriesFromFlags(event.Yizkor) }
