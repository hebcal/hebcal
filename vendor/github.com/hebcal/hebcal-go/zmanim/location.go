package zmanim

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original C version, Copyright (C) 1994-2004 Danny Sadinoff
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

import "strings"

// Location represents a location for Zmanim
type Location struct {
	Name        string  // City name
	CountryCode string  // ISO 3166 two-letter in caps, e.g. "US", "GB", "IL"
	Latitude    float64 // In the range [-90,90]
	Longitude   float64 // In the range [-180,180]
	Elevation   int     // Elevation in meters above sea level (never negative)
	TimeZoneId  string  // timezone identifier such as "America/Los_Angeles" or "Asia/Jerusalem"
}

// NewLocation creates an instance of a Location object.
//
// elevation is the elevation in meters above sea level. It is used only when
// elevation-aware zmanim are requested (see [Zmanim.UseElevation]); negative
// values are clamped to 0.
//
// This function panics if the latitude or longitude are out of range.
func NewLocation(name string, countryCode string, latitude float64, longitude float64, elevation int, tzid string) Location {
	if latitude < -90 || latitude > 90 {
		panic("Latitude out of range [-90,90]")
	}
	if longitude < -180 || longitude > 180 {
		panic("Longitude out of range [-180,180]")
	}
	if elevation < 0 {
		elevation = 0
	}
	return Location{
		Name:        name,
		CountryCode: countryCode,
		Latitude:    latitude,
		Longitude:   longitude,
		Elevation:   elevation,
		TimeZoneId:  tzid,
	}
}

// LookupCity returns a Location object of one of 60 "classic" Hebcal city names.
//
// If not found, returns nil.
//
// The following city names are supported:
//
// Ashdod, Atlanta, Austin, Baghdad, Beer Sheva,
// Berlin, Baltimore, Bogota, Boston, Budapest,
// Buenos Aires, Buffalo, Chicago, Cincinnati, Cleveland,
// Dallas, Denver, Detroit, Eilat, Gibraltar, Haifa,
// Hawaii, Helsinki, Houston, Jerusalem, Johannesburg,
// Kiev, La Paz, Livingston, Las Vegas, London, Los Angeles,
// Marseilles, Miami, Minneapolis, Melbourne, Mexico City,
// Montreal, Moscow, New York, Omaha, Ottawa, Panama City,
// Paris, Pawtucket, Petach Tikvah, Philadelphia, Phoenix,
// Pittsburgh, Providence, Portland, Saint Louis, Saint Petersburg,
// San Diego, San Francisco, Sao Paulo, Seattle, Sydney,
// Tel Aviv, Tiberias, Toronto, Vancouver, White Plains,
// Washington DC, Worcester
//
// City name lookup is case-insensitive.
func LookupCity(name string) *Location {
	str := strings.ToLower(name)
	for _, loc := range classicCities {
		candidate := strings.ToLower(loc.Name)
		if candidate == str {
			return &loc
		}
	}
	return nil
}

func AllCities() []Location {
	return classicCities
}
