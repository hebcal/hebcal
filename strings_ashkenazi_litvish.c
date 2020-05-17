/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf strings_ashkenazi_litvish.gperf  */
/* Computed positions: -k'1,3,5,8-9,11' */

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
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 4 "strings_ashkenazi_litvish.gperf"

#include <string.h>
#line 7 "strings_ashkenazi_litvish.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 190
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 25
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 510
/* maximum key range = 507, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned short asso_values[] =
    {
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511,   0,   0, 511, 511, 511,   0, 511,  70,
        0, 511, 511, 511, 511, 511,  80, 511, 511,   0,
       35,  25,  20,  15,  10,   0,   5, 511, 511,   5,
      511, 511, 511, 511, 511, 115,  55, 155, 150,  15,
        0,  80,   0,   0,   0,  95,   0,  40, 180,   0,
        5,  75,  65,   0,   0,   0,  10, 145, 511,  50,
       30,   0, 511, 511, 511, 511, 511,  30,  10,  15,
        0,  35,  10,   0,  55,  25,  35,   0, 125,  90,
       35, 115,   0,   0,  40,   0,  15,   5,   0, 105,
      511,  90, 150,  95, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511, 511, 511, 511,
      511, 511, 511, 511, 511, 511, 511
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
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
  return hval;
}

struct event_title *
hebcal_lookup_ashkenazi_litvish (register const char *str, register size_t len)
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 174 "strings_ashkenazi_litvish.gperf"
      {"Sigd","Sigd"},
      {""},
#line 176 "strings_ashkenazi_litvish.gperf"
      {"Sukkot","Sukkos"},
#line 188 "strings_ashkenazi_litvish.gperf"
      {"Tu B'Av","Tu B'ov"},
#line 177 "strings_ashkenazi_litvish.gperf"
      {"Sukkot I","Sukkos I"},
#line 178 "strings_ashkenazi_litvish.gperf"
      {"Sukkot II","Sukkos II"},
#line 155 "strings_ashkenazi_litvish.gperf"
      {"Tevet","Teyves"},
#line 129 "strings_ashkenazi_litvish.gperf"
      {"Pesach","Peisach"},
      {""},
#line 130 "strings_ashkenazi_litvish.gperf"
      {"Pesach I","Peisach I"},
#line 131 "strings_ashkenazi_litvish.gperf"
      {"Pesach II","Peisach II"},
      {""},
#line 15 "strings_ashkenazi_litvish.gperf"
      {"Sukkah","Sukko"},
#line 179 "strings_ashkenazi_litvish.gperf"
      {"Sukkot II (CH''M)","Sukkos II (CH''M)"},
#line 180 "strings_ashkenazi_litvish.gperf"
      {"Sukkot III (CH''M)","Sukkos III (CH''M)"},
      {""},
#line 190 "strings_ashkenazi_litvish.gperf"
      {"Tu B'Shvat","Tu BiShvat"},
      {""},
#line 132 "strings_ashkenazi_litvish.gperf"
      {"Pesach II (CH''M)","Peisach II (CH''M)"},
#line 133 "strings_ashkenazi_litvish.gperf"
      {"Pesach III (CH''M)","Peisach III (CH''M)"},
#line 148 "strings_ashkenazi_litvish.gperf"
      {"Elul","Elul"},
#line 138 "strings_ashkenazi_litvish.gperf"
      {"Pesach VII","Peisach VII"},
#line 139 "strings_ashkenazi_litvish.gperf"
      {"Pesach VIII","Peisach VIII"},
#line 183 "strings_ashkenazi_litvish.gperf"
      {"Sukkot VI (CH''M)","Sukkos VI (CH''M)"},
      {""}, {""}, {""}, {""},
#line 137 "strings_ashkenazi_litvish.gperf"
      {"Pesach VI (CH''M)","Peisach VI (CH''M)"},
      {""},
#line 91 "strings_ashkenazi_litvish.gperf"
      {"Tzav","Tzav"},
#line 184 "strings_ashkenazi_litvish.gperf"
      {"Sukkot VII (Hoshana Raba)","Sukkos VII (Heishano Rabo)"},
      {""},
#line 169 "strings_ashkenazi_litvish.gperf"
      {"Shavuot","Shvuos"},
      {""},
#line 170 "strings_ashkenazi_litvish.gperf"
      {"Shavuot I","Shvuos I"},
#line 171 "strings_ashkenazi_litvish.gperf"
      {"Shavuot II","Shvuos II"},
#line 84 "strings_ashkenazi_litvish.gperf"
      {"Shemot","Shmeis"},
#line 33 "strings_ashkenazi_litvish.gperf"
      {"Shevuot","Shvuos"},
      {""}, {""},
#line 25 "strings_ashkenazi_litvish.gperf"
      {"Sotah","Seito"},
#line 32 "strings_ashkenazi_litvish.gperf"
      {"Makkot","Makkos"},
#line 80 "strings_ashkenazi_litvish.gperf"
      {"Pekudei","Pkudey"},
      {""}, {""},
#line 46 "strings_ashkenazi_litvish.gperf"
      {"Midot","Midos"},
#line 75 "strings_ashkenazi_litvish.gperf"
      {"Miketz","Mikeitz"},
#line 10 "strings_ashkenazi_litvish.gperf"
      {"Shabbat","Shabbos"},
      {""}, {""},
#line 189 "strings_ashkenazi_litvish.gperf"
      {"Tu BiShvat","Tu BiShvat"},
      {""},
#line 59 "strings_ashkenazi_litvish.gperf"
      {"Bo","Bei"},
      {""}, {""},
#line 92 "strings_ashkenazi_litvish.gperf"
      {"Vaera","Voeiro"},
#line 11 "strings_ashkenazi_litvish.gperf"
      {"Eruvin","Eiruvin"},
#line 74 "strings_ashkenazi_litvish.gperf"
      {"Metzora","Metzeiro"},
#line 167 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Shuva","Shabbos Shuvo"},
      {""},
#line 73 "strings_ashkenazi_litvish.gperf"
      {"Matot","Matos"},
#line 187 "strings_ashkenazi_litvish.gperf"
      {"Tish'a B'Av","Tisho B'ov"},
      {""}, {""}, {""},
#line 104 "strings_ashkenazi_litvish.gperf"
      {"Yitro","Yisro"},
#line 17 "strings_ashkenazi_litvish.gperf"
      {"Taanit","Taanis"},
#line 81 "strings_ashkenazi_litvish.gperf"
      {"Pinchas","Pinchos"},
#line 126 "strings_ashkenazi_litvish.gperf"
      {"Havdalah","Havdolo"},
#line 168 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Zachor","Shabbos Zochor"},
#line 163 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Nachamu","Shabbos Nachamu"},
      {""},
#line 135 "strings_ashkenazi_litvish.gperf"
      {"Pesach Sheni","Peisach Sheini"},
      {""}, {""},
#line 72 "strings_ashkenazi_litvish.gperf"
      {"Masei","Masei"},
#line 43 "strings_ashkenazi_litvish.gperf"
      {"Meilah","Meilo"},
#line 88 "strings_ashkenazi_litvish.gperf"
      {"Terumah","Trumo"},
#line 50 "strings_ashkenazi_litvish.gperf"
      {"Parashat","Parshas"},
      {""},
#line 140 "strings_ashkenazi_litvish.gperf"
      {"Purim","Purim"},
#line 152 "strings_ashkenazi_litvish.gperf"
      {"Sh'vat","Shvat"},
#line 83 "strings_ashkenazi_litvish.gperf"
      {"Sh'lach","Shlach"},
#line 89 "strings_ashkenazi_litvish.gperf"
      {"Tetzaveh","Tetzave"},
      {""},
#line 30 "strings_ashkenazi_litvish.gperf"
      {"Baba Batra","Bovo Basro"},
#line 165 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Reish Chodesh","Shabbos Reish Cheidesh"},
#line 21 "strings_ashkenazi_litvish.gperf"
      {"Yevamot","Yevomos"},
#line 39 "strings_ashkenazi_litvish.gperf"
      {"Bechorot","Bcheiros"},
      {""},
#line 149 "strings_ashkenazi_litvish.gperf"
      {"Iyyar","Iyyor"},
#line 166 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Shekalim","Shabbos Shkolim"},
#line 41 "strings_ashkenazi_litvish.gperf"
      {"Temurah","Tmuro"},
#line 37 "strings_ashkenazi_litvish.gperf"
      {"Menachot","Menochos"},
      {""},
#line 93 "strings_ashkenazi_litvish.gperf"
      {"Vaetchanan","Voeschanan"},
      {""}, {""},
#line 12 "strings_ashkenazi_litvish.gperf"
      {"Pesachim","Psochim"},
      {""},
#line 54 "strings_ashkenazi_litvish.gperf"
      {"Bechukotai","Bechukeisai"},
#line 97 "strings_ashkenazi_litvish.gperf"
      {"Vayera","Vayeiro"},
#line 95 "strings_ashkenazi_litvish.gperf"
      {"Vayechi","Vayechi"},
#line 120 "strings_ashkenazi_litvish.gperf"
      {"Erev Reish Hashana","Erev Reish-Hashono"},
#line 76 "strings_ashkenazi_litvish.gperf"
      {"Mishpatim","Mishpotim"},
#line 19 "strings_ashkenazi_litvish.gperf"
      {"Moed Katan","Meyed Koton"},
#line 150 "strings_ashkenazi_litvish.gperf"
      {"Kislev","Kislev"},
#line 67 "strings_ashkenazi_litvish.gperf"
      {"Ki Tavo","Ki-Sovo"},
      {""},
#line 58 "strings_ashkenazi_litvish.gperf"
      {"Beshalach","Beshalach"},
#line 56 "strings_ashkenazi_litvish.gperf"
      {"Behar","Behar"},
      {""},
#line 146 "strings_ashkenazi_litvish.gperf"
      {"Av","Ov"},
#line 9 "strings_ashkenazi_litvish.gperf"
      {"Berachot","Brochos"},
      {""},
#line 153 "strings_ashkenazi_litvish.gperf"
      {"Sivan","Sivon"},
#line 124 "strings_ashkenazi_litvish.gperf"
      {"Erev Tish'a B'Av","Erev Tisho Bov"},
      {""},
#line 98 "strings_ashkenazi_litvish.gperf"
      {"Vayeshev","Vayeishev"},
      {""},
#line 63 "strings_ashkenazi_litvish.gperf"
      {"Eikev","Eikev"},
#line 118 "strings_ashkenazi_litvish.gperf"
      {"Erev Pesach","Erev Peisach"},
#line 86 "strings_ashkenazi_litvish.gperf"
      {"Shoftim","Shoftim"},
#line 36 "strings_ashkenazi_litvish.gperf"
      {"Zevachim","Zvochim"},
      {""},
#line 45 "strings_ashkenazi_litvish.gperf"
      {"Tamid","Tomid"},
#line 90 "strings_ashkenazi_litvish.gperf"
      {"Toldot","Toldos"},
#line 22 "strings_ashkenazi_litvish.gperf"
      {"Ketubot","Ksubos"},
#line 57 "strings_ashkenazi_litvish.gperf"
      {"Bereshit","Breishis"},
#line 64 "strings_ashkenazi_litvish.gperf"
      {"Emor","Emor"},
#line 119 "strings_ashkenazi_litvish.gperf"
      {"Erev Purim","Erev Purim"},
      {""},
#line 69 "strings_ashkenazi_litvish.gperf"
      {"Ki Tisa","Ki-Siso"},
#line 99 "strings_ashkenazi_litvish.gperf"
      {"Vayetzei","Vayeitzei"},
      {""},
#line 161 "strings_ashkenazi_litvish.gperf"
      {"Shabbat HaGadol","Shabbos HaGodol"},
#line 70 "strings_ashkenazi_litvish.gperf"
      {"Korach","Keirach"},
      {""},
#line 13 "strings_ashkenazi_litvish.gperf"
      {"Shekalim","Shkolim"},
#line 14 "strings_ashkenazi_litvish.gperf"
      {"Yoma","Yumo"},
#line 71 "strings_ashkenazi_litvish.gperf"
      {"Lech-Lecha","Lech-Lecho"},
      {""},
#line 42 "strings_ashkenazi_litvish.gperf"
      {"Keritot","Krisos"},
      {""},
#line 143 "strings_ashkenazi_litvish.gperf"
      {"Adar","Odor"},
#line 196 "strings_ashkenazi_litvish.gperf"
      {"Yom Kippur","Yom Kippur"},
#line 144 "strings_ashkenazi_litvish.gperf"
      {"Adar I","Odor Aleph"},
#line 145 "strings_ashkenazi_litvish.gperf"
      {"Adar II","Odor Beis"},
      {""}, {""},
#line 102 "strings_ashkenazi_litvish.gperf"
      {"Vayishlach","Vayishlach"},
      {""},
#line 62 "strings_ashkenazi_litvish.gperf"
      {"Devarim","Dvorim"},
#line 173 "strings_ashkenazi_litvish.gperf"
      {"Shushan Purim","Shushon Purim"},
      {""}, {""},
#line 141 "strings_ashkenazi_litvish.gperf"
      {"Purim Katan","Purim Koton"},
#line 181 "strings_ashkenazi_litvish.gperf"
      {"Sukkot IV (CH''M)","Sukkos IV (CH''M)"},
#line 191 "strings_ashkenazi_litvish.gperf"
      {"Tzom Gedaliah","Tzom Gdalyo"},
      {""},
#line 82 "strings_ashkenazi_litvish.gperf"
      {"Re'eh","Reei"},
      {""},
#line 134 "strings_ashkenazi_litvish.gperf"
      {"Pesach IV (CH''M)","Peisach IV (CH''M)"},
#line 175 "strings_ashkenazi_litvish.gperf"
      {"Simchat Torah","Simchas Teiro"},
      {""}, {""},
#line 44 "strings_ashkenazi_litvish.gperf"
      {"Kinnim","Kinnim"},
      {""},
#line 164 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Parah","Shabbos Poro"},
      {""}, {""},
#line 61 "strings_ashkenazi_litvish.gperf"
      {"Chukat","Chukas"},
#line 40 "strings_ashkenazi_litvish.gperf"
      {"Arachin","Arochin"},
#line 65 "strings_ashkenazi_litvish.gperf"
      {"Ha'Azinu","Haazinu"},
      {""},
#line 125 "strings_ashkenazi_litvish.gperf"
      {"Erev Yom Kippur","Erev Yom Kippur"},
#line 182 "strings_ashkenazi_litvish.gperf"
      {"Sukkot V (CH''M)","Sukkos V (CH''M)"},
#line 16 "strings_ashkenazi_litvish.gperf"
      {"Beitzah","Beitzo"},
      {""},
#line 96 "strings_ashkenazi_litvish.gperf"
      {"Vayeilech","Vayeilech"},
#line 77 "strings_ashkenazi_litvish.gperf"
      {"Nasso","Nosso"},
#line 136 "strings_ashkenazi_litvish.gperf"
      {"Pesach V (CH''M)","Peisach V (CH''M)"},
#line 23 "strings_ashkenazi_litvish.gperf"
      {"Nedarim","Nedorim"},
#line 122 "strings_ashkenazi_litvish.gperf"
      {"Erev Simchat Torah","Erev Simchas Teiro"},
      {""},
#line 154 "strings_ashkenazi_litvish.gperf"
      {"Tamuz","Tamuz"},
#line 87 "strings_ashkenazi_litvish.gperf"
      {"Tazria","Tazria"},
      {""},
#line 18 "strings_ashkenazi_litvish.gperf"
      {"Megillah","Megillo"},
#line 31 "strings_ashkenazi_litvish.gperf"
      {"Sanhedrin","Sanhedrin"},
      {""},
#line 47 "strings_ashkenazi_litvish.gperf"
      {"Niddah","Niddo"},
#line 35 "strings_ashkenazi_litvish.gperf"
      {"Horayot","Heiroyos"},
      {""}, {""},
#line 28 "strings_ashkenazi_litvish.gperf"
      {"Baba Kamma","Bovo Kamo"},
#line 123 "strings_ashkenazi_litvish.gperf"
      {"Erev Sukkot","Erev Sukkos"},
      {""},
#line 156 "strings_ashkenazi_litvish.gperf"
      {"Reish Hashana","Reish Hashono"},
      {""},
#line 157 "strings_ashkenazi_litvish.gperf"
      {"Reish Hashana I","Reish Hashono I"},
#line 158 "strings_ashkenazi_litvish.gperf"
      {"Reish Hashana II","Reish Hashono II"},
      {""},
#line 66 "strings_ashkenazi_litvish.gperf"
      {"Kedoshim","Kdeishim"},
      {""}, {""},
#line 85 "strings_ashkenazi_litvish.gperf"
      {"Shmini","Shmini"},
      {""}, {""},
#line 186 "strings_ashkenazi_litvish.gperf"
      {"Ta'anit Esther","Taanis Ester"},
#line 26 "strings_ashkenazi_litvish.gperf"
      {"Gitin","Gitin"},
#line 29 "strings_ashkenazi_litvish.gperf"
      {"Baba Metzia","Bovo Metzio"},
#line 160 "strings_ashkenazi_litvish.gperf"
      {"Shabbat HaChodesh","Shabbos HaCheidesh"},
#line 100 "strings_ashkenazi_litvish.gperf"
      {"Vayigash","Vayigash"},
      {""}, {""},
#line 142 "strings_ashkenazi_litvish.gperf"
      {"Reish Chodesh %s","Reish Cheidesh %s"},
      {""},
#line 49 "strings_ashkenazi_litvish.gperf"
      {"Daf Yomi","Daf Yeimi"},
      {""}, {""}, {""}, {""},
#line 53 "strings_ashkenazi_litvish.gperf"
      {"Bamidbar","Bamidbor"},
      {""},
#line 51 "strings_ashkenazi_litvish.gperf"
      {"Achrei Mot","Achrei"},
      {""},
#line 101 "strings_ashkenazi_litvish.gperf"
      {"Vayikra","Vayikro"},
      {""}, {""},
#line 48 "strings_ashkenazi_litvish.gperf"
      {"Daf Yomi: %s %d","Daf Yeimi: %s %d"},
#line 197 "strings_ashkenazi_litvish.gperf"
      {"Yom Yerushalayim","Yom Yerusholayim"},
      {""},
#line 105 "strings_ashkenazi_litvish.gperf"
      {"Asara B'Tevet","Asoro Beteyves"},
#line 159 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Chazon","Shabbos Chazon"},
#line 79 "strings_ashkenazi_litvish.gperf"
      {"Noach","Neiach"},
      {""}, {""}, {""},
#line 27 "strings_ashkenazi_litvish.gperf"
      {"Kiddushin","Kiddushin"},
      {""},
#line 60 "strings_ashkenazi_litvish.gperf"
      {"Chayei Sara","Chayei Soro"},
      {""},
#line 107 "strings_ashkenazi_litvish.gperf"
      {"Chanukah","Chanukko"},
      {""}, {""}, {""}, {""},
#line 128 "strings_ashkenazi_litvish.gperf"
      {"Leil Selichot","Slichos"},
      {""}, {""}, {""},
#line 38 "strings_ashkenazi_litvish.gperf"
      {"Chullin","Chullin"},
#line 147 "strings_ashkenazi_litvish.gperf"
      {"Cheshvan","Cheshvon"},
      {""}, {""},
#line 194 "strings_ashkenazi_litvish.gperf"
      {"Yom HaShoah","Yom HaShoah"},
#line 162 "strings_ashkenazi_litvish.gperf"
      {"Shabbat Machar Chodesh","Shabbos Mochor Cheidesh"},
#line 108 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 1 Candle","Chanukko: 1 Candle"},
#line 114 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 7 Candles","Chanukko: 7 Candles"},
#line 127 "strings_ashkenazi_litvish.gperf"
      {"Lag BaOmer","Lag Beimer"},
#line 103 "strings_ashkenazi_litvish.gperf"
      {"Vezot Haberakhah","Zeis Habrocho"},
#line 116 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 8th Day","Chanukko: 8th Day"},
      {""},
#line 115 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 8 Candles","Chanukko: 8 Candles"},
      {""}, {""}, {""}, {""},
#line 113 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 6 Candles","Chanukko: 6 Candles"},
#line 68 "strings_ashkenazi_litvish.gperf"
      {"Ki Teitzei","Ki-Seitzei"},
      {""}, {""}, {""},
#line 112 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 5 Candles","Chanukko: 5 Candles"},
      {""}, {""}, {""},
#line 20 "strings_ashkenazi_litvish.gperf"
      {"Chagigah","Chagigo"},
#line 111 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 4 Candles","Chanukko: 4 Candles"},
      {""}, {""}, {""}, {""},
#line 110 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 3 Candles","Chanukko: 3 Candles"},
      {""}, {""}, {""}, {""},
#line 193 "strings_ashkenazi_litvish.gperf"
      {"Yom HaAtzma'ut","Yom HaAtzma'ut"},
      {""},
#line 117 "strings_ashkenazi_litvish.gperf"
      {"Days of the Omer","Eimer"},
#line 55 "strings_ashkenazi_litvish.gperf"
      {"Beha'alotcha","Bahaaloscho"},
      {""},
#line 109 "strings_ashkenazi_litvish.gperf"
      {"Chanukah: 2 Candles","Chanukko: 2 Candles"},
#line 151 "strings_ashkenazi_litvish.gperf"
      {"Nisan","Nison"},
      {""}, {""},
#line 78 "strings_ashkenazi_litvish.gperf"
      {"Nitzavim","Nitzovim"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 52 "strings_ashkenazi_litvish.gperf"
      {"Balak","Bolok"},
      {""},
#line 121 "strings_ashkenazi_litvish.gperf"
      {"Erev Shavuot","Erev Shvuos"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 34 "strings_ashkenazi_litvish.gperf"
      {"Avodah Zarah","Aveydo Zoro"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 24 "strings_ashkenazi_litvish.gperf"
      {"Nazir","Nozir"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 198 "strings_ashkenazi_litvish.gperf"
      {"Yom HaAliyah","Yom HaAliyah"},
#line 195 "strings_ashkenazi_litvish.gperf"
      {"Yom HaZikaron","Yom HaZikaron"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 94 "strings_ashkenazi_litvish.gperf"
      {"Vayakhel","Vayakhel"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 185 "strings_ashkenazi_litvish.gperf"
      {"Ta'anit Bechorot","Taanis Bchoros"},
      {""}, {""},
#line 172 "strings_ashkenazi_litvish.gperf"
      {"Shmini Atzeret","Shmini Atzeres"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 192 "strings_ashkenazi_litvish.gperf"
      {"Tzom Tammuz","Tzom Tammuz"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 106 "strings_ashkenazi_litvish.gperf"
      {"Candle lighting","Hadlokas neyros"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
