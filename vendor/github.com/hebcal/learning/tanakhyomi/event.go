package tanakhyomi

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
	"regexp"
	"strconv"
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/learning/internal/hebrew"
	"github.com/hebcal/learning/internal/sefaria"
	"github.com/hebcal/locales"
)

type tanakhYomiEvent struct {
	date    hdate.HDate
	Reading Reading
}

// NewTanakhYomiEvent returns a calendar event for the Tanakh Yomi reading
// on the given date.
func NewTanakhYomiEvent(hd hdate.HDate, reading Reading) event.CalEvent {
	return tanakhYomiEvent{date: hd, Reading: reading}
}

func (ev tanakhYomiEvent) Date() hdate.HDate {
	return ev.date
}

// Render returns the seder followed by its verse range, e.g.
// "Joshua Seder 9 (14:15-17:3)" or "Kings Seder 6 (I Kings 7:21-8:10)".
// The range names a book only when it differs from the seder's book.
// Hebrew locales use Hebrew book names and numerals throughout.
func (ev tanakhYomiEvent) Render(locale string) string {
	r := ev.Reading
	var s string
	if isHebrew(locale) {
		name, _ := locales.LookupTranslation(r.Name, locale)
		s = name + " ס׳ " + sederHebrew(r.Blatt)
	} else {
		s = translate(r.Name, locale) + " Seder " + r.Blatt
	}
	if r.Verses != "" {
		s += " (" + renderVerses(r, locale) + ")"
	}
	return s
}

// RenderBrief returns Render's text with "Seder" abbreviated, e.g.
// "Kings S6 (I Kings 7:21-8:10)". The verse range stays: it is the reading
// itself, not supplementary detail.
func (ev tanakhYomiEvent) RenderBrief(locale string) string {
	return strings.Replace(ev.Render(locale), " Seder ", " S", 1)
}

// Memo returns the seder's verse range, always naming its book, e.g.
// "Isaiah 1:1-4:2" or "I Kings 7:21-8:10", in every locale.
func (ev tanakhYomiEvent) Memo(locale string) string {
	v := ev.Reading.Verses
	if v != "" && v[0] >= '0' && v[0] <= '9' {
		return ev.Reading.Name + " " + v
	}
	return v
}

func isHebrew(locale string) bool {
	l := strings.ToLower(locale)
	return l == "he" || l == "he-x-nonikud"
}

// hebrewFallback covers book names missing from the locales package.
var hebrewFallback = map[string]map[string]string{
	"Nahum": {"he": "נַחוּם", "he-x-nonikud": "נחום"},
}

func translate(s, locale string) string {
	if name, ok := locales.LookupTranslation(s, locale); ok && name != "" {
		return name
	}
	if name, ok := hebrewFallback[s][strings.ToLower(locale)]; ok {
		return name
	}
	return s
}

// verseRange is a parsed Masoretic verse range such as "14:15-17:3",
// "I Kings 7:21-8:10" or, for a seder that runs into the next book,
// "Hosea 14:6-Joel 2:26". EndBook is empty when the range ends in Book.
// End is a bare verse ("41") when the range ends in the starting chapter.
type verseRange struct {
	Book, Start, EndBook, End string
}

var rangeRE = regexp.MustCompile(`^(?:(.+?) )?(\d+:\d+)(?:-(?:(.+?) )?(\d+(?::\d+)?))?$`)

func parseVerses(r Reading) verseRange {
	m := rangeRE.FindStringSubmatch(r.Verses)
	if m == nil {
		return verseRange{Book: r.Name, Start: r.Verses}
	}
	book := m[1]
	if book == "" {
		book = r.Name
	}
	return verseRange{Book: book, Start: m[2], EndBook: m[3], End: m[4]}
}

func renderVerses(r Reading, locale string) string {
	v := parseVerses(r)
	he := isHebrew(locale)
	num := func(s string) string {
		if !he {
			return s
		}
		parts := strings.Split(s, ":")
		for i, p := range parts {
			if n, err := strconv.Atoi(p); err == nil {
				parts[i] = hebrew.GematriyaNN(n)
			}
		}
		return strings.Join(parts, ":")
	}
	s := ""
	if v.Book != r.Name {
		s = translate(v.Book, locale) + " "
	}
	s += num(v.Start)
	if v.End != "" {
		s += "-"
		if v.EndBook != "" {
			s += translate(v.EndBook, locale) + " "
		}
		s += num(v.End)
	}
	return s
}

// sederHebrew renders a seder number in Hebrew. A plain number becomes
// gematriya; a split seder like "4.1" becomes gematriya(major) + minor.
func sederHebrew(blatt string) string {
	if n, err := strconv.Atoi(blatt); err == nil {
		return hebrew.GematriyaNN(n)
	}
	parts := strings.SplitN(blatt, ".", 2)
	if maj, err := strconv.Atoi(parts[0]); err == nil && len(parts) == 2 {
		return hebrew.GematriyaNN(maj) + parts[1]
	}
	return blatt
}

func (ev tanakhYomiEvent) Flags() event.Flags {
	return event.DailyLearning
}

func (ev tanakhYomiEvent) Emoji() string {
	return ""
}

// Desc returns the untranslated description, the same as Basename.
func (ev tanakhYomiEvent) Desc() string { return ev.Basename() }

func (ev tanakhYomiEvent) Basename() string {
	return ev.Render("en")
}

func (ev tanakhYomiEvent) Categories() []string {
	return []string{"tanakhYomi"}
}

// lastVerse is the final verse of each book in which a seder starts but
// does not end. Sefaria cannot link a range that spans two books, so the
// link for such a seder covers its part in the first book.
var lastVerse = map[string]string{
	"I Samuel":     "31:13",
	"I Kings":      "22:54",
	"Hosea":        "14:10",
	"Joel":         "4:21",
	"Amos":         "9:15",
	"Obadiah":      "1:21",
	"Micah":        "7:20",
	"Zephaniah":    "3:20",
	"Haggai":       "2:23",
	"Zechariah":    "14:21",
	"Ezra":         "10:44",
	"I Chronicles": "29:30",
}

// URL returns a link to sefaria.org for the seder's verse range, e.g.
// https://www.sefaria.org/Isaiah.55.13-58.13?lang=bi .
func (ev tanakhYomiEvent) URL() string {
	if ev.Reading.Verses == "" {
		return ""
	}
	v := parseVerses(ev.Reading)
	end := v.End
	if v.EndBook != "" {
		end = lastVerse[v.Book]
	}
	verses := v.Start
	if end != "" && end != v.Start {
		verses += "-" + end
	}
	return sefaria.URL(v.Book, strings.ReplaceAll(verses, ":", "."))
}
