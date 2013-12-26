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


#include "greg.h"
#include "hebcal.h"
#include "sedra.h"
#include "common.h"
#include "mystring.h"

static const char *sedrot[][3] =
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
    {"Acharei Mot",      "Acharei Mos",   "\340\347\370\351 \356\345\372"},
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

/*
 * These indices were originally included in the emacs 19 distribution.
 * These arrays determine the correct indices into the parsha names
 * -1 means no parsha that week.
 */
static int Sat_short[] = {
    -1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, D(21), 23, 24, -1, 25, D(26), D(28), 30, D(31), 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45, 46, 47,
    48, 49, 50 };

static int Sat_long[] = {
    -1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, D(21), 23, 24, -1, 25, D(26), D(28), 30, D(31), 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45, 46, 47,
    48, 49, D(50) };

static int Mon_short[] = { 
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, D(21), 23, 24, -1, 25, D(26), D(28), 30, D(31), 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45, 46, 47, 48,
    49, D(50) };

static int Mon_long[] = {
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, D(21), 23, 24, -1, 25, D(26), D(28),
    30, D(31), 33, -1, 34, 35, 36, 37, D(38), 40, D(41), 43, 44, 45, 46, 47, 48, 49, D(50) };

static int Thu_normal[] = {
    52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, D(21), 23, 24, -1, -1, 25, D(26), D(28), 30, D(31), 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45, 46, 47,
    48, 49, 50 };
static int Thu_normal_Israel[] = {
    52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, D(21), 23, 24, -1, 25, D(26), D(28), 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45,
    46, 47, 48, 49, 50 };

static int Thu_long[] = {
    52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
    18, 19, 20, 21, 22, 23, 24, -1, 25, D(26), D(28), 30, D(31), 33, 34, 35, 36, 37, 38, 39, 40, D(41), 43, 44, 45, 46, 47,
    48, 49, 50 };

static int Sat_short_leap[] = {
    -1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, D(41),
    43, 44, 45, 46, 47, 48, 49, D(50) };

static int Sat_long_leap[] = {
    -1, 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
    16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, D(38), 40, D(41),
    43, 44, 45, 46, 47, 48, 49, D(50) };

static int Mon_short_leap[] = {
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, D(38), 40, D(41), 43,
    44, 45, 46, 47, 48, 49, D(50) };

static int Mon_short_leap_Israel[] = {
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    D(41), 43, 44, 45, 46, 47, 48, 49, D(50) };

static int Mon_long_leap[] = {
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, D(41),
    43, 44, 45, 46, 47, 48, 49, 50 };
static int Mon_long_leap_Israel[] = {
    51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

static int Thu_short_leap[] = {
    52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50 };

static int Thu_long_leap[] = {
    52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, D(50) };

static int *theSedraArray;
static size_t sedraNumWeeks;
static long int first_saturday;

/* sets static globals based on this year. */
void reset_sedra( int hebYr ) /* the hebrew year */
{
    date_t tempDt;
    int long_c, short_k, rosh_hashana_day, type;
    long int rosh_hashana;
    size_t theSedraArraySize;
    
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

    if (!LEAP_YR_HEB(hebYr)) {
	switch (rosh_hashana_day) {
	case SAT:
	    if (type == INCOMPLETE) {
		theSedraArray = Sat_short;
		theSedraArraySize = sizeof(Sat_short);
	    } else if (type == COMPLETE) {
		theSedraArray = Sat_long;
		theSedraArraySize = sizeof(Sat_long);
	    }
	    break;
	case MON:
	    if (type == INCOMPLETE) {
		theSedraArray = Mon_short;
		theSedraArraySize = sizeof(Mon_short);
	    } else if (type == COMPLETE) {
		theSedraArray = israel_sw ? Mon_short : Mon_long;
		theSedraArraySize = israel_sw ? sizeof(Mon_short) : sizeof(Mon_long);
	    }
	    break;
	case TUE:
	    if (type == REGULAR) {
		theSedraArray = israel_sw ? Mon_short : Mon_long;
		theSedraArraySize = israel_sw ? sizeof(Mon_short) : sizeof(Mon_long);
	    }
	    break;
	case THU:
	    if (type == REGULAR) {
		theSedraArray = israel_sw ? Thu_normal_Israel : Thu_normal;
		theSedraArraySize = israel_sw ? sizeof(Thu_normal_Israel) : sizeof(Thu_normal);
	    } else if (type == COMPLETE) {
		theSedraArray = Thu_long;
		theSedraArraySize = sizeof(Thu_long);
	    }
	    break;
	default:
	    die ("improper sedra year type calculated.", "");
	}
    } else {
	/* leap year */
	switch (rosh_hashana_day) {
	case SAT:
	    if (type == INCOMPLETE) {
		theSedraArray = Sat_short_leap;
		theSedraArraySize = sizeof(Sat_short_leap);
	    } else if (type == COMPLETE) {
		theSedraArray = israel_sw ? Sat_short_leap : Sat_long_leap;
		theSedraArraySize = israel_sw ? sizeof(Sat_short_leap) : sizeof(Sat_long_leap);
	    }
	    break;
	case MON:
	    if (type == INCOMPLETE) {
		theSedraArray = israel_sw ? Mon_short_leap_Israel : Mon_short_leap;
		theSedraArraySize = israel_sw ? sizeof(Mon_short_leap_Israel) : sizeof(Mon_short_leap);
	    } else if (type == COMPLETE) {
		theSedraArray = israel_sw ? Mon_long_leap_Israel : Mon_long_leap;
		theSedraArraySize = israel_sw ? sizeof(Mon_long_leap_Israel) : sizeof(Mon_long_leap);
	    }
	    break;
	case TUE:
	    if (type == REGULAR) {
		theSedraArray = israel_sw ? Mon_long_leap_Israel : Mon_long_leap;
		theSedraArraySize = israel_sw ? sizeof(Mon_long_leap_Israel) : sizeof(Mon_long_leap);
	    }
	    break;
	case THU:
	    if (type == INCOMPLETE) {
		theSedraArray = Thu_short_leap;
		theSedraArraySize = sizeof(Thu_short_leap);
	    } else if (type == COMPLETE) {
		theSedraArray = Thu_long_leap;
		theSedraArraySize = sizeof(Thu_long_leap);
	    }
	    break;
	default:
	    die ("improper sedra year type calculated.", "");
	}
    }
    
    sedraNumWeeks = theSedraArraySize / sizeof(int);
}


/* Fills input "buf" of size buf_len with a string "Parshat <parsha>"
   based on the current parsha number, and returns 1; or returns 0 if
   it can't find the parsha for some reason.
 */
int sedra( long absDate, char *buf, int buf_len )
{
    
    int index;
    int weekNum;
    
    /* find the first saturday on or after today's date */
    absDate = day_on_or_before (6, absDate + 6L);
    
    weekNum = (absDate - first_saturday) / 7;
    if (weekNum >= sedraNumWeeks) {
	int indexLast = theSedraArray[sedraNumWeeks - 1];
	if (indexLast < 0) {
	    /* advance 2 parashiyot ahead after a doubled week */
	    index = U(indexLast) + 2;
	} else {
	    index = indexLast + 1;
	}
    } else {
	index = theSedraArray[weekNum];
    }
    
    *buf = '\0';                        /* reset the return buffer */
    
    if (index >= 0)
        strncpy (buf, LANGUAGE2(sedrot[index]), buf_len);
    else if (-1 == index)
        return 0;
    else
    {
        int i = U (index);      /* undouble the parsha */
        sprintf (buf, "%s-%s",
                 LANGUAGE2(sedrot[i]),
                 LANGUAGE2(sedrot[i + 1]));
    }
    return 1;
}
