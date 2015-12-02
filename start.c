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
#include "mygetopt.h"
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
   "Hebcal Version " VERSION " By Danny Sadinoff",
   "usage: hebcal [-8acdDeEFHhiorsStTwWy]",
   "            [-b candle_lighting_minutes_before_sundown ]",
   "            [-I file]",
   "            [-Y yahrtzeit_file]",
   "            [-C city]",
   "            [-L longitude -l latitude]",
   "            [-m havdalah_minutes_past_sundown ]",
   "            [-z timezone]",
   "            [-f format_option]",
   "            [[ month [ day ]] year ]",
   "       hebcal help",
   "       hebcal info",
   "       hebcal cities",
   "       hebcal warranty",
   "       hebcal copying",
   "OPTIONS:",
   "   -8 : Use 8-bit Hebrew (ISO-8859-8-Logical).",
   "   -a : Use ashkenazis Hebrew.",
   "   -b mins : Set candle-lighting to occur this many minutes before sundown ",
   "   -c : Print candlelighting times.",
   "   -C city : Set latitude, longitude, timezone and daylight",
   "             savings scheme according to specified city. ",
   "             This option implies the -c option.",
   "   -d : print the Hebrew date for the entire date range.",
   "   -D : print the Hebrew date for dates with some event.",
   "   -e : Ouput \"European\" dates -- DD.MM.YYYY format.",
   "   -E : Ouput 24-hour times (e.g. 18:37 instead of 6:37).",
   "   -f FORMAT : change output to FORMAT. see below for format strings",
   "   -F : Output the Daf Yomi for the entire date range.",
   "   -h : Suppress default holidays.",
   "   -H : Use Hebrew date ranges - only needed when e.g. hebcal -H 5373",
   "   -i : Use Israeli sedra scheme.",
   "   -I file : Get non-yahrtzeit Hebrew user events from specified file.",
 "        The format is : mmm dd string, Where mmm is a Hebrew month name.",
   "   -l xx,yy : Set the latitude for solar calculations to",
   "              xx degrees and yy minutes.  Negative values are south.",
   "   -L xx,yy : Set the longitude for solar calculations to",
   "              xx degrees and yy minutes.  *Negative values are EAST*.",
   "        The -l and -L switches must both be used, or not at all.",
   "        These switches override the -C (localize to city) switch.",
   "   -m mins : Set havdalah to occur this many minutes after sundown ",
   "   -M : Print the molad on shabbat mevorchim.",
   "   -o : Add days of the omer.",
   "   -O : Output sunrise and sunset times every day.",
   "   -r : Tab delineated format.",
   "   -s : Add weekly sedrot on saturday.",
   "   -S : Print sedrah of the week on all calendar days.",
   "   -t : Only output for today's date.",
   "   -T : Print today's pertinent information, no gregorian date.",
   "   -w : Add day of the week.",
   "   -W : Sunrise, daf, omer via other options show once a week. ",
   "   -x : Suppress Rosh Chodesh.",
   "   -y : Print only last two digits of year.",
   "   -Y file : Get yahrtzeit dates from specified file.",
   "             The format is: mm dd yyyy string",
   "             the first three fields specify a *Gregorian* date.",
   "   -z : Use specified timezone, overriding -C (localize to city).",
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


/* FIX: this wants to become gnu-ish */
void handleArgs(int argc, char *argv[])
{
   char dummy[10];
   date_t greg_today;
   int option;

   char *usage =		/* not quite sure how compatible this is */
   "usage: \n\
   hebcal [-acdDeEfFhHiMoOrsStTwWxyZ]\n\
          [-b candle_lighting_minutes_before_sundown]\n\
          [-I input_file]\n\
          [-Y yahrtzeit_file]\n\
          [-C city]\n\
          [-L longitude -l latitude]\n\
          [-m havdalah_minutes]\n\
          [-z timezone]\n\
          [[month [day]] year]\n\
   hebcal help\n\
   hebcal info\n\
   hebcal cities\n\
   hebcal warranty\n\
   hebcal copying\n";

   setDate(&greg_today);	/* keep the current greg. date here */

   Getopt(argc, argv, "", 1);
   while (EOF !=
          (option = Getopt(argc, argv, "ab:cC:dDeEFf:hHI:il:L:m:MoOrsStTwWxyY:z:Z8", 0)))
   {
       switch ((char) option)
       {
       case 'a':		/* ashkenazis hebrew */
	   ashkenazis_sw = 1;
	   break;
       case '8':		/* ashkenazis hebrew */
	   iso8859_8_sw = 1;
	   break;
       case 'b':		/* candle_lighting_minutes_before_sundown */
	   if (!(sscanf(Optarg, "%d", &light_offset) == 1))
               die("unable to read candle_lighting_minutes_before_sundown argument: %s", Optarg);
	   light_offset *= -1;
	   break;
       case 'c':		/* calculate candlelighting times on fridays */
	   candleLighting_sw = 1;
	   break;
       case 'C':
	   localize_to_city(Optarg);
	   candleLighting_sw = 1;
	   break;
       case 'd':		/* print hebrew date */
	   printHebDates_sw = 1;
	   break;
       case 'D':		/* print hebrew date when there's */
	   /* something else to print */
	   printSomeHebDates_sw = 1;
	   break;
       case 'I':		/* input file */
	   inputFile_sw = 1;
	   if (!(inFile = fopen(Optarg, "r")))
               die("could not open input file %s.", Optarg);
	   break;
       case 'e':		/* european date format */
	   euroDates_sw = 1;
	   break;
       case 'E':                /* 24-hour time format */
           twentyFourHour_sw = 1;
           break;
       case 'f':		/* output format */
           formatString = strdup(Optarg);
	   break;
       case 'F':    /* Daf Yomi */
	   dafYomi_sw = 1;
	   break;
       case 'h':		/* suppress internal holidays */
	   noHolidays_sw = 1;
	   break;
       case 'H':		/* suppress use hebrew range dates */
	   hebrewDates_sw = 1;
	   break;
       case 'i':		/* use Israeli sedra scheme */
	   israel_sw = 1;
	   break;
       case 'l':		/* latitude */
	   latdeg = latmin = 0;
	   latp = 1;
	   if (sscanf(Optarg, "%d,%d", &latdeg, &latmin) < 2)
               die("unable to read latitude argument: %s", Optarg);
	   if ((abs(latdeg) > 90) ||
	       latmin > 60 || latmin < 0)
	      die("latitude argument out of range", "");
	   latmin = abs(latmin);
	   if (latdeg < 0)
               latmin = -latmin;
	   break;
       case 'L':		/* longitude */
	   longdeg = longmin = 0;
	   longp = 1;
	   if (sscanf(Optarg, "%d,%d", &longdeg, &longmin) < 2)
               die("unable to read longitude argument: %s", Optarg);
	   if ((abs(longdeg) > 180) ||
	       longmin > 60 || longmin < 0)
	      die("longitude argument out of range", "");
	   longmin = abs(longmin);
	   if (longdeg < 0)
               longmin = -longmin;
	   break;
       case 'm':		/* havdalah_minutes */
	   if (!(sscanf(Optarg, "%d", &havdalah_minutes) == 1))
               die("unable to read havdalah_minutes argument: %s", Optarg);
	   break;
       case 'M':		/* print the molad */
           printMolad_sw = 1;
           break;
       case 'o':		/* print the omer */
	   printOmer_sw = 1;
	   break;
       case 'O':		/* print sunrise and sunset */
	   printSunriseSunset_sw = 1;
	   break;
       case 'r':		/* Tab-delineated Format */
	   tabs_sw = 1;
	   break;
       case 's':		/* print sedrot */
	   sedrot_sw = 1;
	   break;
       case 'S':		/* print sedra every day. */
	   sedraAllWeek_sw = 1;
	   break;
       case 'T':		/* do hebcal for today, omit gregorian date. */
	   noGreg_sw = 1;
/*** note this falls through to 't' case ***/
       case 't':		/* do hebcal for today. */
	   printHebDates_sw = 1;
	   rangeType = TODAY;
	   theMonth = greg_today.mm;	/* year and month specified */
	   theDay = greg_today.dd;	/* printc theDay of theMonth */
	   yearDirty = 1;
	   printOmer_sw = 1;
	   break;
       case 'w':		/* print days of the week */
	   weekday_sw = 1;
	   break;
       case 'W':		/* abbreviated week view */
           abbrev_sw = 1;
	   break;
       case 'y':		/* Print only last 2 digits of year */
	   yearDigits_sw = 1;
	   break;
       case 'Y':		/* input file */
	   yahrtzeitFile_sw = 1;
	   if (!(yFile = fopen(Optarg, "r")))
               die("could not open yahrtzeit input file %s.", Optarg);
	   break;
       case 'x':		/* input file */
	   suppress_rosh_chodesh_sw = 1;
	   break;
       case 'z':		/* time zone */
	   TZ_INFO = timelib_parse_tzfile(Optarg, timelib_builtin_db());
	   if (TZ_INFO == NULL)
               die("unable to read time zone argument: %s", Optarg);
           break;
       case 'Z':
	   default_zemanim = (ZMAN_SUNRISE | ZMAN_SZKS | ZMAN_TEFILAH | 
			      ZMAN_CHATZOT |
			      ZMAN_MINCHA_GEDOLA | ZMAN_MINCHA_KETANA |
			      ZMAN_PLAG_HAMINCHA | ZMAN_SUNSET | ZMAN_TZAIT_42);
	   
	   break;
           
       default:
	   die(usage, "");
       }
   }
   
   if (latp)
       cityName = "User Defined City";
   if (latp ^ longp)
       die("You must enter BOTH the latitude and the longitude", "");
   
   if( !strcmp(cityName, "Jerusalem" ))
       light_offset = -40;	/* does everyone hold by this? */

   switch (argc - Optind)	/* suck up the date */
   {
   case 0:			/* process this year */
       if (hebrewDates_sw)
	   theYear = abs2hebrew(greg2abs(greg_today)).yy;
       else
	   theYear = greg_today.yy;
       break;
       
   case 1:
       if (isAllNums(argv[Optind]))
       {
	   theYear = atoi(argv[Optind]);	/* just year specified */
	   yearDirty = 1;		/* print whole year */
       }
       else if (0 == istrncasecmp(5, argv[Optind], "help"))
       {
	   size_t lineNumber;
           
	   for (lineNumber = 0;
		(lineNumber < sizeof(helpArray) / sizeof(char *));
                lineNumber++)
	   {
	      puts(helpArray[lineNumber]);
	   }
	   exit(0);
       }
       else if (0 == istrncasecmp(3, argv[Optind], "info"))
       {
	   print_version_data();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, argv[Optind], "cities"))
       {
	   print_city_data();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, argv[Optind], "copying"))
       {
	   print_copying();
	   exit(0);
       }
       else if (0 == istrncasecmp(3, argv[Optind], "warranty"))
       {
	   print_warranty();
	   exit(0);
       }
       else
	   die(usage, "");
       break;
       
   case 2:
       if (!isAllNums(argv[Optind + 1]))
	   die(usage, "");
       theYear = atoi(argv[Optind + 1]);		/* print theMonth of theYear */
       
       theMonth = lookup_hebrew_month(argv[Optind]);
       
       if (theMonth)
       {
	   hebrewDates_sw = 1;	/* automagically turn it on */
	   if (theMonth == ADAR_II && !LEAP_YR_HEB(theYear))
               theMonth = ADAR_I;	/* silently fix this mistake */
       }
       else if (isAllNums(argv[Optind]))
	   if (hebrewDates_sw)
               die("Don't use numbers to specify Hebrew months.", "");
	   else
	      theMonth = atoi(argv[Optind]);	/* gregorian month */
       else if (hebrewDates_sw)
	   die("Unknown Hebrew month: %s", argv[Optind]);
       else
	   die(usage, "");	/* bad gregorian month. */
       
       Optind++;
       yearDirty = 1;
       rangeType = MONTH;
	break;
        
   case 3:
       if (!(isAllNums(argv[Optind + 1])
             && isAllNums(argv[Optind + 2])))
	   die(usage, "");
       theDay = atoi(argv[Optind + 1]);	/* print theDay of theMonth */
       theYear = atoi(argv[Optind + 2]);		/* print theMonth of theYear */
       
       theMonth = lookup_hebrew_month(argv[Optind]);
       
       if (theMonth)
       {
	   hebrewDates_sw = 1;	/* automagically turn it on */
	   if (theMonth == ADAR_II && !LEAP_YR_HEB(theYear))
               theMonth = ADAR_I;	/* silently fix this mistake */
       }
       else if (isAllNums(argv[Optind]))
	   if (hebrewDates_sw)
               die("Don't use numbers to specify Hebrew months.", "");
	   else
               theMonth = atoi(argv[Optind]);	/* gregorian month */
	else if (hebrewDates_sw)
            die("Unknown Hebrew month: %s", argv[Optind]);
	else
            die("bad month.%s", usage);	/* bad gregorian month. */

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
       die(usage, "");
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
