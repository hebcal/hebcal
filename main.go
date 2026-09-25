package main

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dailylearning"
	"github.com/hebcal/hebcal-go/event"
	"github.com/hebcal/hebcal-go/hebcal"
	"github.com/hebcal/hebcal-go/zmanim"
	_ "github.com/hebcal/learning"
	"github.com/hebcal/locales"
	getopt "github.com/pborman/getopt/v2"
)

// calRange is the span of dates to generate events for.
type calRange int

const (
	rangeYear calRange = iota
	rangeMonth
	rangeDay
	rangeToday
)

// dateFormat is the output format for Gregorian dates.
type dateFormat int

const (
	dateAmerican dateFormat = iota // MM/DD/YYYY
	dateEuro                       // DD.MM.YYYY
	dateISO                        // YYYY-MM-DD
)

var (
	lang                       = "en"
	theYear                    int
	theGregMonth               time.Month
	theHebMonth                hdate.HMonth
	theDay                     int
	rangeType                  calRange
	tabsSw                     bool
	weekdaySw                  bool
	gregDateOutputFormatCodeSw dateFormat
	todaySw                    bool
	noGregSw                   bool
	yearDigitsSw               bool
	isTodayChagSw              bool
	verboseSw                  bool
	noJulianSw                 bool
)

func handleArgs() hebcal.CalOptions {
	calOptions := hebcal.CalOptions{}
	opt := getopt.New()
	opt.SetProgram("hebcal")
	opt.SetParameters("[[ month [ day ]] year]")
	var (
		help           = opt.BoolLong("help", 0, "print this help text")
		ashkenaziSw    = opt.BoolLong("ashkenazi", 'a', "Use Ashkenazi Hebrew transliterations (alias for --lang=ashkenazi)")
		euroDatesSw    = opt.BoolLong("euro-dates", 'e', "Output 'European' dates -- DD.MM.YYYY")
		iso8601datesSw = opt.BoolLong("iso-8601", 'g', "Output ISO 8601 dates -- YYYY-MM-DD")
		versionSw      = opt.BoolLong("version", 0, "Show version number")
		cityNameArg    = opt.StringLong("city", 'C', "", "City for candle-lighting", "CITY")
		utf8HebrewSw   = opt.BoolLong("", '8', "Use UTF-8 Hebrew (alias for --lang=he)")
		schottenstein  = opt.BoolLong("schottenstein", 0, "Use Schottenstein edition of Yerushalmi Yomi")
		dailyLearning  = opt.ListLong("daily-learning", 0, "Output a daily learning schedule by name (e.g. 929, rambam1, rambam3); may be repeated", "NAME")
	)

	var coordinates string
	opt.FlagLong(&coordinates,
		"geo", 0,
		"Set location for solar calculations to decimal values LATITUDE and LONGITUDE.",
		"LATITUDE,LONGITUDE")
	var latitudeStr, longitudeStr, tzid string
	opt.FlagLong(&latitudeStr,
		"latitude", 'l', "Set the latitude for solar calculations to XX degrees and YY minutes. Negative values are south.", "XX,YY")
	opt.FlagLong(&longitudeStr,
		"longitude", 'L', "Set the longitude for solar calculations to XX degrees and YY minutes. Negative values are EAST. The -l and -L switches must both be used, or not at all.", "XX,YY")
	opt.FlagLong(&tzid, "timezone", 'z', "Use specified timezone, overriding the -C (localize to city) switch", "TIMEZONE")

	opt.FlagLong(&todaySw, "today", 't', "Only output for today's date")
	opt.FlagLong(&noGregSw, "today-brief", 'T', "Print today's pertinent information")
	opt.FlagLong(&isTodayChagSw, "exit-if-chag", 'X',
		"Exit silently with non-zero status if today is Shabbat or Chag; exit with 0 status if today is chol")
	opt.FlagLong(&verboseSw, "verbose", 0,
		"Verbose mode, currently used only for --exit-if-chag")
	var chagOnlySw bool
	opt.FlagLong(&chagOnlySw, "chag-only", 0,
		"Output only Chag and Erev Chag events (when melakha/labor is prohibited)")
	opt.FlagLong(&noJulianSw, "no-julian", 0,
		"Disable use of Julian calendar for dates before 1752")

	opt.FlagLong(&yearDigitsSw, "year-abbrev", 'y', "Print only last two digits of year")
	opt.FlagLong(&tabsSw, "tabs", 'r', "Tab delineated format")
	opt.FlagLong(&weekdaySw, "weekday", 'w', "Add day of the week")
	opt.FlagLong(&calOptions.Hour24,
		"24hour", 'E', "Output 24-hour times (e.g. 18:37 instead of 6:37)")
	opt.FlagLong(&calOptions.SunriseSunset,
		"sunrise-and-sunset", 'O', "Output sunrise and sunset times every day")
	opt.FlagLong(&calOptions.DailyZmanim, "zmanim", 'Z', "Output zemanim every day")
	opt.FlagLong(&calOptions.Molad, "molad", 'M', "Print the molad on Shabbat Mevorchim")
	opt.FlagLong(&calOptions.WeeklyAbbreviated,
		"abbrev", 'W', "Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.")

	langList := strings.Join(locales.AllLocales, ", ")
	opt.FlagLong(&lang, "lang", 0, "Use LANG titles ("+langList+")", "LANG")

	opt.FlagLong(&calOptions.CandleLighting,
		"candlelighting", 'c', "Print candlelighting times")
	opt.FlagLong(&calOptions.AddHebrewDates,
		"add-hebrew-dates", 'd', "Print the Hebrew date for the entire date range")
	opt.FlagLong(&calOptions.AddHebrewDatesForEvents, "add-hebrew-dates-for-events", 'D', "Print the Hebrew date for dates with some event")

	opt.FlagLong(&calOptions.IsHebrewYear,
		"hebrew-date", 'H', "Use Hebrew date ranges - only needed when e.g. hebcal -H 5373")

	opt.FlagLong(&calOptions.DafYomi,
		"daf-yomi", 'F', "Output the Daf Yomi (Bavli) for the entire date range")
	opt.FlagLong(&calOptions.MishnaYomi,
		"mishna-yomi", 0, "Output the Mishna Yomi for the entire date range")
	opt.FlagLong(&calOptions.NachYomi,
		"nach-yomi", 0, "Output the Nach Yomi for the entire date range")
	opt.FlagLong(&calOptions.YerushalmiYomi,
		"yerushalmi", 0, "Output the Yerushalmi Yomi for the entire date range")
	opt.FlagLong(&calOptions.YomKippurKatan,
		"ykk", 0, "Include Yom Kippur Katan, minor day of atonement occurring monthly on the day preceding each Rosh Chodesh")
	opt.FlagLong(&calOptions.ShabbatMevarchim, "mevarchim", 0, "Include Shabbat Mevarchim HaChodesh")

	opt.FlagLong(&calOptions.NoHolidays,
		"no-holidays", 'h', "Suppress default holidays")
	opt.FlagLong(&calOptions.NoRoshChodesh,
		"no-rosh-chodesh", 'x', "Suppress Rosh Chodesh")

	opt.FlagLong(&calOptions.IL,
		"israeli", 'i', "Israeli holiday and sedra schedule")
	opt.FlagLong(&calOptions.NoModern,
		"no-modern", 0, "Suppress modern holidays")
	opt.FlagLong(&calOptions.NoMinorFast, "no-mf", 0, "Suppress minor fast days")
	opt.FlagLong(&calOptions.NoSpecialShabbat, "no-special", 0, "Suppress Special Shabbatot")
	opt.FlagLong(&calOptions.Omer,
		"omer", 'o', "Add days of the Omer")
	opt.FlagLong(&calOptions.Sedrot,
		"sedrot", 's', "Add the weekly sedra to the output on Saturdays")
	opt.FlagLong(&calOptions.DailySedra,
		"daily-sedra", 'S', "Add the weekly sedra to the output every day")

	calOptions.CandleLightingMins = 18
	opt.FlagLong(&calOptions.CandleLightingMins,
		"candle-mins", 'b', "Set candle-lighting to occur this many minutes before sundown", "MINUTES")

	opt.FlagLong(&calOptions.HavdalahMins,
		"havdalah-mins", 'm', "Set Havdalah to occur this many minutes after sundown", "MINUTES")
	opt.FlagLong(&calOptions.HavdalahDeg,
		"havdalah-deg", 'G', "Set Havdalah to occur this many degrees below the horizon", "DEGREES")

	calOptions.NumYears = 1
	opt.FlagLong(&calOptions.NumYears,
		"years", 0, "Generate events for N years (default 1)", "N")

	inFileName := opt.StringLong("infile", 'I', "", `Read extra events from FILENAME.
Each line specifies one holiday, with the format:
    MMMM DD Description
where MMMM is a string identifying the Hebrew month,
and DD is a number from 1 to 30.
Description is a newline-terminated string describing
the event. Events are printed regardless of the
-h (suppress holidays) switch.`, "FILENAME")
	yahrzeitFileName := opt.StringLong("yahrtzeit", 'Y', "", `Read yahrtzeit dates from FILENAME.
Each line specifies one death-date, with the format:
    MM DD YYYY Description
where MM, DD and YYYY are the Gregorian date of death.
Description is a newline-terminated string to be printed
on the yahrtzeit. Events are printed regardless of the
-h (suppress holidays) switch.`, "FILENAME")

	// Options from HEBCAL_OPTS come first so that command-line flags override them
	args := append([]string{os.Args[0]}, strings.Fields(os.Getenv("HEBCAL_OPTS"))...)
	args = append(args, os.Args[1:]...)
	if err := opt.Getopt(args, nil); err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	if *help {
		displayHelp(opt)
		os.Exit(0)
	}
	if *versionSw {
		fmt.Printf("Hebcal version %s\n", Version)
		os.Exit(0)
	}

	if *euroDatesSw {
		gregDateOutputFormatCodeSw = dateEuro
	}
	if *iso8601datesSw {
		gregDateOutputFormatCodeSw = dateISO
	}
	if *schottenstein {
		calOptions.YerushalmiYomi = true
		calOptions.YerushalmiEdition = hebcal.Schottenstein
	}
	calOptions.DailyLearning = *dailyLearning
	for _, name := range calOptions.DailyLearning {
		if !dailylearning.Has(name) {
			fmt.Fprintf(os.Stderr, "unknown --daily-learning calendar: %s\n", name)
			fmt.Fprintf(os.Stderr, "valid calendars are: %s\n", strings.Join(dailylearning.GetCalendars(), ", "))
			os.Exit(1)
		}
	}

	if *ashkenaziSw && *utf8HebrewSw {
		fmt.Fprintf(os.Stderr, "Cannot specify both options -a and -8\n")
		os.Exit(1)
	} else if *ashkenaziSw {
		lang = "ashkenazi"
	} else if *utf8HebrewSw {
		lang = "he"
	}
	checkLang()

	validCity := false
	if *cityNameArg != "" {
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

	var latitude, longitude float64
	hasLat := latitudeStr != ""
	if hasLat {
		var err error
		latitude, err = parseDegMin(latitudeStr, 90)
		if err != nil {
			fmt.Fprintf(os.Stderr, "latitude: %v\n", err)
			os.Exit(1)
		}
	}
	hasLong := longitudeStr != ""
	if hasLong {
		var err error
		longitude, err = parseDegMin(longitudeStr, 180)
		if err != nil {
			fmt.Fprintf(os.Stderr, "longitude: %v\n", err)
			os.Exit(1)
		}
		longitude = -longitude // -L uses positive values for WEST
	}

	if coordinates != "" {
		n, err := fmt.Sscanf(coordinates, "%f,%f", &latitude, &longitude)
		if err != nil || n != 2 {
			fmt.Fprintf(os.Stderr, "geo coordinates must be LATITUDE,LONGITUDE: %s\n", coordinates)
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		hasLat = true
		hasLong = true
	}

	if hasLat != hasLong {
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
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		userLocation := zmanim.NewLocation("User Defined City", "", latitude, longitude, 0, tzid)
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

	if noGregSw || isTodayChagSw {
		todaySw = true
	}
	if noJulianSw {
		calOptions.NoJulian = true
	}

	gregTodayYY, gregTodayMM, gregTodayDD := time.Now().Date()

	if todaySw {
		calOptions.AddHebrewDates = true
		rangeType = rangeToday
		theGregMonth = gregTodayMM
		theDay = gregTodayDD
		calOptions.Omer = true
		calOptions.IsHebrewYear = false
	}

	if chagOnlySw {
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
	args = opt.Args()

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
			theYear = yy // just year specified
		} else {
			switch arg0 {
			case "help":
				displayHelp(opt)
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
				t, err := time.Parse("2006-01-02", arg0)
				if err != nil {
					fmt.Fprintf(os.Stderr, "unrecognized command '%s'\n", args[0])
					fmt.Fprintf(os.Stderr, "Usage: hebcal %s\n", opt.UsageLine())
					os.Exit(1)
				}
				theYear, theGregMonth, theDay = t.Date()
				rangeType = rangeDay
			}
		}
	case 2:
		yy, err := strconv.Atoi(args[1])
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		theYear = yy
		parseGregOrHebMonth(&calOptions, theYear, args[0], &theGregMonth, &theHebMonth)
		rangeType = rangeMonth
	case 3:
		dd, err := strconv.Atoi(args[1])
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		theDay = dd
		yy, err := strconv.Atoi(args[2])
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		theYear = yy
		parseGregOrHebMonth(&calOptions, theYear, args[0], &theGregMonth, &theHebMonth)
		rangeType = rangeDay
	default:
		opt.PrintUsage(os.Stderr)
		os.Exit(1)
	}

	if calOptions.NumYears != 1 && rangeType != rangeYear {
		fmt.Fprintf(os.Stderr, "Sorry, --years option works only with entire-year calendars\n")
		os.Exit(1)
	} else if todaySw && rangeType != rangeDay && rangeType != rangeToday {
		fmt.Fprintf(os.Stderr, "Sorry, --today option works only with single-day calendars\n")
		os.Exit(1)
	}
	return calOptions
}

func checkLang() {
	lang = strings.ToLower(lang)
	if lang == "en" {
		return
	}
	for _, a := range locales.AllLocales {
		if strings.EqualFold(a, lang) {
			return
		}
	}
	fmt.Fprintf(os.Stderr, "Unknown lang '%s'; using default\n", lang)
	lang = "en"
}

func parseGregOrHebMonth(calOptions *hebcal.CalOptions, theYear int, arg string, gregMonth *time.Month, hebMonth *hdate.HMonth) {
	mm, err := strconv.Atoi(arg)
	if err == nil {
		if calOptions.IsHebrewYear {
			fmt.Fprintf(os.Stderr, "Don't use numbers to specify Hebrew months.\n")
			os.Exit(1)
		}
		*gregMonth = time.Month(mm)
	} else {
		hm, err := hdate.MonthFromName(arg)
		if err == nil {
			*hebMonth = hm
			calOptions.IsHebrewYear = true // automagically turn it on
			if hm == hdate.Adar2 && !hdate.IsLeapYear(theYear) {
				*hebMonth = hdate.Adar1 // silently fix this mistake
			}
		} else {
			fmt.Fprintf(os.Stderr, "Unknown Hebrew month: %s.\n", arg)
			os.Exit(1)
		}
	}
}

func fromGregorian(year int, month time.Month, day int) hdate.HDate {
	if noJulianSw {
		return hdate.FromProlepticGregorian(year, month, day)
	}
	return hdate.FromGregorian(year, month, day)
}

func main() {
	calOptions := handleArgs()
	switch rangeType {
	case rangeToday:
		calOptions.AddHebrewDates = true
		calOptions.Start = fromGregorian(theYear, theGregMonth, theDay)
		calOptions.End = calOptions.Start
	case rangeDay:
		calOptions.AddHebrewDates = true
		if calOptions.IsHebrewYear {
			calOptions.Start = hdate.New(theYear, theHebMonth, theDay)
		} else {
			calOptions.Start = fromGregorian(theYear, theGregMonth, theDay)
		}
		calOptions.End = calOptions.Start
	case rangeMonth:
		if calOptions.IsHebrewYear {
			calOptions.Start = hdate.New(theYear, theHebMonth, 1)
			calOptions.End = hdate.New(theYear, theHebMonth, calOptions.Start.DaysInMonth())
		} else {
			calOptions.Start = fromGregorian(theYear, theGregMonth, 1)
			calOptions.End = fromGregorian(theYear, theGregMonth, greg.DaysIn(theGregMonth, theYear))
		}
	case rangeYear:
		calOptions.Year = theYear
	default:
		panic("Oh, NO! internal error #17q!")
	}

	events, err := hebcal.HebrewCalendar(&calOptions)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	if isTodayChagSw {
		status, reason := isTodayChag(&calOptions, events)
		if reason != "" && verboseSw {
			fmt.Println(reason)
		}
		os.Exit(status)
	}

	for _, ev := range events {
		fmt.Println(formatGregDate(ev.GetDate()) + ev.Render(lang))
	}
}

func isTodayChag(calOptions *hebcal.CalOptions, events []event.CalEvent) (int, string) {
	if calOptions.Location == nil {
		for _, ev := range events {
			if ev.GetFlags().Has(event.CHAG) {
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
	if rangeType != rangeToday {
		hour, minute, sec := now.Clock()
		now = time.Date(theYear, theGregMonth, theDay, hour, minute, sec, 0, loc)
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
			if ev.GetFlags().Has(event.CHAG) {
				return 1, ev.Render(lang)
			}
		}
	}
	return 0, ""
}

// formatGregDate returns the date and weekday prefix for an output line.
func formatGregDate(hd hdate.HDate) string {
	var sb strings.Builder
	if !noGregSw {
		var year, day int
		var month time.Month
		if noJulianSw {
			year, month, day = hd.ProlepticGreg()
		} else {
			year, month, day = hd.Greg()
		}
		displayYear := year
		if yearDigitsSw {
			displayYear = year % 100
		}
		switch gregDateOutputFormatCodeSw {
		case dateISO:
			sb.WriteString(time.Date(year, month, day, 0, 0, 0, 0, time.UTC).Format("2006-01-02"))
		case dateEuro:
			fmt.Fprintf(&sb, "%d.%d.%d", day, month, displayYear)
		default:
			fmt.Fprintf(&sb, "%d/%d/%d", month, day, displayYear)
		}
		if tabsSw {
			sb.WriteByte('\t')
		} else {
			sb.WriteByte(' ')
		}
	}
	if weekdaySw {
		sb.WriteString(hd.Weekday().String()[:3] + ", ")
	}
	return sb.String()
}

// parseDegMin parses a "DEG,MIN" string such as "40,43" into decimal degrees.
// The sign of DEG applies to the whole value.
func parseDegMin(s string, maxDeg int) (float64, error) {
	var deg, min int
	if _, err := fmt.Sscanf(s, "%d,%d", &deg, &min); err != nil {
		return 0, fmt.Errorf("unable to read argument %q: %w", s, err)
	}
	if deg < -maxDeg || deg > maxDeg || min < 0 || min >= 60 {
		return 0, fmt.Errorf("argument out of range: %s", s)
	}
	val := float64(deg) + float64(min)/60
	if deg < 0 {
		val = float64(deg) - float64(min)/60
	}
	return val, nil
}

func displayHelp(opt *getopt.Set) {
	opt.PrintUsage(os.Stdout)
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
Portions Copyright (c) 2011-2026 Michael J. Radwin. All rights reserved.

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
