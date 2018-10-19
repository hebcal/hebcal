/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf strings_pl.gperf  */
/* Computed positions: -k'1,3,5,11' */

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

#line 4 "strings_pl.gperf"

#include <string.h>
#line 7 "strings_pl.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 99
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 25
#define MIN_HASH_VALUE 6
#define MAX_HASH_VALUE 153
/* maximum key range = 148, duplicates = 0 */

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
  static unsigned char asso_values[] =
    {
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154,  35, 154, 154, 154, 154,  20, 154,  40,
      154, 154, 154, 154, 154,   5, 154, 154, 154,   0,
       30,  25,  20,  15,  10,   5,   0, 154, 154, 154,
      154, 154, 154, 154, 154,  55,  60,  75,  25,   0,
      154,  90,  15, 154, 154,  45,   5,  65, 105, 154,
       45, 154,   0,   0,  30, 154,   0, 154, 154,  55,
      154, 154, 154, 154, 154, 154, 154,  15,   0,   5,
        5,   0,  15,  45,  20,  25, 154,  35,   5,   0,
        0,  10,   0, 154,   0,   5,   0,   5,  55, 154,
      154,  35,  35, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154, 154, 154, 154, 154,
      154, 154, 154, 154, 154, 154
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[10]];
      /*FALLTHROUGH*/
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
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
hebcal_lookup_pl (register const char *str, register size_t len)
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""},
#line 41 "strings_pl.gperf"
      {"Shmini","Szemini"},
      {""}, {""}, {""}, {""}, {""},
#line 15 "strings_pl.gperf"
      {"Shevuot","Szewuot"},
      {""},
#line 98 "strings_pl.gperf"
      {"Shmini Atzeret","Szmini Aceret"},
#line 48 "strings_pl.gperf"
      {"Vaetchanan","Waetchanan"},
#line 40 "strings_pl.gperf"
      {"Shemot","Szemot"},
#line 42 "strings_pl.gperf"
      {"Shoftim","Szoftim"},
      {""}, {""},
#line 47 "strings_pl.gperf"
      {"Vaera","Waera"},
      {""},
#line 9 "strings_pl.gperf"
      {"Shabbat","Szabbat"},
#line 11 "strings_pl.gperf"
      {"Shekalim","Szekalim"},
      {""},
#line 30 "strings_pl.gperf"
      {"Lech-Lecha","Lech Lecha"},
      {""},
#line 95 "strings_pl.gperf"
      {"Shavuot","Szawuot"},
#line 90 "strings_pl.gperf"
      {"Shabbat Parah","Szabbat Parah"},
#line 96 "strings_pl.gperf"
      {"Shavuot I","Szawuot I"},
#line 97 "strings_pl.gperf"
      {"Shavuot II","Szawuot II"},
#line 92 "strings_pl.gperf"
      {"Shabbat Shekalim","Szabbat Szekalim"},
      {""},
#line 93 "strings_pl.gperf"
      {"Shabbat Shuva","Szabbat Szuva"},
#line 94 "strings_pl.gperf"
      {"Shabbat Zachor","Szabbat Zachor"},
#line 89 "strings_pl.gperf"
      {"Shabbat Nachamu","Szabbat Nachamu"},
#line 10 "strings_pl.gperf"
      {"Eruvin","Eruwin"},
#line 44 "strings_pl.gperf"
      {"Terumah","Teruma"},
#line 99 "strings_pl.gperf"
      {"Shushan Purim","Szuszan Purim"},
      {""},
#line 91 "strings_pl.gperf"
      {"Shabbat Rosh Chodesh","Szabbat Rosz Chodesz"},
#line 52 "strings_pl.gperf"
      {"Vayera","Wajera"},
#line 88 "strings_pl.gperf"
      {"Shabbat Machar Chodesh","Szabbat Machar Chodesz"},
#line 54 "strings_pl.gperf"
      {"Vayetzei","Wajece"},
#line 85 "strings_pl.gperf"
      {"Shabbat Chazon","Szabbat Chazon"},
#line 74 "strings_pl.gperf"
      {"Erev Purim","Erew Purim"},
#line 78 "strings_pl.gperf"
      {"Erev Sukkot","Erew Sukkot"},
#line 50 "strings_pl.gperf"
      {"Vayechi","Wajechi"},
#line 53 "strings_pl.gperf"
      {"Vayeshev","Wajeszew"},
#line 46 "strings_pl.gperf"
      {"Tzav","Caw"},
#line 57 "strings_pl.gperf"
      {"Vayishlach","Waiszlach"},
#line 58 "strings_pl.gperf"
      {"Vezot Haberakhah","Wezor Habracha"},
#line 82 "strings_pl.gperf"
      {"Rosh Hashana","rosz Haszana"},
#line 45 "strings_pl.gperf"
      {"Tetzaveh","Tecawe"},
#line 83 "strings_pl.gperf"
      {"Rosh Hashana I","Rosz Haszana I"},
#line 84 "strings_pl.gperf"
      {"Rosh Hashana II","Rosz Haszana II"},
      {""},
#line 76 "strings_pl.gperf"
      {"Erev Shavuot","Erew Szawuot"},
#line 17 "strings_pl.gperf"
      {"Parashat","Parsza"},
#line 12 "strings_pl.gperf"
      {"Yoma","Joma"},
#line 81 "strings_pl.gperf"
      {"Rosh Chodesh %s","Rosz Chodesz %s"},
      {""},
#line 39 "strings_pl.gperf"
      {"Sh'lach","Szelach"},
#line 27 "strings_pl.gperf"
      {"Kedoshim","Kedoszim"},
#line 14 "strings_pl.gperf"
      {"Kiddushin","Kidduszin"},
#line 38 "strings_pl.gperf"
      {"Re'eh","Ree"},
#line 73 "strings_pl.gperf"
      {"Erev Pesach","Erew Pesach"},
#line 75 "strings_pl.gperf"
      {"Erev Rosh Hashana","Erew Rosz Haszana"},
#line 77 "strings_pl.gperf"
      {"Erev Simchat Torah","Erew Simchat Tora"},
#line 51 "strings_pl.gperf"
      {"Vayeilech","Waejlech"},
#line 59 "strings_pl.gperf"
      {"Yitro","Jitro"},
      {""}, {""},
#line 21 "strings_pl.gperf"
      {"Bereshit","Bereszit"},
      {""},
#line 79 "strings_pl.gperf"
      {"Erev Yom Kippur","Erew Jom Kippur"},
      {""},
#line 56 "strings_pl.gperf"
      {"Vayikra","Wajikra"},
#line 49 "strings_pl.gperf"
      {"Vayakhel","Waejlech"},
#line 34 "strings_pl.gperf"
      {"Mishpatim","Miszpatim"},
#line 19 "strings_pl.gperf"
      {"Bechukotai","Bechukotai"},
      {""},
#line 32 "strings_pl.gperf"
      {"Metzora","Mecora"},
#line 105 "strings_pl.gperf"
      {"Yom HaZikaron","Jom HaZikaron"},
#line 102 "strings_pl.gperf"
      {"Ta'anit Esther","Post Esther"},
#line 18 "strings_pl.gperf"
      {"Achrei Mot","Achare Mot"},
      {""},
#line 24 "strings_pl.gperf"
      {"Devarim","Dewarim"},
#line 55 "strings_pl.gperf"
      {"Vayigash","Wajigasz"},
#line 22 "strings_pl.gperf"
      {"Beshalach","Beszalach"},
#line 29 "strings_pl.gperf"
      {"Ki Teitzei","Ki Tece"},
#line 101 "strings_pl.gperf"
      {"Ta'anit Bechorot","Post Bechorot"},
#line 37 "strings_pl.gperf"
      {"Pekudei","Pekude"},
#line 80 "strings_pl.gperf"
      {"Havdalah","Hawdala"},
      {""},
#line 25 "strings_pl.gperf"
      {"Eikev","Ekew"},
#line 43 "strings_pl.gperf"
      {"Tazria","Tazrija"},
      {""},
#line 26 "strings_pl.gperf"
      {"Ha'Azinu","Haazinu"},
#line 103 "strings_pl.gperf"
      {"Yom HaAtzma'ut","Jom HaAtzma'ut"},
#line 31 "strings_pl.gperf"
      {"Masei","Mase"},
#line 104 "strings_pl.gperf"
      {"Yom HaShoah","Jom HaShoah"},
#line 28 "strings_pl.gperf"
      {"Ki Tavo","Ki Tawo"},
#line 62 "strings_pl.gperf"
      {"Chanukah","Chanuka"},
      {""},
#line 100 "strings_pl.gperf"
      {"Sukkot VII (Hoshana Raba)","Sukkot VII (Hoszana Raba)"},
#line 33 "strings_pl.gperf"
      {"Miketz","Mikec"},
#line 86 "strings_pl.gperf"
      {"Shabbat HaChodesh","Szabbat HaChodesz"},
      {""}, {""},
#line 106 "strings_pl.gperf"
      {"Yom Kippur","Jom Kippur"},
#line 72 "strings_pl.gperf"
      {"Days of the Omer","Dni Omeru"},
#line 71 "strings_pl.gperf"
      {"Chanukah: 8th Day","Chanuka: Dzień ósmy"},
#line 63 "strings_pl.gperf"
      {"Chanukah: 1 Candle","Chanuka: Pierwsza świeca"},
#line 70 "strings_pl.gperf"
      {"Chanukah: 8 Candles","Chanuka: Ósma świeca"},
#line 61 "strings_pl.gperf"
      {"Candle lighting","Zapalenie świec"},
#line 23 "strings_pl.gperf"
      {"Chayei Sara","Chaje Sara"},
      {""}, {""},
#line 69 "strings_pl.gperf"
      {"Chanukah: 7 Candles","Chanuka: Siódma świeca"},
#line 87 "strings_pl.gperf"
      {"Shabbat HaGadol","Szabbat HaGadol"},
      {""}, {""},
#line 13 "strings_pl.gperf"
      {"Megillah","Megilla"},
#line 68 "strings_pl.gperf"
      {"Chanukah: 6 Candles","Chanuka: Szósta świeca"},
#line 35 "strings_pl.gperf"
      {"Nasso","Naso"},
      {""}, {""},
#line 36 "strings_pl.gperf"
      {"Nitzavim","Nicawim"},
#line 67 "strings_pl.gperf"
      {"Chanukah: 5 Candles","Chanuka: Piąta świeca"},
#line 16 "strings_pl.gperf"
      {"Daf Yomi: %s %d","Daf Yomi: %s %d"},
      {""}, {""}, {""},
#line 66 "strings_pl.gperf"
      {"Chanukah: 4 Candles","Chanuka: Czwarta świeca"},
      {""}, {""}, {""}, {""},
#line 65 "strings_pl.gperf"
      {"Chanukah: 3 Candles","Chanuka: Trzecia świeca"},
      {""},
#line 107 "strings_pl.gperf"
      {"Yom Yerushalayim","Jom Yerushalayim"},
      {""}, {""},
#line 64 "strings_pl.gperf"
      {"Chanukah: 2 Candles","Chanuka: Druga świeca"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 20 "strings_pl.gperf"
      {"Beha'alotcha","Behaalotcha"},
#line 60 "strings_pl.gperf"
      {"Asara B'Tevet","Asara B'Tevet"}
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
