package sunrise

import (
	"time"
)

// MeanSolarNoon calculates the time at which the sun is at its highest
// altitude. The returned time is in Julian days.
func MeanSolarNoon(longitude float64, year int, month time.Month, day int) float64 {
	t := time.Date(year, month, day, 12, 0, 0, 0, time.UTC)
	return TimeToJulianDay(t) - longitude/360
}
