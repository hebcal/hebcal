/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff
   $Id$

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

   sadinoff@panix.com
 */


#include <stdio.h>
#include <string.h>

#include "myerror.h"
#include "hebcal.h"
#include "common.h"
#include "danlib.h"
#include "rise.h"

#define NM_LEN 60

holstorep_t holidays[14][MAXDAYS], var_holidays[14][MAXDAYS];

extern hmonths_t hMonths;
extern FILE *inFile;

holinput_t inp_holidays[] =
{
   {
      {TISHREI, 2},
      {"Rosh Hashana II"}, YOM_TOV_ENDS},
   {
      {TISHREI, 9},
      {"Erev Yom Kippur"}, LIGHT_CANDLES},
   {
      {TISHREI, 10},
      {"Yom Kippur"}, YOM_TOV_ENDS},

   {
      {TISHREI, 14},
      {"Erev Sukkot", "Erev Sukkos"}, LIGHT_CANDLES},
   {
      {TISHREI, 15},
      {"Sukkot I", "Sukkos I"},},
   {
      {TISHREI, 16},
      {"Sukkot II", "Sukkos II"}, YOM_TOV_ENDS},
   {
      {TISHREI, 17},
      {"Sukkot III (CH''M)", "Sukkos III (CH''M)"},},
   {
      {TISHREI, 18},
      {"Sukkot IV (CH''M)", "Sukkos IV (CH''M)"},},
   {
      {TISHREI, 19},
      {"Sukkot V (CH''M)", "Sukkos V (CH''M)"},},
   {
      {TISHREI, 20},
      {"Sukkot VI (CH''M)", "Sukkos VI (CH''M)"},},
   {
      {TISHREI, 21},
      {"Sukkot VII (Hoshana Raba)", "Sukkos VII (Hoshana Raba)"},
      LIGHT_CANDLES},
   {
      {TISHREI, 22},
      {"Shmini Atzeret", "Shmini Atzeres"},},
   {
      {TISHREI, 23},
      {"Simchat Torah", "Simchas Torah"}, YOM_TOV_ENDS},

   {
      {KISLEV, 24},
      {"Chanukah: 1 Candle"}},
   {
      {KISLEV, 25},
      {"Chanukah: 2 Candles"}},
   {
      {KISLEV, 26},
      {"Chanukah: 3 Candles"}},
   {
      {KISLEV, 27},
      {"Chanukah: 4 Candles"}},
   {
      {KISLEV, 28},
      {"Chanukah: 5 Candles"}},
   {
      {KISLEV, 29},
      {"Chanukah: 6 Candles"}},

   {
      {SHVAT, 15},
      {"Tu B'Shvat"}},

   {
      {NISAN, 14},
      {"Erev Pesach"}, LIGHT_CANDLES},
   {
      {NISAN, 15},
      {"Pesach I"},},
   {
      {NISAN, 16},
      {"Pesach II"}, YOM_TOV_ENDS},
   {
      {NISAN, 17},
      {"Pesach III (CH''M)"},},
   {
      {NISAN, 18},
      {"Pesach IV (CH''M)"},},
   {
      {NISAN, 19},
      {"Pesach V (CH''M)"},},
   {
      {NISAN, 20},
      {"Pesach VI (CH''M)"}, LIGHT_CANDLES},
   {
      {NISAN, 21},
      {"Pesach VII"},},
   {
      {NISAN, 22},
      {"Pesach VIII"}, YOM_TOV_ENDS},


   {
      {IYYAR, 18},
      {"Lag B'Omer"}},
   {
      {IYYAR, 28},
      {"Yom Yerushalayim"}},

   {
      {SIVAN, 5},
      {"Erev Shavuot", "Erev Shavuos"}, LIGHT_CANDLES},
   {
      {SIVAN, 6},
      {"Shavuot I", "Shavuos I"},},
   {
      {SIVAN, 7},
      {"Shavuot II", "Shavuos II"}, YOM_TOV_ENDS},

   {
      {ELUL, 29},
      {"Erev Rosh Hashana"}, LIGHT_CANDLES}
};


/*-------------------------------------------------------------------------*/

holstorep_t getHolstorep 
PROTO ((void))
{				/* return a pointer to a new holiday */
   holstorep_t tmp;
   if (!(tmp = (holstorep_t) malloc ((size_t) (sizeof (holstore_t)))))
      die ("Unable to allocate memory for holiday.", "");
   tmp->next = NULL;
   tmp->name = NULL;
   tmp->typeMask = 0;
   return tmp;
}

/*-------------------------------------------------------------------------*/

/* pushes a copy of a holiday on to a holiday     */
/* list, returning the typemask of the pushed element */
int 
PushHoliday (hp, lp)
holstorep_t
hp,				/* the holiday to be added */
   *lp;				/* pointer to the list to be added to. */
{
   holstorep_t temp;

   temp = getHolstorep ();
   initStr (&temp->name, MAX_LINE_LEN);
   strcpy (temp->name, hp->name);
   temp->typeMask = hp->typeMask;

   if (!*lp)
      /* if there are no holidays here yet, start a new bucket */
      *lp = temp;
   else
   {
      temp->next = *lp;		/* put hp at the head of the list */
      *lp = temp;
   }
   return temp->typeMask;
}

/*-------------------------------------------------------------------------*/

static void 
load_variable_holidays (hYear)
int hYear;
{
   date_t tempDt;
   long int roshHashana, passover, tishaBav, tevet10;
   holstorep_t tmpholp;

   tempDt.yy = hYear;
   tempDt.mm = TISHREI;
   tempDt.dd = 1;
   roshHashana = hebrew2abs (tempDt);

   tempDt.yy = hYear;
   tempDt.mm = TEVET;
   tempDt.dd = 10;
   tevet10 = hebrew2abs (tempDt);

   tempDt.mm = NISAN;
   tempDt.dd = 15;
   passover = hebrew2abs (tempDt);

   tempDt.mm = AV;
   tempDt.dd = 9;
   tishaBav = hebrew2abs (tempDt);

   tmpholp = getHolstorep ();	/* allocate hsnode */
   tmpholp->name = "Tzom Gedaliah";
   PushHoliday (tmpholp, &var_holidays[TISHREI]
		[roshHashana % 7L == THU ? 4 : 3]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Shuvah" : "Shabbat Shuva";
   tempDt = abs2hebrew (day_on_or_before (SAT, 7L + roshHashana));
   PushHoliday (tmpholp, &var_holidays[TISHREI][tempDt.dd]);


/*    printf( "hyear is %d\n",hYear); */
   if (short_kislev (hYear))
   {
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 7 Candles";
      PushHoliday (tmpholp, &var_holidays[TEVET][1]);
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 8 Candles";
      PushHoliday (tmpholp, &var_holidays[TEVET][2]);
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 8th Day";
      PushHoliday (tmpholp, &var_holidays[TEVET][3]);
   }
   else
   {
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 7 Candles";
      PushHoliday (tmpholp, &var_holidays[KISLEV][30]);
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 8 Candles";
      PushHoliday (tmpholp, &var_holidays[TEVET][1]);
      tmpholp = getHolstorep ();
      tmpholp->name = "Chanukah: 8th Day";
      PushHoliday (tmpholp, &var_holidays[TEVET][2]);
   }

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Shekalim" : "Shabbat Shekalim";
   tempDt = abs2hebrew (day_on_or_before (SAT, passover - 43L));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Zachor" : "Shabbat Zachor";
   tempDt = abs2hebrew (day_on_or_before (SAT, passover - 30L));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Ta'anis Esther" : "Ta'anit Esther";
   tempDt = abs2hebrew (passover - (passover % 7L == TUE ? 33L : 31));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   if (LEAP_YR_HEB (hYear))
   {
      tmpholp = getHolstorep ();
      tmpholp->name = ashkenazis_sw ? "Purim Koson" : "Purim Katan";
      PushHoliday (tmpholp, &var_holidays[ADAR_I][14]);

      tmpholp = getHolstorep ();
      tmpholp->name = "Purim";
      PushHoliday (tmpholp, &var_holidays[ADAR_II][14]);
   }
   else
   {
      tmpholp = getHolstorep ();
      tmpholp->name = "Purim";
      PushHoliday (tmpholp, &var_holidays[ADAR_I][14]);
   }

   tmpholp = getHolstorep ();
   tmpholp->name = "Shushan Purim";
   tempDt = abs2hebrew (passover - (passover % 7L == SUN ? 28L : 29));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Parah" : "Shabbat Parah";
   tempDt = abs2hebrew (day_on_or_before (SAT, passover - 14L) - 7L);
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas HaChodesh" : "Shabbat HaChodesh";
   tempDt = abs2hebrew (day_on_or_before (SAT, passover - 14L));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Ta'anis Bechoros" : "Ta'anit Bechorot";
   if ((passover - 1L) % 7L >= 5)
   {				/* saturday or sunday */
      tempDt = abs2hebrew (day_on_or_before (THU, passover));
      PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
   }
   else
      PushHoliday (tmpholp, &var_holidays[NISAN][14]);


   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas HaGadol" : "Shabbat HaGadol";
   tempDt = abs2hebrew (day_on_or_before (SAT, passover - 1L));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   if (hYear > 5698)
   {				/* these only really make sense after 1938 */
      tmpholp = getHolstorep ();
      tmpholp->name = "Yom HaShoah";
      PushHoliday (tmpholp, &var_holidays[NISAN][27]);

      tmpholp = getHolstorep ();
      tmpholp->name = "Yom HaAtzma'ut";
      if (passover % 7L == SUN)
	 tempDt.dd = 3;
      else if (passover % 7L == SAT)
	 tempDt.dd = 4;
      else
	 tempDt.dd = 5;
      PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd]);
      tmpholp = getHolstorep ();
      tmpholp->name = "Yom HaZikaron";
      PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd - 1]);
   }

   tmpholp = getHolstorep ();
   tmpholp->name = "Tzom Tammuz";
   if (tishaBav % 7L == SAT)
      tempDt = abs2hebrew (tishaBav - 20L);
   else
      tempDt = abs2hebrew (tishaBav - 21L);
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Hazon" : "Shabbat Hazon";
   tempDt = abs2hebrew (day_on_or_before (SAT, tishaBav));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = "Tish'a B'Av";
   PushHoliday (tmpholp, &var_holidays[AV]
		[tishaBav % 7L == SAT ? 10 : 9]);

   tmpholp = getHolstorep ();
   tmpholp->name = ashkenazis_sw ? "Shabbas Nachamu" : "Shabbat Nachamu";
   tempDt = abs2hebrew (day_on_or_before (SAT, tishaBav + 7L));
   PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

   tmpholp = getHolstorep ();
   tmpholp->name = "Asara B'Tevet";
   if (tevet10 % 7L >= 5)
   {				/* saturday or sunday */
      tempDt = abs2hebrew (day_on_or_before (THU, tevet10));
      PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
   }
   else
      PushHoliday (tmpholp, &var_holidays[TEVET][10]);

}


/*-------------------------------------------------------------------------*/
/* this function stores the user "holidays" drawn from inFile in the 
   var_holidays array.  The intent is that it will be loaded every new 
   Jewish year.  Gross, I know, but an efficient version will have to wait 
   until %ID% + .1
   */

void 
init_user_holidays (hyear)
int hyear;
{
   holstorep_t tmpholp;

   char *s, *monthStr, *eventStr, nextChar;
   int index, inMonth, inDay, lineNum = 1;

   initStr (&s, MAX_LINE_LEN);
   initStr (&monthStr, MAX_LINE_LEN);
   rewind (inFile);
   nextChar = (char) getc (inFile);	/* priming getc */
   for (; !feof (inFile);
	lineNum++, nextChar = (char) getc (inFile))
   {				/* force an EOF */
      ungetc (nextChar, inFile);
      if (!fgets (s, MAX_LINE_LEN, inFile))
      {
	 warn ("input file read error. Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (s[0] == '\n')		/* blank line */
	 continue;
      if (!sscanf (s, "%s %d%n", monthStr, &inDay, &index))
      {
	 warn ("Error in input file.  Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (isAllNums (monthStr))
      {
	 warn ("Numeric hebrew month in input file.  Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      if (!(inMonth = lookup_hebrew_month (monthStr)))
      {
	 warn ("Unrecognized hebrew month in input file.  Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      if (inDay < 1 || inDay > 30)
      {
	 warn ("Date out of range in input file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }

      if (inMonth == ADAR_II && !LEAP_YR_HEB (hyear))
	 inMonth = ADAR_I;

      eventStr = s + index + 1;	/* get the name of the event */
      if (eventStr[strlen (eventStr) - 1] == '\n')
	 eventStr[strlen (eventStr) - 1] = '\0';		/* chop off the \n */

      /* store the holiday in the LUT */
      tmpholp = getHolstorep ();
      initStr (&tmpholp->name, MAX_LINE_LEN);
      strcpy (tmpholp->name, eventStr);		/* load the user holiday into it. */
      tmpholp->typeMask = USER_EVENT;
      PushHoliday (tmpholp, &var_holidays[inMonth][inDay]);

   }
}


/*-------------------------------------------------------------------------*/
/* this function stores yahrtzeits found in yFile.
   each record is stored in mm dd yyyy str format using GREGORIAN dates 
   */

void 
init_yahrtzeits (hyear)
int hyear;
{
   holstorep_t tmpholp;

   char *s, *monthStr, *eventStr, nextChar;
   int index, inMonth, inDay, inYear, lineNum = 1;
   date_t hDeath, gDeath;

   initStr (&s, MAX_LINE_LEN);
   initStr (&monthStr, MAX_LINE_LEN);
   rewind (yFile);
   nextChar = (char) getc (yFile);	/* priming getc */
   for (; !feof (yFile);
	lineNum++, nextChar = (char) getc (yFile))
   {				/* force an EOF */
      ungetc (nextChar, yFile);
      if (!fgets (s, MAX_LINE_LEN, yFile))
      {
	 warn ("yahrtzeit file read error. Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (s[0] == '\n')		/* blank line */
	 continue;
      if (sscanf (s, "%s %d %d%n", monthStr, &inDay, &inYear, &index) < 3)
      {
	 warn ("Error in yahrtzeit file.  Skipping line %s", hc_itoa (lineNum));
	 continue;
      }
      if (!isAllNums (monthStr))
      {
	 warn ("Non-numeric month in yahrtzeit file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }
      sscanf (monthStr, "%d", &inMonth);

      if (inMonth > 12 || inMonth < 1 ||
	  inDay < 1 || inDay > MonthLengths[LEAP (inYear)][inMonth])
      {

	 warn ("Date out of range in yahrtzeit file. Skipping line %s",
	       hc_itoa (lineNum));
	 continue;
      }

      gDeath.dd = inDay;
      gDeath.mm = inMonth;
      gDeath.yy = inYear;

      hDeath = abs2hebrew (greg2abs (gDeath));

/* If it's Heshvan 30 it depends on the first anniversary; if
   that was not Heshvan 30, use the day before Kislev 1. */

      if (hDeath.mm == CHESHVAN && hDeath.dd == 30 &&
	  !long_cheshvan (hDeath.yy + 1))
      {
	 hDeath.dd = 1;
	 hDeath.mm = KISLEV;
	 hDeath.yy = hyear;
	 hDeath = abs2hebrew (hebrew2abs (hDeath) - 1L);
      }
/* If it's Kislev 30 it depends on the first anniversary; if
   that was not Kislev 30, use the day before Teveth 1. */
      else if (hDeath.mm == KISLEV && hDeath.dd == 30 &&
	       !short_kislev (hDeath.yy + 1))
      {
	 hDeath.dd = 1;
	 hDeath.mm = TEVET;
	 hDeath.yy = hyear;
	 hDeath = abs2hebrew (hebrew2abs (hDeath) - 1L);
      }
/* If it's Adar II, use the same day in last month of
   year (Adar or Adar II). */
      else if (hDeath.mm == ADAR_II)
      {
	 hDeath.mm = MONTHS_IN_HEB (hyear);
      }
/* If it's the 30th in Adar I and year is not a leap year
   (so Adar has only 29 days), use the last day in Shevat. */
      else if (hDeath.mm == ADAR_I && hDeath.dd == 30 &&
	       !LEAP_YR_HEB (hyear))
      {
	 hDeath.dd = 30;
	 hDeath.mm = SHVAT;
      }
/* In all other cases, use the normal anniversary of the date of death. */

      eventStr = s + index + 1;	/* get the name of the event */
/*    if (eventStr[strlen(eventStr)-1] == '\n') */
      eventStr[strlen (eventStr) - 1] = '\0';	/* chop off the \n */

      /* store the holiday in the LUT */
      tmpholp = getHolstorep ();
      initStr (&tmpholp->name, MAX_LINE_LEN);
      strcpy (tmpholp->name, eventStr);		/* load the user holiday into it. */
      tmpholp->typeMask = USER_EVENT;
      PushHoliday (tmpholp, &var_holidays[hDeath.mm][hDeath.dd]);

   }
}



/*-------------------------------------------------------------------------*/
void 
init_holidays (hYear)
int hYear;			/* set up the stored holidays array. */
/* NB that this is dependant on the hYear. */
/* It must therefore be called at the beginning of every hebrew year. */
{

   int d, m;
   holstorep_t tmpholp, tmpholp2;
   holinputp_t todayinp;
   static int first = 1;

   if (first)
   {
      for (m = 0, todayinp = inp_holidays;	/* load constant holidays */
	   m < LAST_INDEX (inp_holidays);
	   m++, todayinp++)
      {
	 tmpholp = getHolstorep ();	/* allocate hsnode */
	 tmpholp->typeMask = todayinp->typeMask;	/*load the new holiday */
	 tmpholp->name = LANGUAGE (todayinp->name);
	 PushHoliday (tmpholp, &holidays[todayinp->date.mm][todayinp->date.dd]);
      }
      first = 0;
   }
   else
   {
      for (m = 0; m <= 13; m++)	/* clear variable holidays buckets */
	 for (d = 0; d < 30; d++)
	 {
	    tmpholp = var_holidays[m][d];
	    while (tmpholp != NULL)
	    {
	       tmpholp2 = tmpholp;
	       tmpholp = tmpholp->next;
	       free (tmpholp2);
	    }
	    var_holidays[m][d] = NULL;
	 }
   }

   load_variable_holidays (hYear);

   if (inputFile_sw)		/* get user-defined "holidays" */
      init_user_holidays (hYear);
   if (yahrtzeitFile_sw)		/* get yahrtzeit dates */
      init_yahrtzeits (hYear);
}



/*-------------------------------------------------------------------------*/
void freeHolidays( holiList )
holstorep_t *holiList;
{
   holstorep_t next;
   holstorep_t cur = *holiList;
   while( cur ) {
      next = cur->next;
      free (cur->name);
      free (cur);
      cur = next;
   }
  
   *holiList = NULL;
}
/*-------------------------------------------------------------------------*/

int 
getHebHolidays (dth, holiList)
/* sets holiList to a linked list of holidadys for that date. */
/* the return value is the | of all the typeMasks of the holidays */
date_t dth;
holstorep_t *holiList;
{
   int tmpMask;
   holstorep_t tmpholip, chp;	/* current holiday pointer */

   tmpMask = 0;
   *holiList = NULL;

   for (chp = holidays[dth.mm][dth.dd];	/* static holidays */
	chp;
	chp = chp->next)
   {
      tmpMask |= PushHoliday (chp, holiList);
   }
   for (chp = var_holidays[dth.mm][dth.dd];	/* variable holidays */
	chp;
	chp = chp->next)
   {
      tmpMask |= PushHoliday (chp, holiList);
   }

   if (dth.dd == 1)
   {
      
      if(dth.mm == TISHREI)	/* special processing for rosh hashana */
      {
	 tmpholip = getHolstorep ();
	 initStr (&tmpholip->name, 22);
	 sprintf (tmpholip->name, "Rosh Hashana %d", dth.yy);
	 PushHoliday (tmpholip, &var_holidays[TISHREI][1]);
	 tmpMask |= PushHoliday (tmpholip, holiList);
      }
      else 
	 if( ! suppress_rosh_chodesh_sw )      /* rosh Chodesh Processing... */
	 {
	    tmpholip = getHolstorep ();
	    initStr (&tmpholip->name, NM_LEN);
	    strcat (tmpholip->name, "Rosh Chodesh ");
	    if (!tabs_sw)
	    {
	       strncat (tmpholip->name, hMonths[LEAP_YR_HEB (dth.yy)][dth.mm], NM_LEN);
	    }
	    else
	    {
	       0;			/* put your special output format here */
	    } /* don't name the rosh chodesh if -y switch */
	    tmpMask |= PushHoliday (tmpholip, holiList);
	 }
   }
   

   if (dth.dd == 30 && ! suppress_rosh_chodesh_sw)
   {
      tmpholip = getHolstorep ();
      initStr (&tmpholip->name, NM_LEN);
      strcat (tmpholip->name, "Rosh Chodesh ");
      if (!tabs_sw)
      {
	 strncat (tmpholip->name,
		  hMonths[LEAP_YR_HEB (dth.yy)][dth.mm + 1], NM_LEN);
      }
      else
      {
	 0;			/* put your special output format here. */
      } /* don't name the rosh chodesh if -y switch */
      tmpMask |= PushHoliday (tmpholip, holiList);
   }
  

   return tmpMask;
}
