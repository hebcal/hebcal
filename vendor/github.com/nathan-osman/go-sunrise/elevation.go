package sunrise

import (
	"math"
	"time"
)

// TimeOfElevation calculates the times of day when the sun is at a given elevation
// above the horizon on a given day at the specified location.
// Returns time.Time{} if there sun does not reach the elevation
func TimeOfElevation(latitude, longitude, elevation float64, year int, month time.Month, day int) (time.Time, time.Time) {
	var (
		d                 = MeanSolarNoon(longitude, year, month, day)
		solarAnomaly      = SolarMeanAnomaly(d)
		equationOfCenter  = EquationOfCenter(solarAnomaly)
		eclipticLongitude = EclipticLongitude(solarAnomaly, equationOfCenter, d)
		solarTransit      = SolarTransit(d, solarAnomaly, eclipticLongitude)
		declination       = Declination(eclipticLongitude)
		// https://solarsena.com/solar-elevation-angle-altitude/
		numerator   = math.Sin(elevation*Degree) - (math.Sin(latitude*Degree) * math.Sin(declination*Degree))
		denominator = math.Cos(latitude*Degree) * math.Cos(declination*Degree)
		hourAngle   = math.Acos(numerator / denominator)
		frac        = hourAngle / (2 * math.Pi)
		morning     = solarTransit - frac
		evening     = solarTransit + frac
	)

	// Check for cases where the sun never reaches the given elevation.
	if math.IsNaN(hourAngle) {
		return time.Time{}, time.Time{}
	}

	return JulianDayToTime(morning), JulianDayToTime(evening)
}

// Elevation calculates the angle of the sun above the horizon at a given moment
// at the specified location.
func Elevation(latitude, longitude float64, when time.Time) float64 {
	var (
		d                 = MeanSolarNoon(longitude, when.Year(), when.Month(), when.Day())
		solarAnomaly      = SolarMeanAnomaly(d)
		equationOfCenter  = EquationOfCenter(solarAnomaly)
		eclipticLongitude = EclipticLongitude(solarAnomaly, equationOfCenter, d)
		solarTransit      = SolarTransit(d, solarAnomaly, eclipticLongitude)
		declination       = Declination(eclipticLongitude)
		frac              = solarTransit - TimeToJulianDay(when)
		hourAngle         = 2 * math.Pi * frac
		// https://solarsena.com/solar-elevation-angle-altitude/
		firstPart         = math.Sin(latitude*Degree) * math.Sin(declination*Degree)
		secondPart        = math.Cos(latitude*Degree) * math.Cos(declination*Degree) * math.Cos(hourAngle)
	)

	return math.Asin(firstPart+secondPart) / Degree
}
