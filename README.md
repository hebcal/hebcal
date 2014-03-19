hebcal [![Build Status](https://travis-ci.org/hebcal/hebcal.png?branch=master)](https://travis-ci.org/hebcal/hebcal)
======

A perpetual Jewish Calendar

by Danny Sadinoff  
portions by Michael J. Radwin

## Description
Hebcal is a program which prints out the days in the Jewish calendar
for a given gregorian year.  Hebcal is fairly flexible in terms of which
events in the Jewish calendar it displays.  Each of the following can
be individualy turned on or off:

* The Hebrew date
* Jewish Holdiays (including Yom Ha'atzmaut and Yom HaShoah etc.)
* The weekly Sedrah
* The day of the week
* The days of the Omer

## Synopsis

```
usage: hebcal [-8acdDeFHhiorsStTwy]
            [-b candle_lighting_minutes_before_sundown ]
            [-I file]
            [-Y yahrtzeit_file]
            [-C city]
            [-L longitude -l latitude]
            [-m havdalah_minutes_past_sundown ]
            [-z timezone]
            [-Z daylight_savings_option]
            [-f format_option]
            [[ month [ day ]] year ]
       hebcal help
       hebcal info
       hebcal DST
       hebcal cities
       hebcal warranty
       hebcal copying
```

### Options
Option | Description
--- | ---
-8 | Use 8-bit Hebrew (ISO-8859-8-Logical).
-a | Use ashkenazis hebrew.
-b mins | Set candle-lighting to occur this many minutes before sundown 
-c | Print candlelighting times.
-C city | Set latitude, longitude, timezone and daylight savings scheme according to specified city. This option implies the -c option.
   -d | print the hebrew date for the entire date range.
   -D | print the hebrew date for dates with some event.
   -e | Ouput "european" dates -- DD.MM.YYYY format.
   -h | Suppress default holidays.
   -H | Use Hebrew date ranges - only needed when e.g. `hebcal -H 5373`
   -i | Use Israeli sedra scheme.
   -f FORMAT | change output to `FORMAT`. see below for format strings
   -F | Output the Daf Yomi for the entire date range.
   -I file | Get non-yahrtzeit Hebrew user events from specified file. The format is: `mmm dd string`, Where `mmm` is a Hebrew month name.
   -l xx,yy | Set the latitude for solar calculations to `xx` degrees and `yy` minutes.  Negative values are south.
   -L xx,yy | Set the longitude for solar calculations to `xx` degrees and `yy` minutes.  *Negative values are EAST*. The `-l` and `-L` switches must both be used, or not at all. These switches override the `-C` (localize to city) switch.
   -m mins | Set havdalah to occur this many minutes after sundown 
   -M | Print the molad on shabbat mevorchim.
   -o | Add days of the omer.
   -O | Output sunrise and sunset times every day.
   -r | Tab delineated format.
   -s | Add weekly sedrot on saturday.
   -S | Print sedrah of the week on all calendar days.
   -t | Only output for today's date.
   -T | Print today's pertinent information, no gregorian date.
   -w | Add day of the week.
   -x | Suppress Rosh Chodesh.
   -y | Print only last two digits of year.
   -Y file | Get yahrtzeit dates from specified file. The format is: `mm dd yyyy string`. The first three fields specify a *Gregorian* date.
   -z | Use specified timezone, disabling daylight savings time, overriding the `-C` (localize to city) switch.
   -Z scheme | change to daylight savings scheme.  The possible values of scheme are currently `usa`, `israel`, `eu`, and `none`.

### Commands
Command | Description
--- | ---
hebcal help    | Print this message.
hebcal info    | Print version and localization data.
hebcal DST     | Print a list of available daylight savings schemes.
hebcal cities  | Print a list of available cities.
hebcal warranty | Tells you how there's NO WARRANTY for hebcal.
hebcal copying | Prints the details of the GNU copyright.

Hebcal prints out Hebrew calendars one solar year at a time.
Given one argument, it will print out the calendar for that year.
Given two numeric arguments `mm yyyy`, it prints out the calendar for 
month `mm` of year `yyyy`.

For example, `hebcal -ho`
will just print out the days of the omer for the current year.
Note: Use COMPLETE Years.  You probably aren't interested in 
hebcal 93, but rather hebcal 1993.


## DISTRIBUTION
   Copyright (C) 1994-2011  Danny Sadinoff  
   Portions Copyright (c) 2011 Michael J. Radwin. All Rights Reserved.

   Hebcal is distributed under the GNU Public License.  The program
   and its source code may be freely distributed.  For details, see
   the file COPYING in the distribution.
   
   If you are going to use this program, please drop me a line.
   I'd like to know who you are, what version you're using, and how
   you're using hebcal, and anything else you'd like to tell me, so
   that I can adjust the program to meet users' needs.

   I am NOT demanding payment for the use of my program, but writing
   this program DID take time.  The "free" in the GNU public license
   refers to distribution, not necessarily payment. Feel free to send
   $10 or multiples of $18 or just a postcard to me at my US Mail
   address (email me for it).

      send email to:
      danny@sadinoff.com

