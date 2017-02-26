/* C code produced by gperf version 3.0.4 */
/* Command-line: gperf strings_fi.gperf  */
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
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 4 "strings_fi.gperf"

#include <string.h>
#line 7 "strings_fi.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 189
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 25
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 460
/* maximum key range = 457, duplicates = 0 */

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
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461,   0,  20, 461, 461, 461,   0, 461, 110,
        0, 461, 461, 461, 461, 461,   5, 461, 461,   0,
       45,  40,  35,  20,  15,  10,   0, 461, 461,   0,
      461, 461, 461, 461, 461,  75,  55,  90, 135,  15,
        5, 110,  45,   0,   0,  80,   5,  40, 140,   5,
        5,  35,  70,   0,   0,   5,  10, 165, 461, 125,
       10,   0, 461, 461, 461, 461, 461,  30,  10,  15,
        0,  35,  65,   0,  80,   5,  80,   0,  75,  55,
       65, 100,   0,   5,   0,   0,  20,   5,   0,  25,
      461,  85, 110, 125, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461, 461, 461, 461,
      461, 461, 461, 461, 461, 461, 461
    };
  register int hval = len;

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

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
struct event_title *
hebcal_lookup_fi (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 173 "strings_fi.gperf"
      {"Sigd","Sigd"},
      {""},
#line 175 "strings_fi.gperf"
      {"Sukkot","Sukot"},
#line 187 "strings_fi.gperf"
      {"Tu B'Av","Tu BeAv"},
#line 176 "strings_fi.gperf"
      {"Sukkot I","Sukot 1. päivä"},
#line 177 "strings_fi.gperf"
      {"Sukkot II","Sukot 2. päivä"},
#line 154 "strings_fi.gperf"
      {"Tevet","Tevet"},
#line 128 "strings_fi.gperf"
      {"Pesach","Pesach"},
      {""},
#line 129 "strings_fi.gperf"
      {"Pesach I","Pesach 1"},
#line 130 "strings_fi.gperf"
      {"Pesach II","Pesach 2"},
      {""},
#line 15 "strings_fi.gperf"
      {"Sukkah","Suka"},
#line 178 "strings_fi.gperf"
      {"Sukkot II (CH''M)","Sukot 2. päivä (Chol Hamoed)"},
#line 179 "strings_fi.gperf"
      {"Sukkot III (CH''M)","Sukot 3. päivä (Chol Hamoed)"},
      {""},
#line 189 "strings_fi.gperf"
      {"Tu B'Shvat","Tu Bishvat"},
      {""},
#line 131 "strings_fi.gperf"
      {"Pesach II (CH''M)","Pesach 2 (Chol Hamoed)"},
#line 132 "strings_fi.gperf"
      {"Pesach III (CH''M)","Pesach 3 (Chol Hamoed)"},
#line 147 "strings_fi.gperf"
      {"Elul","Elul"},
#line 137 "strings_fi.gperf"
      {"Pesach VII","Pesach 7"},
#line 138 "strings_fi.gperf"
      {"Pesach VIII","Pesach 8"},
#line 182 "strings_fi.gperf"
      {"Sukkot VI (CH''M)","Sukot 6. päivä (Chol Hamoed)"},
      {""}, {""},
#line 25 "strings_fi.gperf"
      {"Sotah","Sota"},
      {""},
#line 136 "strings_fi.gperf"
      {"Pesach VI (CH''M)","Pesach 6 (Chol Hamoed)"},
      {""},
#line 90 "strings_fi.gperf"
      {"Tzav","Tsav"},
#line 183 "strings_fi.gperf"
      {"Sukkot VII (Hoshana Raba)","Sukot 7. päivä (Hoshana Raba)"},
      {""},
#line 168 "strings_fi.gperf"
      {"Shavuot","Shavuot"},
      {""},
#line 169 "strings_fi.gperf"
      {"Shavuot I","Shavuot 1"},
#line 170 "strings_fi.gperf"
      {"Shavuot II","Shavuot 2"},
#line 83 "strings_fi.gperf"
      {"Shemot","Shmot"},
#line 33 "strings_fi.gperf"
      {"Shevuot","Shavuot"},
      {""}, {""},
#line 62 "strings_fi.gperf"
      {"Eikev","Eikev"},
#line 32 "strings_fi.gperf"
      {"Makkot","Makot"},
#line 79 "strings_fi.gperf"
      {"Pekudei","Pekudei"},
      {""}, {""},
#line 46 "strings_fi.gperf"
      {"Midot","Midot"},
#line 74 "strings_fi.gperf"
      {"Miketz","Mikets"},
#line 10 "strings_fi.gperf"
      {"Shabbat","Shabbat"},
#line 49 "strings_fi.gperf"
      {"Parashat","Parshat "},
      {""},
#line 70 "strings_fi.gperf"
      {"Lech-Lecha","Lech lecha"},
      {""},
#line 58 "strings_fi.gperf"
      {"Bo","Bo"},
#line 172 "strings_fi.gperf"
      {"Shushan Purim","Shushan Purim"},
      {""},
#line 91 "strings_fi.gperf"
      {"Vaera","Vaera"},
#line 43 "strings_fi.gperf"
      {"Meilah","Meila"},
#line 41 "strings_fi.gperf"
      {"Temurah","Tmura"},
#line 166 "strings_fi.gperf"
      {"Shabbat Shuva","Shabat Shuva"},
      {""},
#line 164 "strings_fi.gperf"
      {"Shabbat Rosh Chodesh","Shabat Rosh Chodesh"},
#line 186 "strings_fi.gperf"
      {"Tish'a B'Av","Tisha BeAv"},
#line 73 "strings_fi.gperf"
      {"Metzora","Metsora"},
#line 12 "strings_fi.gperf"
      {"Pesachim","Psachim"},
      {""},
#line 72 "strings_fi.gperf"
      {"Matot","Matot"},
      {""},
#line 87 "strings_fi.gperf"
      {"Terumah","Truma"},
#line 36 "strings_fi.gperf"
      {"Zevachim","Zvachim"},
#line 167 "strings_fi.gperf"
      {"Shabbat Zachor","Shabat Zachor"},
#line 139 "strings_fi.gperf"
      {"Purim","Purim"},
      {""},
#line 145 "strings_fi.gperf"
      {"Av","Av"},
#line 174 "strings_fi.gperf"
      {"Simchat Torah","Simchat Tora"},
      {""},
#line 162 "strings_fi.gperf"
      {"Shabbat Nachamu","Shabat Nechamu"},
#line 89 "strings_fi.gperf"
      {"Toldot","Toldot"},
#line 80 "strings_fi.gperf"
      {"Pinchas","Pinchas"},
#line 9 "strings_fi.gperf"
      {"Berachot","Brachot"},
      {""}, {""},
#line 69 "strings_fi.gperf"
      {"Korach","Korach "},
#line 134 "strings_fi.gperf"
      {"Pesach Sheni","Pesach Sheni"},
      {""}, {""},
#line 148 "strings_fi.gperf"
      {"Iyyar","Iar"},
      {""},
#line 42 "strings_fi.gperf"
      {"Keritot","Kritot"},
#line 163 "strings_fi.gperf"
      {"Shabbat Parah","Shabat Para"},
#line 57 "strings_fi.gperf"
      {"Beshalach","Beshalach"},
#line 45 "strings_fi.gperf"
      {"Tamid","Tamid"},
#line 165 "strings_fi.gperf"
      {"Shabbat Shekalim","Shabat Shkalim"},
#line 66 "strings_fi.gperf"
      {"Ki Tavo","Ki tavo"},
#line 39 "strings_fi.gperf"
      {"Bechorot","Bechorot"},
      {""},
#line 188 "strings_fi.gperf"
      {"Tu BiShvat","Tu Bishvat"},
#line 96 "strings_fi.gperf"
      {"Vayera","Vajera"},
#line 94 "strings_fi.gperf"
      {"Vayechi","Vajechi"},
#line 56 "strings_fi.gperf"
      {"Bereshit","Bereshit"},
      {""},
#line 152 "strings_fi.gperf"
      {"Sivan","Sivan"},
#line 11 "strings_fi.gperf"
      {"Eruvin","Eruvin"},
      {""},
#line 13 "strings_fi.gperf"
      {"Shekalim","Shkalim"},
#line 142 "strings_fi.gperf"
      {"Adar","Adar"},
#line 53 "strings_fi.gperf"
      {"Bechukotai","Bechukotai"},
#line 60 "strings_fi.gperf"
      {"Chukat","Chukat"},
#line 85 "strings_fi.gperf"
      {"Shoftim","Shoftim"},
#line 98 "strings_fi.gperf"
      {"Vayetzei","Vajetse"},
      {""},
#line 30 "strings_fi.gperf"
      {"Baba Batra","Bava batra"},
#line 17 "strings_fi.gperf"
      {"Taanit","Taanit"},
#line 40 "strings_fi.gperf"
      {"Arachin","Arachin"},
#line 88 "strings_fi.gperf"
      {"Tetzaveh","Tetsave"},
#line 63 "strings_fi.gperf"
      {"Emor","Emor"},
      {""},
#line 123 "strings_fi.gperf"
      {"Erev Tish'a B'Av","Tisha B'Av aatto "},
#line 22 "strings_fi.gperf"
      {"Ketubot","Ketubot"},
#line 97 "strings_fi.gperf"
      {"Vayeshev","Vajeshev"},
#line 75 "strings_fi.gperf"
      {"Mishpatim","Mishpatim"},
#line 71 "strings_fi.gperf"
      {"Masei","Masei"},
#line 151 "strings_fi.gperf"
      {"Sh'vat","Shvat"},
#line 82 "strings_fi.gperf"
      {"Sh'lach","Shlach "},
      {""}, {""},
#line 92 "strings_fi.gperf"
      {"Vaetchanan","Vaetchanan"},
#line 143 "strings_fi.gperf"
      {"Adar I","Adar 1"},
#line 144 "strings_fi.gperf"
      {"Adar II","Adar 2"},
#line 37 "strings_fi.gperf"
      {"Menachot","Menachot"},
      {""},
#line 19 "strings_fi.gperf"
      {"Moed Katan","Moed katan"},
#line 181 "strings_fi.gperf"
      {"Sukkot V (CH''M)","Sukot 5. päivä (Chol Hamoed)"},
#line 119 "strings_fi.gperf"
      {"Erev Rosh Hashana","Rosh Hashana aatto"},
      {""}, {""},
#line 55 "strings_fi.gperf"
      {"Behar","Behar"},
#line 135 "strings_fi.gperf"
      {"Pesach V (CH''M)","Pesach 5 (Chol Hamoed)"},
#line 61 "strings_fi.gperf"
      {"Devarim","Dvarim"},
#line 125 "strings_fi.gperf"
      {"Havdalah","Havdala"},
      {""},
#line 76 "strings_fi.gperf"
      {"Nasso","Naso"},
      {""},
#line 23 "strings_fi.gperf"
      {"Nedarim","Nedarim"},
#line 190 "strings_fi.gperf"
      {"Tzom Gedaliah","Tzom Gedalia"},
      {""},
#line 103 "strings_fi.gperf"
      {"Yitro","Jitro"},
#line 149 "strings_fi.gperf"
      {"Kislev","Kislev"},
#line 159 "strings_fi.gperf"
      {"Shabbat HaChodesh","Shabat HaChodesh"},
#line 52 "strings_fi.gperf"
      {"Bamidbar","Bamidbar"},
      {""},
#line 101 "strings_fi.gperf"
      {"Vayishlach","Vajishlach"},
#line 47 "strings_fi.gperf"
      {"Niddah","Nida"},
#line 38 "strings_fi.gperf"
      {"Chullin","Chulin"},
      {""}, {""},
#line 118 "strings_fi.gperf"
      {"Erev Purim","Purim aatto"},
#line 84 "strings_fi.gperf"
      {"Shmini","Shmini"},
#line 35 "strings_fi.gperf"
      {"Horayot","Horajot"},
#line 65 "strings_fi.gperf"
      {"Kedoshim","Kedoshim"},
      {""},
#line 124 "strings_fi.gperf"
      {"Erev Yom Kippur","Jom kippur aatto"},
      {""},
#line 68 "strings_fi.gperf"
      {"Ki Tisa","Ki Tisa"},
      {""}, {""},
#line 160 "strings_fi.gperf"
      {"Shabbat HaGadol","Shabat HaGadol"},
#line 117 "strings_fi.gperf"
      {"Erev Pesach","Pesach aatto"},
#line 155 "strings_fi.gperf"
      {"Rosh Hashana","Rosh Hashana"},
#line 121 "strings_fi.gperf"
      {"Erev Simchat Torah","Simchat Tora aatto"},
#line 156 "strings_fi.gperf"
      {"Rosh Hashana I","Rosh Hashana 1"},
#line 157 "strings_fi.gperf"
      {"Rosh Hashana II","Rosh Hashana 2"},
#line 122 "strings_fi.gperf"
      {"Erev Sukkot","Sukot aatto"},
#line 100 "strings_fi.gperf"
      {"Vayikra","Vajikra"},
      {""}, {""},
#line 78 "strings_fi.gperf"
      {"Noach","Noach"},
#line 140 "strings_fi.gperf"
      {"Purim Katan","Purim Katan"},
#line 180 "strings_fi.gperf"
      {"Sukkot IV (CH''M)","Sukot 4. päivä (Chol Hamoed)"},
#line 18 "strings_fi.gperf"
      {"Megillah","Megila"},
#line 14 "strings_fi.gperf"
      {"Yoma","Joma"},
#line 153 "strings_fi.gperf"
      {"Tamuz","Tamuz"},
#line 102 "strings_fi.gperf"
      {"Vezot Haberakhah","VeZot Habracha"},
#line 133 "strings_fi.gperf"
      {"Pesach IV (CH''M)","Pesach 4 (Chol Hamoed)"},
      {""}, {""},
#line 81 "strings_fi.gperf"
      {"Re'eh","Re'e"},
      {""},
#line 16 "strings_fi.gperf"
      {"Beitzah","Beitsa"},
      {""},
#line 27 "strings_fi.gperf"
      {"Kiddushin","Kidushin"},
#line 195 "strings_fi.gperf"
      {"Yom Kippur","Jom kipur"},
#line 86 "strings_fi.gperf"
      {"Tazria","Tazria"},
#line 21 "strings_fi.gperf"
      {"Yevamot","Jevamot"},
      {""}, {""}, {""}, {""}, {""},
#line 146 "strings_fi.gperf"
      {"Cheshvan","Cheshvan"},
#line 95 "strings_fi.gperf"
      {"Vayeilech","Vajelech"},
      {""}, {""}, {""},
#line 106 "strings_fi.gperf"
      {"Chanukah","Hanuka"},
      {""},
#line 51 "strings_fi.gperf"
      {"Balak","Balak"},
      {""}, {""}, {""}, {""},
#line 28 "strings_fi.gperf"
      {"Baba Kamma","Bava kama"},
      {""},
#line 115 "strings_fi.gperf"
      {"Chanukah: 8th Day","Hanuka 8 päivä"},
#line 107 "strings_fi.gperf"
      {"Chanukah: 1 Candle","Hanuka 1. Kynttilä"},
#line 114 "strings_fi.gperf"
      {"Chanukah: 8 Candles","Hanuka 8. Kynttilät"},
      {""}, {""},
#line 161 "strings_fi.gperf"
      {"Shabbat Machar Chodesh","Shabat Machar Chodesh"},
      {""},
#line 158 "strings_fi.gperf"
      {"Shabbat Chazon","Shabat Chazon"},
#line 126 "strings_fi.gperf"
      {"Lag BaOmer","Lag BaOmer"},
      {""}, {""}, {""},
#line 113 "strings_fi.gperf"
      {"Chanukah: 7 Candles","Hanuka 7. Kynttilä"},
#line 48 "strings_fi.gperf"
      {"Daf Yomi: %s %d","Daf jomi: %s %d"},
#line 44 "strings_fi.gperf"
      {"Kinnim","Kinim"},
      {""},
#line 77 "strings_fi.gperf"
      {"Nitzavim","Nitsavim"},
#line 112 "strings_fi.gperf"
      {"Chanukah: 6 Candles","Hanuka 6. Kynttilä"},
#line 26 "strings_fi.gperf"
      {"Gitin","Gitin"},
#line 59 "strings_fi.gperf"
      {"Chayei Sara","Chajei Sara"},
#line 120 "strings_fi.gperf"
      {"Erev Shavuot","Shavuot aatto"},
      {""},
#line 111 "strings_fi.gperf"
      {"Chanukah: 5 Candles","Hanuka 5. Kynttilä"},
#line 141 "strings_fi.gperf"
      {"Rosh Chodesh %s","Rosh Chodesh %s"},
      {""}, {""},
#line 104 "strings_fi.gperf"
      {"Asara B'Tevet","Asara BeTevet"},
#line 31 "strings_fi.gperf"
      {"Sanhedrin","Sanhedrin"},
#line 150 "strings_fi.gperf"
      {"Nisan","Nisan"},
      {""},
#line 34 "strings_fi.gperf"
      {"Avodah Zarah","Avoda zara"},
      {""},
#line 185 "strings_fi.gperf"
      {"Ta'anit Esther","Ta'anit Ester"},
      {""}, {""}, {""},
#line 93 "strings_fi.gperf"
      {"Vayakhel","Vajakel"},
#line 110 "strings_fi.gperf"
      {"Chanukah: 4 Candles","Hanuka 4. Kynttilä"},
#line 24 "strings_fi.gperf"
      {"Nazir","Nazir"},
      {""}, {""}, {""},
#line 109 "strings_fi.gperf"
      {"Chanukah: 3 Candles","Hanuka 3. Kynttilä"},
      {""},
#line 196 "strings_fi.gperf"
      {"Yom Yerushalayim","Jom Jerushalaim"},
      {""},
#line 99 "strings_fi.gperf"
      {"Vayigash","Vajigash"},
#line 108 "strings_fi.gperf"
      {"Chanukah: 2 Candles","Hanuka 2. Kynttilä"},
      {""}, {""}, {""}, {""}, {""},
#line 50 "strings_fi.gperf"
      {"Achrei Mot","Achrei mot"},
#line 29 "strings_fi.gperf"
      {"Baba Metzia","Bava metsia"},
      {""},
#line 194 "strings_fi.gperf"
      {"Yom HaZikaron","Jom Hazikaron"},
      {""}, {""}, {""}, {""},
#line 127 "strings_fi.gperf"
      {"Leil Selichot","Slichot ilta (aatto) "},
      {""}, {""}, {""}, {""}, {""},
#line 171 "strings_fi.gperf"
      {"Shmini Atzeret","Shmini Atseret"},
      {""}, {""}, {""},
#line 20 "strings_fi.gperf"
      {"Chagigah","Chagiga"},
      {""}, {""}, {""}, {""},
#line 64 "strings_fi.gperf"
      {"Ha'Azinu","Ha'azinu"},
      {""}, {""},
#line 116 "strings_fi.gperf"
      {"Days of the Omer","Omerin päivä"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 67 "strings_fi.gperf"
      {"Ki Teitzei","Ki tetse"},
#line 184 "strings_fi.gperf"
      {"Ta'anit Bechorot","Ta'anit Bechorot"},
#line 54 "strings_fi.gperf"
      {"Beha'alotcha","Behalotcha"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 193 "strings_fi.gperf"
      {"Yom HaShoah","Jom Hashoa"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 191 "strings_fi.gperf"
      {"Tzom Tammuz","Tzom Tamuz"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 192 "strings_fi.gperf"
      {"Yom HaAtzma'ut","Jom Hatsmaut"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 197 "strings_fi.gperf"
      {"Yom HaAliyah","Jom Alia"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 105 "strings_fi.gperf"
      {"Candle lighting","Kynttilöiden sytytys"}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
