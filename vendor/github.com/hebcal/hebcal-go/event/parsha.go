package event

import (
	"strings"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/sedra"
	"github.com/hebcal/locales"
)

// ParshaEvent is the weekly Torah portion (Parashat HaShavua), read on a
// Saturday.
type ParshaEvent struct {
	date hdate.HDate
	// Parsha is the portion read, or the two of a doubled portion.
	Parsha sedra.Parsha
	// IL is true for the Israel reading schedule, false for the Diaspora's.
	IL bool
}

// NewParshaEvent constructs the event for parsha, read on the Saturday hd
// according to the Israel (il) or Diaspora schedule.
func NewParshaEvent(hd hdate.HDate, parsha sedra.Parsha, il bool) ParshaEvent {
	return ParshaEvent{date: hd, Parsha: parsha, IL: il}
}

// Date returns the Saturday on which the portion is read.
func (ev ParshaEvent) Date() hdate.HDate {
	return ev.date
}

func (ev ParshaEvent) Render(locale string) string {
	locale = strings.ToLower(locale)
	prefix, _ := locales.LookupTranslation("Parashat", locale)
	return prefix + " " + SmartApostrophe(ev.Parsha.Render(locale))
}

func (ev ParshaEvent) Flags() Flags {
	return ParshaHashavua
}

func (ev ParshaEvent) Emoji() string {
	return ""
}

// Desc returns "Parashat " and the portion's untranslated name, e.g.
// "Parashat Vayakhel-Pekudei".
func (ev ParshaEvent) Desc() string {
	return "Parashat " + ev.Basename()
}

// Basename returns the portion's untranslated name, e.g. "Vayakhel-Pekudei".
func (ev ParshaEvent) Basename() string {
	return strings.Join(ev.Parsha.Name, "-")
}

func (ev ParshaEvent) Categories() []string {
	return CategoriesFromFlags(ev.Flags())
}
