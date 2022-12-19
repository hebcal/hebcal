hebcal [![Build Status](https://app.travis-ci.com/hebcal/hebcal.svg?branch=main)](https://app.travis-ci.com/hebcal/hebcal)
======

A perpetual Jewish Calendar

by Danny Sadinoff
(portions by Michael J. Radwin)

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
usage: hebcal [options] [[ month [ day ]] year ]
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
#### General options
Option | Description
--- | ---
  --help | Show help text
  --version | Show version number

#### Input Options
Option | Description
--- | ---
 -H, --hebrew-date | Use Hebrew date ranges - only needed when e.g. `hebcal -H 5373`
 -I, --infile INFILE | Get non-yahrtzeit Hebrew user events from specified file. The format is: `mmm dd string`, Where `mmm` is a Hebrew month name.
 -t, --today | Only output for today's date
 -T, --today-brief | Print today's pertinent information, no Gregorian date.
 -X, --exit-if-chag | Exit silently with non-zero status if today is Shabbat or Chag; exit with 0 status if today is chol.
 -Y, --yahrtzeit YAHRTZEIT | Get yahrtzeit dates from specified file. The format is: `mm dd yyyy string`. The first three fields specify a *Gregorian* date.

#### Output Options
Option | Description
--- | ---
   -8 | Use UTF-8 Hebrew (alias for --lang=he).
   -a, --ashkenazi | Use Ashkenazi Hebrew transliterations (alias for --lang=ashkenazi).
   --chag-only | Output only Chag and Erev Chag events (when melakha/labor is prohibited)
   -d, --add-hebrew-dates | print the Hebrew date for the entire date range.
   -D, --add-hebrew-dates-for-events | print the Hebrew date for dates with some events
   -e, --euro-dates | Output "European" dates -- DD.MM.YYYY format.
   -E, --24hour | Output 24-hour times (e.g. 18:37 instead of 6:37).
   -F, --daf-yomi | Output the Daf Yomi (Bavli) for the entire date range.
   -g, --iso-8601 | Output ISO 8601 dates -- YYYY-MM-DD (this overrides -y)
   -h, --no-holidays | Suppress default holidays.
   -i, --israeli | Use Israeli holiday and sedra schedule.
   --lang LANG | Use ISO 639-1 LANG code (one of `ashkenazi`, `ashkenazi_litvish`, `ashkenazi_poylish`, `ashkenazi_romanian`, `ashkenazi_standard`, `de`, `es`, `fi`, `fr`, `he`, `hu`, `pl`, `ro`, `ru`, `uk`)
   --mevarchim | Include Shabbat Mevarchim HaChodesh.
   --mishna-yomi | Output the Mishna Yomi for the entire date range.
   -M, --molad | Print the molad on shabbat mevorchim.
   --nach-yomi | Output the Nach Yomi for the entire date range.
   --no-mf | Suppress minor fast days.
   --no-modern | Suppress modern Israeli holidays.
   --no-special | Suppress Special Shabbatot.
   -o, --omer | Add days of the omer.
   -O, --sunrise-and-sunset | Output sunrise and sunset times every day.
   -r, --tabs | Tab delineated format.
   -s, --sedrot | Add weekly sedrot on Saturday.
   --schottenstein | Use Schottenstein edition of Yerushalmi Yomi
   -S, --daily-sedra | Print sedrah of the week on all calendar days.
   --verbose | Verbose mode, currently used only for --exit-if-chag
   -w, --weekday | Add day of the week.
   -W, --abbreviated | Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.
   -x, --no-rosh-chodesh | Suppress Rosh Chodesh.
   -y, --year-abbrev | Print only last two digits of year.
   --years N | Generate events for `N` years (default `1`)
   --yerushalmi | Output the Yerushalmi Yomi for the entire date range.
   --ykk | Include Yom Kippur Katan, minor day of atonement occurring monthly on the day preceding each Rosh Chodesh

#### Options related to candle-lighting times
Option | Description
--- | ---
   -b, --candle-mins mins | Set candle-lighting to occur this many minutes before sundown (default 18 [40 in Jerusalem])
   -c, --candlelighting | Print candlelighting times.
   -C, --city city | Set latitude, longitude, and timezone according to specified city. This option implies the -c option.
   --geo LATITUDE,LONGITUDE | Set location for solar calculations to decimal values LATITUDE and LONGITUDE. Negative longitudes are WEST of the Prime Meridian.
   -G, --havdalah-deg DEGREES | Set Havdalah to occur this many degrees below the horizon
   -l, --latitude XX,YY | Set the latitude for solar calculations to `XX` degrees and `YY` minutes. Negative values are south. **Deprecated**: use `--geo` instead.
   -L, --longitude XX,YY | Set the longitude for solar calculations to `XX` degrees and `YY` minutes. *Negative values are EAST*. The `-l` and `-L` switches must both be used, or not at all. These switches override the `-C` (localize to city) switch. **Deprecated**: use `--geo` instead.
   -m, --havdalah-mins MINS | Set havdalah to occur this many minutes after sundown
   -z, --timezone timezone | Use specified timezone, overriding the `-C` (localize to city) switch. For correct DST rules, use a full timezone name (such as `America/New_York`) instead of a timezone abbreviation (such as `EST`)
   -Z, --zmanim | Add zemanim (Alot HaShachar; Misheyakir; Kriat Shema, sof zeman; Tefilah, sof zeman; Chatzot hayom; Mincha Gedolah; Mincha Ketanah; Plag HaMincha; Tzait HaKochavim)

## Candle-lighting and fast start/end times

Hebcal’s candlelighting times are only approximations. If you ever have any doubts about its times, consult your local halachic authority. If you enter geographic coordinates above the arctic circle or antarctic circle, the times are guaranteed to be wrong.

In addition to candle-lighting on Erev Shabbat and Chag, the
`-c` switch also generates Havdalah and fast start/end times.

By default, candle-lighting times are 18 minutes before sunset
(see `-b` option above for exceptions).

Chanukah candle-lighting times are at civil dusk (solar depression
of 6 degrees) on weekdays. Chanukah candles are lit just before
Shabbat candles on Friday, and immediately after Havdalah on
Saturday night.

Havdalah time defaults to 72 minutes after sunset (Rabbeinu Tam).
Adjust with the `-m` (minutes) option or with `-G`
(havdalah degrees below horizon).

Minor fasts begin in the morning at *alot haShachar* (solar
depression 16.1 degrees) and conclude at *tzeit*
for 3 medium sized stars (solar depression 7.083 degrees).
Major fasts (*Yom Kippur* and *Tish'a B'Av*) begin just before sunset
(at regular candle-lighting time) and conclude at Havdalah time.

Hebcal contains a small database of cities with their associated geographic information and time-zone information. Run `hebcal cities` to print a list of cities supported by the `-C city` flag.

If your city is NOT on the list, then in order to customize hebcal to your city, you will need to pass it the latitude, longitude, and timezone (see the manual).

Suppose you live in Oshkosh, Wisconsin.
Your latitude is 44.0181, and your longitude is -88.6353.
You are in timezone `America/Chicago`.
We'll round the geographic coordinates to the nearest minute.

In order to get candlelighting times for the current year, you would type
  ```
  hebcal -ch --geo 44.0181,-88.6353 -z America/Chicago
  ```

The geographic and time information necessary to calculate sundown times can come to hebcal any of three ways:

1. The default: the system manager sets a default city ("New York") when the program is compiled.
2. Hebcal looks in the environment variable `HEBCAL_CITY` for the name of a city in hebcal’s database, and if it finds one, hebcal will make that the new default city.
3. 1 and 2 may be overridden by command line arguments, including
those specified in the `HEBCAL_OPTS` environment variable. The most
natural way to do this is to use the `−C city` command. This will
localize hebcal to city. A list of the cities hebcal knows about can
be obtained by typing `hebcal cities` at the command prompt. If the
city you want isn’t on that list, you can directly control hebcal’s
geographic information with the `--geo` (or `−l` and `−L`) and `−z`
switches.

For a status report on customizations, type `hebcal info` at the command prompt.

## Environment

Hebcal uses two environment variables:
<dl>
<dt>HEBCAL_CITY
<dd>Hebcal uses this value as the default city for sunset calculations. A list of available cities is available with from hebcal with the command: <code>hebcal cities</code>
<dt>HEBCAL_OPTS
<dd>The value of this variable is automatically processed as if it were typed at the command line before any other actual command-line arguments.
</dl>

### HEBCAL_OPTS
Every time hebcal is run, it checks this variable. If it is non-empty, the arguments in that variable are read as though they were typed at the command line before the ones you actually type.

So you might set `HEBCAL_OPTS` to be
   ```
   --geo 44.0181,-88.6353 -z America/Chicago
   ```
and if you type
    ```
    hebcal -ch
    ```
hebcal will think you typed
    ```
    hebcal --geo 44.0181,-88.6353 -z America/Chicago -ch
    ```

For information on setting environment variables, consult your local guru.

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

Version 5 was ported from C to Go.

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

When using the legacy `-L` flag, negative longitudes are EAST of Greenwich.

Some combinations of options produce weird results, e.g.
  `hebcal -dH nisan 5744`
  `hebcal -dH 5744`
This comes into play when you use the *HEBCAL_OPTS* environment variable.

The sunup/sundown routines aren’t accurate enough. If you enter geographic coordinates above the arctic circle or antarctic circle, the times are guaranteed to be wrong.

Hebcal only translates between the Gregorian calendar and the Jewish
calendar. It does not take into account a correction of eleven days
that was introduced by Pope Gregory XIII known as the Gregorian
Reformation. Wednesday, 2 September 1752 was followed by Thursday, 14
September 1752.

This means that the results will be at least partly useless where and
when the Gregorian calendar was not used, e.g. before the 1752 in
Britain and before circa 1918 in Russia. 
See ["Gregorian calendar" on Wikipedia](https://en.wikipedia.org/wiki/Gregorian_calendar#Adoption_by_country)
for a splendid chart depicting when the changeover from the Julian to
the Gregorian calendars occurred in various places.

## Build & Install

To build hebcal from the source repository, you'll need the following:

* Go version 1.13 or higher
* GNU make v3.79 or later

Once you have those, you can prepare the build environment as follows:

```
make clean all
```

## DISTRIBUTION
   Copyright (C) 1994-2011  Danny Sadinoff
   
   Portions Copyright (c) 2011-2022 Michael J. Radwin. All Rights Reserved.

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

