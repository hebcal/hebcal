package event

import (
	"regexp"
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/locales"
)

// HolidayEvent represents a built-in holiday like Pesach, Purim or Tu BiShvat
type HolidayEvent struct {
	Date          hdate.HDate  // Holiday date of occurrence
	Desc          string       // Description (e.g. "Pesach III (CH''M)")
	Flags         HolidayFlags // Event flag bitmask
	Emoji         string       // Holiday-specific emoji
	CholHaMoedDay int          // used only for Pesach and Sukkot
	ChanukahDay   int          // used only for Chanukah
}

func (ev HolidayEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev HolidayEvent) Render(locale string) string {
	switch {
	case (ev.Flags & ROSH_CHODESH) != 0:
		rchStr, _ := locales.LookupTranslation("Rosh Chodesh", locale)
		monthStr, _ := locales.LookupTranslation(ev.Desc[13:], locale)
		return rchStr + " " + monthStr
	case (ev.Flags & SHABBAT_MEVARCHIM) != 0:
		mevarchimStr, _ := locales.LookupTranslation(
			"Shabbat Mevarchim Chodesh",
			locale,
		)
		monthStr, _ := locales.LookupTranslation(
			strings.TrimPrefix(ev.Desc, "Shabbat Mevarchim Chodesh "),
			locale,
		)
		return mevarchimStr + " " + monthStr
	case ev.Date.Month() == hdate.Tishrei && ev.Date.Day() == 1:
		s, _ := locales.LookupTranslation("Rosh Hashana", locale)
		year := ev.Date.Year()
		locale = strings.ToLower(locale)
		if locale == "he" || locale == "he-x-nonikud" {
			return s + " " + gematriya.Gematriya(year)
		}
		return s + " " + strconv.Itoa(year)
	case (ev.Flags & YOM_KIPPUR_KATAN) != 0:
		rchStr, _ := locales.LookupTranslation("Yom Kippur Katan", locale)
		monthStr, _ := locales.LookupTranslation(ev.Desc[17:], locale)
		return rchStr + " " + monthStr
	}
	str, _ := locales.LookupTranslation(ev.Desc, locale)
	return str
}

func (ev HolidayEvent) GetFlags() HolidayFlags {
	return ev.Flags
}

func (ev HolidayEvent) GetEmoji() string {
	if ev.Emoji != "" {
		return ev.Emoji
	}
	switch ev.Flags {
	case SPECIAL_SHABBAT:
		return "🕍"
	case ROSH_CHODESH:
		return "🌒"
	case SHABBAT_MEVARCHIM, YOM_KIPPUR_KATAN | MINOR_FAST:
		return ""
	default:
		return "✡️"
	}
}

var regexes = []*regexp.Regexp{
	regexp.MustCompile(` \d{4}$`),
	regexp.MustCompile(` \(CH''M\)$`),
	regexp.MustCompile(` \(observed\)$`),
	regexp.MustCompile(` \(Hoshana Raba\)$`),
	regexp.MustCompile(` [IV]+$`),
	regexp.MustCompile(`: \d Candles?$`),
	regexp.MustCompile(`: 8th Day$`),
	regexp.MustCompile(`^Erev `),
}

func (ev HolidayEvent) Basename() string {
	str := ev.Desc
	for _, regex := range regexes {
		str = regex.ReplaceAllString(str, "")
	}
	return str
}

// minorHolidays are holidays whose flags do not mark them minor but which are
// nevertheless categorized as minor, matching @hebcal/core HolidayEvent.
var minorHolidays = map[string]bool{
	"Lag BaOmer":             true,
	"Leil Selichot":          true,
	"Pesach Sheni":           true,
	"Erev Purim":             true,
	"Purim Katan":            true,
	"Shushan Purim":          true,
	"Tu B'Av":                true,
	"Tu BiShvat":             true,
	"Rosh Hashana LaBehemot": true,
}

// GetCategories returns the category and sub-categories for a holiday,
// mirroring HolidayEvent.getCategories() in @hebcal/core: chol hamoed days are
// major, otherwise the flag-based category is used, falling back to a minor or
// major holiday when the flags alone are inconclusive.
func (ev HolidayEvent) GetCategories() []string {
	if ev.CholHaMoedDay != 0 {
		return []string{"holiday", "major", "cholhamoed"}
	}
	cats := CategoriesFromFlags(ev.Flags)
	if cats[0] != "unknown" {
		return cats
	}
	if minorHolidays[ev.Desc] {
		return []string{"holiday", "minor"}
	}
	return []string{"holiday", "major"}
}
