package sunrise

import (
	"math"
)

// EquationOfCenter calculates the angular difference between the position of
// the earth in its elliptical orbit and the position it would occupy in a
// circular orbit for the given mean anomaly.
func EquationOfCenter(solarAnomaly float64) float64 {
	var (
		anomalyInRad = solarAnomaly * (math.Pi / 180)
		anomalySin   = math.Sin(anomalyInRad)
		anomaly2Sin  = math.Sin(2 * anomalyInRad)
		anomaly3Sin  = math.Sin(3 * anomalyInRad)
	)
	return 1.9148*anomalySin + 0.0200*anomaly2Sin + 0.0003*anomaly3Sin
}
