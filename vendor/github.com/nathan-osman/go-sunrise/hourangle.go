package sunrise

import (
	"math"
)

// HourAngle calculates the second of the two angles required to locate a point
// on the celestial sphere in the equatorial coordinate system.
func HourAngle(latitude, declination float64) float64 {
	var (
		latitudeRad    = latitude * Degree
		declinationRad = declination * Degree
		numerator      = -0.01449 - math.Sin(latitudeRad)*math.Sin(declinationRad)
		denominator    = math.Cos(latitudeRad) * math.Cos(declinationRad)
	)

	// Check for no sunrise/sunset
	if numerator/denominator > 1 {
		// Sun never rises
		return math.MaxFloat64
	}

	if numerator/denominator < -1 {
		// Sun never sets
		return -1 * math.MaxFloat64
	}

	return math.Acos(numerator/denominator) / Degree
}
