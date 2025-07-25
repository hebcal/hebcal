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
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/hebcal-go/zmanim"
)

func formatTime(t *time.Time, opts *CalOptions) string {
	if opts.Hour24 {
		return t.Format("15:04")
	} else {
		timeStr0 := t.Format(time.Kitchen)
		return timeStr0[0 : len(timeStr0)-2]
	}
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

func (ev TimedEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev TimedEvent) Render(locale string) string {
	desc, _ := locales.LookupTranslation(ev.Desc, locale)
	if ev.Desc == "Havdalah" && ev.sunsetOffset != 0 {
		minStr, _ := locales.LookupTranslation("min", locale)
		desc = fmt.Sprintf("%s (%d %s)", desc, ev.sunsetOffset, minStr)
	}
	timeStr := formatTime(&ev.EventTime, ev.opts)
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
	year, month, day := hd.Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	loc := opts.Location
	z := zmanim.New(loc, gregDate)
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
	loc := opts.Location
	year, month, day := hd.Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	z := zmanim.New(loc, gregDate)
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
	year, month, day := ev.GetDate().Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	loc := opts.Location
	z := zmanim.New(loc, gregDate)
	hd := ev.GetDate()
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

type riseSetEvent struct {
	date hdate.HDate
	opts *CalOptions
}

func (ev riseSetEvent) GetDate() hdate.HDate {
	return ev.date
}

func (ev riseSetEvent) Render(locale string) string {
	loc := ev.opts.Location
	year, month, day := ev.date.Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	z := zmanim.New(loc, gregDate)
	rise := z.Sunrise()
	set := z.Sunset()
	riseStr := formatTime(&rise, ev.opts)
	setStr := formatTime(&set, ev.opts)
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

func dailyZemanim(date hdate.HDate, opts *CalOptions) []event.CalEvent {
	loc := opts.Location
	year, month, day := date.Greg()
	gregDate := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
	z := zmanim.New(loc, gregDate)
	times := []struct {
		desc string
		t    time.Time
	}{
		{"Alot haShachar", z.AlotHaShachar()},
		{"Misheyakir", z.Misheyakir()},
		{"Misheyakir Machmir", z.MisheyakirMachmir()},
		{"Sunrise", z.Sunrise()},
		{"Kriat Shema, sof zeman (MGA)", z.SofZmanShmaMGA()},
		{"Kriat Shema, sof zeman (GRA)", z.SofZmanShma()},
		{"Tefilah, sof zeman (MGA)", z.SofZmanTfillaMGA()},
		{"Tefilah, sof zeman (GRA)", z.SofZmanTfilla()},
		{"Chatzot hayom", z.Chatzot()},
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
