/* C code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf strings_es.gperf  */
/* Computed positions: -k'1,3,5,8-9,11,14,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 4 "strings_es.gperf"

#include <string.h>
#line 7 "strings_es.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 239
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 58
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 548
/* maximum key range = 542, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static unsigned short asso_values[] =
    {
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549,   5,   5, 549, 549, 549,  75, 549,  25,
       65, 110, 549, 549, 549, 549, 145, 549, 549,  20,
       35,  15,   5,  40,  25,  10,   0, 549, 549,   0,
      549, 549, 549, 549, 549,   0,  35, 140,  70,  40,
       20,   0, 135,   0, 185, 100,  15, 180,  90,   5,
        0, 155,   5,  15,  45,  50,   5,   5, 549, 170,
      130,  30, 549, 549, 549, 549, 549,  10,  20,  70,
       15,   0,  75,  30,  25,   0,  50,   5,  60,   5,
        0,   0,  75,  10,   0,   0,  10,  35,  55, 115,
      549,  40,  95, 100, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549, 549, 549, 549,
      549, 549, 549, 549, 549, 549, 549
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[13]];
      /*FALLTHROUGH*/
      case 13:
      case 12:
      case 11:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
      case 9:
        hval += asso_values[(unsigned char)str[8]+1];
      /*FALLTHROUGH*/
      case 8:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]+1];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct event_title *
hebcal_lookup_es (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 92 "strings_es.gperf"
      {"Pinchas","Pinjas"},
#line 245 "strings_es.gperf"
      {"min","min"},
#line 129 "strings_es.gperf"
      {"Omer","Omer"},
#line 153 "strings_es.gperf"
      {"Purim","Purim"},
      {""},
#line 91 "strings_es.gperf"
      {"Pekudei","Pekudei"},
      {""},
#line 157 "strings_es.gperf"
      {"Adar","Adar"},
#line 26 "strings_es.gperf"
      {"Gitin","Gitin"},
      {""},
#line 40 "strings_es.gperf"
      {"Arachin","Arajin"},
      {""}, {""}, {""},
#line 158 "strings_es.gperf"
      {"Adar I","Adar I"},
#line 159 "strings_es.gperf"
      {"Adar II","Adar II"},
#line 143 "strings_es.gperf"
      {"Pesach I","Pésaj I"},
#line 172 "strings_es.gperf"
      {"Rosh Hashana I","Rosh Hashana I"},
#line 173 "strings_es.gperf"
      {"Rosh Hashana II","Rosh Hashana II"},
#line 96 "strings_es.gperf"
      {"Shmini","Shmini"},
#line 232 "strings_es.gperf"
      {"Rosh Chodesh Adar","Rosh Jodesh Adar"},
#line 238 "strings_es.gperf"
      {"Rosh Chodesh Iyyar","Rosh Jodesh Iyar"},
#line 233 "strings_es.gperf"
      {"Rosh Chodesh Adar I","Rosh Jodesh Adar I"},
#line 234 "strings_es.gperf"
      {"Rosh Chodesh Adar II","Rosh Jodesh Adar II"},
      {""},
#line 171 "strings_es.gperf"
      {"Rosh Hashana","Rosh Hashana"},
#line 12 "strings_es.gperf"
      {"Pesachim","Pesahim"},
      {""}, {""}, {""},
#line 70 "strings_es.gperf"
      {"Bo","Bo"},
#line 61 "strings_es.gperf"
      {"Parashat","Parashá"},
      {""},
#line 103 "strings_es.gperf"
      {"Vaera","Vaera"},
      {""},
#line 148 "strings_es.gperf"
      {"Pesach Sheni","Pésaj Sheni"},
#line 242 "strings_es.gperf"
      {"Rosh Chodesh Sivan","Rosh Jodesh Sivan"},
#line 75 "strings_es.gperf"
      {"Emor","Emor"},
#line 163 "strings_es.gperf"
      {"Iyyar","Iyar"},
#line 142 "strings_es.gperf"
      {"Pesach","Pésaj"},
#line 156 "strings_es.gperf"
      {"Rosh Chodesh","Rosh Jodesh"},
#line 64 "strings_es.gperf"
      {"Bamidbar","Bamidbar"},
      {""},
#line 104 "strings_es.gperf"
      {"Vaetchanan","Vaetjanan"},
      {""},
#line 170 "strings_es.gperf"
      {"Tishrei","Tishrei"},
#line 13 "strings_es.gperf"
      {"Shekalim","Shekalim"},
#line 241 "strings_es.gperf"
      {"Rosh Chodesh Sh'vat","Rosh Jodesh Sh'vat"},
#line 25 "strings_es.gperf"
      {"Sotah","Sotah"},
#line 154 "strings_es.gperf"
      {"Purim Katan","Purim Katan"},
#line 160 "strings_es.gperf"
      {"Av","Av"},
      {""},
#line 247 "strings_es.gperf"
      {"Fast ends","El ayuno finaliza"},
#line 93 "strings_es.gperf"
      {"Re'eh","Re´e"},
#line 108 "strings_es.gperf"
      {"Vayera","Vayera"},
#line 97 "strings_es.gperf"
      {"Shoftim","Shoftim"},
      {""},
#line 189 "strings_es.gperf"
      {"Sigd","Sigd"},
#line 67 "strings_es.gperf"
      {"Behar","Bejar"},
      {""},
#line 106 "strings_es.gperf"
      {"Vayechi","Vayeji"},
      {""}, {""},
#line 45 "strings_es.gperf"
      {"Tamid","Tamid"},
#line 15 "strings_es.gperf"
      {"Sukkah","Suca"},
      {""},
#line 68 "strings_es.gperf"
      {"Bereshit","Bereshit"},
      {""},
#line 167 "strings_es.gperf"
      {"Sivan","Sivan"},
#line 166 "strings_es.gperf"
      {"Sh'vat","Sh'vat"},
#line 99 "strings_es.gperf"
      {"Terumah","Terumah"},
#line 9 "strings_es.gperf"
      {"Berachot","Berajot"},
      {""},
#line 235 "strings_es.gperf"
      {"Rosh Chodesh Av","Rosh Jodesh Av"},
      {""},
#line 41 "strings_es.gperf"
      {"Temurah","Temurah"},
#line 244 "strings_es.gperf"
      {"Rosh Chodesh Tevet","Rosh Jodesh Tevet"},
      {""},
#line 28 "strings_es.gperf"
      {"Baba Kamma","Baba Kamma"},
      {""},
#line 33 "strings_es.gperf"
      {"Shevuot","Shavuot"},
#line 110 "strings_es.gperf"
      {"Vayetzei","Vayetze"},
#line 187 "strings_es.gperf"
      {"Shmini Atzeret","Shmini Atzeret"},
#line 30 "strings_es.gperf"
      {"Baba Batra","Baba Batra"},
      {""},
#line 94 "strings_es.gperf"
      {"Sh'lach","Sh'laj"},
#line 52 "strings_es.gperf"
      {"Error in input file.  Skipping line %s","Error en el archivo de entrada. Saltando la línea %s"},
      {""},
#line 165 "strings_es.gperf"
      {"Nisan","Nisán"},
      {""},
#line 184 "strings_es.gperf"
      {"Shavuot","Shavuot"},
      {""},
#line 31 "strings_es.gperf"
      {"Sanhedrin","Sanedrín"},
#line 155 "strings_es.gperf"
      {"Rosh Chodesh %s","Rosh Jodesh %s"},
      {""},
#line 219 "strings_es.gperf"
      {"Pesach Shabbat Chol ha-Moed","Pésaj Shabat Jol ha-Moed"},
#line 192 "strings_es.gperf"
      {"Sukkot I","Sukot I"},
      {""},
#line 138 "strings_es.gperf"
      {"Erev Yom Kippur","Erev Yom Kippur"},
#line 95 "strings_es.gperf"
      {"Shemot","Shemot"},
#line 57 "strings_es.gperf"
      {"Error in yahrtzeit file.  Skipping line %s","Error en el archivo yahrtzeit. Saltando la línea %s"},
      {""}, {""},
#line 132 "strings_es.gperf"
      {"Erev Purim","Erev Purim"},
#line 191 "strings_es.gperf"
      {"Sukkot","Sukot"},
#line 10 "strings_es.gperf"
      {"Shabbat","Shabat"},
#line 190 "strings_es.gperf"
      {"Simchat Torah","Simját Torá"},
#line 102 "strings_es.gperf"
      {"Tzav","Tzav"},
#line 113 "strings_es.gperf"
      {"Vayishlach","Vaishlaj"},
#line 246 "strings_es.gperf"
      {"Fast begins","El ayuno comienza"},
#line 23 "strings_es.gperf"
      {"Nedarim","Nedarim"},
#line 240 "strings_es.gperf"
      {"Rosh Chodesh Nisan","Rosh Jodesh Nisan"},
#line 201 "strings_es.gperf"
      {"Ta'anit Esther","Ta'anit Ester"},
      {""},
#line 17 "strings_es.gperf"
      {"Taanit","Tanit"},
#line 112 "strings_es.gperf"
      {"Vayikra","Vayikra"},
#line 206 "strings_es.gperf"
      {"Tzom Gedaliah","Tzom Gedaliah"},
      {""}, {""},
#line 131 "strings_es.gperf"
      {"Erev Pesach","Erev Pesaj"},
#line 237 "strings_es.gperf"
      {"Rosh Chodesh Elul","Rosh Jodesh Elul"},
#line 111 "strings_es.gperf"
      {"Vayigash","Vayigash"},
      {""},
#line 90 "strings_es.gperf"
      {"Noach","Noaj"},
#line 11 "strings_es.gperf"
      {"Eruvin","Eruvin"},
#line 78 "strings_es.gperf"
      {"Ki Tavo","Ki Tavo"},
#line 100 "strings_es.gperf"
      {"Tetzaveh","Tetzaveh"},
      {""}, {""}, {""},
#line 73 "strings_es.gperf"
      {"Devarim","Devarim"},
#line 89 "strings_es.gperf"
      {"Nitzavim","Nitzavim"},
#line 162 "strings_es.gperf"
      {"Elul","Elul"},
      {""},
#line 136 "strings_es.gperf"
      {"Erev Sukkot","Erev Sukot"},
      {""},
#line 77 "strings_es.gperf"
      {"Kedoshim","Kedoshim"},
      {""}, {""},
#line 81 "strings_es.gperf"
      {"Korach","Koraj"},
#line 221 "strings_es.gperf"
      {"Rosh Hashana I (on Shabbat)","Rosh Hashana I (en Shabat)"},
#line 161 "strings_es.gperf"
      {"Cheshvan","Jeshvan"},
      {""},
#line 169 "strings_es.gperf"
      {"Tevet","Tevet"},
#line 137 "strings_es.gperf"
      {"Erev Tish'a B'Av","Erev Tish'a B'Av"},
#line 42 "strings_es.gperf"
      {"Keritot","Keritot"},
      {""}, {""}, {""},
#line 47 "strings_es.gperf"
      {"Niddah","Nidá"},
      {""}, {""},
#line 69 "strings_es.gperf"
      {"Beshalach","Beshalaj"},
#line 130 "strings_es.gperf"
      {"day of the Omer","día del Omer"},
#line 44 "strings_es.gperf"
      {"Kinnim","Kinim"},
      {""},
#line 135 "strings_es.gperf"
      {"Erev Simchat Torah","Erev Simjat Torá"},
      {""},
#line 63 "strings_es.gperf"
      {"Balak","Balak"},
#line 214 "strings_es.gperf"
      {"Pesach I (on Shabbat)","Pésaj I (en Shabat)"},
#line 16 "strings_es.gperf"
      {"Beitzah","Beitzah"},
#line 243 "strings_es.gperf"
      {"Rosh Chodesh Tamuz","Rosh Jodesh Tamuz"},
#line 51 "strings_es.gperf"
      {"input file read error. Skipping line %s","error de lectura del archivo de entrada. Saltando la línea %s"},
#line 88 "strings_es.gperf"
      {"Nasso","Naso"},
#line 236 "strings_es.gperf"
      {"Rosh Chodesh Cheshvan","Rosh Jodesh Jeshvan"},
#line 80 "strings_es.gperf"
      {"Ki Tisa","Ki Tisa"},
#line 109 "strings_es.gperf"
      {"Vayeshev","Vayeshev"},
      {""},
#line 176 "strings_es.gperf"
      {"Shabbat HaGadol","Shabat HaGadol"},
#line 128 "strings_es.gperf"
      {"Days of the Omer","Los días del Omer"},
#line 203 "strings_es.gperf"
      {"Tu B'Av","Tu B'Av"},
#line 141 "strings_es.gperf"
      {"Leil Selichot","Leil Selijot"},
#line 27 "strings_es.gperf"
      {"Kiddushin","Keddushin"},
      {""}, {""},
#line 230 "strings_es.gperf"
      {"Sukkot Shabbat Chol ha-Moed","Sukot Shabat Jol ha-Moed"},
#line 116 "strings_es.gperf"
      {"Asara B'Tevet","Asara B'Tevet"},
#line 239 "strings_es.gperf"
      {"Rosh Chodesh Kislev","Rosh Jodesh Kislev"},
      {""}, {""},
#line 38 "strings_es.gperf"
      {"Chullin","Julín"},
      {""},
#line 14 "strings_es.gperf"
      {"Yoma","Yoma"},
#line 24 "strings_es.gperf"
      {"Nazir","Nazir"},
      {""},
#line 134 "strings_es.gperf"
      {"Erev Shavuot","Erev Shavuot"},
      {""},
#line 174 "strings_es.gperf"
      {"Shabbat Chazon","Shabbat Jazon"},
#line 204 "strings_es.gperf"
      {"Tu BiShvat","Tu BiShvat"},
#line 101 "strings_es.gperf"
      {"Toldot","Toldot"},
#line 22 "strings_es.gperf"
      {"Ketubot","Ketubot"},
#line 54 "strings_es.gperf"
      {"Unrecognized hebrew month in input file.  Skipping line %s","Un mes hebreo no reconocido en el archivo de entrada. Saltando la línea %s"},
#line 107 "strings_es.gperf"
      {"Vayeilech","Vayelej"},
#line 65 "strings_es.gperf"
      {"Bechukotai","Bejukotai"},
#line 29 "strings_es.gperf"
      {"Baba Metzia","Baba Metzia"},
      {""},
#line 182 "strings_es.gperf"
      {"Shabbat Shuva","Shabbat Shuva"},
#line 87 "strings_es.gperf"
      {"Mishpatim","Mishpatim"},
      {""},
#line 98 "strings_es.gperf"
      {"Tazria","Tazria"},
#line 66 "strings_es.gperf"
      {"Beha'alotcha","Behaaloteja"},
#line 39 "strings_es.gperf"
      {"Bechorot","Bejorot"},
#line 144 "strings_es.gperf"
      {"Pesach II","Pésaj II"},
#line 205 "strings_es.gperf"
      {"Tu B'Shvat","Tu BiShvat"},
#line 72 "strings_es.gperf"
      {"Chukat","Jukat"},
      {""},
#line 58 "strings_es.gperf"
      {"Non-numeric month in yahrtzeit file. Skipping line %s","Mes no numérico en el archivo yahrtzeit. Saltando la línea %s"},
#line 183 "strings_es.gperf"
      {"Shabbat Zachor","Shabat Zajor"},
#line 151 "strings_es.gperf"
      {"Pesach VII","Pésaj VII"},
#line 152 "strings_es.gperf"
      {"Pesach VIII","Pésaj VIII"},
#line 34 "strings_es.gperf"
      {"Avodah Zarah","Avodah Zarah"},
#line 36 "strings_es.gperf"
      {"Zevachim","Zevahim"},
      {""},
#line 74 "strings_es.gperf"
      {"Eikev","Eikev"},
      {""}, {""},
#line 37 "strings_es.gperf"
      {"Menachot","Menajot"},
      {""},
#line 19 "strings_es.gperf"
      {"Moed Katan","Moed Katan"},
#line 181 "strings_es.gperf"
      {"Shabbat Shekalim","Shabat Shekalim"},
      {""},
#line 188 "strings_es.gperf"
      {"Shushan Purim","Shushan Purim"},
      {""},
#line 178 "strings_es.gperf"
      {"Shabbat Nachamu","Shabat Najamu"},
#line 43 "strings_es.gperf"
      {"Meilah","Meilá"},
#line 133 "strings_es.gperf"
      {"Erev Rosh Hashana","Erev Rosh Hashana"},
#line 105 "strings_es.gperf"
      {"Vayakhel","Vaiakel"},
#line 55 "strings_es.gperf"
      {"Date out of range in input file. Skipping line %s","Fecha fuera de rango en el archivo de entrada. Saltando la línea %s"},
#line 83 "strings_es.gperf"
      {"Masei","Masei"},
#line 164 "strings_es.gperf"
      {"Kislev","Kislev"},
#line 147 "strings_es.gperf"
      {"Pesach IV (CH''M)","Pésaj IV (J''M)"},
#line 59 "strings_es.gperf"
      {"Date out of range in yahrtzeit file. Skipping line %s","Fecha fuera de rango en el archivo yahrtzeit. Saltando la línea %s"},
      {""},
#line 84 "strings_es.gperf"
      {"Matot","Matot"},
      {""},
#line 21 "strings_es.gperf"
      {"Yevamot","Yevamot"},
      {""}, {""},
#line 46 "strings_es.gperf"
      {"Midot","Midot"},
#line 225 "strings_es.gperf"
      {"Sukkot I (on Shabbat)","Sucot I (en Shabat)"},
#line 35 "strings_es.gperf"
      {"Horayot","Horayot"},
#line 210 "strings_es.gperf"
      {"Yom HaZikaron","Yom HaZikaron"},
      {""},
#line 168 "strings_es.gperf"
      {"Tamuz","Tamuz"},
      {""}, {""},
#line 56 "strings_es.gperf"
      {"yahrtzeit file read error. Skipping line %s","error de lectura del archivo yahrtzeit. Saltando la línea %s"},
      {""},
#line 218 "strings_es.gperf"
      {"Pesach Chol ha-Moed Day 4","Pésaj Jol ha-Moed Día 4"},
#line 207 "strings_es.gperf"
      {"Tzom Tammuz","Tzom Tammuz"},
      {""},
#line 20 "strings_es.gperf"
      {"Chagigah","Jaguigá"},
#line 126 "strings_es.gperf"
      {"Chanukah: 8 Candles","Janucá: 8 velas"},
#line 115 "strings_es.gperf"
      {"Yitro","Yitro"},
      {""}, {""},
#line 118 "strings_es.gperf"
      {"Chanukah","Jánuca"},
#line 122 "strings_es.gperf"
      {"Chanukah: 4 Candles","Janucá: 4 velas"},
#line 217 "strings_es.gperf"
      {"Pesach Chol ha-Moed Day 3","Pésaj Jol ha-Moed Día 3"},
      {""}, {""},
#line 139 "strings_es.gperf"
      {"Havdalah","Havdalah"},
#line 125 "strings_es.gperf"
      {"Chanukah: 7 Candles","Janucá: 7 velas"},
#line 215 "strings_es.gperf"
      {"Pesach Chol ha-Moed Day 1","Pésaj Jol ha-Moed Día 1"},
      {""}, {""},
#line 18 "strings_es.gperf"
      {"Megillah","Megillah"},
#line 121 "strings_es.gperf"
      {"Chanukah: 3 Candles","Janucá: 3 velas"},
#line 140 "strings_es.gperf"
      {"Lag BaOmer","Lag BaOmer"},
#line 32 "strings_es.gperf"
      {"Makkot","Makot"},
      {""},
#line 119 "strings_es.gperf"
      {"Chanukah: 1 Candle","Janucá: 1 vela"},
#line 185 "strings_es.gperf"
      {"Shavuot I","Shavuot I"},
#line 186 "strings_es.gperf"
      {"Shavuot II","Shavuot II"},
#line 114 "strings_es.gperf"
      {"Vezot Haberakhah","Vezot Haberaja"},
#line 85 "strings_es.gperf"
      {"Metzora","Metzora"},
#line 49 "strings_es.gperf"
      {"Daf Yomi","Daf Yomi"},
#line 124 "strings_es.gperf"
      {"Chanukah: 6 Candles","Janucá: 6 velas"},
#line 216 "strings_es.gperf"
      {"Pesach Chol ha-Moed Day 2","Pésaj Jol ha-Moed Día 2"},
      {""}, {""},
#line 53 "strings_es.gperf"
      {"Numeric hebrew month in input file.  Skipping line %s","El mes hebreo numérico en el archivo de entrada. Saltando la línea %s"},
#line 193 "strings_es.gperf"
      {"Sukkot II","Sukot II"},
      {""},
#line 71 "strings_es.gperf"
      {"Chayei Sara","Jayei Sara"},
#line 127 "strings_es.gperf"
      {"Chanukah: 8th Day","Janucá: 8º día"},
#line 179 "strings_es.gperf"
      {"Shabbat Parah","Shabat Parah"},
#line 120 "strings_es.gperf"
      {"Chanukah: 2 Candles","Janucá: 2 velas"},
      {""}, {""},
#line 175 "strings_es.gperf"
      {"Shabbat HaChodesh","Shabat HaJodesh"},
      {""},
#line 123 "strings_es.gperf"
      {"Chanukah: 5 Candles","Janucá: 5 velas"},
#line 180 "strings_es.gperf"
      {"Shabbat Rosh Chodesh","Shabat Rosh Jodesh"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 177 "strings_es.gperf"
      {"Shabbat Machar Chodesh","Shabbat Majar Jodesh"},
      {""}, {""}, {""},
#line 200 "strings_es.gperf"
      {"Ta'anit Bechorot","Ta'anit Bejorot"},
      {""}, {""}, {""}, {""},
#line 149 "strings_es.gperf"
      {"Pesach V (CH''M)","Pésaj V (J''M)"},
#line 196 "strings_es.gperf"
      {"Sukkot IV (CH''M)","Sukot IV (J''M)"},
      {""}, {""},
#line 82 "strings_es.gperf"
      {"Lech-Lecha","Lej Leja"},
#line 86 "strings_es.gperf"
      {"Miketz","Miketz"},
      {""}, {""}, {""},
#line 117 "strings_es.gperf"
      {"Candle lighting","Iluminación de velas"},
      {""}, {""}, {""},
#line 208 "strings_es.gperf"
      {"Yom HaAtzma'ut","Yom HaAtzma'ut"},
#line 211 "strings_es.gperf"
      {"Yom Kippur","Yom Kippur"},
#line 202 "strings_es.gperf"
      {"Tish'a B'Av","Tish'a B'Av"},
#line 213 "strings_es.gperf"
      {"Yom HaAliyah","Yom Yerushalayim"},
      {""}, {""},
#line 229 "strings_es.gperf"
      {"Sukkot Chol ha-Moed Day 4","Sukot Jol ha-Moed Día 4"},
#line 209 "strings_es.gperf"
      {"Yom HaShoah","Yom HaShoah"},
      {""},
#line 76 "strings_es.gperf"
      {"Ha'Azinu","HaAzinu"},
      {""}, {""}, {""}, {""},
#line 50 "strings_es.gperf"
      {"Unable to allocate memory for holiday.","No se puede asignar memoria para las vacaciones."},
      {""},
#line 228 "strings_es.gperf"
      {"Sukkot Chol ha-Moed Day 3","Sukot Jol ha-Moed Día 3"},
#line 60 "strings_es.gperf"
      {"improper sedra year type calculated.","tipo de año de sedra inapropiado calculado."},
      {""}, {""}, {""},
#line 226 "strings_es.gperf"
      {"Sukkot Chol ha-Moed Day 1","Sukot Jol ha-Moed Día 1"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 79 "strings_es.gperf"
      {"Ki Teitzei","Ki Teitzei"},
      {""}, {""}, {""}, {""},
#line 227 "strings_es.gperf"
      {"Sukkot Chol ha-Moed Day 2","Sukot Jol ha-Moed Día 2"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 62 "strings_es.gperf"
      {"Achrei Mot","Ajarei Mot"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 231 "strings_es.gperf"
      {"Sukkot Final Day (Hoshana Raba)","Día final de Sukot (Hoshana Raba)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 197 "strings_es.gperf"
      {"Sukkot V (CH''M)","Sukot V (J''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 220 "strings_es.gperf"
      {"Shavuot II (on Shabbat)","Shavuot II (en Shabat)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 145 "strings_es.gperf"
      {"Pesach II (CH''M)","Pesaj II (J''M)"},
      {""}, {""}, {""},
#line 212 "strings_es.gperf"
      {"Yom Yerushalayim","Yom Yerushalayim"},
#line 150 "strings_es.gperf"
      {"Pesach VI (CH''M)","Pésaj VI (J''M)"},
      {""}, {""},
#line 199 "strings_es.gperf"
      {"Sukkot VII (Hoshana Raba)","Sukot VII (Hoshana Raba)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 48 "strings_es.gperf"
      {"Daf Yomi: %s %d","Daf Yomi: %s %d"},
      {""}, {""},
#line 222 "strings_es.gperf"
      {"Yom Kippur (on Shabbat)","Yom Kipur (en Shabat)"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 224 "strings_es.gperf"
      {"Yom Kippur (Mincha, Alternate)","Yom Kippur (Minja, Alterno)"},
      {""},
#line 223 "strings_es.gperf"
      {"Yom Kippur (Mincha, Traditional)","Yom Kipur (Minja, Tradicional)"},
#line 146 "strings_es.gperf"
      {"Pesach III (CH''M)","Pésaj III (J''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 194 "strings_es.gperf"
      {"Sukkot II (CH''M)","Sukot II (J''M)"},
      {""}, {""}, {""}, {""},
#line 198 "strings_es.gperf"
      {"Sukkot VI (CH''M)","Sukot VI (J''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 195 "strings_es.gperf"
      {"Sukkot III (CH''M)","Sukot III (J''M)"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
