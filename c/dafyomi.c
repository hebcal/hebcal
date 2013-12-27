/*
   This is the Hebcal daf yomi calculator,
   adapted by Aaron Peromsik from Bob Newell's public domain daf.el.
   more info: https://groups.google.com/forum/#!topic/comp.emacs/Gr-rijg2kgM
*/

#include <stdio.h>
#include <string.h>
#include "hebcal.h"
#include "dafyomi.h"

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
  { "Baba Kamma",     "Baba Kamma",       "בבא קמא",        119     },
  { "Baba Metzia",    "Baba Metzia",      "בבא מציעא"  ,    119     },
  { "Baba Batra",     "Baba Basra",       "בבא בתרא"  ,     176     },
  { "Sanhedrin",      "Sanhedrin",        "סנהדרין"  ,      113     },
  { "Makkot",         "Makkos",           "מכות"  ,         24      },
  { "Shevuot",        "Shevuos",          "שבועות" ,        49      },
  { "Avodah Zarah",   "Avodah Zarah",     "עבודה זרה"   ,   76      },
  { "Horayot",        "Horayos",          "הוריות" ,        14      },
  { "Zevachim",       "Zevachim",         "זבחים",          120     },
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
           iso8859_8_sw ? "דף יומי: %s %d" : "Daf Yomi: %s %d",
           iso8859_8_sw ? shas[count].hname :
             ashkenazis_sw ? shas[count].aname : shas[count].sname,
           blatt );
   PrintGregDate(*greg_day);
   printf("%s\n", buffer);
}

   
