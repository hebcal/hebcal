# gematriya

[![Build Status](https://app.travis-ci.com/hebcal/gematriya.svg?branch=main)](https://app.travis-ci.com/hebcal/gematriya)
[![Go Report Card](https://goreportcard.com/badge/github.com/hebcal/gematriya)](https://goreportcard.com/report/github.com/hebcal/gematriya)
[![GoDoc](https://pkg.go.dev/badge/github.com/hebcal/gematriya?status.svg)](https://pkg.go.dev/github.com/hebcal/gematriya)

Package gematriya provides a system of writing numbers as Hebrew
letters. Numerical values are represented using letters of the Hebrew
alef-bet (alphabet).

Example

```golang
import (
  "fmt"
  "github.com/hebcal/gematriya"
)

str := gematriya.Gematriya(5749)
fmt.Println(str)
// Output: תשמ״ט
```

When specifying years of the Hebrew calendar in the present
millennium, we omit the thousands (which is presently 5, ה). For
example, the Hebrew year 5782 is written as 782 (תשפ״ב) rather than
5782 (ה׳תשפ״ב).

| Value | Name | Letter |
| ------ | ------ | ------ |
| 1 | aleph | א |
| 2 | bet | ב |
| 3 | gimel | ג |
| 4 | dalet | ד |
| 5 | hay | ה |
| 6 | vav | ו |
| 7 | zayin | ז |
| 8 | khet | ח |
| 9 | tet | ט |
| 10 | yud | י |
| 20 | kaf | כ |
| 30 | lamed | ל |
| 40 | mem | מ |
| 50 | nun | נ |
| 60 | samech | ס |
| 70 | ayin | ע |
| 80 | pay | פ |
| 90 | tzadi | צ |
| 100 | kuf | ק |
| 200 | reish | ר |
| 300 | shin | ש |
| 400 | tav | ת |

Note that the numbers 15 and 16 are treated specially, which if
rendered as 10+5 or 10+6 would be a name of God, so they are normally
written ט״ו (tet-vav, 9+6) and ט״ז (tet-zayin, 9+7).
