package event

import (
	"regexp"
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/locales"
)

// HolidayEvent represents a built-in holiday like Pesach, Purim or Tu BiShvat.
// Construct one with NewHolidayEvent.
type HolidayEvent struct {
	date          hdate.HDate
	desc          string
	flags         Flags
	emoji         string
	CholHaMoedDay int // used only for Pesach and Sukkot
	ChanukahDay   int // used only for Chanukah
}

// NewHolidayEvent constructs a holiday on date. desc is its untranslated
// description (e.g. "Pesach III (CH”M)"), which is also the key of its
// translations. emoji is a holiday-specific emoji, or "" for the default
// that Emoji chooses from flags.
func NewHolidayEvent(date hdate.HDate, desc string, flags Flags, emoji string) HolidayEvent {
	return HolidayEvent{date: date, desc: desc, flags: flags, emoji: emoji}
}

// Date returns the Hebrew date on which the holiday occurs.
func (ev HolidayEvent) Date() hdate.HDate {
	return ev.date
}

// Desc returns the untranslated description, e.g. "Pesach III (CH”M)".
func (ev HolidayEvent) Desc() string {
	return ev.desc
}

// Render returns the translated description, with typographic apostrophes
// ("Rosh Chodesh Sh’vat", "Sukkot III (CH’’M)").
//
// Rosh Hashana carries the Hebrew year, written as a number in every locale
// except Hebrew, which writes it in gematriya ("רֹאשׁ הַשָּׁנָה תשפ״ז"). This
// deliberately differs from @hebcal/core, which writes a number in Hebrew too.
func (ev HolidayEvent) Render(locale string) string {
	return SmartApostrophe(ev.render(locale))
}

func (ev HolidayEvent) render(locale string) string {
	switch {
	case ev.flags.Has(RoshChodesh):
		rchStr, _ := locales.LookupTranslation("Rosh Chodesh", locale)
		monthStr, _ := locales.LookupTranslation(strings.TrimPrefix(ev.desc, "Rosh Chodesh "), locale)
		return rchStr + " " + monthStr
	case ev.flags.Has(ShabbatMevarchim):
		mevarchimStr, _ := locales.LookupTranslation(
			"Shabbat Mevarchim Chodesh",
			locale,
		)
		monthStr, _ := locales.LookupTranslation(
			strings.TrimPrefix(ev.desc, "Shabbat Mevarchim Chodesh "),
			locale,
		)
		return mevarchimStr + " " + monthStr
	case ev.date.Month() == hdate.Tishrei && ev.date.Day() == 1:
		s, _ := locales.LookupTranslation("Rosh Hashana", locale)
		year := ev.date.Year()
		locale = strings.ToLower(locale)
		if locale == "he" || locale == "he-x-nonikud" {
			return s + " " + gematriya.Gematriya(year)
		}
		return s + " " + strconv.Itoa(year)
	case ev.flags.Has(YomKippurKatan):
		rchStr, _ := locales.LookupTranslation("Yom Kippur Katan", locale)
		monthStr, _ := locales.LookupTranslation(strings.TrimPrefix(ev.desc, yomKippurKatanPrefix), locale)
		return rchStr + " " + monthStr
	}
	str, _ := locales.LookupTranslation(ev.desc, locale)
	return str
}

// RenderBrief returns the same as Render except for Yom Kippur Katan, which
// drops the month it precedes ("Yom Kippur Katan Tevet" => "Yom Kippur Katan").
func (ev HolidayEvent) RenderBrief(locale string) string {
	if ev.flags.Has(YomKippurKatan) {
		s, _ := locales.LookupTranslation("Yom Kippur Katan", locale)
		return SmartApostrophe(s)
	}
	return ev.Render(locale)
}

func (ev HolidayEvent) Flags() Flags {
	return ev.flags
}

func (ev HolidayEvent) Emoji() string {
	if ev.emoji != "" {
		return ev.emoji
	}
	switch ev.flags {
	case SpecialShabbat:
		return "🕍"
	case RoshChodesh:
		return "🌒"
	case ShabbatMevarchim, YomKippurKatan | MinorFast:
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

// Basename returns a simplified, untranslated name that groups related
// events, e.g. "Sukkot" for "Sukkot III (CH”M)". Rosh Chodesh and Yom
// Kippur Katan keep their full description, so a leap year's "Rosh Chodesh
// Adar I" and "Rosh Chodesh Adar II" stay distinct, matching @hebcal/core's
// RoshChodeshEvent and YomKippurKatanEvent.
func (ev HolidayEvent) Basename() string {
	if ev.flags.HasAny(RoshChodesh | YomKippurKatan) {
		return ev.desc
	}
	str := ev.desc
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

// Categories returns the category and sub-categories for a holiday,
// mirroring HolidayEvent.getCategories() in @hebcal/core: chol hamoed days are
// major, otherwise the flag-based category is used, falling back to a minor or
// major holiday when the flags alone are inconclusive.
func (ev HolidayEvent) Categories() []string {
	if ev.CholHaMoedDay != 0 {
		return []string{"holiday", "major", "cholhamoed"}
	}
	cats := CategoriesFromFlags(ev.flags)
	if cats[0] != "unknown" {
		return cats
	}
	if minorHolidays[ev.desc] {
		return []string{"holiday", "minor"}
	}
	return []string{"holiday", "major"}
}
