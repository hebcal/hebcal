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
#include <getopt.h>
#include "danlib.h"
#include <stdlib.h>
#include "cities.h"
#include "translations.h"

#define ENV_CITY "HEBCAL_CITY"
#define ENV_OPTS "HEBCAL_OPTS"

static int numYears = 1;

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

static char* shortUsageArray[] = {
"Usage: hebcal [--ashkenazi] [--candle-mins CANDLE-MINS] [--candlelighting]",
"              [--city CITY] [--latitude LATITUDE] [--longitude LONGITUDE]",
"              [--havdalah-mins HAVDALAH-MINS] [--add-hebrew-dates]",
"              [--add-hebrew-dates-for-events] [--euro-dates] [--24hour]",
"              [--daf-yomi] [--no-holidays] [--lang LANG]",
"              [--hebrew-date] [--help] [--israeli] [--infile INFILE]",
"              [--molad] [--omer] [--sunrise-and-sunset] [--tabs]",
"              [--sedrot] [--daily-sedra] [--today] [--today-brief]",
"              [--version] [--weekday] [--abbreviated] [--no-rosh-chodesh]",
"              [--year-abbrev] [--yahrtzeit YAHRTZEIT]",
"              [--timezone TIMEZONE] [--years N] [--zmanim]",
""
};

static char* optionsHelpArray[] = {
"Options:",
"    --help                             Show this help",
"  Input  ",
"    -H, --hebrew-date                  Use Hebrew date ranges - only needed",
"                                       when e.g. hebcal -H 5373",
"    -I, --infile INFILE                Get non-yahrtzeit Hebrew user events",
"                                       from specified file. The format is:",
"                                       mmm dd string, Where mmm is a Hebrew",
"                                       month name",
"    -t, --today                        Only output for today's date",
"    -T, --today-brief                  Print today's pertinent information,",
"    -Y, --yahrtzeit YAHRTZEIT          Get yahrtzeit dates from specified",
"                                       file. The format is: mm dd yyyy",
"                                       string the first three fields",
"                                       specify a *Gregorian* date.",
"                                       no gregorian date",
"  Output  ",
"    -a, --ashkenazi                    Use Ashkenazi Hebrew",
"    -d, --add-hebrew-dates             Print the Hebrew date for the entire",
"                                        date range",
"    -D, --add-hebrew-dates-for-events  Print the Hebrew date for dates with",
"                                        some event",
"    -e, --euro-dates                   Output 'European' dates -- DD.MM.YYYY",
"    -E, --24hour                       Output 24-hour times (e.g. 18:37",
"                                        instead of 6:37)",
"    -F, --daf-yomi                     Output the Daf Yomi for the entire",
"                                        date range",
"    -g, --iso-8601                     Ouput ISO 8601 dates -- YYYY-MM-DD",
"                                            this overrides -y", 
"    -h, --no-holidays                  Suppress default holidays",
"    -i, --israeli                      Israeli holiday and sedra schedule",
"    --lang LANG                        Use LANG titles (he, ru, or pl)",
"    -M, --molad                        Print the molad on Shabbat Mevorchim",
"    --no-modern                        Suppress modern holidays",
"    -o, --omer                         Add days of the Omer",
"    -O, --sunrise-and-sunset           Output sunrise and sunset times every",
"                                        day",
"    -r, --tabs                         Tab delineated format",
"    -s, --sedrot                       Add weekly sedrot on Saturday",
"    -S, --daily-sedra                  Print sedrah of the week on all",
"                                        calendar days",
"    --version                          Show version number",
"    -w, --weekday                      Add day of the week",
"    -W, --abbreviated                  Weekly view. Omer, dafyomi, and",
"                                        non-date-specific zemanim are shown",
"                                        once a week, on the day which",
"                                        corresponds to the first day in the",
"                                        range.",
"    -x, --no-rosh-chodesh              Suppress Rosh Chodesh",
"    -y, --year-abbrev                  Print only last two digits of year",
"    --years N                          Generate events for N years (default 1)",
"",
"Candle lighting:",
"  Options related to candle-lighting times.",
"",
"    -b, --candle-mins CANDLE-MINS      Set candle-lighting to occur this",
"                                       many minutes before sundown",
"    -c, --candlelighting               Print candlelighting times",
"    -C, --city CITY                    City for candle-lighting",
"    -l, --latitude LATITUDE            Set the latitude for solar",
"                                       calculations to xx degrees and yy",
"                                       minutes. Negative values are south",
"    -L, --longitude LONGITUDE          Set the longitude for solar",
"                                       calculations to xx degrees and yy",
"                                       minutes. Negative values are EAST.",
"                                       The -l and -L switches must both be",
"                                       used, or not at all. These switches",
"                                       override the -C (localize to city)",
"                                       switch",
"    -m, --havdalah-mins HAVDALAH-MINS  Set Havdalah to occur this many",
"                                       minutes after sundown",
"    -z, --timezone TIMEZONE            Use specified timezone, overriding",
"                                       the -C (localize to city) switch",
"    -Z, --zmanim                       Print zemanim (experimental)",
""
};

void print_version_data(void)
{
    const timelib_tzdb *db = timelib_builtin_db();
    printf("hebcal version " VERSION "\n");
    printf("timezone_db version %s\n", db->version);
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
            if (TZ_INFO == NULL) {
                die("unable to read time zone argument: %s", pcity->tz);
            }
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

static void displayHelp() {
   size_t i;
   printf("Usage: %s\n", helpArray[0]);
   for (i = 0; i < sizeof(optionsHelpArray) / sizeof(char *); i++) {
      puts(optionsHelpArray[i]);
   }
   for (i = 1; i < sizeof(helpArray) / sizeof(char *); i++) {
      puts(helpArray[i]);
   }
}

static void shortUsage() {
   size_t i;
   for (i = 0; i < sizeof(shortUsageArray) / sizeof(char *); i++) {
      puts(shortUsageArray[i]);
   }
}

void handleArgs(int argc, char *argv[])
{
   date_t greg_today;
   char **remain;
   size_t remain_count;
   char *cityNameArg = NULL;
   char *inFileName = NULL;
   char *tzid = NULL;
   char *yahrtzeitFileName = NULL;
   int today_sw = 0;
   int zemanim_sw = 0;
   int help_sw = 0;
   int version_sw = 0;
   int utf8_hebrew_sw = 0;
   char *latitudeStr = NULL;
   char *longitudeStr = NULL;
   char *langStr = NULL;
   static struct option long_options[] = {
      {"24hour", no_argument, 0, 'E'},
      {"abbreviated", no_argument, 0, 'W'},
      {"add-hebrew-dates", no_argument, 0, 'd'},
      {"add-hebrew-dates-for-events", no_argument, 0, 'D'},
      {"ashkenazi", no_argument, 0, 'a'},
      {"candle-mins", required_argument, 0, 'b'},
      {"candlelighting", no_argument, 0, 'c'},
      {"city", required_argument, 0, 'C'},
      {"daf-yomi", no_argument, 0, 'F'},
      {"daily-sedra", no_argument, 0, 'S'},
      {"euro-dates", no_argument, 0, 'e'},
      {"iso-8601", no_argument, 0, 'g'},
      {"format", required_argument, 0, 'f'},
      {"havdalah-mins", required_argument, 0, 'm'},
      {"hebrew-date", no_argument, 0, 'H'},
      {"help", no_argument, 0, 0},
      {"infile", required_argument, 0, 'I'},
      {"israeli", no_argument, 0, 'i'},
      {"latitude", required_argument, 0, 'l'},
      {"lang", required_argument, 0, 0},
      {"longitude", required_argument, 0, 'L'},
      {"molad", no_argument, 0, 'M'},
      {"no-holidays", no_argument, 0, 'h'},
      {"no-modern", no_argument, 0, 0},
      {"no-rosh-chodesh", no_argument, 0, 'x'},
      {"omer", no_argument, 0, 'o'},
      {"sedrot", no_argument, 0, 's'},
      {"sunrise-and-sunset", no_argument, 0, 'O'},
      {"tabs", no_argument, 0, 'r'},
      {"timezone", required_argument, 0, 'z'},
      {"today", no_argument, 0, 't'},
      {"today-brief", no_argument, 0, 'T'},
      {"version", no_argument, 0, 0},
      {"weekday", no_argument, 0, 'w'},
      {"yahrtzeit", required_argument, 0, 'Y'},
      {"years", required_argument, 0, 0},
      {"year-abbrev", no_argument, 0, 'y'},
      {"zmanim", no_argument, 0, 'Z'},
      {NULL, 0, NULL, 0}
   };
   int c;
   int option_index = 0;

   
   setDate(&greg_today);        /* keep the current greg. date here */

   while ((c = getopt_long(argc, argv, "ab:cC:dDeEFghHI:il:L:m:MoOrsStTwWxyY:z:Z8",
                           long_options, &option_index)) != -1) {
       switch (c) {
       case 0: /* long option without short alias */
         if (0 == strcmp("version", long_options[option_index].name)) {
            version_sw = 1;
         } else if (0 == strcmp("help", long_options[option_index].name)) {
            help_sw = 1;
         } else if (0 == strcmp("lang", long_options[option_index].name)) {
            langStr = strdup(optarg);
         } else if (0 == strcmp("years", long_options[option_index].name)) {
           if (!(sscanf(optarg, "%d", &numYears) == 1)) {
               die("unable to read --years argument: %s", optarg);
           }
         } else if (0 == strcmp("no-modern", long_options[option_index].name)) {
            suppressModern_sw = 1;
         } else {
           shortUsage();
           exit(1);
         }
         break;
       case 'a':
           ashkenazis_sw = 1;
           break;
       case '8':
           utf8_hebrew_sw = 1;
           break;
       case 'b':                /* candle_lighting_minutes_before_sundown */
           if (!(sscanf(optarg, "%d", &light_offset) == 1))
               die("unable to read candle_lighting_minutes_before_sundown argument: %s", optarg);
           break;
       case 'c':                /* calculate candlelighting times on fridays */
           candleLighting_sw = 1;
           break;
       case 'C':
           cityNameArg = strdup(optarg);
           break;
       case 'd':                /* print hebrew date */
           printHebDates_sw = 1;
           break;
       case 'D':                /* print hebrew date when there's */
           /* something else to print */
           printSomeHebDates_sw = 1;
           break;
       case 'I':                /* input file */
           inFileName = strdup(optarg);
           break;
       case 'e':                /* european date format */
           gregDateOutputFormatCode_sw = GREG_DATEFORMAT_EURO;
           break;
       case 'g':                /* iso date format */
           gregDateOutputFormatCode_sw = GREG_DATEFORMAT_ISO;
           break;
       case 'E':                /* 24-hour time format */
           twentyFourHour_sw = 1;
           break;
       case 'F':    /* Daf Yomi */
           dafYomi_sw = 1;
           break;
       case 'h':                /* suppress internal holidays */
           noHolidays_sw = 1;
           break;
       case 'H':                /* suppress use hebrew range dates */
           hebrewDates_sw = 1;
           break;
       case 'i':                /* use Israeli sedra scheme */
           israel_sw = 1;
           break;
       case 'l':                /* latitude */
           latitudeStr = strdup(optarg);
           break;
       case 'L':                /* longitude */
           longitudeStr = strdup(optarg);
           break;
       case 'm':                /* havdalah_minutes */
           if (!(sscanf(optarg, "%d", &havdalah_minutes) == 1))
               die("unable to read havdalah_minutes argument: %s", optarg);
           break;
       case 'M':                /* print the molad */
           printMolad_sw = 1;
           break;
       case 'o':                /* print the omer */
           printOmer_sw = 1;
           break;
       case 'O':                /* print sunrise and sunset */
           printSunriseSunset_sw = 1;
           break;
       case 'r':                /* Tab-delineated Format */
           tabs_sw = 1;
           break;
       case 's':                /* print sedrot */
           sedrot_sw = 1;
           break;
       case 'S':                /* print sedra every day. */
           sedraAllWeek_sw = 1;
           break;
       case 'T':                /* do hebcal for today, omit gregorian date. */
           noGreg_sw = 1;
           break;
       case 't':                /* do hebcal for today. */
           today_sw = 1;
           break;
       case 'w':                /* print days of the week */
           weekday_sw = 1;
           break;
       case 'W':                /* abbreviated week view */
           abbrev_sw = 1;
           break;
       case 'y':                /* Print only last 2 digits of year */
           yearDigits_sw = 1;
           break;
       case 'Y':                /* input file */
           yahrtzeitFileName = strdup(optarg);
           break;
       case 'x':                /* input file */
           suppress_rosh_chodesh_sw = 1;
           break;
       case 'z':                /* time zone */
           tzid = strdup(optarg);
           break;
       case 'Z':
           zemanim_sw = 1;
           break;
       default:
           shortUsage();
           exit(1);
           break;
      }
   }

   if (langStr != NULL) {
     hebcal_lang lang = hebcal_get_language(langStr);
     if (lang == HEBCAL_LANG_UNDEFINED) {
       warn("Unknown lang '%s'; using default", langStr);
     } else {
       hebcal_set_language(lang);
     }
     free(langStr);
   } else if (ashkenazis_sw) {
     hebcal_set_language(HEBCAL_LANG_ASHKENAZI);
   } else if (utf8_hebrew_sw) {
     hebcal_set_language(HEBCAL_LANG_HE);
   } else {
     hebcal_set_language(HEBCAL_LANG_DEFAULT);
   }

   if (help_sw) {
      displayHelp();
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
      free(cityNameArg);
   }

   if (inFileName != NULL) {
      inputFile_sw = 1;
      if (!(inFile = fopen(inFileName, "r"))) {
         die("could not open input file %s.", inFileName);
      }
      free(inFileName);
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
      free(yahrtzeitFileName);
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
      free(latitudeStr);
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
      free(longitudeStr);
   }

   if (tzid != NULL) {
      TZ_INFO = timelib_parse_tzfile(tzid, timelib_builtin_db());
      if (TZ_INFO == NULL) {
         die("unable to read time zone argument: %s", tzid);
      }
      free(tzid);
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


   remain_count = argc - optind;
   remain = argv + optind;

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
           displayHelp();
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
           printf("%s: unrecognized command '%s'\n", progname, remain[0]);
           shortUsage();
           exit(1);
       }
       break;
       
   case 2:
       if (!isAllNums(remain[1])) {
           shortUsage();
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
           shortUsage();
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
	   if (theDay > getMonthLength(theYear, theMonth))
               die("Sorry, there aren't that many days in %s (then)",
                   eMonths[theMonth]);
	}
       
       
       rangeType = DAY;
       yearDirty = 1;
       break;
   default:
        displayHelp();
        exit(1);
   }

   if (numYears != 1 && rangeType != YEAR) {
       die("Sorry, --years option works only with entire-year calendars", "");
   }
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
            tempDate.dd = getMonthLength(theYear, theMonth);
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

            tempDate.yy += numYears;
            endAbs = hebrew2abs(tempDate) - 1;
        }
        else
        {
            tempDate.dd = 1;
            tempDate.mm = JAN;
            tempDate.yy = theYear;
            startAbs = greg2abs(tempDate);

            tempDate.yy += numYears;
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
