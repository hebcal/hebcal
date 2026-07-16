package hebcal

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
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
	"fmt"
	"strings"
	"time"

	"github.com/hebcal/hdate"

	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/zmanim"
	"github.com/hebcal/locales"
)

// formatTime renders t as 24-hour "15:04", or as 12-hour time with the AM/PM
// suffix stripped.
func formatTime(t time.Time, opts *CalOptions) string {
	if opts.Hour24 {
		return t.Format("15:04")
	}
	kitchen := t.Format(time.Kitchen)
	return kitchen[:len(kitchen)-2]
}

// TimedEvent is used for Candle-lighting, Havdalah, and fast start/end
type TimedEvent struct {
	event.HolidayEvent
	EventTime    time.Time
	LinkedEvent  event.CalEvent
	sunsetOffset int
	opts         *CalOptions
}

func NewTimedEvent(hd hdate.HDate, desc string, flags event.HolidayFlags, t time.Time,
	sunsetOffset int, linkedEvent event.CalEvent, opts *CalOptions) TimedEvent {
	if t.IsZero() {
		return TimedEvent{}
	}
	var emoji string
	switch flags {
	case event.LIGHT_CANDLES, event.LIGHT_CANDLES_TZEIS:
		emoji = "🕯️"
	case event.YOM_TOV_ENDS:
		emoji = "✨"
	case event.CHANUKAH_CANDLES:
		emoji = chanukahEmoji
	}
	// Round to the nearest minute, matching @hebcal/core's TimedEvent
	// (this.eventTime = Zmanim.roundTime(eventTime)). SunsetOffset already
	// yields a whole minute, so in practice this only affects the degree-based
	// tzeit times (havdalah, fast begin/end, chametz). The ZMANIM daily-times
	// feature has no @hebcal/core equivalent and keeps its unrounded times.
	if flags != event.ZMANIM {
		t = roundToMinute(t)
	}
	return TimedEvent{
		HolidayEvent: event.HolidayEvent{
			Date:  hd,
			Desc:  desc,
			Flags: flags,
			Emoji: emoji,
		},
		EventTime:    t,
		LinkedEvent:  linkedEvent,
		opts:         opts,
		sunsetOffset: sunsetOffset,
	}
}

// roundToMinute rounds t to the nearest minute (>= 30 seconds rounds up),
// matching @hebcal/core Zmanim.roundTime.
func roundToMinute(t time.Time) time.Time {
	if t.IsZero() {
		return t
	}
	if t.Second() == 0 && t.Nanosecond() == 0 {
		return t
	}
	year, month, day := t.Date()
	hour, min, sec := t.Clock()
	if sec >= 30 {
		min++
	}
	return time.Date(year, month, day, hour, min, 0, 0, t.Location())
}

func (ev TimedEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev TimedEvent) Render(locale string) string {
	desc, _ := locales.LookupTranslation(ev.Desc, locale)
	if ev.Desc == "Havdalah" && ev.sunsetOffset != 0 {
		minStr, _ := locales.LookupTranslation("min", locale)
		desc = fmt.Sprintf("%s (%d %s)", desc, ev.sunsetOffset, minStr)
	}
	timeStr := formatTime(ev.EventTime, ev.opts)
	return fmt.Sprintf("%s: %s", desc, timeStr)
}

func (ev TimedEvent) GetFlags() event.HolidayFlags {
	return ev.Flags
}

func (ev TimedEvent) GetEmoji() string {
	return ev.Emoji
}

func (ev TimedEvent) Basename() string {
	return ev.Desc
}

// GetCategories returns the category and sub-categories for a timed event,
// keyed by its description, matching TimedEvent.getCategories() in
// @hebcal/core.
func (ev TimedEvent) GetCategories() []string {
	switch ev.Desc {
	case "Candle lighting":
		return []string{"candles"}
	case "Havdalah":
		return []string{"havdalah"}
	case "Fast begins", "Fast ends":
		return []string{"zmanim", "fast"}
	case "Finish eating chametz":
		return []string{"zmanim", "achilasChametz"}
	case "Biur Chametz":
		return []string{"zmanim", "biurChametz"}
	}
	return []string{"unknown"}
}

// newZmanim builds a Zmanim for the Gregorian date of hd at the options'
// Location, honoring opts.UseElevation so that sunrise/sunset-based times
// account for the location's elevation when requested. Degree-based zmanim are
// never affected by elevation.
func newZmanim(hd hdate.HDate, opts *CalOptions) zmanim.Zmanim {
	year, month, day := hd.Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	z := zmanim.New(opts.Location, gregDate)
	z.UseElevation = opts.UseElevation
	return z
}

func makeCandleEvent(hd hdate.HDate, opts *CalOptions, ev event.CalEvent) TimedEvent {
	havdalahTitle := false
	useHavdalahOffset := false
	dow := hd.Weekday()
	if dow == time.Saturday {
		useHavdalahOffset = true
	}
	flags := event.LIGHT_CANDLES
	if ev != nil {
		flags = ev.GetFlags()
		if dow != time.Friday {
			if (flags & (event.LIGHT_CANDLES_TZEIS | event.CHANUKAH_CANDLES)) != 0 {
				useHavdalahOffset = true
			} else if (flags & event.YOM_TOV_ENDS) != 0 {
				havdalahTitle = true
				useHavdalahOffset = true
			}
		}
	} else if dow == time.Saturday {
		havdalahTitle = true
		flags = event.LIGHT_CANDLES_TZEIS
	}
	// if offset is 0 or undefined, we'll use tzeit time
	offset := opts.CandleLightingMins
	if useHavdalahOffset {
		offset = opts.HavdalahMins
	}
	z := newZmanim(hd, opts)
	var eventTime time.Time
	if offset != 0 {
		eventTime = z.SunsetOffset(offset, true)
	} else {
		eventTime = z.Tzeit(opts.HavdalahDeg)
	}
	if eventTime.IsZero() {
		return TimedEvent{} // no sunset
	}
	desc := "Candle lighting"
	if havdalahTitle {
		desc = "Havdalah"
	}
	return NewTimedEvent(hd, desc, flags, eventTime, offset, ev, opts)
}

func makeChanukahCandleLighting(ev event.HolidayEvent, opts *CalOptions) TimedEvent {
	hd := ev.Date
	dow := hd.Weekday()
	if dow == time.Friday || dow == time.Saturday {
		timedEv := makeCandleEvent(hd, opts, ev)
		if (timedEv == TimedEvent{}) {
			return timedEv // no sunset
		}
		timedEv.Desc = ev.Desc
		timedEv.ChanukahDay = ev.ChanukahDay
		return timedEv
	}
	z := newZmanim(hd, opts)
	candleLightingTime := z.BeinHashmashos()
	if candleLightingTime.IsZero() {
		return TimedEvent{} // no sunset
	}
	return TimedEvent{
		HolidayEvent: ev,
		EventTime:    candleLightingTime,
		LinkedEvent:  ev,
		opts:         opts,
	}
}

func makeFastStartEnd(ev event.CalEvent, opts *CalOptions) (TimedEvent, TimedEvent) {
	hd := ev.GetDate()
	z := newZmanim(hd, opts)
	desc := ev.Render("en")
	flags := ev.GetFlags()
	var startEvent, endEvent TimedEvent
	if desc == "Erev Tish'a B'Av" {
		sunset := z.Sunset()
		startEvent = NewTimedEvent(hd, "Fast begins", flags, sunset, 0, ev, opts)
	} else if strings.HasPrefix(desc, "Tish'a B'Av") {
		tzeit := z.Tzeit(zmanim.Tzeit3MediumStars)
		endEvent = NewTimedEvent(hd, "Fast ends", flags, tzeit, 0, ev, opts)
	} else {
		dawn := z.AlotHaShachar()
		startEvent = NewTimedEvent(hd, "Fast begins", flags, dawn, 0, ev, opts)
		if hd.Weekday() != time.Friday && !(hd.Day() == 14 && hd.Month() == hdate.Nisan) {
			tzeit := z.Tzeit(zmanim.Tzeit3MediumStars)
			endEvent = NewTimedEvent(hd, "Fast ends", flags, tzeit, 0, ev, opts)
		}
	}
	return startEvent, endEvent
}

// makeErevPesachChametz returns the "Finish eating chametz" (sof zman achilat
// chametz) and "Biur Chametz" (sof zman biur chametz) events for Erev Pesach.
// It is modeled on hebcal-es6's makeErevPesachChametzEvents.
//
// Sof zman achilat chametz is the same as sof zman tefilla (Gra, sunrise plus 4
// halachic hours); sof zman biur chametz is one halachic hour later. When Erev
// Pesach falls on Shabbat, chametz cannot be burned on Shabbat, so the Biur
// Chametz event is emitted on the Friday before instead (see makeBiurChametz)
// and is omitted here.
func makeErevPesachChametz(ev event.CalEvent, opts *CalOptions) []event.CalEvent {
	hd := ev.GetDate()
	z := newZmanim(hd, opts)
	events := make([]event.CalEvent, 0, 2)
	achilas := z.SofZmanAchilasChametz()
	if achilas.IsZero() {
		return events
	}
	achilasEv := NewTimedEvent(hd, "Finish eating chametz", 0, achilas, 0, ev, opts)
	achilasEv.Emoji = "🍞"
	events = append(events, achilasEv)
	if hd.Weekday() != time.Saturday {
		if biurEv := makeBiurChametz(hd, opts); (biurEv != TimedEvent{}) {
			events = append(events, biurEv)
		}
	}
	return events
}

// makeBiurChametz returns the "Biur Chametz" (sof zman biur chametz) event for
// the given day, or an empty TimedEvent if the sun does not rise/set.
func makeBiurChametz(hd hdate.HDate, opts *CalOptions) TimedEvent {
	z := newZmanim(hd, opts)
	biur := z.SofZmanBiurChametz()
	if biur.IsZero() {
		return TimedEvent{}
	}
	biurEv := NewTimedEvent(hd, "Biur Chametz", 0, biur, 0, nil, opts)
	biurEv.Emoji = "🔥"
	return biurEv
}

type riseSetEvent struct {
	date hdate.HDate
	opts *CalOptions
}

func (ev riseSetEvent) GetDate() hdate.HDate {
	return ev.date
}

func (ev riseSetEvent) Render(locale string) string {
	z := newZmanim(ev.date, ev.opts)
	riseStr := formatTime(z.Sunrise(), ev.opts)
	setStr := formatTime(z.Sunset(), ev.opts)
	return fmt.Sprintf("Sunrise: %s; Sunset %s", riseStr, setStr)
}

func (ev riseSetEvent) GetFlags() event.HolidayFlags {
	return event.ZMANIM
}

func (ev riseSetEvent) GetEmoji() string {
	return ""
}

func (ev riseSetEvent) Basename() string {
	return ev.Render("en")
}

func (ev riseSetEvent) GetCategories() []string {
	return []string{"zmanim"}
}

func dailyZemanim(date hdate.HDate, opts *CalOptions) []event.CalEvent {
	z := newZmanim(date, opts)
	times := []struct {
		desc string
		t    time.Time
	}{
		{"Alot HaShachar", z.AlotHaShachar()},
		{"Misheyakir", z.Misheyakir()},
		{"Misheyakir Machmir", z.MisheyakirMachmir()},
		{"Sunrise", z.Sunrise()},
		{"Kriat Shema, sof zeman (MGA)", z.SofZmanShmaMGA()},
		{"Kriat Shema, sof zeman (GRA)", z.SofZmanShma()},
		{"Tefilah, sof zeman (MGA)", z.SofZmanTfillaMGA()},
		{"Tefilah, sof zeman (GRA)", z.SofZmanTfilla()},
		{"Chatzot HaYom", z.Chatzot()},
		{"Mincha Gedolah", z.MinchaGedola()},
		{"Mincha Ketanah", z.MinchaKetana()},
		{"Plag HaMincha", z.PlagHaMincha()},
		{"Sunset", z.Sunset()},
		{"Bein HaShemashot", z.BeinHashmashos()},
		{"Tzeit HaKochavim", z.Tzeit(zmanim.Tzeit3SmallStars)},
	}
	events := make([]event.CalEvent, 0, len(times))
	for _, zman := range times {
		if !zman.t.IsZero() {
			ev := NewTimedEvent(date, zman.desc, event.ZMANIM, zman.t, 0, nil, opts)
			events = append(events, ev)
		}
	}
	return events
}
