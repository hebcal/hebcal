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

#include "greg.h"
#include "hebcal.h"
#include "sedra.h"
#include "common.h"

static const char *sedrot[] =
{
    "Bereshit",
    "Noach",
    "Lech-Lecha",
    "Vayera",
    "Chayei Sara",
    "Toldot",
    "Vayetzei",
    "Vayishlach",
    "Vayeshev",
    "Miketz",
    "Vayigash",
    "Vayechi",
    "Shemot",
    "Vaera",
    "Bo",
    "Beshalach",
    "Yitro",
    "Mishpatim",
    "Terumah",
    "Tetzaveh",
    "Ki Tisa",
    "Vayakhel",
    "Pekudei",
    "Vayikra",
    "Tzav",
    "Shmini",
    "Tazria",
    "Metzora",
    "Achrei Mot",
    "Kedoshim",
    "Emor",
    "Behar",
    "Bechukotai",
    "Bamidbar",
    "Nasso",
    "Beha'alotcha",
    "Sh'lach",
    "Korach",
    "Chukat",
    "Balak",
    "Pinchas",
    "Matot",
    "Masei",
    "Devarim",
    "Vaetchanan",
    "Eikev",
    "Re'eh",
    "Shoftim",
    "Ki Teitzei",
    "Ki Tavo",
    "Nitzavim",
    "Vayeilech",
    "Ha'Azinu",
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
