/*
   This is the Hebcal daf yomi calculator,
   adapted by Aaron Peromsik from Bob Newell's public domain daf.el.
   more info: https://groups.google.com/forum/#!topic/comp.emacs/Gr-rijg2kgM
*/

#include <stdio.h>
#include <string.h>
#include "hebcal.h"
#include "dafyomi.h"
#include "config.h"
#include "translations.h"

#define _(String) lookup_translation(String)

#define NM_LEN 60

struct mesechta {
   char *sname;
   int   blatt;
};

static struct mesechta shas[] = {
  { "Berachot",       64      },
  { "Shabbat",        157     },
  { "Eruvin",         105     },
  { "Pesachim",       121     },
  { "Shekalim",       22      },
  { "Yoma",           88      },
  { "Sukkah",         56      },
  { "Beitzah",        40      },
  { "Rosh Hashana",   35      },
  { "Taanit",         31      },
  { "Megillah",       32      },
  { "Moed Katan",     29      },
  { "Chagigah",       27      },
  { "Yevamot",        122     },
  { "Ketubot",        112     },
  { "Nedarim",        91      },
  { "Nazir",          66      },
  { "Sotah",          49      },
  { "Gitin",          90      },
  { "Kiddushin",      82      },
  { "Baba Kamma",     119     },
  { "Baba Metzia",    119     },
  { "Baba Batra",     176     },
  { "Sanhedrin",      113     },
  { "Makkot",         24      },
  { "Shevuot",        49      },
  { "Avodah Zarah",   76      },
  { "Horayot",        14      },
  { "Zevachim",       120     },
  { "Menachot",       110     },
  { "Chullin",        142     },
  { "Bechorot",       61      },
  { "Arachin",        34      },
  { "Temurah",        34      },
  { "Keritot",        28      },
  { "Meilah",         22      },
  { "Kinnim",         4       },
  { "Tamid",          9       },
  { "Midot",          5       },
  { "Niddah",         73      }
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
      dno = (cday - osday) % 2702;
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
            blatt = blatt + 32;
            break;
         default:
            break;
         }
	 /* Bailout */
	 j = 1 + dafcnt;
      }
      j ++;
   }

   sprintf(buffer,
           _("Daf Yomi: %s %d"),
           _(shas[count].sname),
           blatt );
   PrintGregDate(*greg_day);
   printf("%s\n", buffer);
}
