// Package dailylearning is a registry for daily learning
// calendars such as Daf Yomi, Mishna Yomi and Yerushalmi Yomi.
//
// This package intentionally contains no schedules of its own. Instead,
// it provides a plugin registry into which schedule providers (such as
// github.com/hebcal/learning) register their calendars by calling
// AddCalendar, typically from a package init function. This mirrors the
// relationship between the @hebcal/core and @hebcal/learning TypeScript
// packages.
package dailylearning

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
	"sort"
	"strings"
	"sync"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
)

// CalendarFunc calculates the learning event for a given Hebrew date.
//
// It returns nil when there is no learning on that day (for example,
// before the cycle began, or on a day the schedule skips).
type CalendarFunc func(hd hdate.HDate, il bool) event.CalEvent

type calendar struct {
	fn        CalendarFunc
	startDate *hdate.HDate
}

// The registry. Schedule providers normally register from package init
// functions, before anything reads it, but registering at run time is
// allowed, so every access goes through mu.
var (
	mu        sync.RWMutex
	calendars = make(map[string]calendar)
)

// AddCalendar registers a learning calendar under a case-insensitive name.
// Registering a name that already exists overwrites the previous entry.
// It panics if fn is nil.
//
// Use AddCalendarFrom for a calendar whose cycle began on a known date.
func AddCalendar(name string, fn CalendarFunc) {
	add(name, calendar{fn: fn})
}

// AddCalendarFrom is like AddCalendar, and also records startDate, the first
// date for which the calendar is valid. StartDate reports it; it is
// otherwise advisory, and fn itself is responsible for returning nil before
// its cycle begins.
func AddCalendarFrom(name string, fn CalendarFunc, startDate hdate.HDate) {
	add(name, calendar{fn: fn, startDate: &startDate})
}

func add(name string, c calendar) {
	if c.fn == nil {
		// A mistake in the registering package, found at start-up, like
		// database/sql.Register with a nil driver.
		panic("dailylearning: AddCalendar with a nil CalendarFunc")
	}
	mu.Lock()
	defer mu.Unlock()
	calendars[strings.ToLower(name)] = c
}

func get(name string) (calendar, bool) {
	mu.RLock()
	defer mu.RUnlock()
	c, ok := calendars[strings.ToLower(name)]
	return c, ok
}

// Lookup returns the event of the named calendar on hd, for the Israel (il)
// or Diaspora schedule. It returns nil if no calendar of that name is
// registered, or if the calendar has no learning on that day.
func Lookup(name string, hd hdate.HDate, il bool) event.CalEvent {
	c, ok := get(name)
	if !ok {
		return nil
	}
	// called outside the lock: fn may itself use the registry
	return c.fn(hd, il)
}

// Has reports whether a calendar of that name is registered.
func Has(name string) bool {
	_, ok := get(name)
	return ok
}

// Calendars returns the names of the registered calendars, in lower case
// and sorted.
func Calendars() []string {
	mu.RLock()
	names := make([]string, 0, len(calendars))
	for name := range calendars {
		names = append(names, name)
	}
	mu.RUnlock()
	sort.Strings(names)
	return names
}

// StartDate returns the first date for which the named calendar is valid,
// and false if the calendar is unknown or was registered without one.
func StartDate(name string) (hdate.HDate, bool) {
	c, ok := get(name)
	if !ok || c.startDate == nil {
		return hdate.HDate{}, false
	}
	return *c.startDate, true
}
