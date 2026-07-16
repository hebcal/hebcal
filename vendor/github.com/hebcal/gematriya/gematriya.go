// Package gematriya provides a system of writing numbers as Hebrew letters.
// Numerical values are represented using letters of the Hebrew alef-bet
// (alphabet).
package gematriya

// Punctuation that marks a run of letters as a number rather than a word.
const (
	geresh    = '׳' // U+05F3, follows a lone letter
	gershayim = '״' // U+05F4, precedes the final letter of a multi-letter number
)

// Letters for each place value, indexed by digit. Index 0 is unused so that a
// digit can index its own letter directly. Hundreds stop at 300 because 400 and
// above are spelled with one or more tav.
var (
	units    = [10]rune{0, 'א', 'ב', 'ג', 'ד', 'ה', 'ו', 'ז', 'ח', 'ט'}
	tens     = [10]rune{0, 'י', 'כ', 'ל', 'מ', 'נ', 'ס', 'ע', 'פ', 'צ'}
	hundreds = [4]rune{0, 'ק', 'ר', 'ש'}
)

const tav = 'ת' // 400, the largest letter

// appendLetters appends the letters spelling num, most significant first, and
// returns the extended slice. Nothing is appended for num <= 0.
func appendLetters(dst []rune, num int) []rune {
	if num <= 0 {
		return dst
	}
	// There is no letter above 400, so larger values repeat tav: 900 is תתק.
	for num >= 400 {
		dst = append(dst, tav)
		num -= 400
	}
	if h := num / 100; h != 0 {
		dst = append(dst, hundreds[h])
		num %= 100
	}
	// Spelled 10+5 and 10+6, these would be names of God, so 15 and 16 are
	// written as 9+6 (ט״ו) and 9+7 (ט״ז) instead.
	if num == 15 || num == 16 {
		return append(dst, units[9], units[num-9])
	}
	if t := num / 10; t != 0 {
		dst = append(dst, tens[t])
	}
	if u := num % 10; u != 0 {
		dst = append(dst, units[u])
	}
	return dst
}

// Gematriya converts a numerical value to a string of Hebrew letters.
//
// When specifying years of the Hebrew calendar in the present millennium,
// we omit the thousands (which is presently 5 [ה]).
func Gematriya(number int) string {
	// Large enough for any number below 1,000,000; append spills to the heap
	// beyond that.
	var buf [16]rune
	letters := buf[:0]

	if thousands := number / 1000; thousands > 0 && thousands != 5 {
		letters = appendLetters(letters, thousands)
		letters = append(letters, geresh)
	}

	start := len(letters)
	letters = appendLetters(letters, number%1000)
	switch len(letters) - start {
	case 0: // nothing below the thousands, which already carries a geresh
	case 1:
		letters = append(letters, geresh)
	default:
		// Shift the final letter over to make room for the gershayim.
		last := len(letters) - 1
		final := letters[last]
		letters[last] = gershayim
		letters = append(letters, final)
	}
	return string(letters)
}
