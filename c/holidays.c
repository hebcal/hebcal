/*
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994  Danny Sadinoff
   Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

   http://sourceforge.net/projects/hebcal

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
#include <stdlib.h>

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
      {"Rosh Hashana II", NULL, "\370\340\371 \344\371\360\344 \351\345\355 \341'"},
      YOM_TOV_ENDS},
   {
      {TISHREI, 9},
      {"Erev Yom Kippur", NULL, "\362\370\341 \351\345\355 \353\364\345\370"},
      LIGHT_CANDLES},
   {
      {TISHREI, 10},
      {"Yom Kippur", NULL, "\351\345\355 \353\364\345\370"},
      YOM_TOV_ENDS},

   {
      {TISHREI, 14},
      {"Erev Sukkot", "Erev Sukkos", "\362\370\341 \361\345\353\345\372"},
      LIGHT_CANDLES},
   {
      {TISHREI, 15},
      {"Sukkot I", "Sukkos I", "\361\345\353\345\372 \351\345\355 \340'"},
      0},
   {
      {TISHREI, 16},
      {"Sukkot II", "Sukkos II", "\361\345\353\345\372 \351\345\355 \341'"},
      YOM_TOV_ENDS},
   {
      {TISHREI, 17},
      {"Sukkot III (CH''M)", "Sukkos III (CH''M)", "\361\345\353\345\372 \351\345\355 \342' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {TISHREI, 18},
      {"Sukkot IV (CH''M)", "Sukkos IV (CH''M)", "\361\345\353\345\372 \351\345\355 \343' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {TISHREI, 19},
      {"Sukkot V (CH''M)", "Sukkos V (CH''M)", "\361\345\353\345\372 \351\345\355 \344' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {TISHREI, 20},
      {"Sukkot VI (CH''M)", "Sukkos VI (CH''M)", "\361\345\353\345\372 \351\345\355 \345' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {TISHREI, 21},
      {"Sukkot VII (Hoshana Raba)", "Sukkos VII (Hoshana Raba)", "\361\345\353\345\372 \351\345\355 \346' (\344\345\371\360\340 \370\341\344)"},
      LIGHT_CANDLES},
   {
      {TISHREI, 22},
      {"Shmini Atzeret", "Shmini Atzeres", "\371\356\351\360\351 \362\366\370\372"},
      0},
   {
      {TISHREI, 23},
      {"Simchat Torah", "Simchas Torah", "\371\356\347\372 \372\345\370\344"},
      YOM_TOV_ENDS},

   {
      {KISLEV, 24},
      {"Chanukah: 1 Candle", NULL, "\347\360\345\353\344: \340' \360\370"},
      0},
   {
      {KISLEV, 25},
      {"Chanukah: 2 Candles", NULL, "\347\360\345\353\344: \341' \360\370\345\372"},
      0},
   {
      {KISLEV, 26},
      {"Chanukah: 3 Candles", NULL, "\347\360\345\353\344: \342' \360\370\345\372"},
      0},
   {
      {KISLEV, 27},
      {"Chanukah: 4 Candles", NULL, "\347\360\345\353\344: \343' \360\370\345\372"},
      0},
   {
      {KISLEV, 28},
      {"Chanukah: 5 Candles", NULL, "\347\360\345\353\344: \344' \360\370\345\372"},
      0},
   {
      {KISLEV, 29},
      {"Chanukah: 6 Candles", NULL, "\347\360\345\353\344: \345' \360\370\345\372"},
      0},
   {
      {SHVAT, 15},
      {"Tu B'Shvat", NULL, "\350\"\345 \341\371\341\350"},
      0},
   {
      {NISAN, 14},
      {"Erev Pesach", NULL, "\362\370\341 \364\361\347"},
      LIGHT_CANDLES},
   {
      {NISAN, 15},
      {"Pesach I", NULL, "\364\361\347 \351\345\355 \340'"},
      0},
   {
      {NISAN, 16},
      {"Pesach II", NULL, "\364\361\347 \351\345\355 \341'"},
      YOM_TOV_ENDS},
   {
      {NISAN, 17},
      {"Pesach III (CH''M)", NULL, "\364\361\347 \351\345\355 \342' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {NISAN, 18},
      {"Pesach IV (CH''M)", NULL, "\364\361\347 \351\345\355 \343' (\347\354 \344\356\345\362\343)"},
      0},

   {
      {NISAN, 19},
      {"Pesach V (CH''M)", NULL, "\364\361\347 \351\345\355 \344' (\347\354 \344\356\345\362\343)"},
      0},
   {
      {NISAN, 20},
      {"Pesach VI (CH''M)", NULL, "\364\361\347 \351\345\355 \345' (\347\354 \344\356\345\362\343)"},
      LIGHT_CANDLES},
   {
      {NISAN, 21},
      {"Pesach VII", NULL, "\364\361\347 \351\345\355 \346'"},
      0},
   {
      {NISAN, 22},
      {"Pesach VIII", NULL, "\364\361\347 \351\345\355 \347'"},
      YOM_TOV_ENDS},

   {
      {IYYAR, 18},
      {"Lag B'Omer", NULL, "\354\"\342 \341\362\345\356\370"},
      0},
   {
      {IYYAR, 28},
      {"Yom Yerushalayim", NULL, "\351\345\355 \351\370\345\371\354\351\355"},
      0},

   {
      {SIVAN, 5},
      {"Erev Shavuot", "Erev Shavuos", "\362\370\341 \371\341\345\362\345\372"},
      LIGHT_CANDLES},
   {
      {SIVAN, 6},
      {"Shavuot I", "Shavuos I", "\371\341\345\362\345\372 \351\345\355 \340'"},
      0},
   {
      {SIVAN, 7},
      {"Shavuot II", "Shavuos II", "\371\341\345\362\345\372 \351\345\355 \341'"},
      YOM_TOV_ENDS},

   {
      {ELUL, 29},
      {"Erev Rosh Hashana", NULL, "\362\370\341 \370\340\371 \344\371\360\344"},
      LIGHT_CANDLES}
};

#define HOLIDAY_ASARA_B_TEVET   LANGUAGE2(var_hol_names[0].name)
#define HOLIDAY_CHANUKAH_7_CANDLES      LANGUAGE2(var_hol_names[1].name)
#define HOLIDAY_CHANUKAH_8_CANDLES      LANGUAGE2(var_hol_names[2].name)
#define HOLIDAY_CHANUKAH_8TH_DAY        LANGUAGE2(var_hol_names[3].name)
#define HOLIDAY_PURIM   LANGUAGE2(var_hol_names[4].name)
#define HOLIDAY_PURIM_KATAN     LANGUAGE2(var_hol_names[5].name)
#define HOLIDAY_SHABBAT_HACHODESH       LANGUAGE2(var_hol_names[6].name)
#define HOLIDAY_SHABBAT_HAGADOL LANGUAGE2(var_hol_names[7].name)
#define HOLIDAY_SHABBAT_HAZON   LANGUAGE2(var_hol_names[8].name)
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

struct variable_holiday_names {
   char *(name[3]);
} var_hol_names[] = {
  {{"Asara B'Tevet", NULL, "\362\371\370\344 \341\350\341\372"}},
  {{"Chanukah: 7 Candles", NULL, "\347\360\345\353\344: \346' \360\370\345\372"}},
  {{"Chanukah: 8 Candles", NULL, "\347\360\345\353\344: \347' \360\370\345\372"}},
  {{"Chanukah: 8th Day", NULL, "\347\360\345\353\344: \351\345\355 \347'"}},
  {{"Purim", NULL, "\364\345\370\351\355"}},
  {{"Purim Katan", "Purim Koson", "\364\361\347 \367\350\357"}},
  {{"Shabbat HaChodesh", "Shabbas HaChodesh", "\371\341\372 \344\347\343\371"}},
  {{"Shabbat HaGadol", "Shabbas HaGadol", "\371\341\372 \344\342\343\345\354"}},
  {{"Shabbat Hazon", "Shabbas Hazon", "\371\341\372 \344\346\345\357"}},
  {{"Shabbat Nachamu", "Shabbas Nachamu", "\371\341\372 \360\347\356\345"}},
  {{"Shabbat Parah", "Shabbas Parah", "\371\341\372 \364\370\344"}},
  {{"Shabbat Shekalim", "Shabbas Shekalim", "\371\341\372 \371\367\354\351\355"}},
  {{"Shabbat Shuva", "Shabbas Shuvah", "\371\341\372 \371\345\341\344"}},
  {{"Shabbat Zachor", "Shabbas Zachor", "\371\341\372 \346\353\345\370"}},
  {{"Shushan Purim", NULL, "\371\345\371\357 \364\345\370\351\355"}},
  {{"Ta'anit Bechorot", "Ta'anis Bechoros", "\372\362\360\351\372 \341\353\345\370\345\372"}},
  {{"Ta'anit Esther", "Ta'anis Esther", "\372\362\360\351\372 \340\361\372\370"}},
  {{"Tish'a B'Av", NULL, "\372\371\362\344 \341\340\341"}},
  {{"Tzom Gedaliah", NULL, "\366\345\355 \342\343\354\351\344"}},
  {{"Tzom Tammuz", NULL, "\366\345\355 \372\356\345\346"}},
  {{"Yom HaAtzma'ut", NULL, "\351\345\355 \344\362\366\356\340\345\372"}},
  {{"Yom HaShoah", NULL, "\351\345\355 \344\371\345\340\344"}},
  {{"Yom HaZikaron", NULL, "\351\345\355 \344\346\353\370\345\357"}},
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
        PushHoliday (tmpholp, &var_holidays[TEVET][1]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8_CANDLES;
        PushHoliday (tmpholp, &var_holidays[TEVET][2]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8TH_DAY;
        PushHoliday (tmpholp, &var_holidays[TEVET][3]);
    }
    else
    {
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_7_CANDLES;
        PushHoliday (tmpholp, &var_holidays[KISLEV][30]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_CHANUKAH_8_CANDLES;
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
        tmpholp->name = HOLIDAY_PURIM;
        PushHoliday (tmpholp, &var_holidays[ADAR_II][14]);
    }
    else
    {
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
    if ((passover - 1L) % 7L >= 5)
    {                            /* saturday or sunday */
        tempDt = abs2hebrew (day_on_or_before (THU, passover));
        PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
    }
    else
        PushHoliday (tmpholp, &var_holidays[NISAN][14]);

    
    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_HAGADOL;
    tempDt = abs2hebrew (day_on_or_before (SAT, passover - 1L));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
    
    if (hYear > 5698)
    {                            /* these only really make sense after 1938 */
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HASHOAH;
        PushHoliday (tmpholp, &var_holidays[NISAN][27]);
        
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HAATZMA_UT;
        if (passover % 7L == SUN)
            tempDt.dd = 3;
        else if (passover % 7L == SAT)
            tempDt.dd = 4;
        else
            tempDt.dd = 5;
        PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd]);
        tmpholp = getHolstorep ();
        tmpholp->name = HOLIDAY_YOM_HAZIKARON;
        PushHoliday (tmpholp, &var_holidays[IYYAR][tempDt.dd - 1]);
    }
    
    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_TZOM_TAMMUZ;
    if (tishaBav % 7L == SAT)
        tempDt = abs2hebrew (tishaBav - 20L);
    else
        tempDt = abs2hebrew (tishaBav - 21L);
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
    
    tmpholp = getHolstorep ();
    tmpholp->name = HOLIDAY_SHABBAT_HAZON;
    tempDt = abs2hebrew (day_on_or_before (SAT, tishaBav));
    PushHoliday (tmpholp, &var_holidays[tempDt.mm][tempDt.dd]);
    
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
    if (tevet10 % 7L >= 5)
    {                            /* saturday or sunday */
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
            tmpholp = getHolstorep ();     /* allocate hsnode */
            tmpholp->typeMask = todayinp->typeMask;        /*load the new holiday */
            tmpholp->name = LANGUAGE2 (todayinp->name);
            PushHoliday (tmpholp, &holidays[todayinp->date.mm][todayinp->date.dd]);
        }
        first = 0;
    }
    else
    {
        for (m = 0; m <= 13; m++) /* clear variable holidays buckets */
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
                     iso8859_8_sw ? "\370\340\371 \344\371\360\344" :
                     "Rosh Hashana",
                     dth.yy);
            PushHoliday (tmpholip, &var_holidays[TISHREI][1]);
            tmpMask |= PushHoliday (tmpholip, holiList);
        }
        else 
            if( ! suppress_rosh_chodesh_sw )      /* rosh Chodesh Processing... */
            {
                tmpholip = getHolstorep ();
                initStr (&tmpholip->name, NM_LEN);
                strcat (tmpholip->name, 
                    iso8859_8_sw ? "\370\340\371 \347\343\371 " :
                        "Rosh Chodesh ");
                if (!tabs_sw)
                {
                    strncat (tmpholip->name,
                             LANGUAGE2(hMonths[LEAP_YR_HEB (dth.yy)][dth.mm].name),
                             NM_LEN);
                }
                else
                {
                    0;                       /* put your special output format here */
                } /* don't name the rosh chodesh if -y switch */
                tmpMask |= PushHoliday (tmpholip, holiList);
            }
    }
    
    
    if (dth.dd == 30 && ! suppress_rosh_chodesh_sw)
    {
        tmpholip = getHolstorep ();
        initStr (&tmpholip->name, NM_LEN);
        strcat (tmpholip->name, 
                iso8859_8_sw ? "\370\340\371 \347\343\371 " :
                "Rosh Chodesh ");
        if (!tabs_sw)
        {
            strncat (tmpholip->name,
                     LANGUAGE2(hMonths[LEAP_YR_HEB (dth.yy)][dth.mm + 1].name),
                     NM_LEN);
        }
        else
        {
            0;                     /* put your special output format here. */
        } /* don't name the rosh chodesh if -y switch */
        tmpMask |= PushHoliday (tmpholip, holiList);
    }
    

    return tmpMask;
}
