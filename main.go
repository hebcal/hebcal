package main

import (
	"fmt"
	"regexp"
	"strconv"
	"strings"
	"time"

	"os"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/hebcal"
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/hebcal-go/yerushalmi"
	"github.com/hebcal/hebcal-go/zmanim"
	"github.com/spf13/pflag"
)

type RangeType int

const (
	YEAR RangeType = 0 + iota
	MONTH
	DAY
	TODAY
)

type GregDateFormat int

const (
	AMERICAN GregDateFormat = 1 + iota
	EURO
	ISO
)

var lang = "en"
var theYear = 0
var theGregMonth time.Month = 0
var theHebMonth hdate.HMonth = 0
var theDay = 0
var rangeType = YEAR
var tabs_sw = false
var weekday_sw = false
var gregDateOutputFormatCode_sw = AMERICAN
var today_sw = false
var noGreg_sw = false
var yearDigits_sw = false
var isTodayChag_sw = false
var verbose_sw = false
var noJulian_sw = false

func handleArgs() hebcal.CalOptions {
	calOptions := hebcal.CalOptions{}
	flags := pflag.NewFlagSet("hebcal", pflag.ContinueOnError)
	flags.Usage = func() {
		fmt.Fprintf(os.Stderr, "Usage: hebcal [options] [[ month [ day ]] year]\n")
		flags.PrintDefaults()
	}

	langList := strings.Join(locales.AllLocales, ", ")

	var (
		help            = flags.Bool("help", false, "print this help text")
		ashkenazi_sw    = flags.BoolP("ashkenazi", "a", false, "Use Ashkenazi Hebrew transliterations (alias for --lang=ashkenazi)")
		euroDates_sw    = flags.BoolP("euro-dates", "e", false, "Output 'European' dates -- DD.MM.YYYY")
		iso8601dates_sw = flags.BoolP("iso-8601", "g", false, "Output ISO 8601 dates -- YYYY-MM-DD")
		version_sw      = flags.Bool("version", false, "Show version number")
		cityNameArg     = flags.StringP("city", "C", "", "City for candle-lighting")
		utf8_hebrew_sw  = flags.BoolP("utf8-hebrew", "8", false, "Use UTF-8 Hebrew (alias for --lang=he)")
		schottenstein   = flags.Bool("schottenstein", false, "Use Schottenstein edition of Yerushalmi Yomi")
	)

	var coordinates string
	flags.StringVar(&coordinates, "geo", "",
		"Set location for solar calculations to decimal values LATITUDE and LONGITUDE.")
	var latitudeStr, longitudeStr, tzid string
	flags.StringVarP(&latitudeStr, "latitude", "l", "",
		"Set the latitude for solar calculations to XX degrees and YY minutes. Negative values are south.")
	flags.StringVarP(&longitudeStr, "longitude", "L", "",
		"Set the longitude for solar calculations to XX degrees and YY minutes. Negative values are EAST. The -l and -L switches must both be used, or not at all.")
	flags.StringVarP(&tzid, "timezone", "z", "",
		"Use specified timezone, overriding the -C (localize to city) switch")

	flags.BoolVarP(&today_sw, "today", "t", false, "Only output for today's date")
	flags.BoolVarP(&noGreg_sw, "today-brief", "T", false, "Print today's pertinent information")
	flags.BoolVarP(&isTodayChag_sw, "exit-if-chag", "X", false,
		"Exit silently with non-zero status if today is Shabbat or Chag; exit with 0 status if today is chol")
	flags.BoolVar(&verbose_sw, "verbose", false,
		"Verbose mode, currently used only for --exit-if-chag")
	var chagOnly_sw bool
	flags.BoolVar(&chagOnly_sw, "chag-only", false,
		"Output only Chag and Erev Chag events (when melakha/labor is prohibited)")
	flags.BoolVar(&noJulian_sw, "no-julian", false,
		"Disable use of Julian calendar for dates before 1752")

	flags.BoolVarP(&yearDigits_sw, "year-abbrev", "y", false, "Print only last two digits of year")
	flags.BoolVarP(&tabs_sw, "tabs", "r", false, "Tab delineated format")
	flags.BoolVarP(&weekday_sw, "weekday", "w", false, "Add day of the week")
	flags.BoolVarP(&calOptions.Hour24,
		"24hour", "E", false, "Output 24-hour times (e.g. 18:37 instead of 6:37)")
	flags.BoolVarP(&calOptions.SunriseSunset,
		"sunrise-and-sunset", "O", false, "Output sunrise and sunset times every day")
	flags.BoolVarP(&calOptions.DailyZmanim, "zmanim", "Z", false, "Output zemanim every day")
	flags.BoolVarP(&calOptions.Molad, "molad", "M", false, "Print the molad on Shabbat Mevorchim")
	flags.BoolVarP(&calOptions.WeeklyAbbreviated,
		"abbrev", "W", false, "Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.")

	flags.StringVar(&lang, "lang", "en", "Use LANG titles ("+langList+")")

	flags.BoolVarP(&calOptions.CandleLighting,
		"candlelighting", "c", false, "Print candlelighting times")
	flags.BoolVarP(&calOptions.AddHebrewDates,
		"add-hebrew-dates", "d", false, "Print the Hebrew date for the entire date range")
	flags.BoolVarP(&calOptions.AddHebrewDatesForEvents, "add-hebrew-dates-for-events", "D", false, "Print the Hebrew date for dates with some event")

	flags.BoolVarP(&calOptions.IsHebrewYear,
		"hebrew-date", "H", false, "Use Hebrew date ranges - only needed when e.g. hebcal -H 5373")

	flags.BoolVarP(&calOptions.DafYomi,
		"daf-yomi", "F", false, "Output the Daf Yomi (Bavli) for the entire date range")
	flags.BoolVar(&calOptions.MishnaYomi,
		"mishna-yomi", false, "Output the Mishna Yomi for the entire date range")
	flags.BoolVar(&calOptions.NachYomi,
		"nach-yomi", false, "Output the Nach Yomi for the entire date range")
	flags.BoolVar(&calOptions.YerushalmiYomi,
		"yerushalmi", false, "Output the Yerushalmi Yomi for the entire date range")
	flags.BoolVar(&calOptions.YomKippurKatan,
		"ykk", false, "Include Yom Kippur Katan, minor day of atonement occurring monthly on the day preceding each Rosh Chodesh")
	flags.BoolVar(&calOptions.ShabbatMevarchim, "mevarchim", false, "Include Shabbat Mevarchim HaChodesh")

	flags.BoolVarP(&calOptions.NoHolidays,
		"no-holidays", "h", false, "Suppress default holidays")
	flags.BoolVarP(&calOptions.NoRoshChodesh,
		"no-rosh-chodesh", "x", false, "Suppress Rosh Chodesh")

	flags.BoolVarP(&calOptions.IL,
		"israeli", "i", false, "Israeli holiday and sedra schedule")
	flags.BoolVar(&calOptions.NoModern,
		"no-modern", false, "Suppress modern holidays")
	flags.BoolVar(&calOptions.NoMinorFast, "no-mf", false, "Suppress minor fast days")
	flags.BoolVar(&calOptions.NoSpecialShabbat, "no-special", false, "Suppress Special Shabbatot")
	flags.BoolVarP(&calOptions.Omer,
		"omer", "o", false, "Add days of the Omer")
	flags.BoolVarP(&calOptions.Sedrot,
		"sedrot", "s", false, "Add the weekly sedra to the output on Saturdays")
	flags.BoolVarP(&calOptions.DailySedra,
		"daily-sedra", "S", false, "Add the weekly sedra to the output every day")

	flags.IntVarP(&calOptions.CandleLightingMins,
		"candle-mins", "b", 18, "Set candle-lighting to occur this many minutes before sundown")

	flags.IntVarP(&calOptions.HavdalahMins,
		"havdalah-mins", "m", 0, "Set Havdalah to occur this many minutes after sundown")
	flags.Float64VarP(&calOptions.HavdalahDeg,
		"havdalah-deg", "G", 0, "Set Havdalah to occur this many degrees below the horizon")

	flags.IntVar(&calOptions.NumYears,
		"years", 1, "Generate events for N years (default 1)")

	inFileName := flags.StringP("infile", "I", "", `Read extra events from FILENAME.
Each line specifies one holiday, with the format:
    MMMM DD Description
where MMMM is a string identifying the Hebrew month,
and DD is a number from 1 to 30.
Description is a newline-terminated string describing
the event. Events are printed regardless of the
-h (suppress holidays) switch.`)
	yahrzeitFileName := flags.StringP("yahrtzeit", "Y", "", `Read yahrtzeit dates from FILENAME.
Each line specifies one death-date, with the format:
    MM DD YYYY Description
where MM, DD and YYYY are the Gregorian date of death.
Description is a newline-terminated string to be printed
on the yahrtzeit. Events are printed regardless of the
-h (suppress holidays) switch.`)

	// Collect env opts and command-line args; env opts first so command-line overrides them
	var allArgs []string
	envOpts := os.Getenv("HEBCAL_OPTS")
	if envOpts != "" {
		spaceOrTab := func(c rune) bool {
			return c == ' ' || c == '\t'
		}
		allArgs = strings.FieldsFunc(envOpts, spaceOrTab)
	}
	allArgs = append(allArgs, os.Args[1:]...)

	if err := flags.Parse(allArgs); err != nil {
		fmt.Fprintf(os.Stderr, "%v\n", err)
		os.Exit(1)
	}

	if *help {
		displayHelp(flags)
		os.Exit(0)
	}
	if *version_sw {
		fmt.Printf("Hebcal version %s\n", Version)
		os.Exit(0)
	}

	if *euroDates_sw {
		gregDateOutputFormatCode_sw = EURO
	}
	if *iso8601dates_sw {
		gregDateOutputFormatCode_sw = ISO
	}
	if *schottenstein {
		calOptions.YerushalmiYomi = true
		calOptions.YerushalmiEdition = yerushalmi.Schottenstein
	}

	if *ashkenazi_sw && *utf8_hebrew_sw {
		fmt.Fprintf(os.Stderr, "Cannot specify both options -a and -8\n")
		os.Exit(1)
	} else if *ashkenazi_sw {
		lang = "ashkenazi"
	} else if *utf8_hebrew_sw {
		lang = "he"
	}
	checkLang()

	validCity := false
	if cityNameArg != nil && *cityNameArg != "" {
		city := zmanim.LookupCity(*cityNameArg)
		if city == nil {
			fmt.Fprintf(os.Stderr, "unknown city: %s. Use a nearby city or geographic coordinates.\n", *cityNameArg)
			os.Exit(1)
		}
		calOptions.Location = city
		calOptions.CandleLighting = true
		validCity = true
	} else {
		name := os.Getenv("HEBCAL_CITY")
		if name != "" {
			city := zmanim.LookupCity(name)
			if city != nil {
				calOptions.Location = city
				validCity = true
			}
		}
	}

	latitude := 0.0
	hasLat := false
	if latitudeStr != "" {
		latdeg := 0
		latmin := 0
		n, err := fmt.Sscanf(latitudeStr, "%d,%d", &latdeg, &latmin)
		if err != nil || n != 2 {
			fmt.Fprintf(os.Stderr, "unable to read latitude argument: %s\n", latitudeStr)
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		if (intAbs(latdeg) > 90) || latmin > 60 || latmin < 0 {
			fmt.Fprintf(os.Stderr, "Error, latitude argument out of range: %s\n", latitudeStr)
			os.Exit(1)
		}
		latmin = intAbs(latmin)
		if latdeg < 0 {
			latmin = -latmin
		}
		latitude = float64(latdeg) + (float64(latmin) / 60.0)
		hasLat = true
	}

	longitude := 0.0
	hasLong := false
	if longitudeStr != "" {
		longdeg := 0
		longmin := 0
		n, err := fmt.Sscanf(longitudeStr, "%d,%d", &longdeg, &longmin)
		if err != nil || n != 2 {
			fmt.Fprintf(os.Stderr, "unable to read longitude argument: %s\n", longitudeStr)
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		if (intAbs(longdeg) > 180) || longmin > 60 || longmin < 0 {
			fmt.Fprintf(os.Stderr, "Error, longitude argument out of range: %s\n", longitudeStr)
			os.Exit(1)
		}
		longmin = intAbs(longmin)
		if longdeg < 0 {
			longmin = -longmin
		}
		longitude = float64(-1*longdeg) + (float64(longmin) / -60.0)
		hasLong = true
	}

	if coordinates != "" {
		n, err := fmt.Sscanf(coordinates, "%f,%f", &latitude, &longitude)
		if err != nil || n != 2 {
			fmt.Fprintf(os.Stderr, "geo coordinates must be LATITUDE,LONGITUDE: %s\n", coordinates)
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		hasLat = true
		hasLong = true
	}

	if (hasLat && !hasLong) || (hasLong && !hasLat) {
		fmt.Fprintf(os.Stderr, "Error, you must enter BOTH the latitude and the longitude\n")
		os.Exit(1)
	}

	if hasLat && hasLong {
		if tzid == "" {
			fmt.Fprintf(os.Stderr, "Error, latitude and longitude requires -z/--timezone\n")
			os.Exit(1)
		}
		_, err := time.LoadLocation(tzid)
		if err != nil {
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		userLocation := zmanim.NewLocation("User Defined City", "", latitude, longitude, tzid)
		calOptions.Location = &userLocation
		calOptions.CandleLighting = true
		validCity = true
	}

	if !validCity && (calOptions.CandleLighting || calOptions.SunriseSunset || calOptions.DailyZmanim) {
		calOptions.Location = zmanim.LookupCity(defaultCity)
	}

	if calOptions.CandleLighting && calOptions.HavdalahDeg == 0.0 && calOptions.HavdalahMins == 0 {
		calOptions.HavdalahMins = 72
	}

	if noGreg_sw || isTodayChag_sw {
		today_sw = true
	}
	if noJulian_sw {
		calOptions.NoJulian = true
	}

	gregTodayYY, gregTodayMM, gregTodayDD := time.Now().Date()

	if today_sw {
		calOptions.AddHebrewDates = true
		rangeType = TODAY
		theGregMonth = gregTodayMM /* year and month specified */
		theDay = gregTodayDD       /* printc theDay of theMonth */
		calOptions.Omer = true
		calOptions.IsHebrewYear = false
	}

	if chagOnly_sw {
		calOptions.Mask = event.CHAG | event.LIGHT_CANDLES |
			event.LIGHT_CANDLES_TZEIS | event.YOM_TOV_ENDS
	}

	if *yahrzeitFileName != "" {
		calOptions.Yahrzeits = readYahrzeitFile(*yahrzeitFileName)
	}
	if *inFileName != "" {
		calOptions.UserEvents = readUserFile(*inFileName)
	}

	// Get the remaining positional parameters
	args := flags.Args()

	switch len(args) {
	case 0:
		if calOptions.IsHebrewYear {
			hd := hdate.FromGregorian(gregTodayYY, gregTodayMM, gregTodayDD)
			theYear = hd.Year()
		} else {
			theYear = gregTodayYY
		}
	case 1:
		arg0 := strings.TrimSpace(args[0])
		yy, err := strconv.Atoi(arg0)
		if err == nil {
			theYear = yy /* just year specified */
		} else {
			switch arg0 {
			case "help":
				displayHelp(flags)
				os.Exit(0)
			case "version":
				fmt.Printf("Hebcal version %s\n", Version)
				os.Exit(0)
			case "info":
				fmt.Printf("hebcal version %s\n\n", Version)
				fmt.Printf("Default city: %s\n", defaultCity)
				fmt.Println("Environment variable for default city: HEBCAL_CITY")
				fmt.Println("Environment variable for default options: HEBCAL_OPTS")
				os.Exit(0)
			case "cities":
				for _, city := range zmanim.AllCities() {
					fmt.Printf("%s (%.5f,%.5f  %s)\n",
						city.Name, city.Latitude, city.Longitude, city.TimeZoneId)
				}
				os.Exit(0)
			case "copying":
				fmt.Println(gplv2txt)
				fmt.Print(warranty)
				os.Exit(0)
			case "warranty":
				fmt.Print(warranty)
				os.Exit(0)
			default:
				regex := regexp.MustCompile(`^\d\d\d\d-\d\d-\d\d$`)
				if regex.MatchString(arg0) {
					theYear, _ = strconv.Atoi(arg0[0:4])
					gregMonth, _ := strconv.Atoi(arg0[5:7])
					theGregMonth = time.Month(gregMonth)
					theDay, _ = strconv.Atoi(arg0[8:10])
					rangeType = DAY
				} else {
					fmt.Fprintf(os.Stderr, "unrecognized command '%s'\n", args[0])
					fmt.Fprintf(os.Stderr, "Usage: hebcal [options] [[ month [ day ]] year]\n")
					os.Exit(1)
				}
			}
		}
	case 2:
		yy, err := strconv.Atoi(args[1])
		if err != nil {
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		theYear = yy
		parseGregOrHebMonth(&calOptions, theYear, args[0], &theGregMonth, &theHebMonth)
		rangeType = MONTH
	case 3:
		dd, err := strconv.Atoi(args[1])
		if err != nil {
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		theDay = dd
		yy, err := strconv.Atoi(args[2])
		if err != nil {
			fmt.Fprintf(os.Stderr, "%v\n", err)
			os.Exit(1)
		}
		theYear = yy
		parseGregOrHebMonth(&calOptions, theYear, args[0], &theGregMonth, &theHebMonth)
		rangeType = DAY
	default:
		flags.Usage()
		os.Exit(1)
	}

	if calOptions.NumYears != 1 && rangeType != YEAR {
		fmt.Fprintf(os.Stderr, "Sorry, --years option works only with entire-year calendars\n")
		os.Exit(1)
	} else if today_sw && rangeType != DAY && rangeType != TODAY {
		fmt.Fprintf(os.Stderr, "Sorry, --today option works only with single-day calendars\n")
		os.Exit(1)
	}
	return calOptions
}

func checkLang() {
	lang = strings.ToLower(lang)
	if lang != "en" {
		found := false
		for _, a := range locales.AllLocales {
			a = strings.ToLower(a)
			if a == lang {
				found = true
				break
			}
		}
		if !found {
			fmt.Fprintf(os.Stderr, "Unknown lang '%s'; using default\n", lang)
			lang = "en"
		}
	}
}

func parseGregOrHebMonth(calOptions *hebcal.CalOptions, theYear int, arg string, gregMonth *time.Month, hebMonth *hdate.HMonth) {
	mm, err := strconv.Atoi(arg)
	if err == nil {
		if calOptions.IsHebrewYear {
			fmt.Fprintf(os.Stderr, "Don't use numbers to specify Hebrew months.\n")
			os.Exit(1)
		}
		*gregMonth = time.Month(mm) /* gregorian month */
	} else {
		hm, err := hdate.MonthFromName(arg)
		if err == nil {
			*hebMonth = hm
			calOptions.IsHebrewYear = true /* automagically turn it on */
			if hm == hdate.Adar2 && !hdate.IsLeapYear(theYear) {
				*hebMonth = hdate.Adar1 /* silently fix this mistake */
			}
		} else {
			fmt.Fprintf(os.Stderr, "Unknown Hebrew month: %s.\n", arg)
			os.Exit(1)
		}
	}
}

func fromGregorian(year int, month time.Month, day int) hdate.HDate {
	if noJulian_sw {
		return hdate.FromProlepticGregorian(year, month, day)
	} else {
		return hdate.FromGregorian(year, month, day)
	}
}

func main() {
	calOptions := handleArgs()
	switch rangeType {
	case TODAY:
		calOptions.AddHebrewDates = true
		calOptions.Start = fromGregorian(theYear, theGregMonth, theDay)
		calOptions.End = calOptions.Start
	case DAY:
		calOptions.AddHebrewDates = true
		if calOptions.IsHebrewYear {
			calOptions.Start = hdate.New(theYear, theHebMonth, theDay)
		} else {
			calOptions.Start = fromGregorian(theYear, theGregMonth, theDay)
		}
		calOptions.End = calOptions.Start
	case MONTH:
		if calOptions.IsHebrewYear {
			calOptions.Start = hdate.New(theYear, theHebMonth, 1)
			calOptions.End = hdate.New(theYear, theHebMonth, calOptions.Start.DaysInMonth())
		} else {
			calOptions.Start = fromGregorian(theYear, theGregMonth, 1)
			calOptions.End = fromGregorian(theYear, theGregMonth, greg.DaysIn(theGregMonth, theYear))
		}
	case YEAR:
		calOptions.Year = theYear
	default:
		panic("Oh, NO! internal error #17q!")
	}

	events, err := hebcal.HebrewCalendar(&calOptions)
	if err != nil {
		fmt.Println(err)
		os.Exit(1)
	}

	if isTodayChag_sw {
		status, reason := isTodayChag(&calOptions, events)
		if reason != "" && verbose_sw {
			fmt.Println(reason)
		}
		os.Exit(status)
	}

	for _, ev := range events {
		gregDate := printGregDate(ev.GetDate())
		desc := ev.Render(lang)
		fmt.Printf("%s%s\n", gregDate, desc)
	}
}

func isTodayChag(calOptions *hebcal.CalOptions, events []event.CalEvent) (int, string) {
	if calOptions.Location == nil {
		for _, ev := range events {
			if (ev.GetFlags() & event.CHAG) != 0 {
				return 1, ev.Render(lang)
			}
		}
		if calOptions.Start.Weekday() == time.Saturday {
			reason, _ := locales.LookupTranslation("Shabbat", lang)
			return 1, reason
		}
		return 0, ""
	}

	loc, err := time.LoadLocation(calOptions.Location.TimeZoneId)
	if err != nil {
		panic(err)
	}
	calOptions.Hour24 = true

	now := time.Now().In(loc)
	nowSec := now.Unix()
	if rangeType != TODAY {
		hour, min, sec := now.Clock()
		now = time.Date(theYear, theGregMonth, theDay, hour, min, sec, 0, loc)
		nowSec = now.Unix()
	}

	// first pass: find today's candle-lighting and Havdalah events (if any)
	var candleLightingEv *hebcal.TimedEvent
	var havdalahEv *hebcal.TimedEvent
	var candlelightingSec, havdalahSec int64
	for _, ev := range events {
		timedEv, ok := ev.(hebcal.TimedEvent)
		if ok {
			if timedEv.Desc == "Candle lighting" {
				candleLightingEv = &timedEv
				candlelightingSec = candleLightingEv.EventTime.Unix()
			} else if timedEv.Desc == "Havdalah" {
				havdalahEv = &timedEv
				havdalahSec = havdalahEv.EventTime.Unix()
			}
		}
	}
	// If there's a candle-lighting or Havdalah event today, ignore other
	// events and check only if the current time is during the chag window
	if candlelightingSec != 0 && nowSec >= candlelightingSec {
		reason := now.Format(time.RFC1123Z) + " >= " + candleLightingEv.Render(lang)
		if candleLightingEv.LinkedEvent != nil {
			reason += " / " + candleLightingEv.LinkedEvent.Render(lang)
		}
		return 1, reason
	} else if calOptions.Start.Weekday() == time.Saturday && candlelightingSec != 0 && nowSec < candlelightingSec {
		reason, _ := locales.LookupTranslation("Shabbat", lang)
		return 1, reason
	} else if havdalahSec != 0 && nowSec >= havdalahSec {
		return 0, "" // Shabbat or Chag has already ended today
	} else if havdalahSec != 0 && nowSec < havdalahSec {
		reason := now.Format(time.RFC1123Z) + " < " + havdalahEv.Render(lang)
		if havdalahEv.LinkedEvent != nil {
			reason += " / " + havdalahEv.LinkedEvent.Render(lang)
		}
		return 1, reason
	} else {
		// Today still might be chag (e.g. RH first day, or perhaps
		// day 1 of a 2-day chag chutz l'aretz)
		for _, ev := range events {
			if (ev.GetFlags() & event.CHAG) != 0 {
				return 1, ev.Render(lang)
			}
		}
	}
	return 0, ""
}

func printGregDate(hd hdate.HDate) string {
	str := ""
	if !noGreg_sw {
		var year int
		var month time.Month
		var day int
		if noJulian_sw {
			year, month, day = hd.ProlepticGreg()
		} else {
			year, month, day = hd.Greg()
		}
		d := time.Date(year, month, day, 0, 0, 0, 0, time.UTC)
		if gregDateOutputFormatCode_sw == ISO {
			timeStr := d.Format(time.RFC3339)
			idx := strings.IndexRune(timeStr, 'T')
			str += timeStr[:idx]
		} else {
			if gregDateOutputFormatCode_sw == EURO {
				str += fmt.Sprintf("%d.%d.", day, month) /* dd.mm.yyyy */
			} else {
				str += fmt.Sprintf("%d/%d/", month, day) /* mm/dd/yyyy */
			}
			if yearDigits_sw {
				str += strconv.Itoa(year % 100)
			} else {
				str += strconv.Itoa(year)
			}
		}
		if tabs_sw {
			str += "\t"
		} else {
			str += " "
		}
	}
	if weekday_sw {
		tmp := hd.Weekday().String()
		str += tmp[0:3] + ", "
	}
	return str
}

func intAbs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

func displayHelp(flags *pflag.FlagSet) {
	fmt.Printf("Usage: hebcal [options] [[ month [ day ]] year]\n")
	flags.PrintDefaults()
	fmt.Print(usageSummary)
}

var usageSummary = `

hebcal help    -- Print this message.
hebcal info    -- Print version and localization data.
hebcal cities  -- Print a list of available cities.
hebcal warranty -- Tells you how there's NO WARRANTY for hebcal.
hebcal copying -- Prints the details of the GNU copyright.

Hebcal prints out Hebrew calendars one solar year at a time.
Given one argument, it will print out the calendar for that year.
Given two numeric arguments mm yyyy, it prints out the calendar for
month mm of year yyyy.

For example,
   hebcal -ho
will just print out the days of the omer for the current year.
Note: Use COMPLETE Years.  You probably aren't interested in
hebcal 93, but rather hebcal 1993.


Hebcal is copyright (c) 1994-2011 By Danny Sadinoff
Portions Copyright (c) 2011-2022 Michael J. Radwin. All rights reserved.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.
Type "hebcal copying" for more details.

Hebcal is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
Type "hebcal warranty" for more details.

"Free" above means freely distributed.  To donate money to support hebcal,
 see the paypal link at http://www.sadinoff.com/hebcal/
WWW:
            https://github.com/hebcal/hebcal-go
`
