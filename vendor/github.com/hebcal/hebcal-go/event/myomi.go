package event

import (
	"strconv"

	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/locales"
	"github.com/hebcal/hebcal-go/mishnayomi"
)

type mishnaYomiEvent struct {
	Date   hdate.HDate
	Mishna mishnayomi.MishnaPair
}

func NewMishnaYomiEvent(hd hdate.HDate, mishna mishnayomi.MishnaPair) CalEvent {
	return mishnaYomiEvent{Date: hd, Mishna: mishna}
}

func (ev mishnaYomiEvent) GetDate() hdate.HDate {
	return ev.Date
}

func (ev mishnaYomiEvent) Render(locale string) string {
	m1 := ev.Mishna[0]
	m2 := ev.Mishna[1]
	tractate, _ := locales.LookupTranslation(m1.Tractate, locale)
	s := tractate + " " + strconv.Itoa(m1.Chap) + ":" + strconv.Itoa(m1.Verse) + "-"
	sameTractate := m1.Tractate == m2.Tractate
	if !sameTractate {
		tractate, _ := locales.LookupTranslation(m2.Tractate, locale)
		s += tractate + " "
	}
	if sameTractate && m2.Chap == m1.Chap {
		s += strconv.Itoa(m2.Verse)
	} else {
		s += strconv.Itoa(m2.Chap) + ":" + strconv.Itoa(m2.Verse)
	}
	return s
}

func (ev mishnaYomiEvent) GetFlags() HolidayFlags {
	return MISHNA_YOMI
}

func (ev mishnaYomiEvent) GetEmoji() string {
	return ""
}

func (ev mishnaYomiEvent) Basename() string {
	return ev.Mishna.String()
}
