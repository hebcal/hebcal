Changes in 5.0.3
* If `-l` (latitude) is specified, require `-L` (longitude), and vice-versa.
* Clarify in CLI help text that negative values are EAST for longitudes
  specified on the command-line. This is the opposite of most modern
  systems that use lat/long coordinates but is preserved for
  backwards compatibility.

Changes in 5.0.2
* CLI support for legacy `HEBCAL_OPTS` environment variable
* Re-added many cities for candle-lighting times that were recently added
  to Hebcal but missed during Golang port (#220, #233)

Changes in 5.0.1
* Hebcal command-line interface has been rewritten in the Go Programming Language
* Default holidays now include 5 additional modern Israeli holidays when `-i` flag is enabled (can be suppressed with `--no-modern`)
  * Family Day
  * Yitzhak Rabin Memorial Day
  * Jabotinsky Day
  * Herzl Day
  * Ben-Gurion Day

Changes in 4.31
* Updated timezonedb to version 2022.3 (2022c)

Changes in 4.30
* Improve some Ashkenazi transliterations (e.g. Teves), courtesy Chaim Halbert (@chaimleib), (#234)

Changes in 4.29
* fix for Daf Yomi: Midot 34 (#232)

Changes in 4.28
* Add Romanian translations (`--lang=ro` and `--lang=ashkenazi_romanian`), courtesy Florin-Ciprian Bodin (@orynider) (#229, #230)
* Updated timezonedb to version 2022.1 (2022a)

Changes in 4.27
* Updated timezonedb to version 2021.5 (2021e)

Changes in 4.26
* Added Rosh Hashana LaBehemot (#202)

Changes in 4.25
* Updated timezonedb to version 2021.1 (2021a)
* Add German translations (`--lang=de`), courtesy Oliver Maor (#217, #218)
* Add 10 German/Austrian cities: Düsseldorf, Frankfurt, Munich, Vienna, ... (#220)
* Daf Yomi: display correct daf for dates prior to 6/24/1975 (#214)

Changes in 4.24
* Display Hebrew Date in Hebrew numerals for `--lang=he` or `-8` (#206)
  (e.g. displays `1/3/2021 י״ט טֵבֵת תשפ״א` instead of `1/3/2021 19th of Tevet, 5781`)
* Display "Tish'a B'Av (observed)" when the fast of Av is postponed to
  the 10th due to Shabbat

Changes in 4.23
* Add Spanish translations (`--lang=es`), courtesy Iosef Kebesh and
  Sebastian Thierer. (#189, #205)
* Update timezonedb to version 2020.3 (2020d)

Changes in 4.22
* Update timezonedb to version 2020.2 (2020b)

Changes in 4.21
* This relase improves accuracy of candle-lighting and havdalah times.
  - Havdalah times are now rounded up to the nearest minute if the number
    of seconds within the minute is >= 30. The idea here is that it's better
    for the user to wait an additional 1-30 seconds to end Shabbat/yontiff
    than to display a time that could be too early.
    For example, previously Hebcal would display 21:17 for Havdalah even if
    the internal calculation returned 21:17:59. Now Hebcal displays 21:18
    in this case.
  - Merged changes from `timelib`'s `timelib_astro_rise_set_altitude()`.
    This calculates sunset on the correct day, which improves accuaracy
    but shifts candle-lighting and Havdalah times by a minute here and there.

Changes in 4.20
* Update timezone database to version 2020.1
* Fix bug in `--hebrew-date --today` CLI args (#191)

Changes in 4.19
* Fix crash in candle-lighting times on big endian architectures (#170)
* Update timezone database to version 2019.2

Changes in 4.18
* Update timelib to 2018.01 (with small patch to keep candle-lighting consistent)

Changes in 4.17
* Hungarian localisation #174
* Litvish Rosh => Reish #169
* Update timezone database to version 2019.1 (2019a)

Changes in 4.16
* Scratched an itch:  added iso-8601 dates for output
* Update timezone database to version 2018.9 (2018i)

Changes in 4.15
* French translation courtesy @mneiger and @yarons (#158 and #161)

Changes in 4.14
* Update timezone database to 2018.5 (2018e)
* FIXED missing range check on MonthLengths array, courtesy ynonp  (#157)

Changes in 4.13
* Correct candle-lighting times for for fractional timezones such as
  Asia/Kolkata, America/St_Johns, Asia/Kathmandu (#136)
* Update timezone database to 2017.2 (2017b)
* Updated Finnish translations
* Added Helsinki to built-in city list

Changes in 4.12
* Support for `--lang fi`, courtesy Petteri Hjort
* Update timezone database to 2017.1 (2017a)

Changes in 4.11
* Correct yahrzeit calculation when date of death is 30 Kislev (#134)
* Update timezone database to 2016.10 (2016j)

Changes in 4.10
* Add Yom HaAliyah, observed on 7 Cheshvan (#132)
* Add `--no-modern` option to suppress modern holidays (#133)

Changes in 4.9
* Update timezone database to 2016.9 (2016i)

Changes in 4.8
* Update timezone database to 2016.6 (2016f)
* Add Ashkenazi transliteration for Leil Selichos

Changes in 4.7
* Update timezone database to 2016.5 (2016e)
* Fixed long option `--tabs` (#125)

Changes in 4.6
* Update timezone database to 2016.3 (2016c)
* Add candle lighting for second night of Rosh Hashana (#119)
* Compute Mincha Gedolah in the winter using 30 minutes
  instead of half a solar hour (#119)

Changes in 4.5
* Update timezone database to 2016.2 (2016b)

Changes in 4.4
* Remove dependency on GNU gettext and use static string tables (#115)
* Document `--lang LANG` option for ISO 639-1 LANG code (one of `he`, `ru`, or `pl`)
* Change from Tu B'Shvat to Tu BiShvat

Changes in 4.3
* Removed references to non-functional `-f,--format FORMATSTR` option (#123)
* Change from Lag B'Omer to Lag BaOmer
* Update timezone database to 2016.1 (2016a)
* Add `--years N` option to generate longer ranges of events
* Add Russian and Polish transliterations for event titles.

Changes in 4.2
* Move transliterations of event titles into GNU gettext. (#115)

Changes in 4.1
* Zemanim with new `-Z` feature, courtesy @aperomsik (#114)
* Weekly view with new `-W` switch. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.
* Chanukah candle-lighting times when `-c` option enabled

Changes in 4.0.2
* automake/configure script changes to disable "maintainer mode"

Changes in 4.0.1
* Use 24-hour times when both `-E` and `-O` options present

Changes in 4.0
* Use Olson zoneinfo database for time zones (#112).
  The `-z` option now takes a timezone ID such as `America/Chicago`
  and the `-Z` option (formerly Daylight Saving Time rule) has been removed.
* Add -E option for 24-hour times (e.g. 18:37 instead of 6:37pm)

Changes in 3.18
* Add Tu B'Av, minor holiday, on the 15th of Av (#98)
* Add Sigd, modern holiday, on 29th of Cheshvan, beginning in 2008 (#96)

Changes in 3.17
* Build/SCM: include generated configure script in repo to simplify
  user build and remove dependency on autoconf, automake, aclocal.

Changes in 3.16
* Not released

Changes in 3.15
* Daf Yomi via new -F option, courtesy of Aaron Peromsik
  https://github.com/hebcal/hebcal/pull/82
* Build/SCM: moved JavaScript, Perl, and Java implementations
  out of hebcal/hebcal into separate repositories

Changes in 3.14
* Properly display Parashat Vayeilech or Ha'Azinu when using -S option
  (Print sedrah of the week on all calendar days) in certain
  cases. https://github.com/hebcal/hebcal/issues/79
* Added "Leil Selichot" on Saturday night at least 4 days before Rosh
  Hashana. https://github.com/hebcal/hebcal/issues/47
* Make sunset candlelighting offset adjustable with new option
  -b candle_lighting_minutes_before_sundown.
  https://github.com/hebcal/hebcal/issues/52

Changes in 3.13
* Moved from SourceForge to GitHub
* Added a -M option which will print out the molad time on Shabbat
  Mevorchim, courtesy of bensandler (2004-12-21)
  https://github.com/hebcal/hebcal/issues/73
* Added -O option to show daily sunrise and sunset, courtesy of
  bensandler (2007-04-01)
  https://github.com/hebcal/hebcal/issues/76

Changes in 3.12
* When the actual date of Yom Hashoah falls on a Friday, the state of
  Israel observes Yom Hashoah on the preceding Thursday. When it falls
  on a Sunday, Yom Hashoah is observed on the following Monday.

Changes in 3.11
* If a yahrzeit would fall on 30 Cheshvan in a short year, advance
  observance to 1 Kislev. If a yahrzeit would fall on Kislev 30 in a
  short year, advance observance to 1 Tevet.
* Allow Ta'anit Bechorot (Fast of the Firstborn) to fall on a
  Friday. When a fast day falls on Friday, we do in fact observe the
  fast on Friday, even though Shabbat follows immediately. Also, in some
  years, Asarah Betevet (Tenth of Tevet) falls on Friday and is observed
  on that day.
  It is only in years when the date of the fast falls on Shabbat that
  the fast is moved: Ta'anit Ester (Fast of Esther) and Ta'anit Bekhorim
  are moved back to Thursday. Tsom Gedalyah (Fast of Gedaliah) and
  Shiv'ah Asar Betammuz (Seventeeth of Tammuz) are moved ahead to
  Sunday. Tish'ah Be'av (Ninth of Av), a full-day fast, is moved ahead
  to Saturday night. Asarah Betevet does not fall on Shabbat.

Changes in 3.10
* Add Erev Purim and Erev Tish'a B'Av
* Add Pesach Sheni (14 Iyyar)

Changes in 3.9.1
   [revved version number correctly]


Changes in 3.9
* Yom Hazikaron first observed in 1949. Yom HaShoah first observed in 1951.
* Allow Asara B'Tevet to fall on a Friday.


Changes in 3.8
* fixed spelling of shabbat Chazon
* fixed yom Haatzmaut 2007
* added support for Mexico DST scheme (no longer the same as US & Canada)
* Correct Gibraltar time zone to UTC+1, not UTC-10. Thanks to
  Jack Gross <jacobbgross@hotmail.com> for reporting the bug.

Changes in 3.7
* addressed 2004 adjustment to Yom HaZikaron Rules.
http://sourceforge.net/tracker/index.php?func=detail&aid=1553196&group_id=340&atid=100340

Changes in 3.6
* Added Panama City, per Elan Z.
  https://sourceforge.net/tracker/?func=detail&atid=350340&aid=1440788&group_id=340

* Added support for building OSX-binaries disk images

changes in 3.5
*  Added 2007 DST rule, assuming that PL 109-58 will go into effect:
   http://sourceforge.net/tracker/index.php?func=detail&aid=1427033&group_id=340&atid=100340

 In 2005 the 109th Congress enacted P.L. 109-58, the Energy Policy Act
 of 2005.  Section 110 of this Act amends the Uniform Time Act,
 changing the beginning of DST to the second Sunday in March and the
 ending of DST to the first Sunday in November.  This change will take
 effect in March of 2007. The Act requires the Secretary of Commerce to
 report to Congress within nine months of this date on the impact of
 this section on energy consumption in the United States. Congress
 retains the right to revert the Daylight Saving Time back to the 2005
 time schedules once the Department study is complete.
  - CRS Report for Congress
    Mark Gurevitz
    http://opencrs.cdt.org/rpts/RS22284_20050927.pdf

* Clarify in which year the Gregorian calendar starts.
  http://sourceforge.net/tracker/index.php?func=detail&aid=1361957&group_id=340&atid=100340

* Fixed some ashkenazi-spelling errors.
  http://sourceforge.net/tracker/index.php?func=detail&aid=1151200&group_id=340&atid=100340

* Simplified city customization.
  http://sourceforge.net/tracker/index.php?func=detail&aid=991704&group_id=340&atid=100340
  You can now pass a
      --with-default-city=Chicago
  argument to configure, and it will do the right thing.

* Fixed some spelling issues
http://sourceforge.net/tracker/index.php?func=detail&aid=1151200&group_id=340&atid=100340

KNOWN ISSUES in 3.5:
* Triple-Purim not properly represented
  http://sourceforge.net/tracker/index.php?func=detail&aid=1165732&group_id=340&atid=100340
  In years where 15 Adar falls on Saturday, the observances of Shushan purim are
  distributed over Friday, Saturday and Sunday.  This is not represented, and Shushan
  Purim is indicated as falling on Sunday.  The megilah is read on Friday
  in Jerusalem.

* Incorrect weekly sedra displayed near Rosh Hashana
  http://sourceforge.net/tracker/index.php?func=detail&aid=1323641&group_id=340&atid=100340

Changes in 3.4.1
*  Fixed  the lack of manpage in the dist.

Changes in 3.4
* Support for Australia DST scheme
* Add 4 cities: Melbourne, Sydney, Ottawa, Baghdad
* Added manpage to the build

Changes in 3.4-beta3
- removed reference to NAN in the code.  It's a silly idea.
- taught autoconf how to find libm.
- various other autoconf/automake tricks, like "make check" from another directory
- figured out how to distribute gpl.texi
- Broke "no release after 1am rule"

Changes in 3.4-beta2
- fixed massive distribution error, which left the source missing three .h files.
- Resolved never to roll a release after 1am.
- factored out cities into cities.h
- implemented ouline of a test suite

Changes in 3.4-beta1
- Fixed bug #s 797972,645240 "Chanukah 7 candles repeated twice",
    per suggestion & Ambrose C. Li's and Norman Abramovitz's
    suggestions

- Fixed bug 895450: certain out of range years were causing segfaults.

- Preliminary implementation of the -f FORMAT option

CHANGES since version 3.2
   - New option: -8 for hebrew output in iso-8859-8-logical charset
   - Canada and Mexico observe USA Daylight Saving Time rule.
   - Support the 1996 European Union DST scheme for EU cities.
   - Added cities: Ashdod, Be'er Sheva, Eilat, Haifa, Petach Tikvah,
     Tel Aviv, Tiberias
   - Added cities: Kiev, La Paz, Montreal, Moscow, Paris, Saint Petersburg
   - "hebcal cities" prints out lat/long and time zone offset from GMT.
   - Corrected time zone for Johannesburg
   - Corrected DST rule for Phoenix and Hawaii
   - Corrected problem with Yahrzeit/Birthdays observed in Adar II

CHANGES since version 3.0b
   - Shabbat starts 40 minutes before sundown in Jerusalem.
   - Added a -m option to set when havdalah takes place.  It defaults to
     72 minutes after sundown.
   - The environment variables that hebcal reads are no longer
     configurable at compilation time.  That was a needless piece of
     complexity.

CHANGES since version 2.1
   The changes to hebcal since version 2.1 are too numerous to mention
   here;  see the file CHANGES in the distribution.  A brief overview
   follows:

   - The biggest change is the addition of candlelighting times.
   - All of the algorithms have been speeded up.
   - Numerous errors, especially Yahrtzeit handling, have been
     corrected.
   - Hebcal can now print Hebrew date ranges, something I've wanted
     since version 1.  You can now query hebcal with
       hebcal Nisan 5754
       to get the holidays of Nisan 5754.
