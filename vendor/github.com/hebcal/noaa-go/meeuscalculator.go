/*
 * noaa-go - a Go port of the KosherJava Zmanim solar-position calculators
 * Copyright © 2026 Eliyahu Hershfeld (original Java Zmanim API)
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

// MeeusCalculator implements sunrise and sunset methods using the
// higher-accuracy solar-position algorithm of Jean Meeus from Astronomical
// Algorithms (2nd ed.). Unlike [NOAACalculator] (which uses the low-accuracy
// method from the same work, good to roughly 0.01° / about 1 minute of time),
// this calculator computes the Earth's heliocentric position from the abridged
// VSOP87 series in Meeus's Appendix III, then derives the Sun's apparent
// geocentric longitude (applying the FK5 frame correction, nutation in longitude
// and the aberration of light), apparent right ascension and declination, and
// the equation of time per Meeus chapter 28. The resulting solar position is
// good to roughly an arc-second.
//
// For horizon events (sunrise / sunset) the extra precision is largely masked by
// atmospheric refraction and is therefore typically worth only a few seconds over
// [NOAACalculator]. The improvement is most visible for solar noon, the equation
// of time, and the Sun's elevation / azimuth when it is well above the horizon.
//
// # ΔT (terrestrial vs. universal time)
//
// Meeus computes positions in Terrestrial Time (TT), whereas the civil times
// returned here are Universal Time (UT ≈ UTC). This calculator therefore applies
// an estimate of ΔT (currently about a minute) when evaluating the Sun's
// position. This is more correct than [NOAACalculator], which omits ΔT. The
// correction can be disabled via [MeeusCalculator.SetApplyDeltaT] to reproduce
// NOAA-style UT = TT behavior, which is useful for direct comparison.
//
// The zero value is not usable; construct one with [NewMeeusCalculator].
type MeeusCalculator struct {
	astronomicalCalculator

	// applyDeltaT selects whether to apply the ΔT (TT - UT) correction when
	// evaluating the Sun's position. Defaults to true (the astronomically correct
	// behavior).
	applyDeltaT bool
}

// NewMeeusCalculator returns a MeeusCalculator with the default refraction,
// solar radius and earth radius, and with the ΔT correction enabled.
func NewMeeusCalculator() *MeeusCalculator {
	return &MeeusCalculator{
		astronomicalCalculator: newAstronomicalCalculator(),
		applyDeltaT:            true,
	}
}

// Ensure MeeusCalculator satisfies the Calculator interface.
var _ Calculator = (*MeeusCalculator)(nil)

// GetCalculatorName returns the descriptive name of the algorithm.
func (m *MeeusCalculator) GetCalculatorName() string {
	return "Jean Meeus Higher-Accuracy (VSOP87) Algorithm"
}

// SetApplyDeltaT enables or disables the ΔT (TT - UT) correction. Pass true to
// apply ΔT (the default, astronomically correct) or false to omit it and
// reproduce NOAA-style UT = TT behavior.
func (m *MeeusCalculator) SetApplyDeltaT(applyDeltaT bool) {
	m.applyDeltaT = applyDeltaT
}

// IsApplyDeltaT reports whether the ΔT correction is being applied.
func (m *MeeusCalculator) IsApplyDeltaT() bool {
	return m.applyDeltaT
}

// GetUTCSunrise calculates UTC sunrise as well as any time based on an angle
// above or below sunrise. See [Calculator] for the parameter and return
// semantics.
func (m *MeeusCalculator) GetUTCSunrise(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64 {
	return m.getUTCSunRiseSet(date, geoLocation, zenith, adjustForElevation, solarEventSunrise)
}

// GetUTCSunset calculates UTC sunset as well as any time based on an angle above
// or below sunset. See [Calculator] for the parameter and return semantics.
func (m *MeeusCalculator) GetUTCSunset(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64 {
	return m.getUTCSunRiseSet(date, geoLocation, zenith, adjustForElevation, solarEventSunset)
}

func (m *MeeusCalculator) getUTCSunRiseSet(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool, event solarEvent) float64 {
	elevation := 0.0
	if adjustForElevation {
		elevation = geoLocation.Elevation
	}
	adjustedZenith := m.AdjustZenith(zenith, elevation, date)
	riseSet := m.getSunRiseSetUTC(date, geoLocation.Latitude, -geoLocation.Longitude, adjustedZenith, event)
	riseSet = riseSet / 60
	return math.Mod(math.Mod(riseSet, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// GetUTCNoon returns the UTC of solar noon for the given day at the given
// location in 24-hour format, using the high-accuracy equation of time.
func (m *MeeusCalculator) GetUTCNoon(date time.Time, geoLocation *GeoLocation) float64 {
	noon := m.getSolarNoonMidnightUTC(JulianDay(date), -geoLocation.Longitude, solarEventNoon)
	noon = noon / 60
	return math.Mod(math.Mod(noon, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// GetUTCMidnight returns the UTC of solar midnight (about 12 hours after solar
// noon) at the end of the given day at the given location in 24-hour format.
func (m *MeeusCalculator) GetUTCMidnight(date time.Time, geoLocation *GeoLocation) float64 {
	midnight := m.getSolarNoonMidnightUTC(JulianDay(date), -geoLocation.Longitude, solarEventMidnight)
	midnight = midnight / 60
	return math.Mod(math.Mod(midnight, 24)+24, 24) // ensure that the time is >= 0 and < 24
}

// getSolarNoonMidnightUTC returns the UTC of solar noon, or the upcoming midnight
// (about 12 hours after solar noon), in minutes after 0:00 UTC. The longitude is
// in degrees, positive west.
func (m *MeeusCalculator) getSolarNoonMidnightUTC(julianDay, longitude float64, event solarEvent) float64 {
	base := 720.0
	if event != solarEventNoon {
		base = 1440.0
	}
	tnoon := m.getJulianCenturiesFromJulianDay(julianDay + longitude/360.0)
	equationOfTime := m.getEquationOfTime(tnoon)
	solNoonUTC := base + (longitude * 4) - equationOfTime // minutes

	// Refine the equation of time at the calculated transit time.
	for i := 0; i < 3; i++ {
		newt := m.getJulianCenturiesFromJulianDay(julianDay + solNoonUTC/1440.0)
		equationOfTime = m.getEquationOfTime(newt)
		solNoonUTC = base + (longitude * 4) - equationOfTime
	}
	return solNoonUTC
}

// getSunRiseSetUTC returns the UTC of sunrise or sunset in minutes for the given
// day at the given location, using the high-accuracy solar declination and
// equation of time. The longitude is in degrees, positive west. It returns
// math.NaN if the event does not occur (polar day / night).
func (m *MeeusCalculator) getSunRiseSetUTC(date time.Time, latitude, longitude, zenith float64, event solarEvent) float64 {
	julianDay := JulianDay(date)

	// Use the declination at local solar noon as the starting point, then iterate.
	noonmin := m.getSolarNoonMidnightUTC(julianDay, longitude, solarEventNoon)
	tnoon := m.getJulianCenturiesFromJulianDay(julianDay + noonmin/1440.0)
	equationOfTime := m.getEquationOfTime(tnoon)
	solarDeclination := m.getSunDeclination(tnoon)
	hourAngle := meeusSunHourAngle(latitude, solarDeclination, zenith, event)
	delta := longitude - toDegrees(hourAngle)
	timeUTC := 720 + (4 * delta) - equationOfTime

	// Additional passes including the fractional Julian Day for improved accuracy.
	for i := 0; i < 2; i++ {
		newt := m.getJulianCenturiesFromJulianDay(julianDay + timeUTC/1440.0)
		equationOfTime = m.getEquationOfTime(newt)
		solarDeclination = m.getSunDeclination(newt)
		hourAngle = meeusSunHourAngle(latitude, solarDeclination, zenith, event)
		if math.IsNaN(hourAngle) {
			return math.NaN()
		}
		delta = longitude - toDegrees(hourAngle)
		timeUTC = 720 + (4 * delta) - equationOfTime
	}
	return timeUTC
}

// GetTimeAtAzimuth returns the UTC time (24-hour format) that the given solar
// azimuth will occur for the date and location. Only azimuth values of 90°
// (directly east) or 270° (directly west) are supported; any other value returns
// an [InvalidValueError]. math.NaN is returned for dates at latitudes where the
// azimuth does not occur (the tropics, polar regions or the equator).
func (m *MeeusCalculator) GetTimeAtAzimuth(date time.Time, geoLocation *GeoLocation, targetAzimuth float64) (float64, error) {
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
		julianCenturies := m.getJulianCenturiesFromJulianDay(julianDay + dateTime)
		ratio := tanDegrees(m.getSunDeclination(julianCenturies)) / tanDegrees(geoLocation.Latitude)
		if math.IsNaN(ratio) || ratio > 1.0 || ratio < -1.0 { // Tropics, Polar Regions, and Equator line divisions
			return math.NaN(), nil
		}
		sign := -1.0
		if targetAzimuth != 90.0 {
			sign = 1.0
		}
		offset := sign * (acosDegrees(ratio) / 360.0)
		dateTime = solarNoonBase + offset - (m.getEquationOfTime(julianCenturies) / 1440.0)
	}

	timeUTC := dateTime * 24.0
	return math.Mod(math.Mod(timeUTC, 24)+24, 24), nil // normalize to [0,24)
}

// GetSolarElevation returns the solar elevation, in degrees, at the given
// location and time, using the high-accuracy solar declination and equation of
// time. Elevation is sea-level based and corrected for atmospheric refraction; it
// is not adjusted for the observer's altitude.
func (m *MeeusCalculator) GetSolarElevation(instant time.Time, geoLocation *GeoLocation) float64 {
	return m.getSolarElevationAzimuth(instant, geoLocation, false)
}

// GetSolarAzimuth returns the solar azimuth, in degrees, at the given location
// and time, using the high-accuracy solar declination and equation of time. True
// south is 180°.
func (m *MeeusCalculator) GetSolarAzimuth(instant time.Time, geoLocation *GeoLocation) float64 {
	return m.getSolarElevationAzimuth(instant, geoLocation, true)
}

func (m *MeeusCalculator) getSolarElevationAzimuth(instant time.Time, geoLocation *GeoLocation, isAzimuth bool) float64 {
	lat := geoLocation.Latitude
	lon := geoLocation.Longitude
	utc := instant.UTC()
	fractionalDay := (float64(utc.Hour()) + (float64(utc.Minute())+
		(float64(utc.Second())+float64(utc.Nanosecond())/1_000_000_000.0)/60.0)/60.0) / 24.0
	jc := m.getJulianCenturiesFromJulianDay(JulianDay(utc) + fractionalDay)
	decl := m.getSunDeclination(jc)
	eot := m.getEquationOfTime(jc)
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

// meeusSunHourAngle returns the hour angle of the sun in radians for the given
// latitude, solar declination (both in degrees) and zenith. It returns math.NaN
// if the sun never reaches the zenith on that day (polar day / night).
func meeusSunHourAngle(latitude, solarDeclination, zenith float64, event solarEvent) float64 {
	ratio := cosDegrees(zenith)/(cosDegrees(latitude)*cosDegrees(solarDeclination)) -
		tanDegrees(latitude)*tanDegrees(solarDeclination)
	if ratio < -1.0 || ratio > 1.0 {
		return math.NaN() // the sun never reaches this zenith (polar day / night)
	}
	hourAngle := math.Acos(ratio)
	if event == solarEventSunset {
		hourAngle = -hourAngle
	}
	return hourAngle
}

// ---------------------------------------------------------------------------
// Time scales
// ---------------------------------------------------------------------------

// getJulianCenturiesFromJulianDay converts a Universal-Time Julian day to Julian
// centuries since J2000.0 in Terrestrial Time. When IsApplyDeltaT reports true
// the ΔT (TT - UT) offset is added so that the solar-position series, which are
// defined in TT, are evaluated correctly.
func (m *MeeusCalculator) getJulianCenturiesFromJulianDay(julianDayUT float64) float64 {
	jde := julianDayUT
	if m.applyDeltaT {
		jde += getDeltaT(julianDayUT) / 86400.0 // ΔT is in seconds; convert to days
	}
	return (jde - JulianDayJan1_2000) / JulianDaysPerCentury
}

// GetDeltaT estimates ΔT = TT - UT in seconds for the given Julian day, using the
// polynomial expressions of Espenak and Meeus. The value is approximate
// (especially in the far past / future) but accurate to a few seconds for the
// modern era.
func (m *MeeusCalculator) GetDeltaT(julianDay float64) float64 {
	return getDeltaT(julianDay)
}

func getDeltaT(julianDay float64) float64 {
	year := 2000.0 + (julianDay-JulianDayJan1_2000)/365.25
	var t float64
	switch {
	case year >= 2005 && year < 2050:
		t = year - 2000
		return 62.92 + 0.32217*t + 0.005589*t*t
	case year >= 1986 && year < 2005:
		t = year - 2000
		return 63.86 + 0.3345*t - 0.060374*t*t + 0.0017275*t*t*t +
			0.000651814*math.Pow(t, 4) + 0.00002373599*math.Pow(t, 5)
	case year >= 1961 && year < 1986:
		t = year - 1975
		return 45.45 + 1.067*t - t*t/260.0 - t*t*t/718.0
	case year >= 2050 && year < 2150:
		return -20 + 32*math.Pow((year-1820)/100.0, 2) - 0.5628*(2150-year)
	default: // generic parabola of Espenak and Meeus for dates well outside the calibrated range
		u := (year - 1820) / 100.0
		return -20 + 32*u*u
	}
}

// ---------------------------------------------------------------------------
// High-accuracy solar position (Meeus chapters 22, 25, 28 with abridged VSOP87)
// ---------------------------------------------------------------------------

// GetSunDeclination returns the apparent declination of the sun, in degrees,
// computed from the apparent geocentric ecliptic coordinates derived from the
// VSOP87 series. julianCenturies is measured in Terrestrial-Time centuries since
// J2000.0.
func (m *MeeusCalculator) GetSunDeclination(julianCenturies float64) float64 {
	return m.getSunDeclination(julianCenturies)
}

func (m *MeeusCalculator) getSunDeclination(julianCenturies float64) float64 {
	sun := getSunApparentEclipticCoordinates(julianCenturies) // {lambda, beta} in degrees
	lambda := sun[0]
	beta := sun[1]
	epsilon := getTrueObliquity(julianCenturies)
	sinDec := sinDegrees(beta)*cosDegrees(epsilon) + cosDegrees(beta)*sinDegrees(epsilon)*sinDegrees(lambda)
	return asinDegrees(sinDec)
}

// GetSunRightAscension returns the apparent geocentric right ascension of the
// sun, in degrees (0-360). julianCenturies is measured in Terrestrial-Time
// centuries since J2000.0.
func (m *MeeusCalculator) GetSunRightAscension(julianCenturies float64) float64 {
	return getSunRightAscension(julianCenturies)
}

func getSunRightAscension(julianCenturies float64) float64 {
	sun := getSunApparentEclipticCoordinates(julianCenturies)
	lambda := sun[0]
	beta := sun[1]
	epsilon := getTrueObliquity(julianCenturies)
	y := sinDegrees(lambda)*cosDegrees(epsilon) - tanDegrees(beta)*sinDegrees(epsilon)
	x := cosDegrees(lambda)
	ra := toDegrees(math.Atan2(y, x))
	return math.Mod(math.Mod(ra, 360)+360, 360)
}

// GetSunApparentEclipticCoordinates returns the sun's apparent geocentric
// ecliptic longitude and latitude, in degrees, as {longitude, latitude}. They are
// built from the Earth's heliocentric VSOP87 position with the FK5 frame
// correction, nutation in longitude and the aberration of light applied (Meeus
// chapter 25). julianCenturies is measured in Terrestrial-Time centuries since
// J2000.0.
func (m *MeeusCalculator) GetSunApparentEclipticCoordinates(julianCenturies float64) [2]float64 {
	return getSunApparentEclipticCoordinates(julianCenturies)
}

func getSunApparentEclipticCoordinates(julianCenturies float64) [2]float64 {
	tau := julianCenturies / 10.0                   // Julian millennia
	helioL := toDegrees(sumVSOPSeries(earthL, tau)) // Earth heliocentric longitude (deg)
	helioB := toDegrees(sumVSOPSeries(earthB, tau)) // Earth heliocentric latitude (deg)
	radiusR := sumVSOPSeries(earthR, tau)           // Earth radius vector (AU)

	// Geocentric position of the Sun is diametrically opposite the heliocentric Earth.
	theta := helioL + 180.0 // geocentric longitude (deg), dynamical (VSOP87) frame
	beta := -helioB         // geocentric latitude (deg)

	// FK5 frame correction (Meeus 25.9): convert from the VSOP87 dynamical frame to FK5.
	lambdaPrime := theta - 1.397*julianCenturies - 0.00031*julianCenturies*julianCenturies
	deltaLongFK5 := -0.09033 + 0.03916*(cosDegrees(lambdaPrime)+sinDegrees(lambdaPrime))*tanDegrees(beta)
	deltaLatFK5 := 0.03916 * (cosDegrees(lambdaPrime) - sinDegrees(lambdaPrime))
	theta += deltaLongFK5 / 3600.0
	beta += deltaLatFK5 / 3600.0

	// Nutation in longitude (apparent place) and aberration of light.
	nut := getNutation(julianCenturies) // {Δψ, Δε} in arc-seconds
	aberration := -20.4898 / radiusR    // arc-seconds (Meeus 25, abridged)
	lambda := theta + nut[0]/3600.0 + aberration/3600.0

	return [2]float64{math.Mod(math.Mod(lambda, 360)+360, 360), beta}
}

// GetTrueObliquity returns the true obliquity of the ecliptic ε = ε₀ + Δε, in
// degrees (mean obliquity, Meeus 22.2, plus nutation in obliquity).
// julianCenturies is measured in Terrestrial-Time centuries since J2000.0.
func (m *MeeusCalculator) GetTrueObliquity(julianCenturies float64) float64 {
	return getTrueObliquity(julianCenturies)
}

func getTrueObliquity(julianCenturies float64) float64 {
	seconds := 21.448 - julianCenturies*(46.8150+julianCenturies*(0.00059-julianCenturies*0.001813))
	epsilon0 := 23.0 + (26.0+(seconds/60.0))/60.0
	deltaEpsilon := getNutation(julianCenturies)[1] / 3600.0
	return epsilon0 + deltaEpsilon
}

// GetNutation returns the principal terms of the nutation in longitude (Δψ) and
// obliquity (Δε), in arc-seconds, as {Δψ, Δε} (Meeus chapter 22, abridged).
// julianCenturies is measured in Terrestrial-Time centuries since J2000.0.
func (m *MeeusCalculator) GetNutation(julianCenturies float64) [2]float64 {
	return getNutation(julianCenturies)
}

func getNutation(julianCenturies float64) [2]float64 {
	t := julianCenturies
	omega := 125.04452 - 1934.136261*t + 0.0020708*t*t + t*t*t/450000.0 // moon's ascending node
	lSun := 280.4665 + 36000.7698*t                                     // mean longitude of the sun
	lMoon := 218.3165 + 481267.8813*t                                   // mean longitude of the moon
	deltaPsi := -17.20*sinDegrees(omega) - 1.32*sinDegrees(2*lSun) -
		0.23*sinDegrees(2*lMoon) + 0.21*sinDegrees(2*omega)
	deltaEpsilon := 9.20*cosDegrees(omega) + 0.57*cosDegrees(2*lSun) +
		0.10*cosDegrees(2*lMoon) - 0.09*cosDegrees(2*omega)
	return [2]float64{deltaPsi, deltaEpsilon}
}

// GetEquationOfTime returns the equation of time - apparent minus mean solar
// time - in minutes of time, following Meeus chapter 28
// (E = L₀ - 0.0057183° - α + Δψ·cos ε). The sign convention matches
// [NOAACalculator] so the rise/set formulas are interchangeable. julianCenturies
// is measured in Terrestrial-Time centuries since J2000.0.
func (m *MeeusCalculator) GetEquationOfTime(julianCenturies float64) float64 {
	return m.getEquationOfTime(julianCenturies)
}

func (m *MeeusCalculator) getEquationOfTime(julianCenturies float64) float64 {
	tau := julianCenturies / 10.0
	// Mean longitude of the sun (Meeus 28.1), in degrees.
	l0 := 280.4664567 + tau*(360007.6982779+tau*(0.03032028+
		tau*(1.0/49931.0-tau*(1.0/15300.0+tau/2000000.0))))
	l0 = math.Mod(math.Mod(l0, 360)+360, 360)
	alpha := getSunRightAscension(julianCenturies)
	nut := getNutation(julianCenturies)
	epsilon := getTrueObliquity(julianCenturies)
	e := l0 - 0.0057183 - alpha + (nut[0]/3600.0)*cosDegrees(epsilon) // degrees
	// Reduce to (-180, 180] so the *4 conversion gives the customary +/- ~16 minute range.
	e = math.Mod(math.Mod(e+180, 360)+360, 360) - 180
	return e * 4.0 // 1 degree = 4 minutes of time
}

// sumVSOPSeries evaluates a VSOP87 series of the form
// Σₙ (Σᵢ Aᵢ·cos(Bᵢ + Cᵢ·τ)) · τⁿ, where the coefficients are stored in 1e-8 of
// the series' native unit (radians for longitude/latitude, AU for the radius
// vector). tau is the time in Julian millennia from J2000.0.
func sumVSOPSeries(series [][][3]float64, tau float64) float64 {
	total := 0.0
	tauPow := 1.0
	for _, terms := range series {
		sum := 0.0
		for _, term := range terms {
			sum += term[0] * math.Cos(term[1]+term[2]*tau)
		}
		total += sum * tauPow
		tauPow *= tau
	}
	return total / 1.0e8
}
