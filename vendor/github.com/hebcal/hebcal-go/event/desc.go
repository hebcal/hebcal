package event

// Memoer is implemented by events that carry a longer note of their own: the
// molad announced on Shabbat Mevarchim, the meaning of Yom Kippur Katan, the
// Torah portion of the Shabbat that erev-Shabbat candle-lighting begins, the
// Kiddush Levana times, a daily-learning reading's details.
type Memoer interface {
	// Memo returns the note in locale, or "" if the event has none.
	Memo(locale string) string
}

// Memo returns ev's memo in locale, or "" if it has none.
func Memo(ev CalEvent, locale string) string {
	if m, ok := ev.(Memoer); ok {
		return m.Memo(locale)
	}
	return ""
}

// Memo returns the meaning of Yom Kippur Katan, untranslated, e.g. "Minor Day
// of Atonement on the day preceeding Rosh Chodesh Sivan" (sic). The month
// name carries a typographic apostrophe, as in a rendered title ("Sh’vat").
// Other holidays have no memo of their own.
func (ev HolidayEvent) Memo(locale string) string {
	if !ev.flags.Has(YomKippurKatan) || len(ev.desc) <= len(yomKippurKatanPrefix) {
		return ""
	}
	return "Minor Day of Atonement on the day preceeding Rosh Chodesh " +
		SmartApostrophe(ev.desc[len(yomKippurKatanPrefix):])
}

const yomKippurKatanPrefix = "Yom Kippur Katan "
