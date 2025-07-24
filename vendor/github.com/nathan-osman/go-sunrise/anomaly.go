package sunrise

import (
	"math"
)

// SolarMeanAnomaly calculates the angle of the sun relative to the earth for
// the specified Julian day.
func SolarMeanAnomaly(d float64) float64 {
	v := math.Remainder(357.5291+0.98560028*(d-J2000), 360)
	if v < 0 {
		v += 360
	}
	return v
}
