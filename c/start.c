/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   http://sourceforge.net/projects/hebcal

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
   1 Cove La.
   Great Neck, NY
   11024

   sadinoff@pobox.com
 */

/* the following need to be #defined: CITY ENV_CITY ENV_OPTS */

#include <stdio.h>
#include <ctype.h>
#include "mystring.h"
#include "mymath.h"
#include "hebcal.h"
#include "common.h"
#include "mygetopt.h"
#include "danlib.h"
#include "stdlib.h"

#define ENV_CITY "HEBCAL_CITY"
#define ENV_OPTS "HEBCAL_OPTS"

void handleArgs PROTO((int, char **));
int main PROTO((int, char **));
int ok_to_run = 1;

#define YEAR 0
#define MONTH 1
#define DAY 2
#define TODAY 3

#define SCREEN_HEIGHT 22

char *progname;
static int theYear, theMonth, theDay, yearDirty, rangeType, zonep, schemep,
  latp, longp;			/* has the user inputted lat and long? */
static char
 *cityName, *helpArray[] =
{
   "Hebcal Version " VERSION " By Danny Sadinoff",
   "usage: hebcal [-8acdDeHhiorsStTwy]",
   "            [-I file]",
   "            [-Y yahrtzeit_file]",
   "            [-C city]",
   "            [-L longitude -l latitude]",
   "            [-m havdalah_minutes_past_sundown ]",
   "            [-z timezone]",
   "            [-Z daylight_savings_option]",
   "            [[ month [ day ]] year ]",
   "       hebcal help",
   "       hebcal info",
   "       hebcal DST",
   "       hebcal cities",
   "       hebcal warranty",
   "       hebcal copying",
   "OPTIONS:",
   "   -8 : Use 8-bit Hebrew (ISO-8859-8-Logical).",
   "   -a : Use ashkenazis hebrew.",
   "   -c : Print candlelighting times.",
   "   -C city : Set latitude, longitude, timezone and daylight",
   "             savings scheme according to specified city. ",
   "             This option implies the -c option.",
   "   -d : print the hebrew date for the entire date range.",
   "   -D : print the hebrew date for dates with some event.",
   "   -e : Ouput \"european\" dates -- DD.MM.YYYY format.",
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
   "   -o : Add days of the omer.",
   "   -r : Tab delineated format.",
   "   -s : Add weekly sedrot on saturday.",
   "   -S : Print sedrah of the week on all calendar days.",
   "   -t : Only output for today's date.",
   "   -T : Print today's pertinent information, no gregorian date.",
   "   -w : Add day of the week.",
   "   -x : Suppress Rosh Chodesh.",
   "   -y : Print only last two digits of year.",
   "   -Y file : Get yahrtzeit dates from specified file.",
   "             The format is: mm dd yyyy string",
   "             the first three fields specify a *Gregorian* date.",
   "   -z : Use specified timezone, disabling daylight savings time,",
   "        overriding the -C (localize to city) switch.",
   "   -Z scheme : change to daylight savings scheme.  The possible values",
   "        of scheme are currently usa, israel, eu, and none.",
   "",
   "hebcal help    -- Print this message.",
   "hebcal info    -- Print version and localization data.",
   "hebcal DST     -- Print a list of available daylight savings schemes.",
   "hebcal cities  -- Print a list of available cities.",
   "hebcal warranty -- Tells you how there's NO WARRANTY for hebcal.",
   "hebcal copying -- Prints the details of the GNU copyright.",
   "",
   "Hebcal prints out hebrew calendars one solar year at a time.",
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
   "Hebcal is copyright (c) 1994 By Danny Sadinoff",
   "Portions Copyright (c) 2002 Michael J. Radwin. All rights reserved.",
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
   "\"Free\" above means freely distributed.  Checks are always welcome:",
   "  If you like hebcal,",
   "Send money to Danny Sadinoff",
   "            1 Cove La.    ",
   "            Great Neck, NY",
   "                    11024",
   "",
   "WWW:",
   "            http://sourceforge.net/projects/hebcal"
};

typedef struct
{
   char *name;
   int DST_scheme;
}
dst_t;

dst_t savings_bank[] =
{
   {"usa", DST_USOFA},
   {"none", DST_NONE},
   {"israel", DST_ISRAEL},
   {"eu", DST_EU},
   {"", 0}
};

typedef struct
{
   char *name;
   int latdeg, latmin, longdeg, longmin, TZ, DST_scheme;
}
city_t;

city_t cities[] =
{
   {"Atlanta", 33, 45, 84, 23, -5, DST_USOFA},
   {"Austin", 30, 16, 97, 45, -6, DST_USOFA},
   {"Berlin", 52, 31, -13, -24, 1, DST_EU},
   {"Baltimore", 39, 17, 76, 36, -5, DST_USOFA},
   {"Bogota", 4, 36, 74, 5, -5, DST_NONE},
   {"Boston", 42, 20, 71, 4, -5, DST_USOFA},
   {"Buenos Aires", -34, -37, 58, 24, -3, DST_NONE},
   {"Buffalo", 42, 53, 78, 52, -5, DST_USOFA},
   {"Chicago", 41, 50, 87, 45, -6, DST_USOFA},
   {"Cincinnati", 39, 6, 84, 31, -5, DST_USOFA},
   {"Cleveland", 41, 30, 81, 41, -5, DST_USOFA},
   {"Dallas", 32, 47, 96, 48, -6, DST_USOFA},
   {"Denver", 39, 44, 104, 59, -7, DST_USOFA},
   {"Detroit", 42, 20, 83, 2, -5, DST_USOFA},
   {"Gibraltar", 36, 0, 5, 0, -10, DST_USOFA},
   {"Hawaii", 19, 30, 155, 30, -10, DST_USOFA},
   {"Houston", 29, 46, 95, 22, -6, DST_USOFA},
   {"Jerusalem", 31, 47, -35, -14, 2, DST_ISRAEL},
   {"Johannesburg", -28, -3, -26, -12, 1, DST_NONE},
   {"London", 51, 30, 0, 10, 0, DST_EU},
   {"Los Angeles", 34, 4, 118, 15, -8, DST_USOFA},
   {"Miami", 25, 46, 80, 12, -5, DST_USOFA},
   {"Mexico City", 19, 24, 99, 9, -6, DST_USOFA},
   {"New York", 40, 43, 74, 1, -5, DST_USOFA},
   {"Omaha", 41, 16, 95, 56, -7, DST_USOFA},
   {"Philadelphia", 39, 57, 75, 10, -5, DST_USOFA},
   {"Phoenix", 33, 27, 112, 4, -7, DST_USOFA},
   {"Pittsburgh", 40, 26, 80, 0, -5, DST_USOFA},
   {"Saint Louis", 38, 38, 90, 12, -6, DST_USOFA},
   {"San Francisco", 37, 47, 122, 25, -8, DST_USOFA},
   {"Seattle", 47, 36, 122, 20, -8, DST_USOFA},
   {"Toronto", 43, 38, 79, 24, -5, DST_USOFA},
   {"Vancouver", 49, 16, 123, 7, -8, DST_USOFA},
   {"Washington DC", 38, 55, 77, 0, -5, DST_USOFA},
   {0, 0, 0, 0, 0, 0, 0}
};


void
  print_version_data()
{
   printf("hebcal version " VERSION "\n");
   printf("=====Defaults=====\n");
   printf("City: %s\n", cityName);
   printf("          %dd%d' %c latitude\n", abs(latdeg), abs(latmin),
	  latdeg < 0 ? 'S' : 'N');
   printf("          %dd%d' %c longitude\n", abs(longdeg), abs(longmin),
	  longdeg < 0 ? 'W' : 'E');
   printf("\nEnvironment variable for default city: %s\n", ENV_CITY);
   printf("\nEnvironment variable for default options: %s\n", ENV_OPTS);
}

#define LIC_LEN 269
#define WAR_LEN 26
void
  print_warranty()
{

   int cnum;
   char dummy[10];

   for (cnum = 0;
	cnum < WAR_LEN;
	cnum++)
   {
      puts(warranty[cnum]);
      if (0 == (cnum + 2) % SCREEN_HEIGHT)
      {
	 printf(" ---MORE---    Hit Enter To Continue....");
	 fgets(&dummy[0], 10, stdin);
      }
   }
}

void
  print_copying()
{

   int cnum;
   char dummy[10];

   for (cnum = 0;
	cnum < LIC_LEN;
	cnum++)
   {
      puts(license[cnum]);
      if (0 == (cnum + 2) % SCREEN_HEIGHT)
      {
	 printf(" ---MORE---    Hit Enter To Continue....");
	 fgets(&dummy[0], 10, stdin);
      }
   }
   for (cnum = 0;
	cnum < WAR_LEN;
	cnum++)
   {
      puts(warranty[cnum]);
      if (0 == (cnum + 2) % SCREEN_HEIGHT)
      {
	 printf(" ---MORE---    Hit Enter To Continue....");
	 fgets(&dummy[0], 10, stdin);
      }
   }
}

void
  print_city_data()
{

   int cnum;
   char dummy[10];

   for (cnum = 0;
	cities[cnum].name;
	cnum++)
   {
      puts(cities[cnum].name);
      if (0 == (cnum + 2) % SCREEN_HEIGHT)
      {
	 printf(" ---MORE---    Hit Enter To Continue....");
	 fgets(&dummy[0], 10, stdin);
      }
   }
}

void
  print_DST_data()
{

   int cnum;
   char dummy[10];

   for (cnum = 0;
	(cnum < sizeof(savings_bank) / sizeof(dst_t));
	cnum++)
   {
      puts(savings_bank[cnum].name);
      if (0 == (cnum + 1) % SCREEN_HEIGHT)
      {
	 printf(" ---MORE---    Hit Enter To Continue....");
	 fgets(&dummy[0], 10, stdin);
      }
   }
}


void
  localize_to_city(s)
     char *s;
{
   int len = strlen(s);
   char *pc, *cityStr;
   city_t *pcity;

   initStr(&cityStr, strlen(s));
   strcpy(cityStr, s);

   if (cityName != NULL)
      free(cityName);

   /* convert non-alpha to spaces */
   for (pc = cityStr; *pc != '\0'; pc++)
      if (!isalpha(*pc))
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
	 if (!zonep)
	 {
	    TZ = pcity->TZ;
	    DST_scheme = pcity->DST_scheme;
	 }
	 free(cityStr);
	 initStr(&cityName, strlen(pcity->name));
	 strcpy(cityName, pcity->name);
	 return;
      }

   warn("unknown city: %s. Use a nearby city or geographic coordinates.", s);
   warn("run 'hebcal cities' for a list of cities.", "");
   ok_to_run = 0;
}

void
  set_DST_scheme(s)
     char *s;
{
   int len = strlen(s);
   dst_t *pdst;

   for (pdst = savings_bank; pdst->name != NULL; pdst++)
      if (0 == istrncasecmp(len, s, pdst->name))
      {
	 DST_scheme = pdst->DST_scheme;
	 return;
      }

   die("unknown daylight savings scheme: %s.  \"hebcal DST\" for options.", s);
}


void
  set_default_city()
{
   char *s;

#ifdef CITY
   localize_to_city(CITY);
#else
   localize_to_city("NEW YORK");	/* helluva town. */
#endif

/* having set the default city, now check if there is an overriding */
/* default from the environment. */
   if (NULL != (s = getenv(ENV_CITY)))
      localize_to_city(s);

}



void
  handleArgs(argc, argv)
     int argc;
     char *argv[];
{
   char dummy[10];
   date_t greg_today;
   int option;

   char *usage =		/* not quite sure how compatible this is */
   "usage: \n\
   hebcal [-acdDehHiorsStTwxy]\n\
          [-I input_file]\n\
          [-Y yahrtzeit_file]\n\
          [-C city]\n\
          [-L longitude -l latitude]\n\
          [-m havdalah_minutes]\n\
          [-z timezone]\n\
          [-Z daylight_savings_scheme]\n\
          [[month [day]] year]\n\
   hebcal help\n\
   hebcal info\n\
   hebcal DST\n\
   hebcal cities\n\
   hebcal warranty\n\
   hebcal copying\n";

   setDate(&greg_today);	/* keep the current greg. date here */

   Getopt(argc, argv, "", 1);
   while (EOF !=
	(option = Getopt(argc, argv, "acC:dDehHI:il:L:m:orsStTwxyY:z:Z:8", 0)))
   {
      switch ((char) option)
      {
	case 'a':		/* ashkenazis hebrew */
	   ashkenazis_sw = 1;
	   break;
	case '8':		/* ashkenazis hebrew */
	   iso8859_8_sw = 1;
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
	case 'o':		/* print the omer */
	   printOmer_sw = 1;
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
	case 'Z':
	   schemep = 1;
	   sscanf(Optarg, "%s", dummy);
	   set_DST_scheme(dummy);
	   break;
	   

	case 'z':		/* time zone */
	   if (!(sscanf(Optarg, "%d", &TZ) == 1))
	      die("unable to read time zone argument: %s", Optarg);
	   if (!schemep)
	      DST_scheme = DST_NONE;
	   zonep = 1;
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
	   int lineNumber;

	   for (lineNumber = 0;
		(lineNumber < sizeof(helpArray) / sizeof(char *));

		lineNumber++)
	   {
	      puts(helpArray[lineNumber]);
	      
	      if (0 == lineNumber % SCREEN_HEIGHT)
	      {
		 printf(" ---MORE---    Hit Enter To Continue....");
		 fgets(&dummy[0], 10, stdin);
	      }
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
	else if (0 == istrncasecmp(3, argv[Optind], "DST"))
	{
	   print_DST_data();
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
	      die("Don't use numbers to specify hebrew months.", "");
	   else
	      theMonth = atoi(argv[Optind]);	/* gregorian month */
	else if (hebrewDates_sw)
	   die("Unknown hebrew month: %s", argv[Optind]);
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
	      die("Don't use numbers to specify hebrew months.", "");
	   else
	      theMonth = atoi(argv[Optind]);	/* gregorian month */
	else if (hebrewDates_sw)
	   die("Unknown hebrew month: %s", argv[Optind]);
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


int tokenize(str, pargc, argv)
        int *pargc;
        char *argv[], *str;
{
   char *strtok(), *strdup();
   char *s;
   
   argv[0] = progname;
   s = strtok(str, "\t ");
   *pargc = 1;
   do
   {
      argv[(*pargc)++] = strdup(s);
   }
   while ((s = strtok(NULL, "\t ")));


   return *pargc > 1;
}

int main(argc, argv)
    int argc;
    char *argv[];
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
