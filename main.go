package main

import (
	"errors"
	"fmt"
	"io"
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

// config holds everything parsed from the command line.
type config struct {
	cal  hebcal.CalOptions
	lang string

	// Requested date range; month is Hebrew or Gregorian per cal.IsHebrewYear
	rangeType calRange
	year      int
	gregMonth time.Month
	hebMonth  hdate.HMonth
	day       int

	// Output formatting
	dateFmt    dateFormat
	tabs       bool
	weekday    bool
	yearAbbrev bool
	noGreg     bool

	today       bool
	isTodayChag bool
	verbose     bool

	// command is set for informational commands such as "info" or "cities"
	// that print something and exit instead of generating a calendar.
	command    string
	printUsage func(io.Writer)
}

// parseArgs parses argv (including the program name) with the options in
// envOpts (from HEBCAL_OPTS) inserted before the command-line arguments.
func parseArgs(argv []string, envOpts string) (*config, error) {
	c := &config{lang: "en"}
	c.cal.CandleLightingMins = 18
	c.cal.NumYears = 1

	opt := getopt.New()
	opt.SetProgram("hebcal")
	opt.SetParameters("[[ month [ day ]] year]")
	c.printUsage = opt.PrintUsage
	var (
		help           = opt.BoolLong("help", 0, "print this help text")
		ashkenaziSw    = opt.BoolLong("ashkenazi", 'a', "Use Ashkenazi Hebrew transliterations (alias for --lang=ashkenazi)")
		euroDatesSw    = opt.BoolLong("euro-dates", 'e', "Output 'European' dates -- DD.MM.YYYY")
		iso8601datesSw = opt.BoolLong("iso-8601", 'g', "Output ISO 8601 dates -- YYYY-MM-DD")
		versionSw      = opt.BoolLong("version", 0, "Show version number")
		cityName       = opt.StringLong("city", 'C', "", "City for candle-lighting", "CITY")
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

	opt.FlagLong(&c.today, "today", 't', "Only output for today's date")
	opt.FlagLong(&c.noGreg, "today-brief", 'T', "Print today's pertinent information")
	opt.FlagLong(&c.isTodayChag, "exit-if-chag", 'X',
		"Exit silently with non-zero status if today is Shabbat or Chag; exit with 0 status if today is chol")
	opt.FlagLong(&c.verbose, "verbose", 0,
		"Verbose mode, currently used only for --exit-if-chag")
	var chagOnlySw bool
	opt.FlagLong(&chagOnlySw, "chag-only", 0,
		"Output only Chag and Erev Chag events (when melakha/labor is prohibited)")
	opt.FlagLong(&c.cal.NoJulian, "no-julian", 0,
		"Disable use of Julian calendar for dates before 1752")

	opt.FlagLong(&c.yearAbbrev, "year-abbrev", 'y', "Print only last two digits of year")
	opt.FlagLong(&c.tabs, "tabs", 'r', "Tab delineated format")
	opt.FlagLong(&c.weekday, "weekday", 'w', "Add day of the week")
	opt.FlagLong(&c.cal.Hour24,
		"24hour", 'E', "Output 24-hour times (e.g. 18:37 instead of 6:37)")
	opt.FlagLong(&c.cal.SunriseSunset,
		"sunrise-and-sunset", 'O', "Output sunrise and sunset times every day")
	opt.FlagLong(&c.cal.DailyZmanim, "zmanim", 'Z', "Output zemanim every day")
	opt.FlagLong(&c.cal.Molad, "molad", 'M', "Print the molad on Shabbat Mevorchim")
	opt.FlagLong(&c.cal.WeeklyAbbreviated,
		"abbrev", 'W', "Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.")

	langList := strings.Join(locales.AllLocales, ", ")
	opt.FlagLong(&c.lang, "lang", 0, "Use LANG titles ("+langList+")", "LANG")

	opt.FlagLong(&c.cal.CandleLighting,
		"candlelighting", 'c', "Print candlelighting times")
	opt.FlagLong(&c.cal.AddHebrewDates,
		"add-hebrew-dates", 'd', "Print the Hebrew date for the entire date range")
	opt.FlagLong(&c.cal.AddHebrewDatesForEvents, "add-hebrew-dates-for-events", 'D', "Print the Hebrew date for dates with some event")

	opt.FlagLong(&c.cal.IsHebrewYear,
		"hebrew-date", 'H', "Use Hebrew date ranges - only needed when e.g. hebcal -H 5373")

	opt.FlagLong(&c.cal.DafYomi,
		"daf-yomi", 'F', "Output the Daf Yomi (Bavli) for the entire date range")
	opt.FlagLong(&c.cal.MishnaYomi,
		"mishna-yomi", 0, "Output the Mishna Yomi for the entire date range")
	opt.FlagLong(&c.cal.NachYomi,
		"nach-yomi", 0, "Output the Nach Yomi for the entire date range")
	opt.FlagLong(&c.cal.YerushalmiYomi,
		"yerushalmi", 0, "Output the Yerushalmi Yomi for the entire date range")
	opt.FlagLong(&c.cal.YomKippurKatan,
		"ykk", 0, "Include Yom Kippur Katan, minor day of atonement occurring monthly on the day preceding each Rosh Chodesh")
	opt.FlagLong(&c.cal.ShabbatMevarchim, "mevarchim", 0, "Include Shabbat Mevarchim HaChodesh")

	opt.FlagLong(&c.cal.NoHolidays,
		"no-holidays", 'h', "Suppress default holidays")
	opt.FlagLong(&c.cal.NoRoshChodesh,
		"no-rosh-chodesh", 'x', "Suppress Rosh Chodesh")

	opt.FlagLong(&c.cal.IL,
		"israeli", 'i', "Israeli holiday and sedra schedule")
	opt.FlagLong(&c.cal.NoModern,
		"no-modern", 0, "Suppress modern holidays")
	opt.FlagLong(&c.cal.NoMinorFast, "no-mf", 0, "Suppress minor fast days")
	opt.FlagLong(&c.cal.NoSpecialShabbat, "no-special", 0, "Suppress Special Shabbatot")
	opt.FlagLong(&c.cal.Omer,
		"omer", 'o', "Add days of the Omer")
	opt.FlagLong(&c.cal.Sedrot,
		"sedrot", 's', "Add the weekly sedra to the output on Saturdays")
	opt.FlagLong(&c.cal.DailySedra,
		"daily-sedra", 'S', "Add the weekly sedra to the output every day")

	opt.FlagLong(&c.cal.CandleLightingMins,
		"candle-mins", 'b', "Set candle-lighting to occur this many minutes before sundown", "MINUTES")

	opt.FlagLong(&c.cal.HavdalahMins,
		"havdalah-mins", 'm', "Set Havdalah to occur this many minutes after sundown", "MINUTES")
	opt.FlagLong(&c.cal.HavdalahDeg,
		"havdalah-deg", 'G', "Set Havdalah to occur this many degrees below the horizon", "DEGREES")

	opt.FlagLong(&c.cal.NumYears,
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
	args := append([]string{argv[0]}, strings.Fields(envOpts)...)
	args = append(args, argv[1:]...)
	if err := opt.Getopt(args, nil); err != nil {
		return nil, err
	}

	if *help {
		c.command = "help"
		return c, nil
	}
	if *versionSw {
		c.command = "version"
		return c, nil
	}

	if *euroDatesSw {
		c.dateFmt = dateEuro
	}
	if *iso8601datesSw {
		c.dateFmt = dateISO
	}
	if *schottenstein {
		c.cal.YerushalmiYomi = true
		c.cal.YerushalmiEdition = hebcal.Schottenstein
	}
	c.cal.DailyLearning = *dailyLearning
	for _, name := range c.cal.DailyLearning {
		if !dailylearning.Has(name) {
			return nil, fmt.Errorf("unknown --daily-learning calendar: %s\nvalid calendars are: %s",
				name, strings.Join(dailylearning.GetCalendars(), ", "))
		}
	}

	switch {
	case *ashkenaziSw && *utf8HebrewSw:
		return nil, errors.New("Cannot specify both options -a and -8")
	case *ashkenaziSw:
		c.lang = "ashkenazi"
	case *utf8HebrewSw:
		c.lang = "he"
	}
	c.checkLang()

	if err := c.setLocation(*cityName, latitudeStr, longitudeStr, coordinates, tzid); err != nil {
		return nil, err
	}

	if c.cal.CandleLighting && c.cal.HavdalahDeg == 0.0 && c.cal.HavdalahMins == 0 {
		c.cal.HavdalahMins = 72
	}

	if c.noGreg || c.isTodayChag {
		c.today = true
	}

	if c.today {
		c.cal.AddHebrewDates = true
		c.rangeType = rangeToday
		_, c.gregMonth, c.day = time.Now().Date()
		c.cal.Omer = true
		c.cal.IsHebrewYear = false
	}

	if chagOnlySw {
		c.cal.Mask = event.CHAG | event.LIGHT_CANDLES |
			event.LIGHT_CANDLES_TZEIS | event.YOM_TOV_ENDS
	}

	if *yahrzeitFileName != "" {
		yahrzeits, err := readYahrzeitFile(*yahrzeitFileName)
		if err != nil {
			return nil, err
		}
		c.cal.Yahrzeits = yahrzeits
	}
	if *inFileName != "" {
		userEvents, err := readUserFile(*inFileName)
		if err != nil {
			return nil, err
		}
		c.cal.UserEvents = userEvents
	}

	if err := c.parseDateArgs(opt); err != nil {
		return nil, err
	}
	if c.command != "" {
		return c, nil
	}

	if c.cal.NumYears != 1 && c.rangeType != rangeYear {
		return nil, errors.New("Sorry, --years option works only with entire-year calendars")
	}
	if c.today && c.rangeType != rangeDay && c.rangeType != rangeToday {
		return nil, errors.New("Sorry, --today option works only with single-day calendars")
	}
	return c, nil
}

// setLocation sets c.cal.Location from -C, HEBCAL_CITY, -l/-L or --geo,
// falling back to the default city when a location is needed but not given.
func (c *config) setLocation(cityName, latitudeStr, longitudeStr, coordinates, tzid string) error {
	if cityName != "" {
		city := zmanim.LookupCity(cityName)
		if city == nil {
			return fmt.Errorf("unknown city: %s. Use a nearby city or geographic coordinates.", cityName)
		}
		c.cal.Location = city
		c.cal.CandleLighting = true
	} else if name := os.Getenv("HEBCAL_CITY"); name != "" {
		// An unknown HEBCAL_CITY is silently ignored
		c.cal.Location = zmanim.LookupCity(name)
	}

	var latitude, longitude float64
	hasLat := latitudeStr != ""
	if hasLat {
		var err error
		latitude, err = parseDegMin(latitudeStr, 90)
		if err != nil {
			return fmt.Errorf("latitude: %w", err)
		}
	}
	hasLong := longitudeStr != ""
	if hasLong {
		var err error
		longitude, err = parseDegMin(longitudeStr, 180)
		if err != nil {
			return fmt.Errorf("longitude: %w", err)
		}
		longitude = -longitude // -L uses positive values for WEST
	}

	if coordinates != "" {
		if _, err := fmt.Sscanf(coordinates, "%f,%f", &latitude, &longitude); err != nil {
			return fmt.Errorf("geo coordinates must be LATITUDE,LONGITUDE: %s\n%w", coordinates, err)
		}
		hasLat = true
		hasLong = true
	}

	if hasLat != hasLong {
		return errors.New("Error, you must enter BOTH the latitude and the longitude")
	}

	if hasLat {
		if tzid == "" {
			return errors.New("Error, latitude and longitude requires -z/--timezone")
		}
		if _, err := time.LoadLocation(tzid); err != nil {
			return err
		}
		userLocation := zmanim.NewLocation("User Defined City", "", latitude, longitude, 0, tzid)
		c.cal.Location = &userLocation
		c.cal.CandleLighting = true
	}

	if c.cal.Location == nil && (c.cal.CandleLighting || c.cal.SunriseSunset || c.cal.DailyZmanim) {
		c.cal.Location = zmanim.LookupCity(defaultCity)
	}
	return nil
}

// parseDateArgs interprets the positional arguments: [[ month [ day ]] year],
// a YYYY-MM-DD date, or an informational command such as "info".
func (c *config) parseDateArgs(opt *getopt.Set) error {
	args := opt.Args()
	switch len(args) {
	case 0:
		now := time.Now()
		if c.cal.IsHebrewYear {
			c.year = c.fromGregorian(now.Date()).Year()
		} else {
			c.year = now.Year()
		}
	case 1:
		arg := strings.TrimSpace(args[0])
		if yy, err := strconv.Atoi(arg); err == nil {
			c.year = yy // just year specified
			return nil
		}
		switch arg {
		case "help", "version", "info", "cities", "copying", "warranty":
			c.command = arg
			return nil
		}
		t, err := time.Parse("2006-01-02", arg)
		if err != nil {
			return fmt.Errorf("unrecognized command '%s'\nUsage: hebcal %s", args[0], opt.UsageLine())
		}
		c.year, c.gregMonth, c.day = t.Date()
		c.rangeType = rangeDay
	case 2:
		yy, err := strconv.Atoi(args[1])
		if err != nil {
			return err
		}
		c.year = yy
		if err := c.parseMonth(args[0]); err != nil {
			return err
		}
		c.rangeType = rangeMonth
	case 3:
		dd, err := strconv.Atoi(args[1])
		if err != nil {
			return err
		}
		c.day = dd
		yy, err := strconv.Atoi(args[2])
		if err != nil {
			return err
		}
		c.year = yy
		if err := c.parseMonth(args[0]); err != nil {
			return err
		}
		c.rangeType = rangeDay
	default:
		var sb strings.Builder
		opt.PrintUsage(&sb)
		return errors.New(strings.TrimSuffix(sb.String(), "\n"))
	}
	return nil
}

// checkLang normalizes c.lang, falling back to English if it's unknown.
func (c *config) checkLang() {
	c.lang = strings.ToLower(c.lang)
	if c.lang == "en" {
		return
	}
	for _, a := range locales.AllLocales {
		if strings.EqualFold(a, c.lang) {
			return
		}
	}
	fmt.Fprintf(os.Stderr, "Unknown lang '%s'; using default\n", c.lang)
	c.lang = "en"
}

// parseMonth parses a month given either as a Gregorian month number or as a
// Hebrew month name. A Hebrew month name turns on cal.IsHebrewYear.
func (c *config) parseMonth(arg string) error {
	if mm, err := strconv.Atoi(arg); err == nil {
		if c.cal.IsHebrewYear {
			return errors.New("Don't use numbers to specify Hebrew months.")
		}
		if mm < 1 || mm > 12 {
			return fmt.Errorf("Invalid month: %d.", mm)
		}
		c.gregMonth = time.Month(mm)
		return nil
	}
	hm, err := hdate.MonthFromName(arg)
	if err != nil {
		return fmt.Errorf("Unknown Hebrew month: %s.", arg)
	}
	c.hebMonth = hm
	c.cal.IsHebrewYear = true // automagically turn it on
	if hm == hdate.Adar2 && !hdate.IsLeapYear(c.year) {
		c.hebMonth = hdate.Adar1 // silently fix this mistake
	}
	return nil
}

func (c *config) fromGregorian(year int, month time.Month, day int) hdate.HDate {
	if c.cal.NoJulian {
		return hdate.FromProlepticGregorian(year, month, day)
	}
	return hdate.FromGregorian(year, month, day)
}

// setDateRange sets the Start/End or Year fields of c.cal from the
// requested date range.
func (c *config) setDateRange() {
	switch c.rangeType {
	case rangeToday:
		c.cal.AddHebrewDates = true
		c.cal.Start = c.fromGregorian(c.year, c.gregMonth, c.day)
		c.cal.End = c.cal.Start
	case rangeDay:
		c.cal.AddHebrewDates = true
		if c.cal.IsHebrewYear {
			c.cal.Start = hdate.New(c.year, c.hebMonth, c.day)
		} else {
			c.cal.Start = c.fromGregorian(c.year, c.gregMonth, c.day)
		}
		c.cal.End = c.cal.Start
	case rangeMonth:
		if c.cal.IsHebrewYear {
			c.cal.Start = hdate.New(c.year, c.hebMonth, 1)
			c.cal.End = hdate.New(c.year, c.hebMonth, c.cal.Start.DaysInMonth())
		} else {
			c.cal.Start = c.fromGregorian(c.year, c.gregMonth, 1)
			c.cal.End = c.fromGregorian(c.year, c.gregMonth, greg.DaysIn(c.gregMonth, c.year))
		}
	case rangeYear:
		c.cal.Year = c.year
	default:
		panic(fmt.Sprintf("unknown range type %d", c.rangeType))
	}
}

// runCommand runs an informational command such as "info" or "cities".
func (c *config) runCommand() {
	switch c.command {
	case "help":
		c.printUsage(os.Stdout)
		fmt.Print(usageSummary)
	case "version":
		fmt.Printf("Hebcal version %s\n", Version)
	case "info":
		fmt.Printf("hebcal version %s\n\n", Version)
		fmt.Printf("Default city: %s\n", defaultCity)
		fmt.Println("Environment variable for default city: HEBCAL_CITY")
		fmt.Println("Environment variable for default options: HEBCAL_OPTS")
	case "cities":
		for _, city := range zmanim.AllCities() {
			fmt.Printf("%s (%.5f,%.5f  %s)\n",
				city.Name, city.Latitude, city.Longitude, city.TimeZoneId)
		}
	case "copying":
		fmt.Println(gplv2txt)
		fmt.Print(warranty)
	case "warranty":
		fmt.Print(warranty)
	}
}

func main() {
	c, err := parseArgs(os.Args, os.Getenv("HEBCAL_OPTS"))
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}
	if c.command != "" {
		c.runCommand()
		return
	}

	c.setDateRange()
	events, err := hebcal.HebrewCalendar(&c.cal)
	if err != nil {
		fmt.Fprintln(os.Stderr, err)
		os.Exit(1)
	}

	if c.isTodayChag {
		chag, reason, err := c.checkTodayChag(events)
		if err != nil {
			fmt.Fprintln(os.Stderr, err)
			os.Exit(1)
		}
		if reason != "" && c.verbose {
			fmt.Println(reason)
		}
		if chag {
			os.Exit(1)
		}
		return
	}

	for _, ev := range events {
		fmt.Println(c.formatGregDate(ev.GetDate()) + ev.Render(c.lang))
	}
}

// checkTodayChag reports whether it is currently Shabbat or Chag (when
// melakha is prohibited), along with a human-readable reason.
func (c *config) checkTodayChag(events []event.CalEvent) (bool, string, error) {
	if c.cal.Location == nil {
		if chag, reason := findChag(events, c.lang); chag {
			return true, reason, nil
		}
		if c.cal.Start.Weekday() == time.Saturday {
			reason, _ := locales.LookupTranslation("Shabbat", c.lang)
			return true, reason, nil
		}
		return false, "", nil
	}

	loc, err := time.LoadLocation(c.cal.Location.TimeZoneId)
	if err != nil {
		return false, "", err
	}
	// TimedEvents keep a pointer to c.cal, so this affects how they render
	c.cal.Hour24 = true

	now := time.Now().In(loc)
	if c.rangeType != rangeToday {
		hour, minute, sec := now.Clock()
		now = time.Date(c.year, c.gregMonth, c.day, hour, minute, sec, 0, loc)
	}

	// find today's candle-lighting and Havdalah events (if any)
	var candleLighting, havdalah *hebcal.TimedEvent
	for _, ev := range events {
		if timedEv, ok := ev.(hebcal.TimedEvent); ok {
			switch timedEv.Desc {
			case "Candle lighting":
				candleLighting = &timedEv
			case "Havdalah":
				havdalah = &timedEv
			}
		}
	}

	// If there's a candle-lighting or Havdalah event today, ignore other
	// events and check only if the current time is during the chag window
	nowStr := now.Format(time.RFC1123Z)
	switch {
	case candleLighting != nil && !now.Before(candleLighting.EventTime):
		return true, nowStr + " >= " + renderWithLinked(candleLighting, c.lang), nil
	case candleLighting != nil && c.cal.Start.Weekday() == time.Saturday:
		reason, _ := locales.LookupTranslation("Shabbat", c.lang)
		return true, reason, nil
	case havdalah != nil && !now.Before(havdalah.EventTime):
		return false, "", nil // Shabbat or Chag has already ended today
	case havdalah != nil:
		return true, nowStr + " < " + renderWithLinked(havdalah, c.lang), nil
	}
	// Today still might be chag (e.g. RH first day, or perhaps
	// day 1 of a 2-day chag chutz l'aretz)
	chag, reason := findChag(events, c.lang)
	return chag, reason, nil
}

// findChag returns the first event that is a Chag, if any.
func findChag(events []event.CalEvent, lang string) (bool, string) {
	for _, ev := range events {
		if ev.GetFlags().Has(event.CHAG) {
			return true, ev.Render(lang)
		}
	}
	return false, ""
}

func renderWithLinked(ev *hebcal.TimedEvent, lang string) string {
	s := ev.Render(lang)
	if ev.LinkedEvent != nil {
		s += " / " + ev.LinkedEvent.Render(lang)
	}
	return s
}

// formatGregDate returns the date and weekday prefix for an output line.
func (c *config) formatGregDate(hd hdate.HDate) string {
	var sb strings.Builder
	if !c.noGreg {
		var year, day int
		var month time.Month
		if c.cal.NoJulian {
			year, month, day = hd.ProlepticGreg()
		} else {
			year, month, day = hd.Greg()
		}
		displayYear := year
		if c.yearAbbrev {
			displayYear = year % 100
		}
		switch c.dateFmt {
		case dateISO:
			sb.WriteString(time.Date(year, month, day, 0, 0, 0, 0, time.UTC).Format("2006-01-02"))
		case dateEuro:
			fmt.Fprintf(&sb, "%d.%d.%d", day, month, displayYear)
		default:
			fmt.Fprintf(&sb, "%d/%d/%d", month, day, displayYear)
		}
		if c.tabs {
			sb.WriteByte('\t')
		} else {
			sb.WriteByte(' ')
		}
	}
	if c.weekday {
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
