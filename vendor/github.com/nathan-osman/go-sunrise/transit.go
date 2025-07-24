package sunrise

import (
	"math"
)

// SolarTransit calculates the Julian data for the local true solar transit.
func SolarTransit(d, solarAnomaly, eclipticLongitude float64) float64 {
	equationOfTime := 0.0053*math.Sin(solarAnomaly*Degree) -
		0.0069*math.Sin(2*eclipticLongitude*Degree)
	return d + equationOfTime
}
