# greg

[![Build Status](https://github.com/hebcal/greg/actions/workflows/go.yml/badge.svg)](https://github.com/hebcal/greg/actions/workflows/go.yml)
[![Go Report Card](https://goreportcard.com/badge/github.com/hebcal/greg)](https://goreportcard.com/report/github.com/hebcal/greg)
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
