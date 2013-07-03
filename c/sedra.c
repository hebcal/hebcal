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

/*
 * Many of the following algorithms were taken from hebrew calendar
 * routines by Maimonedes, from his Mishneh Torah, and implemented by
 *  Nachum Dershowitz                Department of Computer Science
 *  (217) 333-4219                   University of Illinois at Urbana-Champaign
 *  nachum@cs.uiuc.edu               1304 West Springfield Avenue
 *                                   Urbana, Illinois 61801
 *
 * The routines were included in the emacs 19 distribution.
 *
 */

#include <string.h>

#include "hebcal.h"
#include "sedra.h"
#include "common.h"

const char *sedrot[][3] =
{
    {"Bereshit",        "Bereshis",     "\341\370\340\371\351\372"},
    {"Noach",   "Noach",        "\360\347"},
    {"Lech-Lecha",      "Lech-Lecha",   "\354\352\354\352"},
    {"Vayera",  "Vayera",       "\345\351\370\340"},
    {"Chayei Sara",     "Chayei Sara",  "\347\351\351 \371\370\344"},
    {"Toldot",  "Toldos",       "\372\345\354\343\345\372"},
    {"Vayetzei",        "Vayetzei",     "\345\351\366\340"},
    {"Vayishlach",      "Vayishlach",   "\345\351\371\354\347"},
    {"Vayeshev",        "Vayeshev",     "\345\351\371\341"},
    {"Miketz",  "Miketz",       "\356\367\365"},
    {"Vayigash",        "Vayigash",     "\345\351\342\371"},
    {"Vayechi", "Vayechi",      "\345\351\347\351"},
    {"Shemot",  "Shemos",       "\371\356\345\372"},
    {"Vaera",   "Vaera",        "\345\340\370\340"},
    {"Bo",      "Bo",   "\341\340"},
    {"Beshalach",       "Beshalach",    "\341\371\354\347"},
    {"Yitro",   "Yisro",        "\351\372\370\345"},
    {"Mishpatim",       "Mishpatim",    "\356\371\364\350\351\355"},
    {"Terumah", "Terumah",      "\372\370\345\356\344"},
    {"Tetzaveh",        "Tetzaveh",     "\372\366\345\344"},
    {"Ki Tisa", "Ki Sisa",      "\353\351 \372\371\340"},
    {"Vayakhel",        "Vayakhel",     "\345\351\367\344\354"},
    {"Pekudei", "Pekudei",      "\364\367\345\343\351"},
    {"Vayikra", "Vayikra",      "\345\351\367\370\340"},
    {"Tzav",    "Tzav", "\366\345"},
    {"Shmini",  "Shmini",       "\371\356\351\360\351"},
    {"Tazria",  "Sazria",       "\372\346\370\351\362"},
    {"Metzora", "Metzora",      "\356\366\370\362"},
    {"Achrei Mot",      "Achrei Mos",   "\340\347\370\351 \356\345\372"},
    {"Kedoshim",        "Kedoshim",     "\367\343\371\351\355"},
    {"Emor",    "Emor", "\340\356\345\370"},
    {"Behar",   "Behar",        "\341\344\370"},
    {"Bechukotai",      "Bechukosai",   "\341\347\367\372\351"},
    {"Bamidbar",        "Bamidbar",     "\341\356\343\341\370"},
    {"Nasso",   "Nasso",        "\360\371\340"},
    {"Beha'alotcha",    "Beha'aloscha", "\341\344\362\354\372\352"},
    {"Sh'lach", "Sh'lach",      "\371\354\347\354\352"},
    {"Korach",  "Korach",       "\367\345\370\347"},
    {"Chukat",  "Chukas",       "\347\367\372"},
    {"Balak",   "Balak",        "\341\354\367"},
    {"Pinchas", "Pinchas",      "\364\351\360\347\361"},
    {"Matot",   "Matos",        "\356\350\345\372"},
    {"Masei",   "Masei",        "\356\361\362\351"},
    {"Devarim", "Devarim",      "\343\341\370\351\355"},
    {"Vaetchanan",      "Vaeschanan",   "\345\340\372\347\360\357"},
    {"Eikev",   "Eikev",        "\362\367\341"},
    {"Re'eh",   "Re'eh",        "\370\340\344"},
    {"Shoftim", "Shoftim",      "\371\345\364\350\351\355"},
    {"Ki Teitzei",      "Ki Seitzei",   "\353\351\372\366\340"},
    {"Ki Tavo", "Ki Savo",      "\353\351\372\341\345\340"},
    {"Nitzavim",        "Nitzavim",     "\360\366\341\351\355"},
    {"Vayeilech",       "Vayeilech",    "\345\351\354\352"},
    {"Ha'Azinu",        "Ha'Azinu",     "\344\340\346\351\360\345"},
};


#define INCOMPLETE 0
#define REGULAR 1
#define COMPLETE 2

/* parsha doubler */
#define D(p) (-(p))

/* parsha undoubler */
#define U(p) (-(p))

typedef int sedraIndex_t[55];

/* The ordinary year types (keviot) */

sedraIndex_t nonleap_monday_incomplete =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31), 33, 34,
 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)};
/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Tuesday. */

sedraIndex_t nonleap_monday_complete_diaspora =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31), 33,
 -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)};
/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */

#define nonleap_monday_complete_israel nonleap_monday_incomplete
/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */

#define nonleap_tuesday_regular_diaspora nonleap_monday_complete_diaspora
/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Thursday. */

#define nonleap_tuesday_regular_israel nonleap_monday_incomplete
/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Thursday. */

sedraIndex_t nonleap_thursday_regular_diaspora =
{52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, -1, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */

sedraIndex_t nonleap_thursday_regular_israel =
{52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Thursday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */

sedraIndex_t nonleap_thursday_complete =
{52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, -1, 25, D (26), D (28), 30, D (31), 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Sunday. */
sedraIndex_t nonleap_saturday_incomplete =
{-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
 15, 16, 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30, D (31),
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 50};

/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and Kislev
   each have 29 days), and has Passover start on Sunday. */

sedraIndex_t nonleap_saturday_complete =
{-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, D (21), 23, 24, -1, 25, D (26), D (28), 30,
 D (31), 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47,
 48, 49, D (50)};
/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Tuesday. */

/* --  The leap year types (keviot) -- */

sedraIndex_t leap_monday_incomplete_diaspora =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)};
/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Thursday. */

sedraIndex_t leap_monday_incomplete_israel =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, D (50)};
/* Hebrew year that starts on Monday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Thursday. */


sedraIndex_t leap_monday_complete_diaspora =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Saturday. */

sedraIndex_t leap_monday_complete_israel =
{51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Monday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Saturday. */

#define leap_tuesday_regular_diaspora leap_monday_complete_diaspora
/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */

#define leap_tuesday_regular_israel leap_monday_complete_israel
/* Hebrew year that starts on Tuesday, is `regular' (Heshvan has 29
   days and Kislev has 30 days), and has Passover start on Saturday. */

sedraIndex_t leap_thursday_incomplete =
{52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
/* Hebrew year that starts on Thursday, is `incomplete' (Heshvan and
   Kislev both have 29 days), and has Passover start on Sunday. */

sedraIndex_t leap_thursday_complete =
{52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33,
 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, D (50)};
/* Hebrew year that starts on Thursday, is `complete' (Heshvan and
   Kislev both have 30 days), and has Passover start on Tuesday. */

sedraIndex_t leap_saturday_incomplete =
{-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32,
 33, 34, 35, 36, 37, 38, 39, 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)};
/* Hebrew year that starts on Saturday, is `incomplete' (Heshvan and
   Kislev each have 29 days), and has Passover start on Tuesday. */

sedraIndex_t leap_saturday_complete_diaspora =
{-1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32,
 33, -1, 34, 35, 36, 37, D (38), 40, D (41), 43, 44, 45, 46, 47, 48, 49,
 D (50)};
/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */

#define leap_saturday_complete_israel leap_saturday_incomplete
/* Hebrew year that starts on Saturday, is `complete' (Heshvan and
   Kislev each have 30 days), and has Passover start on Thursday. */



#define ROSH_DAY_INDEX(x) ( ((x)==1) ? 0 : ((x)/2) )

/* sedra_years_array[leap][rosh_day][type][israel/diaspora] */
int *sedra_years_array[2][4][3][2] =
{
    {                             /* nonleap years */
        
        {                           /* monday */
            {                         /* incomplete */
                nonleap_monday_incomplete,
                nonleap_monday_incomplete
            },
            
            {                         /* regular */
                NULL, NULL
            },
            
            {                         /* complete */
                nonleap_monday_complete_diaspora,
                nonleap_monday_complete_israel
            },
            
        },
        
        {                           /* tuesday */
            {                         /* incomplete */
                NULL, NULL
            },
            
            {                         /* regular */
                nonleap_tuesday_regular_diaspora,
                nonleap_tuesday_regular_israel
                
            },
            
            {                         /* complete */
                NULL, NULL
            },
        },
        
        {                           /* thursday */
            {                         /* incomplete */
                NULL, NULL
            },
            
            {                         /* regular */
                nonleap_thursday_regular_diaspora,
                nonleap_thursday_regular_israel
            },
            
            {                         /* complete */
                nonleap_thursday_complete,
                nonleap_thursday_complete
            },
        },
        
        {                           /* saturday */
            {                         /* incomplete */
                nonleap_saturday_incomplete,
                nonleap_saturday_incomplete
            },
            
            {                         /* regular */
                NULL, NULL
            },
            
            {                         /* complete */
                nonleap_saturday_complete, nonleap_saturday_complete
            },
        },
    },
    
    
    {                             /* leap years */
        {                           /* monday */
            {                         /* incomplete */
                leap_monday_incomplete_diaspora,
                leap_monday_incomplete_israel
            },
            
            {                         /* regular */
                NULL, NULL
            },
            
            {                         /* complete */
                leap_monday_complete_diaspora,
                leap_monday_complete_israel
            },
        },
        
        {                           /* tuesday */
            {                         /* incomplete */
        NULL, NULL
            },
            
            {                         /* regular */
                leap_tuesday_regular_diaspora,
                leap_tuesday_regular_israel,
            },
            
            {                         /* complete */
                NULL, NULL
            },
        },
        
    {                           /* thursday */
        {                         /* incomplete */
            leap_thursday_incomplete,
            leap_thursday_incomplete
        },
        
        {                         /* regular */
            NULL, NULL
        },
        
        {                         /* complete */
            leap_thursday_complete,
            leap_thursday_complete
        },
    },
        
        {                           /* saturday */
            {                         /* incomplete */
                leap_saturday_incomplete,
                leap_saturday_incomplete
            },
            
            {                         /* regular */
                NULL, NULL
            },
            
            {                         /* complete */
                leap_saturday_complete_diaspora,
                leap_saturday_complete_israel
            },
        },
    }
};

static int *theSedraArray;
static long int first_saturday;

/* sets static globals based on this year. */
void reset_sedra( int hebYr ) /* the hebrew year */
{
    date_t tempDt;
    int long_c, short_k, rosh_hashana_day, type;
    long int rosh_hashana;
    
    long_c = long_cheshvan (hebYr);
    short_k = short_kislev (hebYr);
    
    if (long_c && !short_k)
        type = COMPLETE;
    else if (!long_c && short_k)
        type = INCOMPLETE;
    else
        type = REGULAR;
    
    tempDt.dd = 1;
    tempDt.mm = TISHREI;
    tempDt.yy = hebYr;
    rosh_hashana = hebrew2abs (tempDt);
    rosh_hashana_day = (int) (rosh_hashana % 7L);

    /* find the first saturday on or after Rosh Hashana */
    first_saturday = day_on_or_before (6, rosh_hashana + 6L);
    
    if (NULL == (theSedraArray = sedra_years_array
                 [LEAP_YR_HEB (hebYr)]
                 [ROSH_DAY_INDEX (rosh_hashana_day)]
                 [type]
                 [israel_sw]))
        die ("improper sedra year type calculated.", "");
    
}


/* returns a string "Parshat <parsha>" based on the current parsha number */
/* NOTE-- the returned value lives in a static buffer. */
char * sedra( long absDate )
{
    
    int index;
    static char buf[40];        /* FIX: eeeevill */
    
    /* find the first saturday on or after today's date */
    absDate = day_on_or_before (6, absDate + 6L);
    
    index = theSedraArray[(absDate - first_saturday) / 7];
    
    *buf = '\0';                        /* reset the return buffer */
    
    if (index >= 0)
        strncpy (buf, LANGUAGE2(sedrot[index]), 40);
    else if (-1 == index)
        return NULL;
    else
    {
        int i = U (index);      /* undouble the parsha */
        sprintf (buf, "%s-%s",
                 LANGUAGE2(sedrot[i]),
                 LANGUAGE2(sedrot[i + 1]));
    }
    return buf;
}
