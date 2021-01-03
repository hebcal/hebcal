/* C code produced by gperf version 3.0.3 */
/* Command-line: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/gperf strings_he.gperf  */
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

#line 4 "strings_he.gperf"

#include <string.h>
#line 7 "strings_he.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 231
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 32
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 446
/* maximum key range = 440, duplicates = 0 */

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
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447,  15,   5, 447, 447, 447,  10, 447,  25,
        0,  25, 447, 447, 447, 447,  80, 447, 447,  90,
       45,  10,   0,  55,  25,  20,  15, 447, 447,   0,
      447, 447, 447, 447, 447,   0,  35,  90,   0, 145,
      105, 115,  25,   0,  70, 195,  90, 245, 100,  35,
        0,  40,   5,  15,  45,  20,   5,  50, 447,  45,
      160,   0, 447, 447, 447, 447, 447,  10,  20,  65,
       15,   0,  25,  55,  25,   0, 115,   5, 100,   5,
        0,   0,  60,   0,   0,   0,  10,  85,  55,  20,
      447, 110,  30,  75, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447, 447, 447, 447,
      447, 447, 447, 447, 447, 447, 447
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
hebcal_lookup_he (str, len)
     register const char *str;
     register unsigned int len;
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 81 "strings_he.gperf"
      {"Pinchas","פִּינְחָס"},
#line 235 "strings_he.gperf"
      {"min","דקות"},
      {""},
#line 142 "strings_he.gperf"
      {"Purim","פּוּרִים"},
      {""},
#line 80 "strings_he.gperf"
      {"Pekudei","פְקוּדֵי"},
      {""},
#line 146 "strings_he.gperf"
      {"Adar","אַדָר"},
      {""}, {""},
#line 40 "strings_he.gperf"
      {"Arachin","ערכין"},
      {""}, {""}, {""},
#line 147 "strings_he.gperf"
      {"Adar I","אַדָר א׳"},
#line 148 "strings_he.gperf"
      {"Adar II","אַדָר ב׳"},
#line 132 "strings_he.gperf"
      {"Pesach I","פֶּסַח יוֹם א׳"},
#line 161 "strings_he.gperf"
      {"Rosh Hashana I","רֹאשׁ הַשָּׁנָה יוֹם א׳"},
#line 162 "strings_he.gperf"
      {"Rosh Hashana II","רֹאשׁ הַשָּׁנָה יוֹם ב׳"},
#line 85 "strings_he.gperf"
      {"Shmini","שְּׁמִינִי"},
#line 222 "strings_he.gperf"
      {"Rosh Chodesh Adar","רֹאשׁ חוֹדֶשׁ אַדָר"},
#line 228 "strings_he.gperf"
      {"Rosh Chodesh Iyyar","רֹאשׁ חוֹדֶשׁ אִיָיר"},
#line 223 "strings_he.gperf"
      {"Rosh Chodesh Adar I","רֹאשׁ חוֹדֶשׁ אַדָר א׳"},
#line 224 "strings_he.gperf"
      {"Rosh Chodesh Adar II","רֹאשׁ חוֹדֶשׁ אַדָר ב׳"},
      {""},
#line 160 "strings_he.gperf"
      {"Rosh Hashana","רֹאשׁ הַשָּׁנָה"},
#line 12 "strings_he.gperf"
      {"Pesachim","פסחים"},
      {""},
#line 144 "strings_he.gperf"
      {"Rosh Chodesh %s","רֹאשׁ חוֹדֶשׁ %s"},
      {""},
#line 59 "strings_he.gperf"
      {"Bo","בֹּא"},
#line 50 "strings_he.gperf"
      {"Parashat","פרשת"},
#line 118 "strings_he.gperf"
      {"Omer","עוֹמֶר"},
#line 92 "strings_he.gperf"
      {"Vaera","וָאֵרָא"},
      {""},
#line 137 "strings_he.gperf"
      {"Pesach Sheni","פֶּסַח שני"},
#line 232 "strings_he.gperf"
      {"Rosh Chodesh Sivan","רֹאשׁ חוֹדֶשׁ סִיוָן"},
      {""}, {""},
#line 131 "strings_he.gperf"
      {"Pesach","פֶּסַח"},
#line 145 "strings_he.gperf"
      {"Rosh Chodesh","רֹאשׁ חוֹדֶשׁ"},
#line 53 "strings_he.gperf"
      {"Bamidbar","בְּמִדְבַּר"},
#line 31 "strings_he.gperf"
      {"Sanhedrin","סנהדרין"},
#line 93 "strings_he.gperf"
      {"Vaetchanan","וָאֶתְחַנַּן"},
      {""},
#line 159 "strings_he.gperf"
      {"Tishrei","תִשְׁרֵי"},
#line 13 "strings_he.gperf"
      {"Shekalim","שקלים"},
#line 231 "strings_he.gperf"
      {"Rosh Chodesh Sh'vat","רֹאשׁ חוֹדֶשׁ שְׁבָט"},
#line 25 "strings_he.gperf"
      {"Sotah","סוטה"},
      {""},
#line 149 "strings_he.gperf"
      {"Av","אָב"},
#line 238 "strings_he.gperf"
      {"Rosh Hashanah LaBehemot","ראש השנה למעשר בהמה"},
      {""},
#line 82 "strings_he.gperf"
      {"Re'eh","רְאֵה"},
      {""},
#line 211 "strings_he.gperf"
      {"Rosh Hashana I (on Shabbat)","רֹאשׁ הַשָּׁנָה יוֹם א׳ (בְּשַׁבָּת)"},
      {""},
#line 14 "strings_he.gperf"
      {"Yoma","יומא"},
#line 56 "strings_he.gperf"
      {"Behar","בְּהַר"},
      {""},
#line 62 "strings_he.gperf"
      {"Devarim","דְּבָרִים"},
      {""}, {""},
#line 45 "strings_he.gperf"
      {"Tamid","תמיד"},
#line 15 "strings_he.gperf"
      {"Sukkah","סוכה"},
#line 35 "strings_he.gperf"
      {"Horayot","הוריות"},
#line 57 "strings_he.gperf"
      {"Bereshit","בְּרֵאשִׁית"},
      {""},
#line 156 "strings_he.gperf"
      {"Sivan","סִיוָן"},
#line 155 "strings_he.gperf"
      {"Sh'vat","שְׁבָט"},
#line 88 "strings_he.gperf"
      {"Terumah","תְּרוּמָה"},
#line 9 "strings_he.gperf"
      {"Berachot","ברכות"},
      {""},
#line 225 "strings_he.gperf"
      {"Rosh Chodesh Av","רֹאשׁ חוֹדֶשׁ אָב"},
#line 203 "strings_he.gperf"
      {"Pesach I (on Shabbat)","פֶּסַח יוֹם א׳ (בְּשַׁבָּת)"},
#line 41 "strings_he.gperf"
      {"Temurah","תמורה"},
#line 234 "strings_he.gperf"
      {"Rosh Chodesh Tevet","רֹאשׁ חוֹדֶשׁ טֵבֵת"},
      {""},
#line 28 "strings_he.gperf"
      {"Baba Kamma","בבא קמא"},
      {""},
#line 33 "strings_he.gperf"
      {"Shevuot","שבועות"},
#line 181 "strings_he.gperf"
      {"Sukkot I","סוּכּוֹת יוֹם א׳"},
#line 178 "strings_he.gperf"
      {"Sigd","סיגד"},
#line 30 "strings_he.gperf"
      {"Baba Batra","בבא בתרא"},
#line 84 "strings_he.gperf"
      {"Shemot","שְׁמוֹת"},
#line 83 "strings_he.gperf"
      {"Sh'lach","שְׁלַח־לְךָ"},
#line 179 "strings_he.gperf"
      {"Simchat Torah","שִׂמְחַת תּוֹרָה"},
#line 133 "strings_he.gperf"
      {"Pesach II","פֶּסַח יוֹם ב׳"},
      {""},
#line 180 "strings_he.gperf"
      {"Sukkot","סוּכּוֹת"},
#line 173 "strings_he.gperf"
      {"Shavuot","שָׁבוּעוֹת"},
#line 150 "strings_he.gperf"
      {"Cheshvan","חֶשְׁוָן"},
      {""},
#line 140 "strings_he.gperf"
      {"Pesach VII","פֶּסַח יוֹם ז׳"},
#line 141 "strings_he.gperf"
      {"Pesach VIII","פֶּסַח יוֹם ח׳"},
#line 209 "strings_he.gperf"
      {"Pesach Shabbat Chol ha-Moed","פֶּסַח שַׁבָּת חוֹל הַמוֹעֵד"},
#line 233 "strings_he.gperf"
      {"Rosh Chodesh Tamuz","רֹאשׁ חוֹדֶשׁ תָּמוּז"},
      {""},
#line 154 "strings_he.gperf"
      {"Nisan","נִיסָן"},
#line 143 "strings_he.gperf"
      {"Purim Katan","פּוּרִים קָטָן"},
#line 10 "strings_he.gperf"
      {"Shabbat","שַׁבָּת"},
      {""}, {""}, {""}, {""},
#line 86 "strings_he.gperf"
      {"Shoftim","שׁוֹפְטִים"},
      {""},
#line 91 "strings_he.gperf"
      {"Tzav","צַו"},
#line 152 "strings_he.gperf"
      {"Iyyar","אִיָיר"},
      {""},
#line 21 "strings_he.gperf"
      {"Yevamot","יבמות"},
      {""}, {""},
#line 104 "strings_he.gperf"
      {"Yitro","יִתְרוֹ"},
#line 226 "strings_he.gperf"
      {"Rosh Chodesh Cheshvan","רֹאשׁ חוֹדֶשׁ חֶשְׁוָן"},
#line 192 "strings_he.gperf"
      {"Tu B'Av","טוּ בְּאָב"},
#line 195 "strings_he.gperf"
      {"Tzom Gedaliah","צוֹם גְּדַלְיָה"},
      {""}, {""},
#line 196 "strings_he.gperf"
      {"Tzom Tammuz","צוֹם תָּמוּז"},
#line 23 "strings_he.gperf"
      {"Nedarim","נדרים"},
#line 230 "strings_he.gperf"
      {"Rosh Chodesh Nisan","רֹאשׁ חוֹדֶשׁ נִיסָן"},
#line 163 "strings_he.gperf"
      {"Shabbat Chazon","שַׁבָּת חֲזוֹן"},
#line 26 "strings_he.gperf"
      {"Gitin","גיטין"},
#line 97 "strings_he.gperf"
      {"Vayera","וַיֵּרָא"},
#line 136 "strings_he.gperf"
      {"Pesach IV (CH''M)","פֶּסַח יוֹם ד׳ (חוֹל הַמוֹעֵד)"},
#line 89 "strings_he.gperf"
      {"Tetzaveh","תְּצַוֶּה"},
      {""},
#line 24 "strings_he.gperf"
      {"Nazir","נזיר"},
      {""},
#line 95 "strings_he.gperf"
      {"Vayechi","וַיְחִי"},
      {""},
#line 176 "strings_he.gperf"
      {"Shmini Atzeret","שְׁמִינִי עֲצֶרֶת"},
#line 79 "strings_he.gperf"
      {"Noach","נֹחַ"},
      {""},
#line 16 "strings_he.gperf"
      {"Beitzah","ביצה"},
      {""},
#line 237 "strings_he.gperf"
      {"Fast ends","סיום הצום"},
      {""},
#line 215 "strings_he.gperf"
      {"Sukkot I (on Shabbat)","סוּכּוֹת יוֹם א׳ (בְּשַׁבָּת)"},
      {""},
#line 78 "strings_he.gperf"
      {"Nitzavim","נִצָּבִים"},
#line 64 "strings_he.gperf"
      {"Emor","אֱמוֹר"},
      {""}, {""},
#line 134 "strings_he.gperf"
      {"Pesach II (CH''M)","פֶּסַח יוֹם ב׳ (חוֹל הַמוֹעֵד)"},
#line 105 "strings_he.gperf"
      {"Asara B'Tevet","עֲשָׂרָה בְּטֵבֵת"},
#line 58 "strings_he.gperf"
      {"Beshalach","בְּשַׁלַּח"},
#line 194 "strings_he.gperf"
      {"Tu B'Shvat","טוּ בִּשְׁבָט"},
      {""},
#line 139 "strings_he.gperf"
      {"Pesach VI (CH''M)","פֶּסַח יוֹם ו׳ (חוֹל הַמוֹעֵד)"},
#line 128 "strings_he.gperf"
      {"Havdalah","הַבדָלָה"},
#line 182 "strings_he.gperf"
      {"Sukkot II","סוּכּוֹת יוֹם ב׳"},
#line 157 "strings_he.gperf"
      {"Tamuz","תַּמּוּז"},
      {""}, {""},
#line 199 "strings_he.gperf"
      {"Yom HaZikaron","יוֹם הַזִּכָּרוֹן"},
      {""},
#line 77 "strings_he.gperf"
      {"Nasso","נָשׂא"},
#line 47 "strings_he.gperf"
      {"Niddah","נדה"},
#line 220 "strings_he.gperf"
      {"Sukkot Shabbat Chol ha-Moed","סוּכּוֹת שַׁבָּת חוֹל הַמוֹעֵד"},
#line 135 "strings_he.gperf"
      {"Pesach III (CH''M)","פֶּסַח יוֹם ג׳ (חוֹל הַמוֹעֵד)"},
      {""}, {""}, {""}, {""},
#line 177 "strings_he.gperf"
      {"Shushan Purim","שׁוּשָׁן פּוּרִים"},
#line 174 "strings_he.gperf"
      {"Shavuot I","שָׁבוּעוֹת יוֹם א׳"},
#line 175 "strings_he.gperf"
      {"Shavuot II","שָׁבוּעוֹת יוֹם ב׳"},
#line 29 "strings_he.gperf"
      {"Baba Metzia","בבא מציעא"},
      {""}, {""},
#line 197 "strings_he.gperf"
      {"Yom HaAtzma'ut","יוֹם הָעַצְמָאוּת"},
      {""},
#line 138 "strings_he.gperf"
      {"Pesach V (CH''M)","פֶּסַח יוֹם ה׳ (חוֹל הַמוֹעֵד)"},
#line 239 "strings_he.gperf"
      {"Tish'a B'Av (observed)","תִּשְׁעָה בְּאָב נִדחֶה"},
#line 168 "strings_he.gperf"
      {"Shabbat Parah","שַׁבָּת פּרה"},
#line 172 "strings_he.gperf"
      {"Shabbat Zachor","שַׁבָּת זָכוֹר"},
#line 102 "strings_he.gperf"
      {"Vayishlach","וַיִּשְׁלַח"},
#line 17 "strings_he.gperf"
      {"Taanit","תענית"},
#line 38 "strings_he.gperf"
      {"Chullin","חולין"},
#line 39 "strings_he.gperf"
      {"Bechorot","בכורות"},
      {""}, {""},
#line 60 "strings_he.gperf"
      {"Chayei Sara","חַיֵּי שָֹרָה"},
#line 55 "strings_he.gperf"
      {"Beha'alotcha","בְּהַעֲלֹתְךָ"},
#line 49 "strings_he.gperf"
      {"Daf Yomi","דף יומי"},
      {""},
#line 54 "strings_he.gperf"
      {"Bechukotai","בְּחֻקֹּתַי"},
#line 198 "strings_he.gperf"
      {"Yom HaShoah","יוֹם הַשּׁוֹאָה"},
#line 185 "strings_he.gperf"
      {"Sukkot IV (CH''M)","סוּכּוֹת יוֹם ד׳ (חוֹל הַמוֹעֵד)"},
#line 100 "strings_he.gperf"
      {"Vayigash","וַיִּגַּשׁ"},
      {""},
#line 158 "strings_he.gperf"
      {"Tevet","טֵבֵת"},
#line 191 "strings_he.gperf"
      {"Tish'a B'Av","תִּשְׁעָה בְּאָב"},
#line 123 "strings_he.gperf"
      {"Erev Shavuot","עֶרֶב שָׁבוּעוֹת"},
      {""}, {""}, {""},
#line 87 "strings_he.gperf"
      {"Tazria","תַזְרִיעַ"},
      {""},
#line 99 "strings_he.gperf"
      {"Vayetzei","וַיֵּצֵא"},
#line 111 "strings_he.gperf"
      {"Chanukah: 4 Candles","חֲנוּכָּה: ד׳ נֵרוֹת"},
      {""},
#line 61 "strings_he.gperf"
      {"Chukat","חֻקַּת"},
      {""},
#line 107 "strings_he.gperf"
      {"Chanukah","חֲנוּכָּה"},
#line 190 "strings_he.gperf"
      {"Ta'anit Esther","תַּעֲנִית אֶסְתֵּר"},
      {""}, {""},
#line 183 "strings_he.gperf"
      {"Sukkot II (CH''M)","סוּכּוֹת יוֹם ב׳ (חוֹל הַמוֹעֵד)"},
      {""},
#line 110 "strings_he.gperf"
      {"Chanukah: 3 Candles","חֲנוּכָּה: ג׳ נֵרוֹת"},
#line 188 "strings_he.gperf"
      {"Sukkot VII (Hoshana Raba)","סוּכּוֹת יוֹם ז׳ (הוֹשַׁעְנָא רַבָּה)"},
#line 90 "strings_he.gperf"
      {"Toldot","תּוֹלְדוֹת"},
#line 187 "strings_he.gperf"
      {"Sukkot VI (CH''M)","סוּכּוֹת יוֹם ו׳ (חוֹל הַמוֹעֵד)"},
      {""},
#line 115 "strings_he.gperf"
      {"Chanukah: 8 Candles","חֲנוּכָּה: ח׳ נֵרוֹת"},
#line 119 "strings_he.gperf"
      {"day of the Omer","בָּעוֹמֶר"},
      {""},
#line 122 "strings_he.gperf"
      {"Erev Rosh Hashana","עֶרֶב רֹאשׁ הַשָּׁנָה"},
#line 210 "strings_he.gperf"
      {"Shavuot II (on Shabbat)","שָׁבוּעוֹת יוֹם ב׳ (בְּשַׁבָּת)"},
#line 114 "strings_he.gperf"
      {"Chanukah: 7 Candles","חֲנוּכָּה: ז׳ נֵרוֹת"},
#line 63 "strings_he.gperf"
      {"Eikev","עֵקֶב"},
#line 120 "strings_he.gperf"
      {"Erev Pesach","עֶרֶב פֶּסַח"},
#line 101 "strings_he.gperf"
      {"Vayikra","וַיִּקְרָא"},
#line 184 "strings_he.gperf"
      {"Sukkot III (CH''M)","סוּכּוֹת יוֹם ג׳ (חוֹל הַמוֹעֵד)"},
#line 113 "strings_he.gperf"
      {"Chanukah: 6 Candles","חֲנוּכָּה: ו׳ נֵרוֹת"},
#line 48 "strings_he.gperf"
      {"Daf Yomi: %s %d","דף יומי: %s %d"},
#line 117 "strings_he.gperf"
      {"Days of the Omer","עוֹמֶר"},
#line 67 "strings_he.gperf"
      {"Ki Tavo","כִּי־תָבוֹא"},
#line 66 "strings_he.gperf"
      {"Kedoshim","קְדשִׁים"},
      {""},
#line 207 "strings_he.gperf"
      {"Pesach Chol ha-Moed Day 4","פֶּסַח חוֹל הַמוֹעֵד יוֹם ד׳"},
#line 70 "strings_he.gperf"
      {"Korach","קוֹרַח"},
      {""},
#line 98 "strings_he.gperf"
      {"Vayeshev","וַיֵּשֶׁב"},
      {""},
#line 52 "strings_he.gperf"
      {"Balak","בָּלָק"},
#line 186 "strings_he.gperf"
      {"Sukkot V (CH''M)","סוּכּוֹת יוֹם ה׳ (חוֹל הַמוֹעֵד)"},
#line 34 "strings_he.gperf"
      {"Avodah Zarah","עבודה זרה"},
#line 36 "strings_he.gperf"
      {"Zevachim","זבחים"},
      {""},
#line 206 "strings_he.gperf"
      {"Pesach Chol ha-Moed Day 3","פֶּסַח חוֹל הַמוֹעֵד יוֹם ג׳"},
      {""},
#line 164 "strings_he.gperf"
      {"Shabbat HaChodesh","שַׁבָּת הַחֹדֶשׁ"},
      {""},
#line 109 "strings_he.gperf"
      {"Chanukah: 2 Candles","חֲנוּכָּה: ב׳ נֵרוֹת"},
#line 169 "strings_he.gperf"
      {"Shabbat Rosh Chodesh","שַׁבָּת רֹאשׁ חוֹדֶשׁ"},
#line 126 "strings_he.gperf"
      {"Erev Tish'a B'Av","עֶרֶב תִּשְׁעָה בְּאָב"},
      {""},
#line 171 "strings_he.gperf"
      {"Shabbat Shuva","שַׁבָּת שׁוּבָה"},
#line 76 "strings_he.gperf"
      {"Mishpatim","מִּשְׁפָּטִים"},
#line 127 "strings_he.gperf"
      {"Erev Yom Kippur","עֶרֶב יוֹם כִּפּוּר"},
#line 103 "strings_he.gperf"
      {"Vezot Haberakhah","וְזֹאת הַבְּרָכָה"},
      {""}, {""},
#line 112 "strings_he.gperf"
      {"Chanukah: 5 Candles","חֲנוּכָּה: ה׳ נֵרוֹת"},
#line 200 "strings_he.gperf"
      {"Yom Kippur","יוֹם כִּפּוּר"},
#line 189 "strings_he.gperf"
      {"Ta'anit Bechorot","תַּעֲנִית בְּכוֹרוֹת"},
      {""},
#line 124 "strings_he.gperf"
      {"Erev Simchat Torah","עֶרֶב שִׂמְחַת תּוֹרָה"},
      {""},
#line 193 "strings_he.gperf"
      {"Tu BiShvat","טוּ בִּשְׁבָט"},
#line 170 "strings_he.gperf"
      {"Shabbat Shekalim","שַׁבָּת שְׁקָלִים"},
#line 227 "strings_he.gperf"
      {"Rosh Chodesh Elul","רֹאשׁ חוֹדֶשׁ אֱלוּל"},
#line 20 "strings_he.gperf"
      {"Chagigah","חגיגה"},
#line 27 "strings_he.gperf"
      {"Kiddushin","קידושין"},
#line 129 "strings_he.gperf"
      {"Lag BaOmer","ל״ג בָּעוֹמֶר"},
      {""}, {""}, {""},
#line 229 "strings_he.gperf"
      {"Rosh Chodesh Kislev","רֹאשׁ חוֹדֶשׁ כִּסְלֵו"},
#line 121 "strings_he.gperf"
      {"Erev Purim","עֶרֶב פּוּרִים"},
#line 153 "strings_he.gperf"
      {"Kislev","כִּסְלֵו"},
      {""}, {""}, {""},
#line 205 "strings_he.gperf"
      {"Pesach Chol ha-Moed Day 2","פֶּסַח חוֹל הַמוֹעֵד יוֹם ב׳"},
#line 125 "strings_he.gperf"
      {"Erev Sukkot","עֶרֶב סוּכּוֹת"},
#line 22 "strings_he.gperf"
      {"Ketubot","כתובות"},
#line 37 "strings_he.gperf"
      {"Menachot","מנחות"},
      {""},
#line 19 "strings_he.gperf"
      {"Moed Katan","מועד קטן"},
#line 236 "strings_he.gperf"
      {"Fast begins","תחילת הצום"},
      {""}, {""}, {""},
#line 208 "strings_he.gperf"
      {"Pesach Chol ha-Moed Day 5","פֶּסַח חוֹל הַמוֹעֵד יוֹם ה׳"},
#line 43 "strings_he.gperf"
      {"Meilah","מעילה"},
#line 42 "strings_he.gperf"
      {"Keritot","כריתות"},
#line 108 "strings_he.gperf"
      {"Chanukah: 1 Candle","חֲנוּכָּה: א׳ נֵר"},
      {""},
#line 68 "strings_he.gperf"
      {"Ki Teitzei","כִּי־תֵצֵא"},
      {""}, {""},
#line 65 "strings_he.gperf"
      {"Ha'Azinu","הַאֲזִינוּ"},
      {""},
#line 219 "strings_he.gperf"
      {"Sukkot Chol ha-Moed Day 4","סוּכּוֹת יוֹם ו׳ (חוֹל הַמוֹעֵד)"},
      {""}, {""}, {""}, {""},
#line 167 "strings_he.gperf"
      {"Shabbat Nachamu","שַׁבָּת נַחֲמוּ"},
      {""},
#line 202 "strings_he.gperf"
      {"Yom HaAliyah","יוֹם העלייה"},
      {""}, {""},
#line 218 "strings_he.gperf"
      {"Sukkot Chol ha-Moed Day 3","סוּכּוֹת יוֹם ה׳ (חוֹל הַמוֹעֵד)"},
      {""},
#line 166 "strings_he.gperf"
      {"Shabbat Machar Chodesh","שַׁבָּת מָחָר חוֹדֶשׁ"},
#line 212 "strings_he.gperf"
      {"Yom Kippur (on Shabbat)","יוֹם כִּפּוּר (בְּשַׁבָּת)"},
      {""},
#line 71 "strings_he.gperf"
      {"Lech-Lecha","לֶךְ־לְךָ"},
#line 44 "strings_he.gperf"
      {"Kinnim","קינים"},
      {""}, {""}, {""},
#line 214 "strings_he.gperf"
      {"Yom Kippur (Mincha, Alternate)","יוֹם כִּפּוּר מנחה"},
#line 32 "strings_he.gperf"
      {"Makkot","מכות"},
#line 213 "strings_he.gperf"
      {"Yom Kippur (Mincha, Traditional)","יוֹם כִּפּוּר מנחה"},
      {""},
#line 96 "strings_he.gperf"
      {"Vayeilech","וַיֵּלֶךְ"},
#line 204 "strings_he.gperf"
      {"Pesach Chol ha-Moed Day 1","פֶּסַח חוֹל הַמוֹעֵד יוֹם א׳"},
      {""},
#line 74 "strings_he.gperf"
      {"Metzora","מְּצֹרָע"},
      {""},
#line 151 "strings_he.gperf"
      {"Elul","אֱלוּל"},
#line 165 "strings_he.gperf"
      {"Shabbat HaGadol","שַׁבָּת הַגָּדוֹל"},
      {""},
#line 116 "strings_he.gperf"
      {"Chanukah: 8th Day","חֲנוּכָּה: יוֹם ח׳"},
      {""}, {""}, {""}, {""},
#line 69 "strings_he.gperf"
      {"Ki Tisa","כִּי תִשָּׂא"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 217 "strings_he.gperf"
      {"Sukkot Chol ha-Moed Day 2","סוּכּוֹת יוֹם ד׳ (חוֹל הַמוֹעֵד)"},
#line 11 "strings_he.gperf"
      {"Eruvin","עירובין"},
      {""}, {""}, {""},
#line 73 "strings_he.gperf"
      {"Matot","מַּטּוֹת"},
      {""}, {""},
#line 130 "strings_he.gperf"
      {"Leil Selichot","סליחות"},
      {""},
#line 46 "strings_he.gperf"
      {"Midot","מדות"},
      {""}, {""},
#line 18 "strings_he.gperf"
      {"Megillah","מגילה"},
      {""},
#line 72 "strings_he.gperf"
      {"Masei","מַסְעֵי"},
#line 221 "strings_he.gperf"
      {"Sukkot Final Day (Hoshana Raba)","סוּכּוֹת יוֹם ז׳ (הוֹשַׁעְנָא רַבָּה)"},
      {""}, {""}, {""}, {""},
#line 75 "strings_he.gperf"
      {"Miketz","מִקֵּץ"},
      {""}, {""}, {""},
#line 51 "strings_he.gperf"
      {"Achrei Mot","אַחֲרֵי מוֹת"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 216 "strings_he.gperf"
      {"Sukkot Chol ha-Moed Day 1","סוּכּוֹת יוֹם ג׳ (חוֹל הַמוֹעֵד)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 106 "strings_he.gperf"
      {"Candle lighting","הַדלָקָת נֵרוֹת"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 94 "strings_he.gperf"
      {"Vayakhel","וַיַּקְהֵל"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 201 "strings_he.gperf"
      {"Yom Yerushalayim","יוֹם יְרוּשָׁלַיִם"}
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
