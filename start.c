/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2006  Danny Sadinoff
   Portions Copyright (c) 2010 Michael J. Radwin. All Rights Reserved.

   https://github.com/hebcal/hebcal

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Danny Sadinoff can be reached at
   danny@sadinoff.com
 */

/* the following need to be #defined: CITY ENV_CITY ENV_OPTS */

#include <stdio.h>
#include <ctype.h>
#include "config.h"
#include <string.h>
#include <math.h>
#include "hebcal.h"
#include "common.h"
#include <errno.h>
#include "cargo.h"
#include "danlib.h"
#include <stdlib.h>
#include "cities.h"

#define ENV_CITY "HEBCAL_CITY"
#define ENV_OPTS "HEBCAL_OPTS"

void handleArgs( int, char ** );
int main( int, char ** );
int ok_to_run = 1;

#define YEAR 0
#define MONTH 1
#define DAY 2
#define TODAY 3

char *progname;
static int theYear, theMonth, theDay, yearDirty, rangeType,
    latp, longp;			/* has the user inputted lat and long? */
static char
    *cityName, *helpArray[] =
{
   "hebcal [options] [[ month [ day ]] year ]",
   "",
   "hebcal help    -- Print this message.",
   "hebcal info    -- Print version and localization data.",
   "hebcal cities  -- Print a list of available cities.",
   "hebcal warranty -- Tells you how there's NO WARRANTY for hebcal.",
   "hebcal copying -- Prints the details of the GNU copyright.",
   "",
   "Hebcal prints out Hebrew calendars one solar year at a time.",
   "Given one argument, it will print out the calendar for that year.",
   "Given two numeric arguments mm yyyy, it prints out the calendar for ",
   "month mm of year yyyy.",
   "",
   "For example, ",
   "   hebcal -ho",
   "will just print out the days of the omer for the current year.",
   "Note: Use COMPLETE Years.  You probably aren't interested in ",
   "hebcal 93, but rather hebcal 1993.",
   "", "",
   "Hebcal is copyright (c) 1994-2011 By Danny Sadinoff",
   "Portions Copyright (c) 2011 Michael J. Radwin. All rights reserved.",
   "",
   "This program is free software; you can redistribute it and/or",
   "modify it under the terms of the GNU General Public License",
   "as published by the Free Software Foundation; either version 2",
   "of the License, or (at your option) any later version.",
   "Type \"hebcal copying\" for more details.",
   "",
   "Hebcal is distributed in the hope that it will be useful,",
   "but WITHOUT ANY WARRANTY; without even the implied warranty of",
   "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  ",
   "Type \"hebcal warranty\" for more details.",
   "  ",
   "\"Free\" above means freely distributed.  To donate money to support hebcal,", 
   " see the paypal link at http://www.sadinoff.com/hebcal/ "
   "",
   "WWW:",
   "            https://github.com/hebcal/hebcal"
};

void print_version_data(void)
{
    printf("hebcal version " VERSION "\n");
    printf("=====Defaults=====\n");
    printf("City: %s\n", cityName);
    printf("          %dd%d' %c latitude\n", abs(latdeg), abs(latmin),
           latdeg < 0 ? 'S' : 'N');
    printf("          %dd%d' %c longitude\n", abs(longdeg), abs(longmin),
           longdeg > 0 ? 'W' : 'E');
    printf("          Timezone: %s\n", TZ_INFO->name);
    printf("\nEnvironment variable for default city: %s\n", ENV_CITY);
    printf("\nEnvironment variable for default options: %s\n", ENV_OPTS);
}

void print_warranty( void )
{
    int cnum;

    for (cnum = 0;
         warranty[cnum];
         cnum++)
    {
        puts(warranty[cnum]);
    }
}

void print_copying( void )
{

    int cnum;

    for (cnum = 0;
         license[cnum];
         cnum++)
    {
        puts(license[cnum]);
    }

    for (cnum = 0;
         warranty[cnum];
         cnum++)
   {
       puts(warranty[cnum]);
   }
}

void print_city_data( void )
{
    int cnum;

    for (cnum = 0;
         cities[cnum].name;
         cnum++)
    {
        printf("%s (%dd%d' %c lat, %dd%d' %c long, %s)\n",
              cities[cnum].name,
               abs(cities[cnum].latdeg), abs(cities[cnum].latmin),
               cities[cnum].latdeg < 0 ? 'S' : 'N',
               abs(cities[cnum].longdeg), abs(cities[cnum].longmin),
               cities[cnum].longdeg > 0 ? 'W' : 'E',
               cities[cnum].tz
            );
    }
}

void localize_to_city(const char *cityNameArg)
{
    size_t len = strlen(cityNameArg);
    char *pc, *cityStr;
    city_t *pcity;

    initStr(&cityStr, strlen(cityNameArg));
    strcpy(cityStr, cityNameArg);

    if (cityName != NULL)
        free(cityName);

    /* convert non-alpha to spaces */
    for ( pc = cityStr; *pc != '\0'; pc++ )
        if ( ! isalpha( (int)*pc ) )
            *pc = ' ';

    for (pcity = &cities[0]; pcity->name != NULL; pcity++)
        if (0 == istrncasecmp(len, cityStr, pcity->name))
        {
            if (!(longp || latp))	/* -l and -L override -C  */
            {
                latdeg = pcity->latdeg;
                latmin = pcity->latmin;
                longdeg = pcity->longdeg;
                longmin = pcity->longmin;
            }
	    TZ_INFO = timelib_parse_tzfile(pcity->tz, timelib_builtin_db());
            free(cityStr);
            initStr(&cityName, strlen(pcity->name));
            strcpy(cityName, pcity->name);
            return;
      }

    warn("unknown city: %s. Use a nearby city or geographic coordinates.", cityNameArg);
    warn("run 'hebcal cities' for a list of cities.", "");
    ok_to_run = 0;
}

void set_default_city( void )
{
    const char *cityName;

#ifdef CITY
   localize_to_city(CITY);
#else
   localize_to_city("NEW YORK");	/* helluva town. */
#endif

   /* having set the default city, now check if there is an overriding */
   /* default from the environment. */
   if (NULL != (cityName = getenv(ENV_CITY)))
      localize_to_city(cityName);
}


static void displayHelp(cargo_t cargo) {
   size_t i, numLines = sizeof(helpArray) / sizeof(char *);
   cargo_print_usage(cargo, CARGO_USAGE_FULL);
   for (i = 0; i < numLines; i++) {
      puts(helpArray[i]);
   }
}

void handleArgs(int argc, char *argv[])
{
   date_t greg_today;
   cargo_t cargo;
   int ret = 0;
   const char **remain;
   size_t remain_count;
   char *cityNameArg = NULL;
   char *inFileName = NULL;
   char *tzid = NULL;
   char *yahrtzeitFileName = NULL;
   int today_sw = 0;
   int zemanim_sw = 0;
   int help_sw = 0;
   int version_sw = 0;
   char *latitudeStr = NULL;
   char *longitudeStr = NULL;

   setDate(&greg_today);        /* keep the current greg. date here */

   if (cargo_init(&cargo, CARGO_NO_AUTOHELP, "%s", argv[0])) {
     fprintf(stderr, "Failed to init command line parsing\n");
     return;
   }

   ret |= cargo_add_option(cargo, 0, "--ashkenazis -a",
                           "Use ashkenazis Hebrew",
                           "b", &ashkenazis_sw);

   ret |= cargo_add_group(cargo, 0, "candles",
                           "Candle lighting",
                           "Options related to candle-lighting times.");

   ret |= cargo_add_option(cargo, 0, "<candles> --candlelighting -c",
                           "Print candlelighting times",
                           "b", &candleLighting_sw);

   ret |= cargo_add_option(cargo, 0, "<candles> --city -C",
                           "City for candle-lighting",
                           "s", &cityNameArg);

   ret |= cargo_add_option(cargo, 0, "<candles> --candle-mins -b",
                           "Set candle-lighting to occur this many minutes before sundown",
                           "i", &light_offset);

   ret |= cargo_add_option(cargo, 0, "<candles> --havdalah-mins -m",
                           "Set Havdalah to occur this many minutes after sundown",
                           "i", &havdalah_minutes);

   ret |= cargo_add_option(cargo, 0, "<candles> --latitude -l",
                           "Set the latitude for solar calculations to xx degrees and yy minutes. Negative values are south",
                           "s", &latitudeStr);

   ret |= cargo_add_option(cargo, 0, "<candles> --longitude -L",
                           "Set the longitude for solar calculations to xx degrees and yy minutes. Negative values are EAST. The -l and -L switches must both be used, or not at all. These switches override the -C (localize to city) switch",
                           "s", &longitudeStr);

   ret |= cargo_add_option(cargo, 0, "--dates-all -d",
                           "Print the Hebrew date for the entire date range",
                           "b", &printHebDates_sw);

   ret |= cargo_add_option(cargo, 0, "--dates-some -D",
                           "Print the Hebrew date for dates with some event",
                           "b", &printSomeHebDates_sw);

   ret |= cargo_add_option(cargo, 0, "--infile -I",
                           "Get non-yahrtzeit Hebrew user events from specified file. The format is: mmm dd string, Where mmm is a Hebrew month name",
                           "s", &inFileName);

   ret |= cargo_add_option(cargo, 0, "--euro-dates -e",
                           "Ouput 'European' dates -- DD.MM.YYYY format",
                           "b", &euroDates_sw);

   ret |= cargo_add_option(cargo, 0, "--twenty-four-hour -E", "Ouput 24-hour times (e.g. 18:37 instead of 6:37)",
                           "b", &twentyFourHour_sw);

   ret |= cargo_add_option(cargo, 0, "--format -f",
                           "change output to FORMAT",
                           "s", &formatString);

   ret |= cargo_add_option(cargo, 0, "--dafyomi -F",
                           "Output the Daf Yomi for the entire date range",
                           "b", &dafYomi_sw);

   ret |= cargo_add_option(cargo, 0, "--no-holidays -h",
                           "Suppress default holidays",
                           "b", &noHolidays_sw);

   ret |= cargo_add_option(cargo, 0, "--hebrew-date -H",
                           "Use Hebrew date ranges - only needed when e.g. hebcal -H 5373",
                           "b", &hebrewDates_sw);

   ret |= cargo_add_option(cargo, 0, "--israeli -i",
                           "Israeli holiday and sedra schedule",
                           "b", &israel_sw);

   ret |= cargo_add_option(cargo, 0, "--molad -M",
                           "Print the molad on Shabbat Mevorchim",
                           "b", &printMolad_sw);

   ret |= cargo_add_option(cargo, 0, "--omer -o",
                           "Add days of the Omer",
                           "b", &printOmer_sw);

   ret |= cargo_add_option(cargo, 0, "--sunriseset -O",
                           "Output sunrise and sunset times every day",
                           "b", &printSunriseSunset_sw);

   ret |= cargo_add_option(cargo, 0, "--tabs -r",
                           "Tab delineated format",
                           "b", &tabs_sw);

   ret |= cargo_add_option(cargo, 0, "--sedrot -s",
                           "Add weekly sedrot on Saturday",
                           "b", &sedrot_sw);

   ret |= cargo_add_option(cargo, 0, "--daily-sedra -S",
                           "Print sedrah of the week on all calendar days",
                           "b", &sedraAllWeek_sw);

   ret |= cargo_add_option(cargo, 0, "--today -t", "Only output for today's date",
                           "b", &today_sw);

   ret |= cargo_add_option(cargo, 0, "--today-nogreg -T", "Print today's pertinent information, no gregorian date",
                           "b", &noGreg_sw);

   ret |= cargo_add_option(cargo, 0, "--weekday -w", "Add day of the week",
                           "b", &weekday_sw);

   ret |= cargo_add_option(cargo, 0, "--abbreviated -W", "Weekly view. Omer, dafyomi, and non-date-specific zemanim are shown once a week, on the day which corresponds to the first day in the range.",
                           "b", &abbrev_sw);

   ret |= cargo_add_option(cargo, 0, "--year-abbrev -y", "Print only last two digits of year",
                           "b", &yearDigits_sw);

   ret |= cargo_add_option(cargo, 0, "--yahrtzeit -Y",
                           "Get yahrtzeit dates from specified file."
                           " The format is: mm dd yyyy string"
                           " the first three fields specify a *Gregorian* date.",
                           "s", &yahrtzeitFileName);

   ret |= cargo_add_option(cargo, 0, "--no-rosh-chodesh -x", "Suppress Rosh Chodesh",
                           "b", &suppress_rosh_chodesh_sw);

   ret |= cargo_add_option(cargo, 0, "<candles> --timezone -z", "Use specified timezone, overriding the -C (localize to city) switch",
                           "s", &tzid);

   ret |= cargo_add_option(cargo, 0, "<candles> --zmanim -Z", "Print zemanim (experimental)",
                           "b", &zemanim_sw);

   ret |= cargo_add_option(cargo, 0, "--help", "Show this help",
                           "b", &help_sw);

   ret |= cargo_add_option(cargo, 0, "--version", "Show version number",
                           "b", &version_sw);

   if (cargo_parse(cargo, 0, 1, argc, argv)) {
      exit(1);
   }

   if (help_sw) {
      displayHelp(cargo);
      exit(0);
   }

   if (version_sw) {
      printf("Hebcal version %s\n", VERSION);
      exit(0);
   }

   errno = 0;

   if (light_offset > 0) {
      light_offset *= -1;
   }

   if (cityNameArg != NULL) {
      candleLighting_sw = 1;
      localize_to_city(cityNameArg);
   }

   if (inFileName != NULL) {
      inputFile_sw = 1;
      if (!(inFile = fopen(inFileName, "r"))) {
         die("could not open input file %s.", inFileName);
      }
   }

   if (noGreg_sw) {
      today_sw = 1;
   }

   if (today_sw) {
      printHebDates_sw = 1;
      rangeType = TODAY;
      theMonth = greg_today.mm;    /* year and month specified */
      theDay = greg_today.dd;      /* printc theDay of theMonth */
      yearDirty = 1;
      printOmer_sw = 1;
   }

   if (yahrtzeitFileName != NULL) {
      yahrtzeitFile_sw = 1;
      if (!(yFile = fopen(yahrtzeitFileName, "r"))) {
         die("could not open yahrtzeit input file %s.", yahrtzeitFileName);
      }
   }

   if (latitudeStr != NULL) {
      if (sscanf(latitudeStr, "%d,%d", &latdeg, &latmin) < 2) {
         die("unable to read latitude argument: %s", latitudeStr);
      }
      latp = 1;
      if ((abs(latdeg) > 90) || latmin > 60 || latmin < 0) {
         die("Error, latitude argument out of range: %s", latitudeStr);
      }
      latmin = abs(latmin);
      if (latdeg < 0) {
         latmin = -latmin;
      }
   }

   if (longitudeStr != NULL) {
      if (sscanf(longitudeStr, "%d,%d", &longdeg, &longmin) < 2) {
         die("unable to read longitude argument: %s", longitudeStr);
      }
      longp = 1;
      if ((abs(longdeg) > 180) || longmin > 60 || longmin < 0) {
         die("Error, longitude argument out of range: %s", longitudeStr);
      }
      longmin = abs(longmin);
      if (longdeg < 0) {
         longmin = -longmin;
      }
   }

   if (tzid != NULL) {
      TZ_INFO = timelib_parse_tzfile(tzid, timelib_builtin_db());
      if (TZ_INFO == NULL) {
         die("unable to read time zone argument: %s", tzid);
      }
   }

   if (zemanim_sw) {
      default_zemanim = (ZMAN_SUNRISE | ZMAN_SZKS | ZMAN_TEFILAH |
                           ZMAN_CHATZOT |
                           ZMAN_MINCHA_GEDOLA | ZMAN_MINCHA_KETANA |
                           ZMAN_PLAG_HAMINCHA | ZMAN_SUNSET | ZMAN_TZAIT_42);
   }

   if (latp)
       cityName = "User Defined City";
   if (latp ^ longp)
       die("You must enter BOTH the latitude and the longitude", "");
   
   if( !strcmp(cityName, "Jerusalem" ))
       light_offset = -40;	/* does everyone hold by this? */


   // Print remaining options.
   remain = cargo_get_args(cargo, &remain_count);

   switch (remain_count)	/* suck up the date */
   {
   case 0:			/* process this year */
       if (hebrewDates_sw)
	   theYear = abs2hebrew(greg2abs(greg_today)).yy;
       else
	   theYear = greg_today.yy;
       break;
       
   case 1:
       if (isAllNums(remain[0]))
       {
	   theYear = atoi(remain[0]);	/* just year specified */
	   yearDirty = 1;		/* print whole year */
       }
       else if (0 == istrncasecmp(5, remain[0], "help"))
       {
           displayHelp(cargo);
           exit(0);
       }
       else if (0 == istrncasecmp(3, remain[0], "info"))
       {
	   print_version_data();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, remain[0], "cities"))
       {
	   print_city_data();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, remain[0], "copying"))
       {
	   print_copying();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, remain[0], "warranty"))
       {
	   print_warranty();
	   exit(0);
       }
       else
       {
           cargo_print_usage(cargo, CARGO_USAGE_SHORT);
           exit(1);
       }
       break;
       
   case 2:
       if (!isAllNums(remain[1])) {
           cargo_print_usage(cargo, CARGO_USAGE_SHORT);
           exit(1);
       }
       theYear = atoi(remain[1]);		/* print theMonth of theYear */
       
       theMonth = lookup_hebrew_month(remain[0]);
       
       if (theMonth) {
	   hebrewDates_sw = 1;	/* automagically turn it on */
	   if (theMonth == ADAR_II && !LEAP_YR_HEB(theYear)) {
               theMonth = ADAR_I;	/* silently fix this mistake */
           }
       } else if (isAllNums(remain[0])) {
	   if (hebrewDates_sw) {
               die("Don't use numbers to specify Hebrew months.", "");
	   } else {
	      theMonth = atoi(remain[0]);	/* gregorian month */
           }
       } else if (hebrewDates_sw) {
	   die("Unknown Hebrew month: %s", remain[0]);
       } else {
            die("bad gregorian month: %s", remain[0]);
       }
       
       yearDirty = 1;
       rangeType = MONTH;
	break;
        
   case 3:
       if (!(isAllNums(remain[1]) && isAllNums(remain[2]))) {
           cargo_print_usage(cargo, CARGO_USAGE_SHORT);
           exit(1);
       }
       theDay = atoi(remain[1]);	/* print theDay of theMonth */
       theYear = atoi(remain[2]);		/* print theMonth of theYear */
       
       theMonth = lookup_hebrew_month(remain[0]);
       
       if (theMonth) {
	   hebrewDates_sw = 1;	/* automagically turn it on */
	   if (theMonth == ADAR_II && !LEAP_YR_HEB(theYear)) {
               theMonth = ADAR_I;	/* silently fix this mistake */
           }
       } else if (isAllNums(remain[0])) {
	   if (hebrewDates_sw) {
               die("Don't use numbers to specify Hebrew months.", "");
	   } else {
               theMonth = atoi(remain[0]);	/* gregorian month */
           }
	} else if (hebrewDates_sw) {
            die("Unknown Hebrew month: %s", remain[0]);
        } else {
            die("bad gregorian month: %s", remain[0]);
        }

       if (theDay < 1)
	   die("The day of the month must be greater than 0", "");
       if (theMonth < 1)
	   die("The month must be greater than 0", "");
       
       if (hebrewDates_sw)
       {
	   if (theDay > max_days_in_heb_month(theMonth, theYear))
               die("Sorry, there aren't that many days in %s (then)",
                   LANGUAGE2(hMonths[LEAP_YR_HEB(theYear)][theMonth].name));
       }
       else
       {
	   if (theMonth > 12)
               die("The month must be less than 13", "");
	   if (theDay > MonthLengths[LEAP(theYear)][theMonth])
               die("Sorry, there aren't that many days in %s (then)",
                   eMonths[theMonth]);
	}
       
       
       rangeType = DAY;
       yearDirty = 1;
       break;
   default:
        displayHelp(cargo);
        exit(1);
   }

   cargo_destroy(&cargo);
}


int tokenize(char *str, int *pargc, char* argv[])
{
/*     char *strtok(), *strdup(); */
    char *s;
    
    argv[0] = progname;
    s = strtok(str, "\t ");
    *pargc = 1;
    do
    {
        argv[(*pargc)++] = strdup(s);
    } while ((s = strtok(NULL, "\t ")));
    
    
    return *pargc > 1;
}

int main(int argc, char* argv[])
{
    date_t tempDate;
    long startAbs, endAbs;
    char *envStr;
    int envArgc;
    char *envArgv[40];		/* this should be big enough */

    progname = argv[0];

    set_default_city();
    if ((envStr = getenv(ENV_OPTS)) && strcmp(envStr, ""))
    {
        int i;

        tokenize(envStr, &envArgc, envArgv);
        for (i = 1; i < argc; i++)	/* append argv onto envArgv  */
            envArgv[envArgc++] = argv[i];
        handleArgs(envArgc, envArgv);
    }
    else
        handleArgs(argc, argv);

    tempDate.yy = theYear;
    if (theYear < (hebrewDates_sw ? 3761 : 1))
        die("Sorry, hebcal can only handle dates in the common era.", "");

    switch (rangeType)
    {
    case TODAY:
        printHebDates_sw = 1;
        tempDate.dd = theDay;
        tempDate.mm = theMonth;
        tempDate.yy = theYear;
        startAbs = endAbs = greg2abs(tempDate);
        break;
    case DAY:
        printHebDates_sw = 1;
        tempDate.dd = theDay;
        tempDate.mm = theMonth;
        tempDate.yy = theYear;
        if (hebrewDates_sw)
            startAbs = endAbs = hebrew2abs(tempDate);
        else
            startAbs = endAbs = greg2abs(tempDate);
        break;
    case MONTH:
        tempDate.dd = 1;
        tempDate.mm = theMonth;
        tempDate.yy = theYear;
        if (hebrewDates_sw)
        {
            startAbs = hebrew2abs(tempDate);
            tempDate.dd = max_days_in_heb_month(tempDate.mm, tempDate.yy);
            endAbs = hebrew2abs(tempDate);
        }
        else
        {
            startAbs = greg2abs(tempDate);
            tempDate.dd = MonthLengths[LEAP(theYear)][theMonth];
            endAbs = greg2abs(tempDate);
        }
        break;

    case YEAR:
        if (hebrewDates_sw)
        {
            tempDate.dd = 1;
            tempDate.mm = TISHREI;
            tempDate.yy = theYear;
            startAbs = hebrew2abs(tempDate);
	    /* start yearly calendar with the day before RH (i.e. Erev
	     * Rosh Hashanah)
	     */
	    startAbs--;

            tempDate.yy++;
            endAbs = hebrew2abs(tempDate) - 1;
        }
        else
        {
            tempDate.dd = 1;
            tempDate.mm = JAN;
            tempDate.yy = theYear;
            startAbs = greg2abs(tempDate);

            tempDate.yy++;
            endAbs = greg2abs(tempDate) - 1;
        }
        break;

    default:
        die("Oh, NO! internal error #17q!", "");
        /* this is dead code, but it silences some uninitialized variable 
           warnings in gcc   */
        startAbs = endAbs =0;
    }

    tempDate = abs2hebrew(startAbs);
    if (ok_to_run)
    {
        init_holidays(tempDate.yy);	/* load the holiday array */
        main_calendar(startAbs, endAbs);

        return 0;			/* success!  Kol hakavod to thorough programmers */
    }
    else
        return 1;
}
