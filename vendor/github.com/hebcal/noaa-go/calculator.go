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

// Package noaa calculates astronomical solar positions (sunrise, sunset, solar
// noon and midnight, solar elevation and azimuth) using the algorithms from the
// KosherJava Zmanim API (https://github.com/KosherJava/zmanim).
//
// Two algorithms are provided, both implementing the [Calculator] interface:
//
//   - [NOAACalculator] implements the widely used
//     National Oceanic and Atmospheric Administration algorithm, itself based
//     on the low-accuracy solar-position method from Jean Meeus's
//     Astronomical Algorithms.
//   - [MeeusCalculator] implements the higher-accuracy solar-position method
//     from the same work, computing the Earth's heliocentric position from the
//     abridged VSOP87 series.
//
// The package focuses exclusively on solar-position math; it deliberately omits
// the halachic (zmanim) calendar layers of the original library.
package noaa

import (
	"math"
	"time"
)

// GeometricZenith is the zenith of astronomical sunrise and sunset. The sun is
// 90° from the vertical 0°.
const GeometricZenith = 90.0

// JulianDayJan1_2000 is the Julian day of January 1, 2000, known as
// J2000.0 (https://en.wikipedia.org/wiki/Epoch_(astronomy)#J2000).
const JulianDayJan1_2000 = 2451545.0

// JulianDaysPerCentury is the number of Julian days per century.
const JulianDaysPerCentury = 36525.0

// Calculator is implemented by all solar-position calculating types. It allows
// the algorithm used to be swapped at runtime, making it easy to compare the
// results of different algorithms.
type Calculator interface {
	// GetCalculatorName returns the descriptive name of the algorithm.
	GetCalculatorName() string

	// GetUTCSunrise calculates UTC sunrise as well as any time based on an angle
	// above or below sunrise. The zenith is the angle below the vertical zenith
	// of 90°. When adjustForElevation is true the location's elevation is taken
	// into account. The result is the UTC time of sunrise in 24-hour format
	// (5:45:00 AM returns 5.75). math.NaN is returned if the event does not
	// occur (expected near the poles).
	GetUTCSunrise(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64

	// GetUTCSunset calculates UTC sunset as well as any time based on an angle
	// above or below sunset. See GetUTCSunrise for the parameter and return
	// semantics.
	GetUTCSunset(date time.Time, geoLocation *GeoLocation, zenith float64, adjustForElevation bool) float64

	// GetUTCNoon returns the UTC of solar noon for the given day at the given
	// location in 24-hour format (1:45:00 PM returns 13.75).
	GetUTCNoon(date time.Time, geoLocation *GeoLocation) float64

	// GetUTCMidnight returns the UTC of solar midnight (about 12 hours after
	// solar noon) at the end of the given day at the given location in 24-hour
	// format.
	GetUTCMidnight(date time.Time, geoLocation *GeoLocation) float64

	// GetTimeAtAzimuth returns the UTC time (24-hour format) that the given solar
	// azimuth will occur for the date and location. An error is returned for
	// unsupported azimuth values.
	GetTimeAtAzimuth(date time.Time, geoLocation *GeoLocation, azimuth float64) (float64, error)

	// GetSolarElevation returns the solar elevation, in degrees, for the
	// horizontal coordinate system at the given location and time. It can be
	// negative when the sun is below the horizon.
	GetSolarElevation(instant time.Time, geoLocation *GeoLocation) float64

	// GetSolarAzimuth returns the solar azimuth, in degrees, for the horizontal
	// coordinate system at the given location and time. True south is 180°.
	GetSolarAzimuth(instant time.Time, geoLocation *GeoLocation) float64
}

// solarEvent indicates what type of solar event is being calculated.
type solarEvent int

const (
	solarEventSunrise solarEvent = iota
	solarEventSunset
	solarEventNoon
	solarEventMidnight
)

// astronomicalCalculator holds the shared configuration (refraction, solar
// radius and earth radius) and helper methods used by all concrete calculators.
// It is embedded into [NOAACalculator] and [MeeusCalculator], which promote its
// exported methods.
type astronomicalCalculator struct {
	// refraction is the average solar refraction in degrees (default 34′, or
	// 0.5666°).
	refraction float64

	// solarRadius is the fixed, date-independent sun's radius in degrees
	// (default 16′, or 0.2666°). It is used only when useApparentSolarRadius is
	// false.
	solarRadius float64

	// useApparentSolarRadius selects whether sunrise and sunset use the
	// date-based apparent solar radius (the default) rather than the fixed
	// solarRadius.
	useApparentSolarRadius bool

	// earthRadius is the average earth radius in KM (default 6371.0088, the IUGG
	// mean radius R1 = (2a + b) / 3 of the WGS-84 ellipsoid). It only affects the
	// elevation adjustment, not the underlying solar-position calculations.
	earthRadius float64
}

func newAstronomicalCalculator() astronomicalCalculator {
	return astronomicalCalculator{
		refraction:             34.0 / 60.0,
		solarRadius:            16.0 / 60.0,
		useApparentSolarRadius: true,
		earthRadius:            6371.0088,
	}
}

// GetEarthRadius returns the earth radius in KM. It defaults to the commonly
// used average earth radius of 6371.0088 KM. It only affects the elevation
// adjustment, not the underlying solar-position calculations.
func (c *astronomicalCalculator) GetEarthRadius() float64 {
	return c.earthRadius
}

// SetEarthRadius sets the earth's radius in KM.
func (c *astronomicalCalculator) SetEarthRadius(earthRadius float64) {
	c.earthRadius = earthRadius
}

// GetRefraction returns the refraction value, in degrees, used when calculating
// sunrise and sunset. The default value is 34 arcminutes (0.5666°). The
// refraction, solar radius and elevation adjustment are added to the zenith
// used to calculate sunrise and sunset.
func (c *astronomicalCalculator) GetRefraction() float64 {
	return c.refraction
}

// SetRefraction overrides the default refraction of the calculator, in degrees.
func (c *astronomicalCalculator) SetRefraction(refraction float64) {
	c.refraction = refraction
}

// GetSolarRadius returns the fixed sun's radius in degrees, used only when
// IsUseApparentSolarRadius reports false. The default value is 16 arcminutes.
// The sun's apparent radius in fact differs by the time of the year, which is
// exactly the error the date-based apparent solar radius eliminates; see
// GetApparentSolarRadius.
func (c *astronomicalCalculator) GetSolarRadius() float64 {
	return c.solarRadius
}

// SetSolarRadius sets the Sun's radius as a fixed, date-independent value in
// degrees, for example 16.0/60.0 for the conventional 16′. This is typically
// used to match another implementation that assumes a constant solar radius.
//
// A fixed radius has no effect while IsUseApparentSolarRadius reports true (the
// default), so this method also disables that mode (equivalent to calling
// SetUseApparentSolarRadius(false)). To return to the date-based radius
// afterward, pass true to SetUseApparentSolarRadius.
//
// An error is returned if solarRadius is negative or NaN.
func (c *astronomicalCalculator) SetSolarRadius(solarRadius float64) error {
	if solarRadius < 0 || math.IsNaN(solarRadius) {
		return &InvalidValueError{Field: "solarRadius", Message: "solar radius must be a non-negative number"}
	}
	c.solarRadius = solarRadius
	c.useApparentSolarRadius = false
	return nil
}

// IsUseApparentSolarRadius reports whether the date-based apparent solar radius
// is used (the default), rather than the fixed GetSolarRadius value.
func (c *astronomicalCalculator) IsUseApparentSolarRadius() bool {
	return c.useApparentSolarRadius
}

// SetUseApparentSolarRadius sets whether the date-based apparent solar radius
// should be used (the default is true).
func (c *astronomicalCalculator) SetUseApparentSolarRadius(useApparentSolarRadius bool) {
	c.useApparentSolarRadius = useApparentSolarRadius
}

// GetElevationAdjustment returns the adjustment to the zenith, in degrees,
// required to account for the elevation in Meters. Since a person at a higher
// elevation can see farther below the horizon, the calculation for sunrise /
// sunset is calculated below the horizon used at sea level. This is only used
// for sunrise and sunset.
//
// The algorithm used is:
//
//	elevationAdjustment = acos(earthRadiusInMeters / (earthRadiusInMeters + elevationMeters))
//
// The source of this algorithm is Calendrical Calculations by Edward M. Reingold
// and Nachum Dershowitz.
func (c *astronomicalCalculator) GetElevationAdjustment(elevation float64) float64 {
	return toDegrees(math.Acos(c.earthRadius / (c.earthRadius + (elevation / 1000))))
}

// AdjustZenith adjusts the zenith of astronomical sunrise and sunset to account
// for solar refraction, solar radius and elevation. The total value for the
// zenith is 90+(5/6) or 90.8333333° for true sunrise/sunset. Note that this only
// adjusts the value if the zenith is exactly 90° ([GeometricZenith]); for values
// below and above this no correction is done.
//
// The date is used only for the apparent solar radius; a zero date is treated as
// unavailable, in which case the fixed GetSolarRadius value is used.
func (c *astronomicalCalculator) AdjustZenith(zenith, elevation float64, date time.Time) float64 {
	adjustedZenith := zenith
	if zenith == GeometricZenith { // only adjust if it is exactly sunrise or sunset
		if c.useApparentSolarRadius && !date.IsZero() {
			adjustedZenith = zenith + c.GetApparentSolarRadius(date) + c.GetRefraction() + c.GetElevationAdjustment(elevation)
		} else {
			adjustedZenith = zenith + c.GetSolarRadius() + c.GetRefraction() + c.GetElevationAdjustment(elevation)
		}
	}
	return adjustedZenith
}

// GetApparentSolarRadius returns the Sun's apparent angular semi-diameter (the
// "solar radius") for the given date, in degrees. The Sun's apparent size
// changes over the year as the Earth-Sun distance varies between the perihelion
// (~Jan 3, when the Sun is largest) and the aphelion (~Jul 5, when it is
// smallest). The value is read from a precomputed table keyed by calendar day;
// February 29 reuses the February 28 value.
//
// This is the value the calculator applies automatically for sunrise and sunset
// while IsUseApparentSolarRadius reports true (the default). A zero date returns
// the default fixed solar radius of 16/60.
//
// The result should not be passed to SetSolarRadius: doing so would freeze a
// single day's value onto the calculator and apply it incorrectly to every other
// date.
func (c *astronomicalCalculator) GetApparentSolarRadius(date time.Time) float64 {
	if date.IsZero() {
		return 16.0 / 60.0
	}
	month, day := date.Month(), date.Day()
	// 2050 is a common year; a February 29 input resolves to February 28.
	if month == time.February && day == 29 {
		day = 28
	}
	dayOfYear := time.Date(2050, month, day, 0, 0, 0, 0, time.UTC).YearDay()
	return solarRadiusByDayOfYear[dayOfYear-1]
}

// JulianDay returns the Julian day (https://en.wikipedia.org/wiki/Julian_day)
// for the calendar date of the given time. Only the year, month and day of t are
// used, in t's own location. The number returned is for the start of the Julian
// day; fractional days / time should be added later.
func JulianDay(t time.Time) float64 {
	year, month, day := t.Date()
	return julianDay(year, int(month), day)
}

func julianDay(year, month, day int) float64 {
	if month <= 2 {
		year -= 1
		month += 12
	}
	a := year / 100
	b := 2 - a + a/4
	return math.Floor(365.25*float64(year+4716)) + math.Floor(30.6001*float64(month+1)) + float64(day) + float64(b) - 1524.5
}

// JulianCenturiesFromJulianDay converts a Julian day to the number of Julian
// centuries since J2000.0.
func JulianCenturiesFromJulianDay(julianDay float64) float64 {
	return (julianDay - JulianDayJan1_2000) / JulianDaysPerCentury
}

// ---------------------------------------------------------------------------
// Trigonometric helpers operating in degrees.
// ---------------------------------------------------------------------------

func toRadians(deg float64) float64 { return deg * math.Pi / 180.0 }
func toDegrees(rad float64) float64 { return rad * 180.0 / math.Pi }

// tanDegrees returns the tangent of an angle expressed in degrees.
func tanDegrees(angle float64) float64 { return math.Tan(toRadians(angle)) }

// sinDegrees returns the sine of an angle expressed in degrees.
func sinDegrees(angle float64) float64 { return math.Sin(toRadians(angle)) }

// cosDegrees returns the cosine of an angle expressed in degrees.
func cosDegrees(angle float64) float64 { return math.Cos(toRadians(angle)) }

// acosDegrees returns the arc cosine, in degrees, of the given value.
func acosDegrees(x float64) float64 { return toDegrees(math.Acos(x)) }

// asinDegrees returns the arc sine, in degrees, of the given value.
func asinDegrees(x float64) float64 { return toDegrees(math.Asin(x)) }
