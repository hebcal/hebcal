package locales

var dict_he_x_NoNikud = map[string]string{
	"Chukat": "חוקת",
	"Korach": "קורח",
	"Yom Kippur": "יום כיפור",
	"Yom Kippur Katan": "יום כיפור קטן",
}

func Lookup_he_x_NoNikud(s string) (string, bool) {
	v, ok := dict_he_x_NoNikud[s]
	if ok {
		return v, true
	}
	return s, false
}
