package event

import (
	"math/bits"
	"strconv"
	"strings"
)

// Flags is a bitmask of event attributes and categories. An event carries
// the flags that apply to it, and Options.Mask selects events by them.
type Flags uint32

// When adding a flag, also add its name to flagNames below.
const (
	// Chag, yontiff, yom tov
	Chag Flags = 1 << iota
	// Light candles 18 minutes before sundown
	LightCandles
	// End of holiday (end of Yom Tov)
	YomTovEnds
	// Observed only in the Diaspora (chutz l'aretz)
	ChulOnly
	// Observed only in Israel
	ILOnly
	// Light candles in the evening at Tzeit time (3 small stars)
	LightCandlesTzeis
	// Candle-lighting for Chanukah
	ChanukahCandles
	// Rosh Chodesh, beginning of a new Hebrew month
	RoshChodesh
	// Minor fasts like Tzom Tammuz, Ta'anit Esther, ...
	MinorFast
	// Shabbat Shekalim, Zachor, ...
	SpecialShabbat
	// Weekly sedrot on Saturdays
	ParshaHashavua
	// Daily page of Talmud (Bavli)
	DafYomi
	// Days of the Omer
	OmerCount
	// Yom HaShoah, Yom HaAtzma'ut, ...
	ModernHoliday
	// Yom Kippur and Tish'a B'Av
	MajorFast
	// On the Saturday before Rosh Chodesh
	ShabbatMevarchim
	// Molad
	Molad
	// A personal event: a Yahrzeit, birthday or Hebrew anniversary
	// (see UserEvent).
	User
	// Daily Hebrew date ("11th of Sivan, 5780")
	HebrewDate
	// A holiday that's not major, modern, rosh chodesh, or a fast day
	MinorHoliday
	// Evening before a major or minor holiday
	Erev
	// Chol haMoed, intermediate days of Pesach or Sukkot
	CholHamoed
	// Mishna Yomi
	MishnaYomi
	// Yom Kippur Katan, minor day of atonement on the day preceding each Rosh Chodesh
	YomKippurKatan
	// Zemanim, halachic times of day
	Zmanim
	// Daily page of Jerusalem Talmud (Yerushalmi)
	YerushalmiYomi
	// Daily page of Nach (Nevi'im + Ketuvim)
	NachYomi
	// Daily learning schedule supplied by a plugin (e.g. 929, Daily Rambam)
	// via the dailylearning registry, with no dedicated flag of its own.
	DailyLearning
	// BeHaB fast days on Monday, Thursday and Monday after Pesach and Sukkot
	BeHaB
	// Latest time for Kiddush Levana, according to the Maharil
	KiddushLevana
	// Yizkor, the memorial prayer recited on Yom Kippur, Shmini Atzeret, the
	// last day of Pesach and the last day of Shavuot
	Yizkor
)

// Has reports whether f contains every flag set in flags.
// For a single flag, e.g. ev.Flags().Has(event.Chag), this is
// simply "is this flag set". Has(0) is always true.
func (f Flags) Has(flags Flags) bool {
	return f&flags == flags
}

// HasAny reports whether f contains at least one of the flags set in flags,
// e.g. ev.Flags().HasAny(event.MajorFast | event.MinorFast).
// HasAny(0) is always false.
func (f Flags) HasAny(flags Flags) bool {
	return f&flags != 0
}

// With returns a copy of f with the given flags set.
func (f Flags) With(flags Flags) Flags {
	return f | flags
}

// Without returns a copy of f with the given flags cleared.
func (f Flags) Without(flags Flags) Flags {
	return f &^ flags
}

// flagNames holds the name of each flag's constant, indexed by bit position.
var flagNames = [...]string{
	"Chag",
	"LightCandles",
	"YomTovEnds",
	"ChulOnly",
	"ILOnly",
	"LightCandlesTzeis",
	"ChanukahCandles",
	"RoshChodesh",
	"MinorFast",
	"SpecialShabbat",
	"ParshaHashavua",
	"DafYomi",
	"OmerCount",
	"ModernHoliday",
	"MajorFast",
	"ShabbatMevarchim",
	"Molad",
	"User",
	"HebrewDate",
	"MinorHoliday",
	"Erev",
	"CholHamoed",
	"MishnaYomi",
	"YomKippurKatan",
	"Zmanim",
	"YerushalmiYomi",
	"NachYomi",
	"DailyLearning",
	"BeHaB",
	"KiddushLevana",
	"Yizkor",
}

// String returns the names of the flags set in f joined by "|",
// e.g. "Chag|LightCandles". It returns "0" when no flags are set.
// Bits without a defined flag are rendered in hex.
func (f Flags) String() string {
	if f == 0 {
		return "0"
	}
	var sb strings.Builder
	for rest := f; rest != 0; {
		i := bits.TrailingZeros32(uint32(rest))
		bit := Flags(1) << i
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
