package event

import (
	"math/bits"
	"strconv"
	"strings"
)

// HolidayFlags is a bitmask of event attributes and categories.
type HolidayFlags uint32

// When adding a flag, also add its name to flagNames below.
const (
	// Chag, yontiff, yom tov
	CHAG HolidayFlags = 1 << iota
	// Light candles 18 minutes before sundown
	LIGHT_CANDLES
	// End of holiday (end of Yom Tov)
	YOM_TOV_ENDS
	// Observed only in the Diaspora (chutz l'aretz)
	CHUL_ONLY
	// Observed only in Israel
	IL_ONLY
	// Light candles in the evening at Tzeit time (3 small stars)
	LIGHT_CANDLES_TZEIS
	// Candle-lighting for Chanukah
	CHANUKAH_CANDLES
	// Rosh Chodesh, beginning of a new Hebrew month
	ROSH_CHODESH
	// Minor fasts like Tzom Tammuz, Ta'anit Esther, ...
	MINOR_FAST
	// Shabbat Shekalim, Zachor, ...
	SPECIAL_SHABBAT
	// Weekly sedrot on Saturdays
	PARSHA_HASHAVUA
	// Daily page of Talmud (Bavli)
	DAF_YOMI
	// Days of the Omer
	OMER_COUNT
	// Yom HaShoah, Yom HaAtzma'ut, ...
	MODERN_HOLIDAY
	// Yom Kippur and Tish'a B'Av
	MAJOR_FAST
	// On the Saturday before Rosh Chodesh
	SHABBAT_MEVARCHIM
	// Molad
	MOLAD
	// Yahrzeit or Hebrew Anniversary
	USER_EVENT
	// Daily Hebrew date ("11th of Sivan, 5780")
	HEBREW_DATE
	// A holiday that's not major, modern, rosh chodesh, or a fast day
	MINOR_HOLIDAY
	// Evening before a major or minor holiday
	EREV
	// Chol haMoed, intermediate days of Pesach or Sukkot
	CHOL_HAMOED
	// Mishna Yomi
	MISHNA_YOMI
	// Yom Kippur Katan, minor day of atonement on the day preceding each Rosh Chodesh
	YOM_KIPPUR_KATAN
	// Zemanim, halachic times of day
	ZMANIM
	// Daily page of Jerusalem Talmud (Yerushalmi)
	YERUSHALMI_YOMI
	// Daily page of Nach (Nevi'im + Ketuvim)
	NACH_YOMI
	// Daily learning schedule supplied by a plugin (e.g. 929, Daily Rambam)
	// via the dailylearning registry, with no dedicated flag of its own.
	DAILY_LEARNING
)

// Has reports whether f contains every flag set in flags.
// For a single flag, e.g. ev.GetFlags().Has(event.CHAG), this is
// simply "is this flag set". Has(0) is always true.
func (f HolidayFlags) Has(flags HolidayFlags) bool {
	return f&flags == flags
}

// HasAny reports whether f contains at least one of the flags set in flags,
// e.g. ev.GetFlags().HasAny(event.MAJOR_FAST | event.MINOR_FAST).
// HasAny(0) is always false.
func (f HolidayFlags) HasAny(flags HolidayFlags) bool {
	return f&flags != 0
}

// With returns a copy of f with the given flags set.
func (f HolidayFlags) With(flags HolidayFlags) HolidayFlags {
	return f | flags
}

// Without returns a copy of f with the given flags cleared.
func (f HolidayFlags) Without(flags HolidayFlags) HolidayFlags {
	return f &^ flags
}

// flagNames holds the constant name for each flag, indexed by bit position.
var flagNames = [...]string{
	"CHAG",
	"LIGHT_CANDLES",
	"YOM_TOV_ENDS",
	"CHUL_ONLY",
	"IL_ONLY",
	"LIGHT_CANDLES_TZEIS",
	"CHANUKAH_CANDLES",
	"ROSH_CHODESH",
	"MINOR_FAST",
	"SPECIAL_SHABBAT",
	"PARSHA_HASHAVUA",
	"DAF_YOMI",
	"OMER_COUNT",
	"MODERN_HOLIDAY",
	"MAJOR_FAST",
	"SHABBAT_MEVARCHIM",
	"MOLAD",
	"USER_EVENT",
	"HEBREW_DATE",
	"MINOR_HOLIDAY",
	"EREV",
	"CHOL_HAMOED",
	"MISHNA_YOMI",
	"YOM_KIPPUR_KATAN",
	"ZMANIM",
	"YERUSHALMI_YOMI",
	"NACH_YOMI",
	"DAILY_LEARNING",
}

// String returns the names of the flags set in f joined by "|",
// e.g. "CHAG|LIGHT_CANDLES". It returns "0" when no flags are set.
// Bits without a defined flag are rendered in hex.
func (f HolidayFlags) String() string {
	if f == 0 {
		return "0"
	}
	var sb strings.Builder
	for rest := f; rest != 0; {
		i := bits.TrailingZeros32(uint32(rest))
		bit := HolidayFlags(1) << i
		rest &^= bit
		if sb.Len() != 0 {
			sb.WriteByte('|')
		}
		if i < len(flagNames) {
			sb.WriteString(flagNames[i])
		} else {
			sb.WriteString("0x")
			sb.WriteString(strconv.FormatUint(uint64(bit), 16))
		}
	}
	return sb.String()
}
