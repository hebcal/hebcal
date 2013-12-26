/*
   $Id: hebcal.c,v 1.9 2002/09/13 19:41:29 sadinoff Exp $
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

   This is the Hebcal daf yomi calculator,
   adapted by Aaron Peromsik from Bob Newell's public domain daf.el.
   more info: https://groups.google.com/forum/#!topic/comp.emacs/Gr-rijg2kgM
*/

#include <stdio.h>
#include "hebcal.h"
#include "mystring.h"

#define NM_LEN 60

struct mesechta {
   char *sname;
   char *aname;
   char *hname;
   int   blatt;
};

static struct mesechta shas[] = {
  { "Berachot",       "Berachos",         "Berachos",       64      },
  { "Shabbat",        "Shabbos",          "Shabbos",        157     },
  { "Eruvin",         "Eruvin",           "Eruvin",         105     },
  { "Pesachim",       "Pesachim",         "Pesachim",       121     },
  { "Shekalim",       "Shekalim",         "Shekalim",       22      },
  { "Yoma",           "Yoma",             "Yoma",           88      },
  { "Sukkah",         "Sukkah",           "Sukkah",         56      },
  { "Beitzah",        "Beitzah",          "Beitzah",        40      },
  { "Rosh Hashana",   "Rosh Hashana",     "Rosh Hashana",   35      },
  { "Taanit",         "Taanis",           "Taanis",         31      },
  { "Megillah",       "Megillah",         "Megillah",       32      },
  { "Moed Katan",     "Moed Katan",       "Moed Katan",     29      },
  { "Chagigah",       "Chagigah",         "Chagigah",       27      },
  { "Yevamot",        "Yevamos",          "Yevamos",        122     },
  { "Ketubot",        "Kesubos",          "Kesubos",        112     },
  { "Nedarim",        "Nedarim",          "Nedarim",        91      },
  { "Nazir",          "Nazir",            "Nazir",          66      },
  { "Sotah",          "Sotah",            "Sotah",          49      },
  { "Gitin",          "Gitin",            "Gitin",          90      },
  { "Kiddushin",      "Kiddushin",        "Kiddushin",      82      },
  { "Baba Kamma",     "Baba Kamma",       "��� ���",        119     },
  { "Baba Metzia",    "Baba Metzia",      "��� �����"  ,    119     },
  { "Baba Batra",     "Baba Basra",       "��� ����"  ,     176     },
  { "Sanhedrin",      "Sanhedrin",        "�������"  ,      113     },
  { "Makkot",         "Makkos",           "����"  ,         24      },
  { "Shevuot",        "Shevuos",          "������" ,        49      },
  { "Avodah Zarah",   "Avodah Zarah",     "����� ���"   ,   76      },
  { "Horayot",        "Horayos",          "������" ,        14      },
  { "Zevachim",       "Zevachim",         "�����",          120     },
  { "Menachot",       "Menachos",         "Menachos",       110     },
  { "Chullin",        "Chullin",          "Chullin",        142     },
  { "Bechorot",       "Bechoros",         "Bechoros",       61      },
  { "Arachin",        "Arachin",          "Arachin",        34      },
  { "Temurah",        "Temurah",          "Temurah",        34      },
  { "Keritot",        "Kerisos",          "Kerisos",        28      },
  { "Meilah",         "Meilah",           "Meilah",         22      },
  { "Kinnim",         "Kinnim",           "Kinnim",         4       },
  { "Tamid",          "Tamid",            "Tamid",          10      },
  { "Midot",          "Midos",            "Midos",          4       },
  { "Niddah",         "Niddah",           "Niddah",         73      }
};

void hebcal_dafyomi( date_t *greg_day  )
{
   int dafcnt = 40;
   int cno, dno, osday, nsday, total, count, j, cday, blatt;
   date_t tmp_date;
   char buffer[NM_LEN];

   tmp_date.mm = 9;
   tmp_date.dd = 11;
   tmp_date.yy = 1923;
   osday = greg2abs(tmp_date);
   tmp_date.mm = 6;
   tmp_date.dd = 24;
   tmp_date.yy = 1975;
   nsday = greg2abs(tmp_date);
   cday = greg2abs(*greg_day);
   
   /*  No cycle, new cycle, old cycle */
   if (cday < osday)
      return; /* daf yomi hadn't started yet */
   if (cday >= nsday)
   {
      cno = 8 + ( (cday - nsday) / 2711 );
      dno = (cday - nsday) % 2711;
   }
   else
   {
      cno = 1 + ( (cday - osday) / 2702 );
      dno = (cday - osday) / 2702;
   }

/* Find the daf taking note that the cycle changed slightly after cycle 7. */

   total = blatt = 0;
   count = -1;

   /* Fix Shekalim for old cycles */
   if (cno <= 7)
      shas[4].blatt = 13;
   else
      shas[4].blatt = 22;

   /* Find the daf */
   j = 0;
   while (j < dafcnt)
   {
      count++;
      total = total + shas[j].blatt - 1;
      if (dno < total)
      {
         blatt = (shas[j].blatt + 1) - (total - dno);
         /* fiddle with the weird ones near the end */
         switch(count)
         {
         case 36:
            blatt = blatt + 21;
            break;
         case 37:
            blatt = blatt + 24;
            break;
         case 38:
            blatt = blatt + 33;
            break;
         default:
            j = 1 + dafcnt;
            break;
         }
      }
      j ++;
   }         

   sprintf(buffer,
           iso8859_8_sw ? "�� ����: %s %d" : "Daf Yomi: %s %d",
           iso8859_8_sw ? shas[count].hname :
             ashkenazis_sw ? shas[count].aname : shas[count].sname,
           blatt );
   DeclareEvent( greg_day, NULL, buffer, 1 );
}

   
