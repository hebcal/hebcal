# hdate

[![Build Status](https://github.com/hebcal/hdate/actions/workflows/go.yml/badge.svg)](https://github.com/hebcal/hdate/actions/workflows/go.yml)
[![Go Report Card](https://goreportcard.com/badge/github.com/hebcal/hdate)](https://goreportcard.com/report/github.com/hebcal/hdate)
[![GoDoc](https://pkg.go.dev/badge/github.com/hebcal/hdate?status.svg)](https://pkg.go.dev/github.com/hebcal/hdate)

Hebcal's `hdate` package converts between Hebrew and Gregorian dates.

It also includes functions for calculating personal anniversaries
(Yahrzeit, Birthday) according to the Hebrew calendar.

Internally, it uses R.D. ([Rata Die](https://en.wikipedia.org/wiki/Rata_Die))
day numbers.

Example

```golang
package main

import (
	"fmt"
	"time"

	"github.com/hebcal/hdate"
)

func main() {
	hd := hdate.New(5765, hdate.Adar2, 22)
	year, month, day := hd.Greg()
	fmt.Println(year, month, day)
	// Output: 2005 April 2

	hd2 := hdate.FromGregorian(2008, time.November, 13)
	fmt.Println(hd2)
	// Output: 15 Cheshvan 5769
}
```
