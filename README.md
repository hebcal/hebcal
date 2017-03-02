hebcal [![Build Status](https://travis-ci.org/hebcal/hebcal.png?branch=master)](https://travis-ci.org/hebcal/hebcal)
======

A perpetual Jewish Calendar

by Danny Sadinoff
portions by Michael J. Radwin

## Description
Hebcal is a program which prints out the days in the Jewish calendar
for a given Gregorian year.  Hebcal is fairly flexible in terms of which
events in the Jewish calendar it displays.  Each of the following can
be individualy turned on or off:

* The Hebrew date
* Jewish Holidays (including Yom Ha'atzmaut and Yom HaShoah etc.)
* The weekly Sedrah
* The day of the week
* The days of the Omer

## Synopsis

```
usage: hebcal [-8acdDeEFHhiorsStTwWyZ]
            [-b candle_lighting_minutes_before_sundown ]
            [-I file]
            [-Y yahrtzeit_file]
            [-C city]
            [-L longitude -l latitude]
            [-m havdalah_minutes_past_sundown ]
            [-z timezone]
            [[ month [ day ]] year ]
       hebcal help
       hebcal info
       hebcal cities
       hebcal warranty
       hebcal copying
```

Hebcal prints out Hebrew calendars one solar year at a time.
Given one argument, it will print out the calendar for that year.
Given two numeric arguments `mm yyyy`, it prints out the calendar for
month `mm` of year `yyyy`.

For example, `hebcal -ho`
will just print out the days of the omer for the current year.
Note: Use COMPLETE Years.  You probably aren't interested in
hebcal 93, but rather hebcal 1993.

### Options
Option | Description
--- | ---
-8 | Use 8-bit Hebrew (ISO-8859-8-Logical).
-a | Use Ashkenazi Hebrew.
-b mins | Set candle-lighting to occur this many minutes before sundown
-c | Print candlelighting times.
-C city | Set latitude, longitude, and timezone according to specified city. This option implies the -c option.
   -d | print the Hebrew date for the entire date range.
   -D | print the Hebrew date for dates with some events
   -e | Output "European" dates -- DD.MM.YYYY format.
   -E | Output 24-hour times (e.g. 18:37 instead of 6:37).
   -F | Output the Daf Yomi for the entire date range.
   -h | Suppress default holidays.
   -H | Use Hebrew date ranges - only needed when e.g. `hebcal -H 5373`
   -i | Use Israeli sedra scheme.
   -I file | Get non-yahrtzeit Hebrew user events from specified file. The format is: `mmm dd string`, Where `mmm` is a Hebrew month name.
   -l xx,yy | Set the latitude for solar calculations to `xx` degrees and `yy` minutes.  Negative values are south.
   -L xx,yy | Set the longitude for solar calculations to `xx` degrees and `yy` minutes.  *Negative values are EAST*. The `-l` and `-L` switches must both be used, or not at all. These switches override the `-C` (localize to city) switch.
   --lang LANG | Use ISO 639-1 LANG code (one of "he", "ru", "pl", or "fi")
   -m mins | Set havdalah to occur this many minutes after sundown
   -M | Print the molad on shabbat mevorchim.
   -o | Add days of the omer.
   -O | Output sunrise and sunset times every day.
   -r | Tab delineated format.
   -s | Add weekly sedrot on Saturday.
   -S | Print sedrah of the week on all calendar days.
   -t | Only output for today's date.
   -T | Print today's pertinent information, no Gregorian date.
   -w | Add day of the week.
   -W | Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.
   -x | Suppress Rosh Chodesh.
   -y | Print only last two digits of year.
   --years N | Generate events for `N` years (default `1`)
   -Y file | Get yahrtzeit dates from specified file. The format is: `mm dd yyyy string`. The first three fields specify a *Gregorian* date.
   -z timezone | Use specified timezone, overriding the `-C` (localize to city) switch.
   -Z | **EXPERIMENTAL** Add zemanim (Alot HaShachar; Misheyakir; Kriat Shema, sof zeman; Tefilah, sof zeman; Chatzot hayom; Mincha Gedolah; Mincha Ketanah; Plag HaMincha; Tzait HaKochavim)
   --help | Show help text
   --version | Show version number

## Candle-lighting times

Hebcal’s candlelighting times are only approximations. If you ever have any doubts about it’s times, consult your local halachic authority. If you enter geographic coordinates above the artic circle or antarctic circle, the times are guaranteed to be wrong.

Hebcal contains a small database of cities with their associated geographic information and time-zone information. The geographic and time information necessary to calculate sundown times can come to hebcal any of three ways:

1. The default: the system manager sets a default city when the program is compiled.
2. Hebcal looks in the environment variable `HEBCAL_CITY` for the name of a city in hebcal’s database, and if it finds one, hebcal will make that the new default city.
3. 1 and 2 may be overridden by command line arguments, including those specified in the `HEBCAL_OPTS` environment variable. The most natural way to do this is to use the `−c city` command. This will localize hebcal to city. A list of the cities hebcal knows about can be obtained by typing `hebcal cities` at the command prompt. If the city you want isn’t on that list, you can directly control hebcal’s geographic information with the `−l`, `−L`, and `−z` switches. Note that changing the geographic coordinates causes the timezone to default to 'UTC'.
For a status report on customizations, type `hebcal info` at the command prompt.

## Environment

Hebcal uses two environment variables:
<dl>
<dt>HEBCAL_CITY
<dd>Hebcal uses this value as the default city for sunset calculations. A list of available cities is available with from hebcal with the command: <code>hebcal cities</code>
<dt>HEBCAL_OPTS
<dd>The value of this variable is automatically processed as if it were typed at the command line before any other actual command-line arguments.
</dl>

## Author
Danny Sadinoff

With contributions from

* Michael J. Radwin
* Eyal Schachter (JavaScript port)
* Aaron Peromsik (Daf Yomi, experimental zmanim feature)
* Ben Sandler (Molad and daily Sunrise/Sunset features)

## See Also

calendar(1), emacs(1), hcal(1), hdate(1), omer(1), remind(1), rise(1)

The latest version of the code will be available from https://github.com/hebcal/hebcal

The original motivation for the algorithms in this program was the _Tur Shulchan Aruch_.

For version 3, much of the program was rewritten using Emacs 19’s calendar routines by Edward M. Reingold and Nachum Dershowitz. Their program is extremely clear and provides many instructive examples of fine calendar code in emacs-LISP.

For version 4, candle-lighting times were rewritting using Derick Rethans's [timelib](https://github.com/derickr/timelib).

A well written treatment of the Jewish calendar for the layman can be found in _Understanding the Jewish Calendar_ by Rabbi Nathan Bushwick. A more complete bibliography on the topic can be found there, as well as in the _Encyclopedia Judaica_ entry on the calendar.


## Diagnostics
<dl>
<dt>hebcal help
<dd>Prints a shorter version of this manpage, with comments on each option.
<dt>hebcal info
<dd>Prints the version number and default values of the program.
<dt>hebcal cities
<dd>Prints a list of cities which hebcal knows about, suitable as arguments to the −C city option.
<dt>hebcal copying
<dd>Prints the GNU license, with information about copying the program. See below.
<dt>hebcal warranty
<dd>Tells you how there’s NO WARRANTY for hebcal.
</dl>

## Disclaimer
This is just a program I wrote during summer school and while avoiding my senior project. It should not be invested with any sort of halachic authority.

## Bugs
Hebrew dates are only valid before sundown on that secular date. An option to control this will be added in a later release.

Negative longitudes are EAST of Greenwich.

Some combinations of options produce weird results, e.g.
  `hebcal -dH nisan 5744`
  `hebcal -dH 5744`
This comes into play when you use the *HEBCAL_OPTS* environment variable.

The sunup/sundown routines aren’t accurate enough. If you enter geographic coordinates above the artic circle or antarctic circle, the times are guaranteed to be wrong.

Hebcal only translates between the Gregorian calendar and the Jewish calendar. This means that the results will be at least partly useless where and when the Gregorian calendar was not used, e.g. before the 1752 in Britain and before circa 1918 in Russia. See ["Daylight saving time" on Wikipedia](https://en.wikipedia.org/wiki/Daylight_saving_time) for a splendid chart depicting when the changeover from the Julian to the Gregorian calendars occurred in various places.

Hebcal cannot handle date computations before 2 C.E. sorry.

## Build & Install

To build hebcal from the source repository, you'll need a few more tools than are needed in order to build from distributions. In particular, you'll need

* GNU autoconf version 2.59
* GNU automake version 1.9.5  or later
* GNU m4 version 1.4.3 or later
* GNU make v3.79 or later
* Perl v5.0 or later

Once you have those, you can prepare the build environment as follows:

```
aclocal && autoconf && automake --foreign --add-missing && ./configure && make
```

If you would like customize the program for your city, pass additional options to `configure` above.

Examine `cities.h`.  If your city is in there, run `configure` using the
  `--with-default-city=CITYNAME` option as follows:
   ```
   ./configure --with-default-city=Chicago
   ```
you may have to quote spaces:
   ```
   ./configure --with-default-city="Los Angeles"
   ```

If your city is NOT on the list, then in order to customize hebcal to your city, you will need to pass it the latitude, longitude, and timezone (see the manual).

Suppose you live in Oshkosh, Wisconsin.
Your lattitude is 44d1'29", and your longitude is 88d32'33".
You are in timezone `America/Chicago`.
We'll round the geographic coordinates to the nearest minute.

In order to get candlelighting times for the current year, you would type
  ```
  hebcal -ch -l44,1 -L 88,33 -z America/Chicago
  ```

Now this can get rough on the fingers if you do it a lot, so the `HEBCAL_OPTS` environment variable is available for you to use.  Every time hebcal is run, it checks this variable.  If it is non-empty, the arguments in that variable are read as though they were typed at the command line before the ones you actually type.

So you might set `HEBCAL_OPTS` to be
   ```
   -l44,1 -L 88,33 -z America/Chicago
   ```
and if you type
    ```
    hebcal -ch
    ```
hebcal will think you typed
    ```
    hebcal -l44,1 -L 88,33 -z America/Chicago -ch
    ```

REMEMBER: negative longitudes are EAST of Greenwich.

For information on setting environment variables, consult your local guru.

Once an install is complete, there are three ways to change cities, or pick a city not on the list:

1. change the `CITY` environment variable
2. change the `HEBCAL_OPTS` variable to reflect the new city's coordinates.
3. pass a `-C city` argument to hebcal.

You can check where hebcal thinks it is by typing
    `hebcal info`
at the command line.


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

