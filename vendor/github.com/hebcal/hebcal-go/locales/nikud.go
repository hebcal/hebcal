package locales

import (
	"strings"
)

func HebrewStripNikkud(str string) string {
	src := []rune(str)
	var builder strings.Builder
	builder.Grow(len(src))
	for _, r := range src {
		if r < 0x0590 || r > 0x05c7 || (r > 0x05bd && r < 0x05bf) {
			builder.WriteRune(r)
		}
	}
	return builder.String()
}
