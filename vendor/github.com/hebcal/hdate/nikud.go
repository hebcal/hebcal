package hdate

import (
	"strings"
)

// isNikkud reports whether r is a Hebrew niqqud (vowel point) or cantillation
// mark. U+05BE MAQAF is excluded: it is punctuation, not a diacritic.
func isNikkud(r rune) bool {
	return r >= 0x0590 && r <= 0x05c7 && r != 0x05be
}

// HebrewStripNikkud removes niqqud (vowel points) and cantillation marks from
// str, leaving the consonantal letters and the maqaf (־) in place.
//
// Strings with no such marks are returned unchanged.
func HebrewStripNikkud(str string) string {
	// Most strings have nothing to strip; returning them as-is costs no
	// allocation. IndexFunc also gives the prefix that can be copied wholesale.
	i := strings.IndexFunc(str, isNikkud)
	if i == -1 {
		return str
	}
	var sb strings.Builder
	sb.Grow(len(str))
	sb.WriteString(str[:i])
	for _, r := range str[i:] {
		if !isNikkud(r) {
			sb.WriteRune(r)
		}
	}
	return sb.String()
}
