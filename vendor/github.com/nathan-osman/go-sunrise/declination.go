package sunrise

import (
	"math"
)

// Declination calculates one of the two angles required to locate a point on
// the celestial sphere in the equatorial coordinate system. The ecliptic
// longitude parameter must be in degrees.
func Declination(eclipticLongitude float64) float64 {
	return math.Asin(math.Sin(eclipticLongitude*Degree)*0.39779) / Degree
}
