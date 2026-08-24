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

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/learning/dafyomi"
	"github.com/hebcal/learning/internal/hebrew"
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

type yyomiEvent struct {
	Date    hdate.HDate
	Daf     dafyomi.Daf
	Edition Edition
}

// NewYerushalmiYomiEvent returns a calendar event for the Yerushalmi Yomi
// (Jerusalem Talmud) on the given date. edition is Vilna or Schottenstein;
// only the Vilna edition carries a sefaria.org URL.
func NewYerushalmiYomiEvent(hd hdate.HDate, daf dafyomi.Daf, edition Edition) event.CalEvent {
	return yyomiEvent{Date: hd, Daf: daf, Edition: edition}
}

func (ev yyomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev yyomiEvent) Render(locale string) string {
	yerushalmiStr, _ := locales.LookupTranslation("Yerushalmi", locale)
	name, _ := locales.LookupTranslation(ev.Daf.Name, locale)
	locale = strings.ToLower(locale)
	if locale == "he" || locale == "he-x-nonikud" {
		return yerushalmiStr + " " + name + " דף " + hebrew.GematriyaNN(ev.Daf.Blatt)
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

func (ev yyomiEvent) GetCategories() []string {
	return []string{"yerushalmi"}
}

// URL returns a link to sefaria.org for the daf, e.g.
// https://www.sefaria.org/Jerusalem_Talmud_Berakhot.1.5.9-14?lang=bi . Only
// the Vilna edition is mapped to Sefaria references; the Schottenstein
// edition and dapim without a mapping return "".
func (ev yyomiEvent) URL() string {
	if ev.Edition != Vilna {
		return ""
	}
	pageMap, ok := vilnaMap[ev.Daf.Name]
	if !ok {
		return ""
	}
	idx := ev.Daf.Blatt - 1
	if idx < 0 || idx >= len(pageMap) {
		return ""
	}
	verses0 := pageMap[idx]
	if verses0 == "" {
		return ""
	}
	verses := strings.ReplaceAll(verses0, ":", ".")
	return sefaria.URL("Jerusalem Talmud "+ev.Daf.Name, verses)
}
