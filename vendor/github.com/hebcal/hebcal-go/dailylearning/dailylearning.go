// Hebcal's dailylearning package is a registry for daily learning
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

var calendars = make(map[string]calendar)

// AddCalendar registers a new learning calendar with a case-insensitive name.
//
// The optional startDate marks the first date for which the calendar is
// valid; it is exposed via GetStartDate and is otherwise advisory (the
// CalendarFunc itself is responsible for returning nil before its cycle
// begins).
//
// Registering a name that already exists overwrites the previous entry.
func AddCalendar(name string, fn CalendarFunc, startDate ...hdate.HDate) {
	c := calendar{fn: fn}
	if len(startDate) > 0 {
		hd := startDate[0]
		c.startDate = &hd
	}
	calendars[strings.ToLower(name)] = c
}

// Lookup retrieves the learning event for a given Hebrew date from a
// registered calendar. Name matching is case-insensitive.
//
// It returns nil if the calendar isn't registered or if there is no
// learning on that day.
func Lookup(name string, hd hdate.HDate, il bool) event.CalEvent {
	c, ok := calendars[strings.ToLower(name)]
	if !ok {
		return nil
	}
	return c.fn(hd, il)
}

// Has reports whether a calendar with the given name has been registered.
// Name matching is case-insensitive.
func Has(name string) bool {
	_, ok := calendars[strings.ToLower(name)]
	return ok
}

// GetCalendars returns the names of all registered calendars (lowercased),
// sorted alphabetically.
func GetCalendars() []string {
	names := make([]string, 0, len(calendars))
	for name := range calendars {
		names = append(names, name)
	}
	sort.Strings(names)
	return names
}

// GetStartDate returns the first Hebrew date a calendar is valid for, as
// registered. The boolean is false if the calendar is unregistered or was
// registered without a start date.
func GetStartDate(name string) (hdate.HDate, bool) {
	c, ok := calendars[strings.ToLower(name)]
	if !ok || c.startDate == nil {
		return hdate.HDate{}, false
	}
	return *c.startDate, true
}
