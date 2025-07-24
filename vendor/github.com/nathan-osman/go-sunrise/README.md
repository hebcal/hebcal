## go-sunrise

[![Build Status](https://app.travis-ci.com/nathan-osman/go-sunrise.svg?branch=master)](https://app.travis-ci.com/nathan-osman/go-sunrise)
[![Coverage Status](https://coveralls.io/repos/github/nathan-osman/go-sunrise/badge.svg?branch=master)](https://coveralls.io/github/nathan-osman/go-sunrise?branch=master)
[![Go Report Card](https://goreportcard.com/badge/github.com/nathan-osman/go-sunrise)](https://goreportcard.com/report/github.com/nathan-osman/go-sunrise)
[![GoDoc](https://godoc.org/github.com/nathan-osman/go-sunrise?status.svg)](https://godoc.org/github.com/nathan-osman/go-sunrise)
[![MIT License](http://img.shields.io/badge/license-MIT-9370d8.svg?style=flat)](http://opensource.org/licenses/MIT)

Go package for calculating the sunrise and sunset times for a given location based on [this method](https://en.wikipedia.org/wiki/Sunrise_equation#Complete_calculation_on_Earth).

### Usage

To calculate sunrise and sunset times, you will need the following information:

- the date for which you wish to calculate the times
- the latitude and longitudinal coordinates of the location

Begin by importing the package:

    import "github.com/nathan-osman/go-sunrise"

Next, feed the information into the SunriseSunset() method:

    rise, set := sunrise.SunriseSunset(
        43.65, -79.38,          // Toronto, CA
        2000, time.January, 1,  // 2000-01-01
    )

The two return values will be the sunrise and sunset times for the location on the given day as time.Time values. If sun does not rise or set, both return values will be time.Time{}.
