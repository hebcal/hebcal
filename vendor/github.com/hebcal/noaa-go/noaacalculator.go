/*
 * noaa-go - a Go port of the KosherJava Zmanim solar-position calculators
 * Copyright © 2011-2026 Eliyahu Hershfeld (original Java Zmanim API)
 * Go port Copyright © 2026 Michael J. Radwin
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; version 2.1 of the License.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 */

package noaa

import (
	"math"
	"time"
)

// NOAACalculator implements sunrise and sunset methods based on the NOAA
// (National Oceanic and Atmospheric Administration, https://noaa.gov) algorithm.
// The algorithm is itself based on equations from Astronomical Algorithms by Jean
// Meeus. Added to the algorithm is an adjustment of the zenith to account for
// elevation. See the Wikipedia Sunrise Equation article
// (https://en.wikipedia.org/wiki/Sunrise_equation).
//
// The zero value is not usable; construct one with [NewNOAACalculator].
type NOAACalculator struct {
	astronomicalCalculator
}

// NewNOAACalculator returns a NOAACalculator with the default refraction, solar
// radius and earth radius.
func NewNOAACalculator() *NOAACalculator {
	return &NOAACalculator{newAstronomicalCalculator()}
}

// Ensure NOAACalculator satisfies the Calculator interface.
var _ Calculator = (*NOAACalculator)(nil)

// GetCalculatorName returns the descriptive name of the algorithm.
func (n *NOAACalculator) GetCalculatorName() string {
	return "US National Oceanic and Atmospheric Administration Algorithm"
}

// GetUTCSunrise calculates UTC sunrise as well as any time based on an angle
// above or below sunrise. See [Calculator] for the parameter and return
// semantics.
func (n *NOAACalculator) GetUTCSunrise(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64 {
	return n.getUTCSunRiseSet(date, geoLocation, zenith, adjustForElevation, solarEventSunrise)
}

// GetUTCSunset calculates UTC sunset as well as any time based on an angle above
// or below sunset. See [Calculator] for the parameter and return semantics.
func (n *NOAACalculator) GetUTCSunset(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64 {
	return n.getUTCSunRiseSet(date, geoLocation, zenith, adjustForElevation, solarEventSunset)
}

// getUTCSunRiseSet calculates UTC sunrise or sunset as well as any time based on
// an angle above or below sunrise / sunset and returns it as a float64 in
// 24-hour format (5:45:00 AM returns 5.75).
func (n *NOAACalculator) getUTCSunRiseSet(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool, event solarEvent) float64 {
	elevation := 0.0
	if adjustForElevation {
		elevation = geoLocation.Elevation
	}
	adjustedZenith := n.AdjustZenith(zenith, elevation, date)
	riseSet := getSunRiseSetUTC(date, geoLocation.Latitude, -geoLocation.Longitude, adjustedZenith, event)
	riseSet = riseSet / 60
	return math.Mod(math.Mod(riseSet, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// GetSunGeometricMeanLongitude returns the Geometric Mean Longitude of the Sun,
// in degrees, for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunGeometricMeanLongitude(julianCenturies float64) float64 {
	return getSunGeometricMeanLongitude(julianCenturies)
}

// GetSunGeometricMeanAnomaly returns the Geometric Mean Anomaly of the Sun, in
// degrees, for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunGeometricMeanAnomaly(julianCenturies float64) float64 {
	return getSunGeometricMeanAnomaly(julianCenturies)
}

// GetEarthOrbitEccentricity returns the unitless eccentricity of earth's orbit
// for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetEarthOrbitEccentricity(julianCenturies float64) float64 {
	return getEarthOrbitEccentricity(julianCenturies)
}

// GetSunEquationOfCenter returns the equation of center for the sun, in degrees,
// for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunEquationOfCenter(julianCenturies float64) float64 {
	return getSunEquationOfCenter(julianCenturies)
}

// GetSunTrueLongitude returns the true longitude of the sun, in degrees, for the
// given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunTrueLongitude(julianCenturies float64) float64 {
	return getSunGeometricMeanLongitude(julianCenturies) + getSunEquationOfCenter(julianCenturies)
}

// GetSunApparentLongitude returns the apparent longitude of the sun, in degrees,
// for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunApparentLongitude(julianCenturies float64) float64 {
	return getSunApparentLongitude(julianCenturies)
}

// GetMeanObliquityOfEcliptic returns the mean obliquity of the ecliptic (axial
// tilt), in degrees, for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetMeanObliquityOfEcliptic(julianCenturies float64) float64 {
	return getMeanObliquityOfEcliptic(julianCenturies)
}

// GetObliquityCorrection returns the corrected obliquity of the ecliptic (axial
// tilt), in degrees, for the given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetObliquityCorrection(julianCenturies float64) float64 {
	return getObliquityCorrection(julianCenturies)
}

// GetSunDeclination returns the declination of the sun, in degrees, for the
// given number of Julian centuries since J2000.0.
func (n *NOAACalculator) GetSunDeclination(julianCenturies float64) float64 {
	return getSunDeclination(julianCenturies)
}

// GetEquationOfTime returns the equation of time - the difference between true
// solar time and mean solar time - in minutes of time, for the given number of
// Julian centuries since J2000.0.
func (n *NOAACalculator) GetEquationOfTime(julianCenturies float64) float64 {
	return getEquationOfTime(julianCenturies)
}

func getSunGeometricMeanLongitude(julianCenturies float64) float64 {
	longitude := 280.46646 + julianCenturies*(36000.76983+0.0003032*julianCenturies)
	return math.Mod(math.Mod(longitude, 360)+360, 360) // in the range 0 - 360
}

func getSunGeometricMeanAnomaly(julianCenturies float64) float64 {
	return 357.52911 + julianCenturies*(35999.05029-0.0001537*julianCenturies)
}

func getEarthOrbitEccentricity(julianCenturies float64) float64 {
	return 0.016708634 - julianCenturies*(0.000042037+0.0000001267*julianCenturies)
}

func getSunEquationOfCenter(julianCenturies float64) float64 {
	m := getSunGeometricMeanAnomaly(julianCenturies)
	sinm := sinDegrees(m)
	sin2m := sinDegrees(m + m)
	sin3m := sinDegrees(m + m + m)
	return sinm*(1.914602-julianCenturies*(0.004817+0.000014*julianCenturies)) +
		sin2m*(0.019993-0.000101*julianCenturies) + sin3m*0.000289
}

func getSunTrueLongitude(julianCenturies float64) float64 {
	return getSunGeometricMeanLongitude(julianCenturies) + getSunEquationOfCenter(julianCenturies)
}

func getSunApparentLongitude(julianCenturies float64) float64 {
	sunTrueLongitude := getSunTrueLongitude(julianCenturies)
	omega := 125.04 - 1934.136*julianCenturies
	return sunTrueLongitude - 0.00569 - 0.00478*sinDegrees(omega)
}

func getMeanObliquityOfEcliptic(julianCenturies float64) float64 {
	seconds := 21.448 - julianCenturies*(46.8150+julianCenturies*(0.00059-julianCenturies*(0.001813)))
	return 23.0 + (26.0+(seconds/60.0))/60.0
}

func getObliquityCorrection(julianCenturies float64) float64 {
	obliquityOfEcliptic := getMeanObliquityOfEcliptic(julianCenturies)
	omega := 125.04 - 1934.136*julianCenturies
	return obliquityOfEcliptic + 0.00256*cosDegrees(omega)
}

func getSunDeclination(julianCenturies float64) float64 {
	obliquityCorrection := getObliquityCorrection(julianCenturies)
	lambda := getSunApparentLongitude(julianCenturies)
	sint := sinDegrees(obliquityCorrection) * sinDegrees(lambda)
	return asinDegrees(sint)
}

func getEquationOfTime(julianCenturies float64) float64 {
	epsilon := getObliquityCorrection(julianCenturies)
	geomMeanLongSun := getSunGeometricMeanLongitude(julianCenturies)
	eccentricityEarthOrbit := getEarthOrbitEccentricity(julianCenturies)
	geomMeanAnomalySun := getSunGeometricMeanAnomaly(julianCenturies)
	y := tanDegrees(epsilon / 2.0)
	y *= y
	sin2l0 := sinDegrees(2.0 * geomMeanLongSun)
	sinm := sinDegrees(geomMeanAnomalySun)
	cos2l0 := cosDegrees(2.0 * geomMeanLongSun)
	sin4l0 := sinDegrees(4.0 * geomMeanLongSun)
	sin2m := sinDegrees(2.0 * geomMeanAnomalySun)
	equationOfTime := y*sin2l0 - 2.0*eccentricityEarthOrbit*sinm + 4.0*eccentricityEarthOrbit*y*
		sinm*cos2l0 - 0.5*y*y*sin4l0 - 1.25*eccentricityEarthOrbit*eccentricityEarthOrbit*sin2m
	return toDegrees(equationOfTime) * 4.0
}

// getSunHourAngle returns the hour angle of the sun in radians for the given
// latitude, solar declination (both in degrees) and zenith.
func getSunHourAngle(latitude, solarDeclination, zenith float64, event solarEvent) float64 {
	ratio := cosDegrees(zenith)/(cosDegrees(latitude)*cosDegrees(solarDeclination)) -
		tanDegrees(latitude)*tanDegrees(solarDeclination)
	hourAngle := math.Acos(ratio)
	if event == solarEventSunset {
		hourAngle = -hourAngle
	}
	return hourAngle
}

// GetSolarElevation returns the solar elevation, in degrees, for the horizontal
// coordinate system at the given location and time. It can be negative when the
// sun is below the horizon. Elevation is sea-level based and is not adjusted for
// altitude.
func (n *NOAACalculator) GetSolarElevation(instant time.Time, geoLocation *GeoLocation) float64 {
	return n.getSolarElevationAzimuth(instant, geoLocation, false)
}

// GetSolarAzimuth returns the solar azimuth, in degrees, for the horizontal
// coordinate system at the given location and time. True south is 180°.
func (n *NOAACalculator) GetSolarAzimuth(instant time.Time, geoLocation *GeoLocation) float64 {
	return n.getSolarElevationAzimuth(instant, geoLocation, true)
}

func (n *NOAACalculator) getSolarElevationAzimuth(instant time.Time, geoLocation *GeoLocation, isAzimuth bool) float64 {
	lat := geoLocation.Latitude
	lon := geoLocation.Longitude
	utc := instant.UTC()
	fractionalDay := (float64(utc.Hour()) + (float64(utc.Minute())+
		(float64(utc.Second())+float64(utc.Nanosecond())/1_000_000_000.0)/60.0)/60.0) / 24.0
	jd := JulianDay(utc) + fractionalDay
	jc := JulianCenturiesFromJulianDay(jd)
	decl := getSunDeclination(jc)
	eot := getEquationOfTime(jc)
	trueSolarTime := math.Mod((fractionalDay+eot/1440.0+lon/360.0)+2, 1)
	hourAngle := trueSolarTime*2*math.Pi - math.Pi
	cosZenith := sinDegrees(lat)*sinDegrees(decl) + cosDegrees(lat)*cosDegrees(decl)*math.Cos(hourAngle)
	zenithDeg := acosDegrees(math.Max(-1, math.Min(1, cosZenith)))
	elevation := (90.0 - zenithDeg) + adjustElevationForRefraction(90.0-zenithDeg)
	var azimuth float64
	azDenom := cosDegrees(lat) * sinDegrees(zenithDeg)
	if math.Abs(azDenom) > 0.001 {
		az := (sinDegrees(lat)*cosDegrees(zenithDeg) - sinDegrees(decl)) / azDenom
		sign := 1.0
		if hourAngle > 0 {
			sign = -1.0
		}
		azimuth = 180 - acosDegrees(math.Max(-1, math.Min(1, az)))*sign
	} else if lat > 0 {
		azimuth = 180
	} else {
		azimuth = 0
	}
	if isAzimuth {
		return math.Mod(azimuth+360, 360)
	}
	return elevation
}

// adjustElevationForRefraction applies a refraction adjustment, in degrees, to
// the given solar elevation.
func adjustElevationForRefraction(elevation float64) float64 {
	if elevation > 85.0 {
		return 0.0
	}
	te := tanDegrees(elevation)
	var correction float64
	if elevation > 5.0 {
		correction = 58.1/te - 0.07/math.Pow(te, 3) + 0.000086/math.Pow(te, 5)
	} else if elevation > -0.575 {
		correction = 1735.0 + elevation*(-518.2+elevation*(103.4+elevation*(-12.79+0.711*elevation)))
	} else {
		correction = -20.774 / te
	}
	return correction / 3600.0
}

// GetUTCNoon returns the UTC of solar noon for the given day at the given
// location in 24-hour format (1:45:00 PM returns 13.75).
func (n *NOAACalculator) GetUTCNoon(date time.Time, geoLocation *GeoLocation) float64 {
	noon := getSolarNoonMidnightUTC(JulianDay(date), -geoLocation.Longitude, solarEventNoon)
	noon = noon / 60
	return math.Mod(math.Mod(noon, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// GetUTCMidnight returns the UTC of solar midnight (about 12 hours after solar
// noon) at the end of the given day at the given location in 24-hour format.
func (n *NOAACalculator) GetUTCMidnight(date time.Time, geoLocation *GeoLocation) float64 {
	midnight := getSolarNoonMidnightUTC(JulianDay(date), -geoLocation.Longitude, solarEventMidnight)
	midnight = midnight / 60
	return math.Mod(math.Mod(midnight, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// getSolarNoonMidnightUTC returns the UTC of the current day's solar noon or the
// upcoming midnight (about 12 hours after solar noon) of the given day at the
// given location, in minutes after 0:00 UTC. The longitude is in degrees,
// positive west.
func getSolarNoonMidnightUTC(julianDay, longitude float64, event solarEvent) float64 {
	// no day-half shift: the loop epoch julianDay + solNoonUTC/1440 already lands on the event
	// First pass for approximate solar noon to calculate equation of time
	tnoon := JulianCenturiesFromJulianDay(julianDay + longitude/360.0)
	equationOfTime := getEquationOfTime(tnoon)
	solNoonUTC := (longitude * 4) - equationOfTime // minutes

	// Refine the equation of time at the calculated transit time.
	for i := 0; i < 2; i++ {
		newt := JulianCenturiesFromJulianDay(julianDay + solNoonUTC/1440.0)
		equationOfTime = getEquationOfTime(newt)
		base := 720.0
		if event != solarEventNoon {
			base = 1440.0
		}
		solNoonUTC = base + (longitude * 4) - equationOfTime
	}
	return solNoonUTC
}

// getSunRiseSetUTC returns the UTC of sunrise or sunset, in minutes, for the
// given day at the given location. The longitude is in degrees, positive west.
func getSunRiseSetUTC(date time.Time, latitude, longitude, zenith float64, event solarEvent) float64 {
	julianDay := JulianDay(date)

	// Find the time of solar noon at the location, and use that declination.
	noonmin := getSolarNoonMidnightUTC(julianDay, longitude, solarEventNoon)
	tnoon := JulianCenturiesFromJulianDay(julianDay + noonmin/1440.0)
	// First calculates sunrise and approximate length of day
	equationOfTime := getEquationOfTime(tnoon)
	solarDeclination := getSunDeclination(tnoon)
	hourAngle := getSunHourAngle(latitude, solarDeclination, zenith, event)
	delta := longitude - toDegrees(hourAngle)
	timeDiff := 4 * delta
	timeUTC := 720 + timeDiff - equationOfTime
	// Second pass includes fractional Julian Day in gamma calc
	newt := JulianCenturiesFromJulianDay(julianDay + timeUTC/1440.0)
	equationOfTime = getEquationOfTime(newt)
	solarDeclination = getSunDeclination(newt)
	hourAngle = getSunHourAngle(latitude, solarDeclination, zenith, event)
	delta = longitude - toDegrees(hourAngle)
	timeDiff = 4 * delta
	timeUTC = 720 + timeDiff - equationOfTime
	return timeUTC
}

// GetTimeAtAzimuth returns the UTC time (24-hour format) that the given solar
// azimuth will occur for the date and location. Only azimuth values of 90°
// (directly east) or 270° (directly west) are supported; any other value returns
// an [InvalidValueError]. math.NaN is returned for dates at latitudes where the
// azimuth does not occur (e.g. the tropics, poles or the equator).
func (n *NOAACalculator) GetTimeAtAzimuth(date time.Time, geoLocation *GeoLocation, targetAzimuth float64) (float64, error) {
	if targetAzimuth != 90.0 && targetAzimuth != 270.0 {
		return 0, &InvalidValueError{Field: "targetAzimuth", Message: "the targetAzimuth must be 90 or 270; other azimuth values are not supported"}
	}

	julianDay := JulianDay(date)
	solarNoonBase := 0.5 - (geoLocation.Longitude / 360.0)
	offsetBase := 0.25
	if targetAzimuth != 90.0 {
		offsetBase = 0.75
	}
	dateTime := solarNoonBase + offsetBase

	for i := 0; i < 3; i++ {
		julianCenturies := JulianCenturiesFromJulianDay(julianDay + dateTime)
		ratio := tanDegrees(getSunDeclination(julianCenturies)) / tanDegrees(geoLocation.Latitude)
		if math.IsNaN(ratio) || ratio > 1.0 || ratio < -1.0 { // Tropics, Poles, and Equator line divisions
			return math.NaN(), nil
		}
		sign := -1.0
		if targetAzimuth != 90.0 {
			sign = 1.0
		}
		offset := sign * (acosDegrees(ratio) / 360.0)
		dateTime = solarNoonBase + offset - (getEquationOfTime(julianCenturies) / 1440.0)
	}

	return math.Mod(math.Mod(dateTime*24, 24)+24, 24), nil
}
