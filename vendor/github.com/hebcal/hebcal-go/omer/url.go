package omer

import "fmt"

// URL returns the Omer day's page on hebcal.com, e.g.
// https://www.hebcal.com/omer/5786/33 .
//
// The site only has pages for Hebrew years it covers, so years outside that
// range return "" rather than a link that would 404.
func (ev OmerEvent) URL() string {
	hy := ev.date.Year()
	if hy < 5513 || hy > 6260 {
		return ""
	}
	return fmt.Sprintf("https://www.hebcal.com/omer/%d/%d", hy, ev.OmerDay)
}
