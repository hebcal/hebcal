package molad

import "time"

// harHabayisLMTOffset is the local mean time offset of Har Habayis from
// Jerusalem standard time (UTC+2). Its longitude, 35.2354°, is 5.2354° east of
// the 30° meridian the zone is named for, which at 4 minutes per degree is
// 20 minutes and 56.496 seconds.
const harHabayisLMTOffset = 1256496 * time.Millisecond

// jerusalemStandardTime is UTC+2, never Israel daylight time.
var jerusalemStandardTime = time.FixedZone("UTC+2", 2*60*60)

// Instant returns the moment of the molad, in UTC.
//
// The molad's wall clock is local mean time at Har Habayis, so it is read in
// Jerusalem standard time (UTC+2) and shifted back by 20 minutes and 56.496
// seconds. Daylight saving time is intentionally not applied; convert with
// [time.Time.In] for display. Chalakim (10/3 of a second each) are truncated
// to the millisecond. The instant is correct for dates before the
// Gregorian reform too, since [time.Time] is proleptic Gregorian.
func (m Molad) Instant() time.Time {
	millis := int64(m.Chalakim) * 10000 / 3
	gy, gm, gd := m.Date.ProlepticGreg()
	t := time.Date(gy, gm, gd, m.Hours, m.Minutes, 0, 0, jerusalemStandardTime)
	return t.Add(time.Duration(millis)*time.Millisecond - harHabayisLMTOffset).UTC()
}

// TchilasZmanKidushLevana3Days returns the earliest time of Kiddush Levana
// according to Rabbeinu Yonah's opinion, 3 days (72 hours) after the molad.
// The time is returned even if it falls during the day, when Kiddush Levana
// cannot be said.
func (m Molad) TchilasZmanKidushLevana3Days() time.Time {
	return m.Instant().Add(72 * time.Hour)
}

// TchilasZmanKidushLevana7Days returns the earliest time of Kiddush Levana
// according to the opinions that it is not said until 7 days (168 hours) after
// the molad. The time is returned even if it falls during the day.
func (m Molad) TchilasZmanKidushLevana7Days() time.Time {
	return m.Instant().Add(168 * time.Hour)
}

// SofZmanKidushLevanaBetweenMoldos returns the latest time of Kiddush Levana
// according to the Maharil's opinion: halfway between molad and molad. That
// adds half of 29 days, 12 hours and 793 chalakim, or 14 days, 18 hours,
// 22 minutes and 1.666 seconds, to the molad. The time is returned even if it
// falls during the day.
func (m Molad) SofZmanKidushLevanaBetweenMoldos() time.Time {
	const half = (24*14+18)*time.Hour + 22*time.Minute + 1666*time.Millisecond
	return m.Instant().Add(half)
}

// SofZmanKidushLevana15Days returns the latest time of Kiddush Levana
// according to the Shulchan Aruch (Orach Chaim 426), 15 days (360 hours) after
// the molad. The time is returned even if it falls during the day.
func (m Molad) SofZmanKidushLevana15Days() time.Time {
	return m.Instant().Add(15 * 24 * time.Hour)
}
