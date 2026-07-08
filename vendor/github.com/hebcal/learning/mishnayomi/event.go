package mishnayomi

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
	"strconv"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/locales"
)

type mishnaYomiEvent struct {
	Date   hdate.HDate
	Mishna MishnaPair
}

// NewMishnaYomiEvent returns a calendar event for the Mishna Yomi on the
// given date.
func NewMishnaYomiEvent(hd hdate.HDate, mishna MishnaPair) event.CalEvent {
	return mishnaYomiEvent{Date: hd, Mishna: mishna}
}

func (ev mishnaYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev mishnaYomiEvent) Render(locale string) string {
	m1 := ev.Mishna[0]
	m2 := ev.Mishna[1]
	tractate, _ := locales.LookupTranslation(m1.Tractate, locale)
	s := tractate + " " + strconv.Itoa(m1.Chap) + ":" + strconv.Itoa(m1.Verse) + "-"
	sameTractate := m1.Tractate == m2.Tractate
	if !sameTractate {
		tractate, _ := locales.LookupTranslation(m2.Tractate, locale)
		s += tractate + " "
	}
	if sameTractate && m2.Chap == m1.Chap {
		s += strconv.Itoa(m2.Verse)
	} else {
		s += strconv.Itoa(m2.Chap) + ":" + strconv.Itoa(m2.Verse)
	}
	return s
}

func (ev mishnaYomiEvent) GetFlags() event.HolidayFlags {
	return event.MISHNA_YOMI
}

func (ev mishnaYomiEvent) GetEmoji() string {
	return ""
}

func (ev mishnaYomiEvent) Basename() string {
	return ev.Mishna.String()
}

func (ev mishnaYomiEvent) GetCategories() []string {
	return []string{"mishnayomi"}
}
