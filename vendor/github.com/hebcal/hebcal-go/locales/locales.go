package locales

import "strings"

// AllLocales is an array of all supported locale names.
var AllLocales = []string{
	"en",
	"ashkenazi",
	"he",
	"he-x-NoNikud",
	"ashkenazi_litvish",
	"ashkenazi_poylish",
	"ashkenazi_romanian",
	"ashkenazi_standard",
	"de",
	"es",
	"fi",
	"fr",
	"hu",
	"pl",
	"pt",
	"ro",
	"ru",
	"uk",
}

// LookupTranslation returns a message for the given key.
// It returns false for ok if such a message could not be found.
func LookupTranslation(key string, locale string) (string, bool) {
	lang := strings.ToLower(locale)
	switch lang {
	case "", "en", "sephardic":
		return key, true
	case "he-x-nonikud":
		v, ok := Lookup_he_x_NoNikud(key)
		if ok {
			return v, true
		}
		v, ok = Lookup_he(key)
		if ok {
			return HebrewStripNikkud(v), true
		}
		return v, ok
	case "ashkenazi":
		return Lookup_ashkenazi(key)
	case "he":
		return Lookup_he(key)
	case "ashkenazi_litvish":
		return Lookup_ashkenazi_litvish(key)
	case "ashkenazi_poylish":
		return Lookup_ashkenazi_poylish(key)
	case "ashkenazi_romanian":
		return Lookup_ashkenazi_romanian(key)
	case "ashkenazi_standard":
		return Lookup_ashkenazi_standard(key)
	case "de":
		return Lookup_de(key)
	case "es":
		return Lookup_es(key)
	case "fi":
		return Lookup_fi(key)
	case "fr":
		return Lookup_fr(key)
	case "hu":
		return Lookup_hu(key)
	case "pl":
		return Lookup_pl(key)
	case "pt":
		return Lookup_pt(key)
	case "ro":
		return Lookup_ro(key)
	case "ru":
		return Lookup_ru(key)
	case "uk":
		return Lookup_uk(key)
	}
	return key, false
}
