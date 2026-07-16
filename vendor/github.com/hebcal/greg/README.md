# greg

[![Build Status](https://github.com/hebcal/greg/actions/workflows/go.yml/badge.svg)](https://github.com/hebcal/greg/actions/workflows/go.yml)
[![GoDoc](https://pkg.go.dev/badge/github.com/hebcal/greg?status.svg)](https://pkg.go.dev/github.com/hebcal/greg)

Hebcal's greg package converts between Gregorian/Julian dates
and R.D. ([Rata Die](https://en.wikipedia.org/wiki/Rata_Die))
day numbers. Dates before September 14, 1752 are interpreted using the Julian calendar.

Example

```golang
package main

import (
	"fmt"
	"time"

	"github.com/hebcal/greg"
)

func main() {
	rataDie := greg.ToRD(1995, time.December, 17)
	fmt.Println(rataDie)
	// Output: 728644
}
```

## R.D. day numbers

An R.D. number counts elapsed days from a fixed point, using negative numbers
for days before it. Turning dates into plain integers means any two of them can
be compared or subtracted directly.

The fixed point is the Gregorian date December 31, 1 BCE. R.D. 1 is therefore
January 1 of year 1 CE — the same day the Julian calendar calls January 3,
1 CE.

That last point is the key to this package: both pairs of functions share one
day-numbering scale, so a given R.D. number always refers to the same physical
day. What differs is which *calendar* is used to name it.

```golang
greg.FromRD(1)           // year 1, January 3    (Julian labelling)
greg.ProlepticFromRD(1)  // year 1, January 1    (Gregorian labelling)
```

## Two calendars

The package offers two ways to interpret a calendar date.

**Gregorian/Julian** — `ToRD`, `FromRD`, `IsLeapYear`, `DaysIn`

This is the historical calendar as actually used in the British Empire (and
its American colonies). Dates from September 14, 1752 onwards are Gregorian;
earlier dates are Julian.

**Proleptic Gregorian** — `ProlepticToRD`, `ProlepticFromRD`

This applies the Gregorian rules to *all* dates, projecting them backwards
before the calendar was adopted. There is no Julian period. Use it when you
want a single uniform rule, or when interoperating with systems such as
ISO 8601 that assume a proleptic Gregorian calendar.

The two agree from September 14, 1752 onwards and diverge before it:

```golang
greg.ToRD(2024, time.March, 1)           // 738946
greg.ProlepticToRD(2024, time.March, 1)  // 738946  (identical)

greg.ToRD(1700, time.March, 1)           // 620618
greg.ProlepticToRD(1700, time.March, 1)  // 620607  (11 days apart)
```

### Three differences to be aware of

**1. The missing 11 days of September 1752.** When the Gregorian calendar was
adopted, September 3–13, 1752 were skipped: September 2 was followed directly
by September 14. `ToRD` **panics** on a date in that gap, because it never
occurred. `ProlepticToRD` accepts it, since the proleptic calendar has no gap.

```golang
greg.FromRD(639796)  // 1752-September-2
greg.FromRD(639797)  // 1752-September-14   <- the next day

greg.ToRD(1752, time.September, 10)           // panics: date is in the gap
greg.ProlepticToRD(1752, time.September, 10)  // 639793
```

**2. Leap year rules.** The Julian calendar has a leap year every 4 years with
no exceptions. The Gregorian calendar drops the century years that are not
divisible by 400. Because `IsLeapYear` follows the historical calendar, it
reports pre-1752 century years such as 1500 and 1700 as leap years — the
Julian rule — where a proleptic Gregorian calendar would not.

| Year | `IsLeapYear` (historical) | Proleptic Gregorian |
| ---- | ------------------------- | ------------------- |
| 1500 | true (Julian rule)        | false               |
| 1700 | true (Julian rule)        | false               |
| 1800 | false                     | false               |
| 2000 | true                      | true                |

**3. Year numbering.** `ToRD` uses historical numbering, in which there is no
year 0: year 1 CE is preceded directly by 1 BCE, passed as `-1`. Calling it
with year 0 **panics**. `ProlepticToRD` uses astronomical numbering, in which
year 0 is valid and denotes 1 BCE.

```golang
greg.ToRD(-1, time.January, 1)           // -367   (1 BCE)
greg.ToRD(0, time.January, 1)            // panics: there was no year 0

greg.ProlepticToRD(0, time.January, 1)   // -365   (year 0 == 1 BCE)
```

### Which should I use?

Use the Gregorian/Julian functions for historical dates as they were actually
written at the time — this is the default, and what Hebcal uses. Use the
proleptic functions when you need one uniform rule across all of history, or
when exchanging dates with software that assumes a proleptic Gregorian
calendar.
