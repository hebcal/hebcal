// Package learning registers all of Hebcal's daily learning schedules
// (Daf Yomi, Mishna Yomi, Nach Yomi, Yerushalmi Yomi, 929, Daily Rambam,
// Daf-a-Week, Perek Yomi, Daily Psalms, Pirkei Avot, and Tanakh Yomi)
// with the dailylearning registry in github.com/hebcal/hebcal-go.
//
// Import this package for its side effects to make every schedule
// available to hebcal-go's calendar generation:
//
//	import _ "github.com/hebcal/learning"
//
// To register only a subset of schedules, import the individual
// sub-packages (e.g. github.com/hebcal/learning/dafyomi) instead.
//
// This mirrors the relationship between the @hebcal/core and
// @hebcal/learning TypeScript packages.
package learning

import (
	_ "github.com/hebcal/learning/dafweekly"
	_ "github.com/hebcal/learning/dafyomi"
	_ "github.com/hebcal/learning/mishnayomi"
	_ "github.com/hebcal/learning/nachyomi"
	_ "github.com/hebcal/learning/nine29"
	_ "github.com/hebcal/learning/perekyomi"
	_ "github.com/hebcal/learning/pirkeiavot"
	_ "github.com/hebcal/learning/psalms"
	_ "github.com/hebcal/learning/rambam"
	_ "github.com/hebcal/learning/tanakhyomi"
	_ "github.com/hebcal/learning/yerushalmi"
)
