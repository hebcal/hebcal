/*
 * noaa-go - a Go port of the KosherJava Zmanim solar-position calculators
 * Copyright © 2004-2026 Eliyahu Hershfeld (original Java Zmanim API)
 * Go port Copyright © 2026 Michael J. Radwin
 *
 * Licensed under the GNU Lesser General Public License, version 2.1.
 */

package noaa

import "fmt"

// InvalidValueError is returned when a value passed to a constructor or setter
// is outside its permitted range. It corresponds to the IllegalArgumentException
// thrown by the original Java API.
type InvalidValueError struct {
	// Field is the name of the offending parameter.
	Field string
	// Message describes the constraint that was violated.
	Message string
}

func (e *InvalidValueError) Error() string {
	return fmt.Sprintf("noaa: invalid %s: %s", e.Field, e.Message)
}
