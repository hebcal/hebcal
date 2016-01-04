/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

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


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "myerror.h"
#include "hebcal.h"
#include "common.h"
#include "danlib.h"

#define _(String) gettext(String)

#define NM_LEN MAX_LINE_LEN

holstorep_t holidays[14][MAXDAYS], var_holidays[14][MAXDAYS];

#define IGNORE_YEAR 0


holinput_t inp_holidays[] =
{
   {
      {TISHREI, 2, IGNORE_YEAR},
      "Rosh Hashana II",
      YOM_TOV_ENDS,
      NULL
   },
   {
      {TISHREI, 9, IGNORE_YEAR},
      "Erev Yom Kippur",
      LIGHT_CANDLES,
      NULL
   },
   {
      {TISHREI, 10, IGNORE_YEAR},
      "Yom Kippur",
      YOM_TOV_ENDS,
      NULL
   },

   {
      {TISHREI, 14, IGNORE_YEAR},
      "Erev Sukkot",
      LIGHT_CANDLES,
      NULL
   },
   {
      {TISHREI, 15, IGNORE_YEAR},
      "Sukkot I",
      CHUL_ONLY,
      NULL
   },
   {
      {TISHREI, 15, IGNORE_YEAR},
      "Sukkot I",
      YOM_TOV_ENDS|IL_ONLY,
      NULL
   },
   {
      {TISHREI, 16, IGNORE_YEAR},
      "Sukkot II",
      YOM_TOV_ENDS|CHUL_ONLY,
      NULL
   },
   {
      {TISHREI, 16, IGNORE_YEAR},
      "Sukkot II (CH''M)",
      IL_ONLY,
      NULL
   },
   {
      {TISHREI, 17, IGNORE_YEAR},
      "Sukkot III (CH''M)",
      0,
      NULL
   },
   {
      {TISHREI, 18, IGNORE_YEAR},
      "Sukkot IV (CH''M)",
      0,
      NULL
   },
   {
      {TISHREI, 19, IGNORE_YEAR},
      "Sukkot V (CH''M)",
      0,
      NULL
   },
   {
      {TISHREI, 20, IGNORE_YEAR},
      "Sukkot VI (CH''M)",
      0,
      NULL
   },
   {
      {TISHREI, 21, IGNORE_YEAR},
      "Sukkot VII (Hoshana Raba)",
      LIGHT_CANDLES,
      NULL
   },
   {
      {TISHREI, 22, IGNORE_YEAR},
      "Shmini Atzeret",
      CHUL_ONLY,
      NULL
   },
   {
      {TISHREI, 22, IGNORE_YEAR},
      "Shmini Atzeret",
      IL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {TISHREI, 23, IGNORE_YEAR},
      "Simchat Torah",
      CHUL_ONLY|YOM_TOV_ENDS,
      NULL
   },

   {
      {KISLEV, 24, IGNORE_YEAR},
      "Chanukah: 1 Candle",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {KISLEV, 25, IGNORE_YEAR},
      "Chanukah: 2 Candles",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {KISLEV, 26, IGNORE_YEAR},
      "Chanukah: 3 Candles",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {KISLEV, 27, IGNORE_YEAR},
      "Chanukah: 4 Candles",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {KISLEV, 28, IGNORE_YEAR},
      "Chanukah: 5 Candles",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {KISLEV, 29, IGNORE_YEAR},
      "Chanukah: 6 Candles",
      CHANUKAH_CANDLES,
      NULL
   },
   {
      {SHVAT, 15, IGNORE_YEAR},
      "Tu B'Shvat",
      0,
      NULL
   },
   {
      {NISAN, 14, IGNORE_YEAR},
      "Erev Pesach",
      LIGHT_CANDLES,
      NULL
   },
   {
      {NISAN, 15, IGNORE_YEAR},
      "Pesach I",
      CHUL_ONLY,
      NULL
   },
   {
      {NISAN, 15, IGNORE_YEAR},
      "Pesach I",
      IL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {NISAN, 16, IGNORE_YEAR},
      "Pesach II",
      CHUL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {NISAN, 16, IGNORE_YEAR},
      "Pesach II (CH''M)",
      IL_ONLY,
      NULL
   },
   {
      {NISAN, 17, IGNORE_YEAR},
      "Pesach III (CH''M)",
      0,
      NULL
   },
   {
      {NISAN, 18, IGNORE_YEAR},
      "Pesach IV (CH''M)",
      0,
      NULL
   },

   {
      {NISAN, 19, IGNORE_YEAR},
      "Pesach V (CH''M)",
      0,
      NULL
   },
   {
      {NISAN, 20, IGNORE_YEAR},
      "Pesach VI (CH''M)",
      LIGHT_CANDLES,
      NULL
   },
   {
      {NISAN, 21, IGNORE_YEAR},
      "Pesach VII",
      CHUL_ONLY,
      NULL
   },
   {
      {NISAN, 21, IGNORE_YEAR},
      "Pesach VII",
      IL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {NISAN, 22, IGNORE_YEAR},
      "Pesach VIII",
      CHUL_ONLY|YOM_TOV_ENDS,
      NULL
   },

   {
      {IYYAR, 14, IGNORE_YEAR},
      "Pesach Sheni",
      0,
      NULL
   },

   {
      {IYYAR, 18, IGNORE_YEAR},
      "Lag B'Omer",
      0,
      NULL
   },

   {
      {SIVAN, 5, IGNORE_YEAR},
      "Erev Shavuot",
      LIGHT_CANDLES,
      NULL
   },
   {
      {SIVAN, 6, IGNORE_YEAR},
      "Shavuot I",
      CHUL_ONLY,
      NULL
   },
   {
      {SIVAN, 6, IGNORE_YEAR},
      "Shavuot I",
      IL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {SIVAN, 7, IGNORE_YEAR},
      "Shavuot II",
      CHUL_ONLY|YOM_TOV_ENDS,
      NULL
   },
   {
      {AV, 15, IGNORE_YEAR},
      "Tu B'Av",
      0,
      NULL
   },
   {
      {ELUL, 29, IGNORE_YEAR},
      "Erev Rosh Hashana",
      LIGHT_CANDLES,
      NULL
   }
};

#define HOLIDAY_ASARA_B_TEVET   LANGUAGE2(var_hol_names[0].name)
#define HOLIDAY_CHANUKAH_7_CANDLES      LANGUAGE2(var_hol_names[1].name)
#define HOLIDAY_CHANUKAH_8_CANDLES      LANGUAGE2(var_hol_names[2].name)
#define HOLIDAY_CHANUKAH_8TH_DAY        LANGUAGE2(var_hol_names[3].name)
#define HOLIDAY_PURIM   LANGUAGE2(var_hol_names[4].name)
#define HOLIDAY_PURIM_KATAN     LANGUAGE2(var_hol_names[5].name)
#define HOLIDAY_SHABBAT_HACHODESH       LANGUAGE2(var_hol_names[6].name)
#define HOLIDAY_SHABBAT_HAGADOL LANGUAGE2(var_hol_names[7].name)
#define HOLIDAY_SHABBAT_CHAZON   LANGUAGE2(var_hol_names[8].name)
#define HOLIDAY_SHABBAT_NACHAMU LANGUAGE2(var_hol_names[9].name)
#define HOLIDAY_SHABBAT_PARAH   LANGUAGE2(var_hol_names[10].name)
#define HOLIDAY_SHABBAT_SHEKALIM        LANGUAGE2(var_hol_names[11].name)
#define HOLIDAY_SHABBAT_SHUVA   LANGUAGE2(var_hol_names[12].name)
#define HOLIDAY_SHABBAT_ZACHOR  LANGUAGE2(var_hol_names[13].name)
#define HOLIDAY_SHUSHAN_PURIM   LANGUAGE2(var_hol_names[14].name)
#define HOLIDAY_TA_ANIT_BECHOROT        LANGUAGE2(var_hol_names[15].name)
#define HOLIDAY_TA_ANIT_ESTHER  LANGUAGE2(var_hol_names[16].name)
#define HOLIDAY_TISH_A_B_AV     LANGUAGE2(var_hol_names[17].name)
#define HOLIDAY_TZOM_GEDALIAH   LANGUAGE2(var_hol_names[18].name)
#define HOLIDAY_TZOM_TAMMUZ     LANGUAGE2(var_hol_names[19].name)
#define HOLIDAY_YOM_HAATZMA_UT  LANGUAGE2(var_hol_names[20].name)
#define HOLIDAY_YOM_HASHOAH     LANGUAGE2(var_hol_names[21].name)
#define HOLIDAY_YOM_HAZIKARON   LANGUAGE2(var_hol_names[22].name)
#define HOLIDAY_YOM_YERUSHALAYIM   LANGUAGE2(var_hol_names[23].name)
#define HOLIDAY_EREV_PURIM   LANGUAGE2(var_hol_names[24].name)
#define HOLIDAY_EREV_TISH_A_B_AV   LANGUAGE2(var_hol_names[25].name)
#define HOLIDAY_LEIL_SELICHOT      LANGUAGE2(var_hol_names[26].name)
#define HOLIDAY_SIGD      LANGUAGE2(var_hol_names[27].name)

struct variable_holiday_names {
   const char *name;
} var_hol_names[] = {
    {"Asara B'Tevet"},
    {"Chanukah: 7 Candles"},
    {"Chanukah: 8 Candles"},
    {"Chanukah: 8th Day"},
    {"Purim"},
    {"Purim Katan"},
    {"Shabbat HaChodesh"},
    {"Shabbat HaGadol"},
    {"Shabbat Chazon"},
    {"Shabbat Nachamu"},
    {"Shabbat Parah"},
    {"Shabbat Shekalim"},
    {"Shabbat Shuva"},
    {"Shabbat Zachor"},
    {"Shushan Purim"},
    {"Ta'anit Bechorot"},
    {"Ta'anit Esther"},
    {"Tish'a B'Av"},
    {"Tzom Gedaliah"},
    {"Tzom Tammuz"},
    {"Yom HaAtzma'ut"},
    {"Yom HaShoah"},
    {"Yom HaZikaron"},
    {"Yom Yerushalayim"},
    {"Erev Purim"},
    {"Erev Tish'a B'Av"},
    {"Leil Selichot"},
    {"Sigd"},
};

/*-------------------------------------------------------------------------*/

holstorep_t getHolstorep( void )
{                               /* return a pointer to a new holiday */
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
int PushHoliday(
    holstorep_t hp, /* the holiday to be added */
    holstorep_t *lp         /* pointer to the list to be added to. */
    )
{
    holstorep_t temp;

    temp = getHolstorep ();
    initStr (&temp->name, MAX_LINE_LEN);
    strcpy (temp->name, hp->name);
    temp->typeMask = hp->typeMask;

    if ( ! *lp )     /* if there are no holidays here yet, start a new bucket */
        *lp = temp;
    else
    {
        temp->next = *lp;         /* put hp at the head of the list */
        *lp = temp;
    }
    return temp->typeMask;
}

/*-------------------------------------------------------------------------*/

static void load_variable_holidays( int hYear )
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

    tmpholp = getHolstorep ();   /* allocate hsnode */
    tmpholp->name = HOLIDAY_TZOM_GEDALIAH;
    PushHoliday (tmpholp, &var_holidays[TISHREI]
                 [roshHashana % 7L == THU ? 4 : 3]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_SHUVA;
    tempDt = abs2hebrew (day_on_or_before (SAT, 7L + roshHashana));
    PushHoliday (tmpholp, &var_holidays[TISHREI][tempDt.dd]);


    /*    printf( "hyear is %d\n",hYear); */
    if (short_kislev (hYear))
    {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_7_CANDLES;
        tmpholp->typeMask = CHANUKAH_CANDLES;
        PushHoliday (tmpholp, &var_holidays[TEVET][1]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8_CANDLES;
        tmpholp->typeMask = CHANUKAH_CANDLES;
        PushHoliday (tmpholp, &var_holidays[TEVET][2]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8TH_DAY;
        PushHoliday (tmpholp, &var_holidays[TEVET][3]);
    }
    else
    {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_7_CANDLES;
        tmpholp->typeMask = CHANUKAH_CANDLES;
        PushHoliday (tmpholp, &var_holidays[KISLEV][30]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8_CANDLES;
        tmpholp->typeMask = CHANUKAH_CANDLES;
        PushHoliday (tmpholp, &var_holidays[TEVET][1]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8TH_DAY;
        PushHoliday (tmpholp, &var_holidays[TEVET][2]);
    }

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_SHEKALIM;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 43L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_ZACHOR;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 30L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_TA_ANIT_ESTHER;
    tempDt = abs2hebrew (passover - (passover % 7L == TUE ? 33L : 31));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    if (LEAP_YR_HEB (hYear))
    {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_PURIM_KATAN;
        PushHoliday (tmpholp, &var_holidays[ADAR_I][14]);

        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_EREV_PURIM;
        PushHoliday (tmpholp, &var_holidays[ADAR_II][13]);

        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_PURIM;
        PushHoliday (tmpholp, &var_holidays[ADAR_II][14]);
    }
    else
    {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_EREV_PURIM;
        PushHoliday (tmpholp, &var_holidays[ADAR_I][13]);

        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_PURIM;
        PushHoliday (tmpholp, &var_holidays[ADAR_I][14]);
    }

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHUSHAN_PURIM;
    tempDt = abs2hebrew (passover - (passover % 7L == SUN ? 28L : 29));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_PARAH;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 14L) - 7L);
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_HACHODESH;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 14L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_TA_ANIT_BECHOROT;
    if ((passover - 1L) % 7L == SAT)
    {   /* if the fast falls on Shabbat, move to Thursday */
        tempDt = abs2hebrew (day_on_or_before (THU, passover));
        PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
    }
    else
        PushHoliday (tmpholp, &var_holidays[NISAN][14]);


    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_HAGADOL;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 1L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    if (hYear >= 5711)
    {                            /* Yom HaShoah first observed in 1951 */
			long int nisan27;
			int nisan_day = 27;

			tempDt.mm = NISAN;
			tempDt.dd = 27;
			nisan27 = hebrew2abs (tempDt);

			/* When the actual date of Yom Hashoah falls on a Friday, the
			 * state of Israel observes Yom Hashoah on the preceding
			 * Thursday. When it falls on a Sunday, Yom Hashoah is observed
			 * on the following Monday.
			 * http://www.ushmm.org/remembrance/dor/calendar/
			 */
        if (nisan27 % 7L == FRI)
	    		nisan_day = 26;
        else if (nisan27 % 7L == SUN)
	    		nisan_day = 28;

        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HASHOAH;
        PushHoliday (tmpholp, &var_holidays[NISAN][nisan_day]);
    }

    if (hYear > 5708)
    {                            /* only really makes sense after 1948 */
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HAZIKARON;
        if (passover % 7L == SUN)
            tempDt.dd = 3;
        else if (passover % 7L == SAT)
            tempDt.dd = 4;
        else if( hYear < 5764 )
            tempDt.dd = 5;
        else if( passover % 7L == TUE ) /* no Yom Hazikaron on motzei shabbat allowed after 5764*/
            tempDt.dd = 6;
        else
            tempDt.dd = 5;
        PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd - 1]);

        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HAATZMA_UT;
        PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd]);
    }

    if (hYear > 5727)
    {                            /* only really makes sense after 1967 */
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_YERUSHALAYIM;
        PushHoliday (tmpholp, &var_holidays[IYYAR][28]);
    }

    if (hYear >= 5769) {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_SIGD;
        PushHoliday (tmpholp, &var_holidays[CHESHVAN][29]);
    }

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_TZOM_TAMMUZ;
    if (tishaBav % 7L == SAT)
        tempDt = abs2hebrew (tishaBav - 20L);
    else
        tempDt = abs2hebrew (tishaBav - 21L);
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_CHAZON;
    tempDt = abs2hebrew (day_on_or_before (SAT, tishaBav));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_EREV_TISH_A_B_AV;
    PushHoliday (tmpholp, &var_holidays[AV]
                 [tishaBav % 7L == SAT ? 9 : 8]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_TISH_A_B_AV;
    PushHoliday (tmpholp, &var_holidays[AV]
                 [tishaBav % 7L == SAT ? 10 : 9]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_NACHAMU;
    tempDt = abs2hebrew (day_on_or_before (SAT, tishaBav + 7L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);

    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_ASARA_B_TEVET;
    if (tevet10 % 7L == SAT)
        PushHoliday (tmpholp, &var_holidays[TEVET][11]);
    else
        PushHoliday (tmpholp, &var_holidays[TEVET][10]);

    tempDt.mm = TISHREI;
    tempDt.dd = 1;
    tempDt.yy = hYear + 1;
    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_LEIL_SELICHOT;
    tempDt = abs2hebrew (day_on_or_before (SAT, hebrew2abs (tempDt) - 4L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
}


/*-------------------------------------------------------------------------*/
/* this function stores the user "holidays" drawn from inFile in the
   var_holidays array.  The intent is that it will be loaded every new
   Jewish year.  Gross, I know, but an efficient version will have to wait
   until %ID% + .1
   */

void init_user_holidays( int hyear )
{
    holstorep_t tmpholp;

    char *s, *monthStr, *eventStr, nextChar;
    int index, inMonth, inDay, lineNum = 1;

    initStr (&s, MAX_LINE_LEN);
    initStr (&monthStr, MAX_LINE_LEN);
    rewind (inFile);
    nextChar = (char) getc (inFile);     /* priming getc */
    for (; !feof (inFile);
         lineNum++, nextChar = (char) getc (inFile))
    {                            /* force an EOF */
        ungetc (nextChar, inFile);
        if (!fgets (s, MAX_LINE_LEN, inFile))
        {
            warn ("input file read error. Skipping line %s", hc_itoa (lineNum));
            continue;
        }
        if (s[0] == '\n')         /* blank line */
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

        eventStr = s + index + 1; /* get the name of the event */
        if (eventStr[strlen (eventStr) - 1] == '\n')
            eventStr[strlen (eventStr) - 1] = '\0';                /* chop off the \n */

        /* store the holiday in the LUT */
        tmpholp = getHolstorep ();
        initStr (&tmpholp->name, MAX_LINE_LEN);
        strcpy (tmpholp->name, eventStr);         /* load the user holiday into it. */
        tmpholp->typeMask = USER_EVENT;
        PushHoliday (tmpholp, &var_holidays[inMonth][inDay]);

    }
}


/*-------------------------------------------------------------------------*/
/* this function stores yahrtzeits found in yFile.
   each record is stored in mm dd yyyy str format using GREGORIAN dates
   */

void init_yahrtzeits( int hyear )
{
    holstorep_t tmpholp;

    char *s, *monthStr, *eventStr, nextChar;
    int index, inMonth, inDay, inYear, lineNum = 1;
    date_t hDeath, gDeath;

    initStr (&s, MAX_LINE_LEN);
    initStr (&monthStr, MAX_LINE_LEN);
    rewind (yFile);
    nextChar = (char) getc (yFile);      /* priming getc */
    for (; !feof (yFile);
         lineNum++, nextChar = (char) getc (yFile))
    {                            /* force an EOF */
        ungetc (nextChar, yFile);
        if (!fgets (s, MAX_LINE_LEN, yFile))
        {
            warn ("yahrtzeit file read error. Skipping line %s", hc_itoa (lineNum));
            continue;
        }
        if (s[0] == '\n')         /* blank line */
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

        /* advance day to rosh chodesh if needed */
        if (hDeath.mm == CHESHVAN && hDeath.dd == 30
            && !long_cheshvan(hyear)) {
            hDeath.mm = KISLEV;
            hDeath.dd = 1;
        } else if (hDeath.mm == KISLEV && hDeath.dd == 30
                   && short_kislev(hyear)) {
            hDeath.mm = TEVET;
            hDeath.dd = 1;
        }

        eventStr = s + index + 1; /* get the name of the event */
        /*    if (eventStr[strlen(eventStr)-1] == '\n') */
        eventStr[strlen (eventStr) - 1] = '\0';   /* chop off the \n */

        /* store the holiday in the LUT */
        tmpholp = getHolstorep ();
        initStr (&tmpholp->name, MAX_LINE_LEN);
        strcpy (tmpholp->name, eventStr);         /* load the user holiday into it. */
        tmpholp->typeMask = USER_EVENT;
        PushHoliday (tmpholp, &var_holidays[hDeath.mm][hDeath.dd]);

    }
}



/*-------------------------------------------------------------------------*/
/** set up the stored holidays array. */
void init_holidays( int hYear )
/* NB that this is dependant on the hYear. */
/* It must therefore be called at the beginning of every hebrew year. */
{

    int d, m;
    holstorep_t tmpholp, tmpholp2;
    holinputp_t todayinp;
    static int first = 1;

    if (first)
    {
        for (m = 0, todayinp = inp_holidays;      /* load constant holidays */
             m < LAST_INDEX (inp_holidays);
             m++, todayinp++)
        {
	    if (!(todayinp->typeMask & (IL_ONLY|CHUL_ONLY))
                || ((todayinp->typeMask & IL_ONLY) && israel_sw)
                || ((todayinp->typeMask & CHUL_ONLY) && !israel_sw)) {
		tmpholp = getHolstorep ();	/* allocate hsnode */
		tmpholp->typeMask = todayinp->typeMask;	/*load the new holiday */
		tmpholp->name = LANGUAGE2(todayinp->name);
		PushHoliday (tmpholp, &holidays[todayinp->date.mm][todayinp->date.dd]);
	    }
        }
        first = 0;
    }
    else
    {
        for (m = 0; m <= 13; m++) /* clear variable holidays buckets */
	{
            for (d = 0; d <= 30; d++)
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
    }

    load_variable_holidays (hYear);

    if (inputFile_sw)            /* get user-defined "holidays" */
        init_user_holidays (hYear);
    if (yahrtzeitFile_sw)                /* get yahrtzeit dates */
        init_yahrtzeits (hYear);
}



/*-------------------------------------------------------------------------*/
void freeHolidays( holstorep_t *holiList )
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

/* sets holiList to a linked list of holidadys for that date. */
/* the return value is the | of all the typeMasks of the holidays */
int getHebHolidays( date_t dth, holstorep_t *holiList )
{
    int tmpMask;
    holstorep_t tmpholip, chp;   /* current holiday pointer */

    tmpMask = 0;
    *holiList = NULL;

    for (chp = holidays[dth.mm][dth.dd]; /* static holidays */
         chp;
         chp = chp->next)
    {
        tmpMask |= PushHoliday (chp, holiList);
    }
    for (chp = var_holidays[dth.mm][dth.dd];     /* variable holidays */
         chp;
         chp = chp->next)
    {
        tmpMask |= PushHoliday (chp, holiList);
    }

    if (dth.dd == 1)
    {

        if(dth.mm == TISHREI)     /* special processing for rosh hashana */
        {
            tmpholip = getHolstorep ();
            initStr (&tmpholip->name, 22);
            sprintf (tmpholip->name, "%s %d",
                     _("Rosh Hashana"),
                     dth.yy);
            PushHoliday (tmpholip, &var_holidays[TISHREI][1]);
            tmpMask |= PushHoliday (tmpholip, holiList);
        }
        else
        {
            if( ! suppress_rosh_chodesh_sw )      /* rosh Chodesh Processing... */
            {
                tmpholip = getHolstorep();
                initStr (&tmpholip->name, NM_LEN);
                sprintf (tmpholip->name, _("Rosh Chodesh %s"),
                         LANGUAGE2(hMonths[LEAP_YR_HEB (dth.yy)][dth.mm].name)); 
                tmpMask |= PushHoliday (tmpholip, holiList);
            }
        }
    }


    if (dth.dd == 30 && ! suppress_rosh_chodesh_sw)
    {
        tmpholip = getHolstorep ();
        initStr (&tmpholip->name, NM_LEN);
        sprintf (tmpholip->name, _("Rosh Chodesh %s"),
                 LANGUAGE2(hMonths[LEAP_YR_HEB (dth.yy)][dth.mm + 1].name)); 
        tmpMask |= PushHoliday (tmpholip, holiList);
    }


    return tmpMask;
}
