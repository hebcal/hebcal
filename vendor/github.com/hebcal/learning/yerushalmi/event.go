package yerushalmi

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
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/learning/dafyomi"
)

type yyomiEvent struct {
	Date hdate.HDate
	Daf  dafyomi.Daf
}

// NewYerushalmiYomiEvent returns a calendar event for the Yerushalmi Yomi
// (Jerusalem Talmud) on the given date.
func NewYerushalmiYomiEvent(hd hdate.HDate, daf dafyomi.Daf) event.CalEvent {
	return yyomiEvent{Date: hd, Daf: daf}
}

func (ev yyomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev yyomiEvent) Render(locale string) string {
	yerushalmiStr, _ := locales.LookupTranslation("Yerushalmi", locale)
	name, _ := locales.LookupTranslation(ev.Daf.Name, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		return yerushalmiStr + " " + name + " דף " + gematriya.Gematriya(ev.Daf.Blatt)
	}
	return yerushalmiStr + " " + name + " " + strconv.Itoa(ev.Daf.Blatt)
}

func (ev yyomiEvent) GetFlags() event.HolidayFlags {
	return event.YERUSHALMI_YOMI
}

func (ev yyomiEvent) GetEmoji() string {
	return ""
}

func (ev yyomiEvent) Basename() string {
	return ev.Daf.String()
}
