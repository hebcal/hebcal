/*
 * noaa-go - a Go port of the KosherJava Zmanim solar-position calculators
 * Copyright © 2004-2026 Eliyahu Hershfeld (original Java Zmanim API)
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
	"fmt"
	"math"
	"time"
)

// distanceType selects which quantity vincentyInverseFormula computes.
type distanceType int

const (
	distance       distanceType = iota // a distance-type calculation
	initialBearing                     // an initial-bearing-type calculation
	finalBearing                       // a final-bearing-type calculation
)

const (
	minuteMillis = 60 * 1000
	hourMillis   = minuteMillis * 60
)

// GeoLocation contains the location information (latitude, longitude, elevation
// and time zone) required for astronomical calculations. The elevation field is
// not used by every calculation engine and is ignored if not applicable; check
// the documentation for the specific [Calculator] to see whether elevation is
// used.
type GeoLocation struct {
	// Name is the location name used for display, for example "Lakewood, NJ".
	Name string

	// Latitude is the latitude in degrees, for example 40.096 for Lakewood, NJ.
	// It must be between -90° and 90°; values south of the equator are negative.
	Latitude float64

	// Longitude is the longitude in degrees, for example -74.222 for Lakewood,
	// NJ. It must be between -180° and 180°; values west of the Prime Meridian
	// (Greenwich) are negative.
	Longitude float64

	// Elevation is the elevation in Meters above sea level. It must not be
	// negative.
	Elevation float64

	// TimeZone is the time zone for the location.
	TimeZone *time.Location
}

// NewGeoLocation creates a GeoLocation with the given parameters, validating the
// latitude, longitude and elevation.
//
// latitude must be between -90 and 90; latitudes south of the equator should be
// negative. longitude must be between -180 and 180; longitudes west of the Prime
// Meridian should be negative. elevation is in Meters above sea level and must
// not be negative.
func NewGeoLocation(name string, latitude, longitude, elevation float64, timeZone *time.Location) (*GeoLocation, error) {
	if latitude > 90 || latitude < -90 || math.IsNaN(latitude) {
		return nil, &InvalidValueError{Field: "latitude", Message: "latitude must be between -90 and 90"}
	}
	if longitude > 180 || longitude < -180 || math.IsNaN(longitude) {
		return nil, &InvalidValueError{Field: "longitude", Message: "longitude must be between -180 and 180"}
	}
	if elevation < 0 || math.IsNaN(elevation) || math.IsInf(elevation, 0) {
		return nil, &InvalidValueError{Field: "elevation", Message: "elevation cannot be negative, NaN or infinite"}
	}
	return &GeoLocation{
		Name:      name,
		Latitude:  latitude,
		Longitude: longitude,
		Elevation: elevation,
		TimeZone:  timeZone,
	}, nil
}

// NewGeoLocationGreenwich returns the default GeoLocation: the Prime Meridian at
// Greenwich, England (longitude 0, latitude 51.4772, the location of the Royal
// Observatory, Greenwich) with a time zone of GMT.
func NewGeoLocationGreenwich() *GeoLocation {
	return &GeoLocation{
		Name:      "Greenwich, England",
		Latitude:  51.4772,
		Longitude: 0,
		Elevation: 0,
		TimeZone:  time.UTC,
	}
}

// GetLocalMeanTimeOffset returns the location's local mean time offset from the
// local clock time defined by the time zone offset in effect at the supplied
// instant. The globe is split into 360°, with 15° per hour of the day. For a
// location at a longitude evenly divisible by 15, at solar noon (adjusted for the
// equation of time) the sun is directly overhead, so a user 1° west of this will
// have noon 4 minutes after local clock noon. Lakewood, NJ, whose longitude is
// -74.222, is 0.778° away from the closest multiple of 15 at -75°, multiplied by
// 4 to yield 3 minutes and 10 seconds earlier than local clock time.
//
// A positive value is returned East of the 15° time-zone line and a negative
// value West of it.
func (g *GeoLocation) GetLocalMeanTimeOffset(instant time.Time) time.Duration {
	_, offsetSeconds := instant.In(g.TimeZone).Zone()
	timezoneOffsetMillis := int64(offsetSeconds) * 1000
	// Match the Java truncation: the whole double expression is cast to long
	// (truncated toward zero) only after the subtraction.
	millis := int64(g.Longitude*4*minuteMillis - float64(timezoneOffsetMillis))
	return time.Duration(millis) * time.Millisecond
}

// GetAntimeridianAdjustment returns the number of days to adjust the date for
// antimeridian (180th meridian) crossover. This is needed for edge cases such as
// Samoa that use a different calendar date than expected based on their
// geographic location. The result is typically 0 unless the date crosses the
// antimeridian, in which case it is +1 or -1.
func (g *GeoLocation) GetAntimeridianAdjustment(instant time.Time) int {
	localHoursOffset := float64(g.GetLocalMeanTimeOffset(instant).Milliseconds()) / float64(hourMillis)
	if localHoursOffset >= 20 { // offset 20 hours or more in the future (e.g. Samoa)
		return 1 // roll the date forward a day
	} else if localHoursOffset <= -20 { // offset 20 hours or more in the past
		return -1 // roll the date back a day
	}
	return 0 // 99.999% of the world will have no adjustment
}

// GetGeodesicInitialBearing calculates the initial geodesic bearing between this
// location and the destination using Thaddeus Vincenty's inverse formula.
func (g *GeoLocation) GetGeodesicInitialBearing(location *GeoLocation) float64 {
	return g.vincentyInverseFormula(location, initialBearing)
}

// GetGeodesicFinalBearing calculates the final geodesic bearing between this
// location and the destination using Thaddeus Vincenty's inverse formula.
func (g *GeoLocation) GetGeodesicFinalBearing(location *GeoLocation) float64 {
	return g.vincentyInverseFormula(location, finalBearing)
}

// GetGeodesicDistance calculates the geodesic distance in Meters between this
// location and the destination using Thaddeus Vincenty's inverse formula.
func (g *GeoLocation) GetGeodesicDistance(location *GeoLocation) float64 {
	return g.vincentyInverseFormula(location, distance)
}

// vincentyInverseFormula calculates the initial bearing, final bearing or
// geodesic distance between this location and the destination using Thaddeus
// Vincenty's inverse formula. See T Vincenty, "Direct and Inverse Solutions of
// Geodesics on the Ellipsoid with application of nested equations", Survey
// Review, vol XXII no 176, 1975.
func (g *GeoLocation) vincentyInverseFormula(location *GeoLocation, formula distanceType) float64 {
	majorSemiAxis := 6378137.0
	minorSemiAxis := 6356752.3142
	f := 1 / 298.257223563 // WGS-84 ellipsoid
	l := toRadians(location.Longitude - g.Longitude)
	u1 := math.Atan((1 - f) * math.Tan(toRadians(g.Latitude)))
	u2 := math.Atan((1 - f) * math.Tan(toRadians(location.Latitude)))
	sinU1, cosU1 := math.Sin(u1), math.Cos(u1)
	sinU2, cosU2 := math.Sin(u2), math.Cos(u2)

	lambda := l
	lambdaP := 2 * math.Pi
	iterLimit := 20
	var sinLambda, cosLambda, sinSigma, cosSigma, sigma float64
	var sinAlpha, cosSqAlpha, cos2SigmaM, c float64
	for math.Abs(lambda-lambdaP) > 1e-12 && iterLimit > 0 {
		iterLimit--
		sinLambda = math.Sin(lambda)
		cosLambda = math.Cos(lambda)
		sinSigma = math.Sqrt((cosU2*sinLambda)*(cosU2*sinLambda) +
			(cosU1*sinU2-sinU1*cosU2*cosLambda)*(cosU1*sinU2-sinU1*cosU2*cosLambda))
		if sinSigma == 0 {
			return 0 // co-incident points
		}
		cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda
		sigma = math.Atan2(sinSigma, cosSigma)
		sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma
		cosSqAlpha = 1 - sinAlpha*sinAlpha
		cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha
		if math.IsNaN(cos2SigmaM) {
			cos2SigmaM = 0 // equatorial line: cosSqAlpha=0 (§6)
		}
		c = f / 16 * cosSqAlpha * (4 + f*(4-3*cosSqAlpha))
		lambdaP = lambda
		lambda = l + (1-c)*f*sinAlpha*
			(sigma+c*sinSigma*(cos2SigmaM+c*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)))
	}
	if iterLimit == 0 {
		return math.NaN() // formula failed to converge
	}

	uSq := cosSqAlpha * (majorSemiAxis*majorSemiAxis - minorSemiAxis*minorSemiAxis) / (minorSemiAxis * minorSemiAxis)
	a := 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)))
	b := uSq / 1024 * (256 + uSq*(-128+uSq*(74-47*uSq)))
	deltaSigma := b * sinSigma *
		(cos2SigmaM + b/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-b/6*cos2SigmaM*
			(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)))
	dist := minorSemiAxis * a * (sigma - deltaSigma)

	// initial bearing
	fwdAz := toDegrees(math.Atan2(cosU2*sinLambda, cosU1*sinU2-sinU1*cosU2*cosLambda))
	// final bearing
	revAz := toDegrees(math.Atan2(cosU1*sinLambda, -sinU1*cosU2+cosU1*sinU2*cosLambda))
	switch formula {
	case distance:
		return dist
	case initialBearing:
		return fwdAz
	case finalBearing:
		return revAz
	default: // should never happen
		return math.NaN()
	}
}

// GetRhumbLineBearing returns the rhumb line bearing, in degrees, from this
// location to the destination.
func (g *GeoLocation) GetRhumbLineBearing(location *GeoLocation) float64 {
	dLon := toRadians(location.Longitude - g.Longitude)
	dPhi := math.Log(math.Tan(toRadians(location.Latitude)/2+math.Pi/4) /
		math.Tan(toRadians(g.Latitude)/2+math.Pi/4))
	if math.Abs(dLon) > math.Pi {
		if dLon > 0 {
			dLon = -(2*math.Pi - dLon)
		} else {
			dLon = 2*math.Pi + dLon
		}
	}
	return toDegrees(math.Atan2(dLon, dPhi))
}

// GetRhumbLineDistance returns the rhumb line distance, in Meters, from this
// location to the destination.
func (g *GeoLocation) GetRhumbLineDistance(location *GeoLocation) float64 {
	earthRadius := 6378137.0 // Earth's radius in meters (WGS-84)
	dLat := toRadians(location.Latitude) - toRadians(g.Latitude)
	dLon := math.Abs(toRadians(location.Longitude) - toRadians(g.Longitude))
	dPhi := math.Log(math.Tan(toRadians(location.Latitude)/2+math.Pi/4) /
		math.Tan(toRadians(g.Latitude)/2+math.Pi/4))
	q := dLat / dPhi
	if !(math.Abs(q) <= math.MaxFloat64) {
		q = math.Cos(toRadians(g.Latitude))
	}
	// if dLon over 180° take shorter rhumb across 180° meridian:
	if dLon > math.Pi {
		dLon = 2*math.Pi - dLon
	}
	d := math.Sqrt(dLat*dLat + q*q*dLon*dLon)
	return d * earthRadius
}

// String returns a human-readable representation of the location.
func (g *GeoLocation) String() string {
	tz := "UTC"
	if g.TimeZone != nil {
		tz = g.TimeZone.String()
	}
	return fmt.Sprintf("Location Name:\t\t\t%s\nLatitude:\t\t\t%v°\nLongitude:\t\t\t%v°\nElevation:\t\t\t%v Meters\nTimezone ID:\t\t\t%s",
		g.Name, g.Latitude, g.Longitude, g.Elevation, tz)
}
