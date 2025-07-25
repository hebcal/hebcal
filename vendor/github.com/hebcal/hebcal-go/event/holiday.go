package event

import (
	"regexp"
	"strconv"
	"strings"

	"github.com/hebcal/gematriya"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/locales"
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
	if (ev.Flags & ROSH_CHODESH) != 0 {
		rchStr, _ := locales.LookupTranslation("Rosh Chodesh", locale)
		monthStr, _ := locales.LookupTranslation(ev.Desc[13:], locale)
		return rchStr + " " + monthStr
	} else if ev.Date.Month() == hdate.Tishrei && ev.Date.Day() == 1 {
		s, _ := locales.LookupTranslation("Rosh Hashana", locale)
		year := ev.Date.Year()
		locale = strings.ToLower(locale)
		if locale == "he" || locale == "he-x-nonikud" {
			return s + " " + gematriya.Gematriya(year)
		}
		return s + " " + strconv.Itoa(year)
	} else if (ev.Flags & YOM_KIPPUR_KATAN) != 0 {
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
