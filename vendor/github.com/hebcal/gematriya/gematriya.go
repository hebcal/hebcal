// Package gematriya provides a system of writing numbers as Hebrew letters.
// Numerical values are represented using letters of the Hebrew alef-bet
// (alphabet).
package gematriya

const geresh = "׳"
const gershayim = "״"

func num2heb(num int) string {
	switch num {
	case 1:
		return "א"
	case 2:
		return "ב"
	case 3:
		return "ג"
	case 4:
		return "ד"
	case 5:
		return "ה"
	case 6:
		return "ו"
	case 7:
		return "ז"
	case 8:
		return "ח"
	case 9:
		return "ט"
	case 10:
		return "י"
	case 20:
		return "כ"
	case 30:
		return "ל"
	case 40:
		return "מ"
	case 50:
		return "נ"
	case 60:
		return "ס"
	case 70:
		return "ע"
	case 80:
		return "פ"
	case 90:
		return "צ"
	case 100:
		return "ק"
	case 200:
		return "ר"
	case 300:
		return "ש"
	case 400:
		return "ת"
	case 500:
		return "תק"
	case 600:
		return "תר"
	case 700:
		return "תש"
	case 800:
		return "תת"
	case 900:
		return "תתק"
	case 1000:
		return "תתר"
	default:
		return "*INVALID*"
	}
}

func num2digits(number int) []int {
	digits := make([]int, 0, 10)
	num := number
	for num > 0 {
		if num == 15 || num == 16 {
			digits = append(digits, 9)
			digits = append(digits, num-9)
			break
		}
		incr := 100
		var i int
		for i = 400; i > num; i -= incr {
			if i == incr {
				incr = incr / 10
			}
		}
		digits = append(digits, i)
		num -= i
	}
	return digits
}

// Gematriya converts a numerical value to a string of Hebrew letters.
//
// When specifying years of the Hebrew calendar in the present millennium,
// we omit the thousands (which is presently 5 [ה]).
func Gematriya(number int) string {
	thousands := number / 1000
	str := ""
	if thousands > 0 && thousands != 5 {
		tdigits := num2digits(thousands)
		for _, digit := range tdigits {
			str += num2heb(digit)
		}
		str += geresh
	}
	digits := num2digits(number % 1000)
	if len(digits) == 1 {
		return str + num2heb(digits[0]) + geresh
	}
	for idx, digit := range digits {
		if idx+1 == len(digits) {
			str += gershayim
		}
		str += num2heb(digit)
	}
	return str
}
