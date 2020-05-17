/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf strings_hu.gperf  */
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

#line 4 "strings_hu.gperf"

#include <string.h>
#line 7 "strings_hu.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 201
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 58
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 465
/* maximum key range = 462, duplicates = 0 */

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
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466,   0,   0, 466, 466, 466,   0, 466,  55,
      100, 466, 466, 466, 466, 466,  75, 466, 466,   5,
       40,  35,  30,  20,  15,  10,   5, 466, 466,   0,
      466, 466, 466, 466, 466, 150,   5, 105,  70,   0,
       10,  35,  85,  20,  40,  35,   5,  80,  20,  30,
        0,  15, 150,  40,   5,  35,  95,   0, 466, 115,
      115,  10, 466, 466, 466, 466, 466,  10,   5,  35,
        5,   5,  50,   0, 105,  10,  55,  90,  75,  10,
        0,  25,  70,   0,   0,   0,   0,   0,   5,  35,
      466, 110, 185, 165, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466, 466, 466, 466,
      466, 466, 466, 466, 466, 466, 466
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
hebcal_lookup_hu (register const char *str, register size_t len)
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 159 "strings_hu.gperf"
      {"Elul","Elul"},
#line 150 "strings_hu.gperf"
      {"Purim","Purim"},
      {""},
#line 69 "strings_hu.gperf"
      {"Bo","Bo"},
#line 60 "strings_hu.gperf"
      {"Parashat","Párását"},
      {""}, {""},
#line 139 "strings_hu.gperf"
      {"Pesach","Peszách"},
#line 98 "strings_hu.gperf"
      {"Terumah","Trumá"},
#line 67 "strings_hu.gperf"
      {"Bereshit","Bérésit"},
      {""},
#line 166 "strings_hu.gperf"
      {"Tevet","Tévét"},
      {""},
#line 91 "strings_hu.gperf"
      {"Pinchas","Pinchász"},
#line 9 "strings_hu.gperf"
      {"Berachot","Bráchot"},
#line 101 "strings_hu.gperf"
      {"Tzav","Cáv"},
#line 30 "strings_hu.gperf"
      {"Baba Batra","Bává Bátrá"},
#line 151 "strings_hu.gperf"
      {"Purim Katan","Purim Kátán"},
#line 41 "strings_hu.gperf"
      {"Temurah","Tmurá"},
#line 12 "strings_hu.gperf"
      {"Pesachim","Peszáchim"},
      {""},
#line 45 "strings_hu.gperf"
      {"Tamid","Támid"},
      {""}, {""},
#line 63 "strings_hu.gperf"
      {"Bamidbar","Bámidbár"},
#line 74 "strings_hu.gperf"
      {"Emor","Emor"},
#line 28 "strings_hu.gperf"
      {"Baba Kamma","Bává Kámá"},
      {""},
#line 23 "strings_hu.gperf"
      {"Nedarim","Nödárim"},
#line 140 "strings_hu.gperf"
      {"Pesach I","Peszách I"},
      {""}, {""},
#line 47 "strings_hu.gperf"
      {"Niddah","Nidá"},
      {""},
#line 51 "strings_hu.gperf"
      {"Error in input file.  Skipping line %s","Hiba a beviteli fájlban. Sor kihagyása %s"},
      {""},
#line 135 "strings_hu.gperf"
      {"Erev Yom Kippur","Erev Jom kipur"},
#line 134 "strings_hu.gperf"
      {"Erev Tish'a B'Av","Erev Tisá böÁv"},
#line 42 "strings_hu.gperf"
      {"Keritot","Kritot"},
#line 88 "strings_hu.gperf"
      {"Nitzavim","Nicávim"},
#line 185 "strings_hu.gperf"
      {"Sigd","Sigd"},
#line 89 "strings_hu.gperf"
      {"Noach","Noách"},
#line 80 "strings_hu.gperf"
      {"Korach","Korách"},
#line 77 "strings_hu.gperf"
      {"Ki Tavo","Ki Távó"},
#line 132 "strings_hu.gperf"
      {"Erev Simchat Torah","Erev Szimchát Tóra"},
      {""},
#line 162 "strings_hu.gperf"
      {"Nisan","Niszán"},
      {""},
#line 56 "strings_hu.gperf"
      {"Error in yahrtzeit file.  Skipping line %s","Hiba a jorcájt fájlban. Sor kihagyása %s"},
      {""}, {""},
#line 25 "strings_hu.gperf"
      {"Sotah","Szotá"},
      {""},
#line 33 "strings_hu.gperf"
      {"Shevuot","Sávuot"},
#line 76 "strings_hu.gperf"
      {"Kedoshim","Ködosim"},
      {""},
#line 64 "strings_hu.gperf"
      {"Bechukotai","Bechukotáj"},
#line 11 "strings_hu.gperf"
      {"Eruvin","Éruvin"},
#line 180 "strings_hu.gperf"
      {"Shavuot","Sávuot"},
#line 202 "strings_hu.gperf"
      {"Tzom Gedaliah","Com Gedáljá"},
#line 68 "strings_hu.gperf"
      {"Beshalach","Bösálách"},
#line 26 "strings_hu.gperf"
      {"Gitin","Gitin"},
      {""},
#line 145 "strings_hu.gperf"
      {"Pesach Sheni","Peszách séni"},
#line 13 "strings_hu.gperf"
      {"Shekalim","Skálim"},
      {""},
#line 129 "strings_hu.gperf"
      {"Erev Purim","Erev Purim"},
      {""},
#line 96 "strings_hu.gperf"
      {"Shoftim","Softim"},
      {""},
#line 141 "strings_hu.gperf"
      {"Pesach II","Peszách II"},
#line 164 "strings_hu.gperf"
      {"Sivan","Sziván"},
#line 17 "strings_hu.gperf"
      {"Taanit","Táánit"},
#line 22 "strings_hu.gperf"
      {"Ketubot","Kötubot"},
#line 184 "strings_hu.gperf"
      {"Shushan Purim","Susán purim"},
      {""},
#line 200 "strings_hu.gperf"
      {"Tu BiShvat","Tu biSvát"},
#line 95 "strings_hu.gperf"
      {"Shmini","Smini"},
#line 72 "strings_hu.gperf"
      {"Devarim","Dvárim"},
#line 143 "strings_hu.gperf"
      {"Pesach III (CH''M)","Peszách III (CH''M)"},
      {""},
#line 83 "strings_hu.gperf"
      {"Matot","Mátot"},
      {""},
#line 131 "strings_hu.gperf"
      {"Erev Shavuot","Erev Sávuot"},
      {""},
#line 27 "strings_hu.gperf"
      {"Kiddushin","Kidusin"},
#line 46 "strings_hu.gperf"
      {"Midot","Midot"},
#line 161 "strings_hu.gperf"
      {"Kislev","Kiszlév"},
#line 10 "strings_hu.gperf"
      {"Shabbat","Sábát"},
#line 37 "strings_hu.gperf"
      {"Menachot","Mönáchot"},
      {""},
#line 87 "strings_hu.gperf"
      {"Nasso","Nászo"},
#line 44 "strings_hu.gperf"
      {"Kinnim","Kinim"},
#line 79 "strings_hu.gperf"
      {"Ki Tisa","Ki Tiszá"},
#line 52 "strings_hu.gperf"
      {"Numeric hebrew month in input file.  Skipping line %s","Numerikus héber hónap a beviteli fájlban. Sor kihagyása %s"},
#line 86 "strings_hu.gperf"
      {"Mishpatim","Mispátim"},
#line 19 "strings_hu.gperf"
      {"Moed Katan","Moéd Kátán"},
#line 43 "strings_hu.gperf"
      {"Meilah","Möilá"},
#line 90 "strings_hu.gperf"
      {"Pekudei","Pekudé"},
#line 178 "strings_hu.gperf"
      {"Shabbat Shuva","Sábát Suvá"},
#line 181 "strings_hu.gperf"
      {"Shavuot I","Sávuot I"},
#line 182 "strings_hu.gperf"
      {"Shavuot II","Sávuot II"},
#line 163 "strings_hu.gperf"
      {"Sh'vat","Svát"},
#line 93 "strings_hu.gperf"
      {"Sh'lach","Slách"},
#line 186 "strings_hu.gperf"
      {"Simchat Torah","Szimchát Tóra"},
#line 197 "strings_hu.gperf"
      {"Ta'anit Esther","Táánit Eszter"},
#line 102 "strings_hu.gperf"
      {"Vaera","Vaéra"},
#line 177 "strings_hu.gperf"
      {"Shabbat Shekalim","Sábát Skálim"},
#line 199 "strings_hu.gperf"
      {"Tu B'Av","Tu böÁv"},
#line 175 "strings_hu.gperf"
      {"Shabbat Parah","Sábát Párá"},
      {""},
#line 66 "strings_hu.gperf"
      {"Behar","Böhár"},
#line 71 "strings_hu.gperf"
      {"Chukat","Chukát"},
#line 130 "strings_hu.gperf"
      {"Erev Rosh Hashana","Erev Ros hásáná"},
#line 39 "strings_hu.gperf"
      {"Bechorot","Böchorot"},
      {""},
#line 103 "strings_hu.gperf"
      {"Vaetchanan","Váetchánán"},
#line 94 "strings_hu.gperf"
      {"Shemot","Smot"},
#line 38 "strings_hu.gperf"
      {"Chullin","Chulin"},
#line 99 "strings_hu.gperf"
      {"Tetzaveh","Tecáve"},
      {""},
#line 201 "strings_hu.gperf"
      {"Tu B'Shvat","Tu biSvát"},
#line 128 "strings_hu.gperf"
      {"Erev Pesach","Erev Peszách"},
#line 21 "strings_hu.gperf"
      {"Yevamot","Jövámot"},
#line 158 "strings_hu.gperf"
      {"Cheshvan","Chesván"},
#line 14 "strings_hu.gperf"
      {"Yoma","Jomá"},
#line 73 "strings_hu.gperf"
      {"Eikev","Ékev"},
      {""}, {""}, {""},
#line 31 "strings_hu.gperf"
      {"Sanhedrin","Szánhedrin"},
#line 160 "strings_hu.gperf"
      {"Iyyar","Ijár"},
      {""}, {""},
#line 57 "strings_hu.gperf"
      {"Non-numeric month in yahrtzeit file. Skipping line %s","Nem numerikus hónap a jorcájt fájlban. Sor kihagyása %s"},
#line 50 "strings_hu.gperf"
      {"input file read error. Skipping line %s","Olvasási hiba a beviteli fájlban. Sor kihagyása %s"},
#line 82 "strings_hu.gperf"
      {"Masei","Mászé"},
#line 15 "strings_hu.gperf"
      {"Sukkah","Szukká"},
#line 144 "strings_hu.gperf"
      {"Pesach IV (CH''M)","Peszách IV (CH''M)"},
#line 36 "strings_hu.gperf"
      {"Zevachim","Zváchim"},
#line 179 "strings_hu.gperf"
      {"Shabbat Zachor","Sábát Záchor"},
#line 176 "strings_hu.gperf"
      {"Shabbat Rosh Chodesh","Sábát Ros chódes"},
      {""}, {""}, {""}, {""},
#line 148 "strings_hu.gperf"
      {"Pesach VII","Peszách VII"},
      {""},
#line 157 "strings_hu.gperf"
      {"Av","Áv"},
      {""}, {""},
#line 172 "strings_hu.gperf"
      {"Shabbat HaGadol","Sábát háGádol"},
#line 100 "strings_hu.gperf"
      {"Toldot","Toldot"},
#line 84 "strings_hu.gperf"
      {"Metzora","Möcorá"},
#line 53 "strings_hu.gperf"
      {"Unrecognized hebrew month in input file.  Skipping line %s","Ismeretlen héber hónap a beviteli fájlban. Sor kihagyása %s"},
      {""},
#line 62 "strings_hu.gperf"
      {"Balak","Bálák"},
      {""},
#line 173 "strings_hu.gperf"
      {"Shabbat Machar Chodesh","Sábát Máchár chódes"},
      {""},
#line 154 "strings_hu.gperf"
      {"Adar","Ádár"},
#line 174 "strings_hu.gperf"
      {"Shabbat Nachamu","Sábát Náchámu"},
#line 155 "strings_hu.gperf"
      {"Adar I","Ádár I"},
#line 156 "strings_hu.gperf"
      {"Adar II","Ádár II"},
      {""},
#line 54 "strings_hu.gperf"
      {"Date out of range in input file. Skipping line %s","Dátum a beviteli fájlon kívül. Sor kihagyása %s"},
#line 81 "strings_hu.gperf"
      {"Lech-Lecha","Lech Löchá"},
#line 149 "strings_hu.gperf"
      {"Pesach VIII","Peszách VIII"},
#line 167 "strings_hu.gperf"
      {"Rosh Hashana","Ros hásáná"},
#line 58 "strings_hu.gperf"
      {"Date out of range in yahrtzeit file. Skipping line %s","Jorcájton kívül eső dátum. Sor kihagyása %s"},
#line 168 "strings_hu.gperf"
      {"Rosh Hashana I","Ros hásáná I"},
#line 169 "strings_hu.gperf"
      {"Rosh Hashana II","Ros hásáná II"},
#line 85 "strings_hu.gperf"
      {"Miketz","Mikéc"},
#line 40 "strings_hu.gperf"
      {"Arachin","Áráchin"},
      {""},
#line 170 "strings_hu.gperf"
      {"Shabbat Chazon","Sábát Cházon"},
#line 137 "strings_hu.gperf"
      {"Lag BaOmer","Lág báOmer"},
#line 133 "strings_hu.gperf"
      {"Erev Sukkot","Erev Szukkot"},
#line 142 "strings_hu.gperf"
      {"Pesach II (CH''M)","Peszách II (CH''M)"},
      {""}, {""},
#line 165 "strings_hu.gperf"
      {"Tamuz","Támuz"},
      {""},
#line 16 "strings_hu.gperf"
      {"Beitzah","Bécá"},
      {""}, {""},
#line 114 "strings_hu.gperf"
      {"Yitro","Jitró"},
#line 59 "strings_hu.gperf"
      {"improper sedra year type calculated.","Nem megfelelő szidra évtípus lett kiszámítva."},
#line 153 "strings_hu.gperf"
      {"Rosh Chodesh","Ros chódes"},
      {""}, {""},
#line 152 "strings_hu.gperf"
      {"Rosh Chodesh %s","Ros chódes %s"},
#line 29 "strings_hu.gperf"
      {"Baba Metzia","Bává Möciá"},
      {""}, {""}, {""}, {""},
#line 127 "strings_hu.gperf"
      {"Days of the Omer","Az omer napjai"},
      {""},
#line 18 "strings_hu.gperf"
      {"Megillah","Mögilá"},
      {""}, {""},
#line 187 "strings_hu.gperf"
      {"Sukkot","Szukkot"},
      {""},
#line 136 "strings_hu.gperf"
      {"Havdalah","Hávdálá"},
      {""},
#line 24 "strings_hu.gperf"
      {"Nazir","Názír"},
#line 107 "strings_hu.gperf"
      {"Vayera","Váérá"},
      {""}, {""}, {""},
#line 207 "strings_hu.gperf"
      {"Yom Kippur","Jom kipur"},
      {""},
#line 105 "strings_hu.gperf"
      {"Vayechi","Vájechi"},
#line 108 "strings_hu.gperf"
      {"Vayeshev","Vájésev"},
      {""},
#line 92 "strings_hu.gperf"
      {"Re'eh","Röé"},
#line 146 "strings_hu.gperf"
      {"Pesach V (CH''M)","Peszách V (CH''M)"},
      {""},
#line 109 "strings_hu.gperf"
      {"Vayetzei","Vájécé"},
      {""}, {""},
#line 198 "strings_hu.gperf"
      {"Tish'a B'Av","Tisá böÁv"},
#line 171 "strings_hu.gperf"
      {"Shabbat HaChodesh","Sábát háChódes"},
#line 188 "strings_hu.gperf"
      {"Sukkot I","Szukkot I"},
      {""},
#line 112 "strings_hu.gperf"
      {"Vayishlach","Vájislách"},
#line 70 "strings_hu.gperf"
      {"Chayei Sara","Chájé Szárá"},
      {""},
#line 117 "strings_hu.gperf"
      {"Chanukah","Chánuká"},
      {""}, {""},
#line 203 "strings_hu.gperf"
      {"Tzom Tammuz","Com Támuz"},
      {""}, {""}, {""}, {""},
#line 196 "strings_hu.gperf"
      {"Ta'anit Bechorot","Táánit Bechorot"},
      {""},
#line 206 "strings_hu.gperf"
      {"Yom HaZikaron","Jom háZikáron"},
#line 183 "strings_hu.gperf"
      {"Shmini Atzeret","Smini áceret"},
      {""},
#line 32 "strings_hu.gperf"
      {"Makkot","Mákot"},
#line 126 "strings_hu.gperf"
      {"Chanukah: 8th Day","Chánuká: 8. nap"},
#line 118 "strings_hu.gperf"
      {"Chanukah: 1 Candle","Chánuká: 1 gyertya"},
#line 125 "strings_hu.gperf"
      {"Chanukah: 8 Candles","Chánuká: 8 gyertya"},
      {""},
#line 97 "strings_hu.gperf"
      {"Tazria","Tázriá"},
      {""},
#line 49 "strings_hu.gperf"
      {"Daf Yomi","Daf Jomi"},
#line 124 "strings_hu.gperf"
      {"Chanukah: 7 Candles","Chánuká: 7 gyertya"},
      {""}, {""},
#line 147 "strings_hu.gperf"
      {"Pesach VI (CH''M)","Peszách VI (CH''M)"},
      {""},
#line 123 "strings_hu.gperf"
      {"Chanukah: 6 Candles","Chánuká: 6 gyertya"},
#line 48 "strings_hu.gperf"
      {"Daf Yomi: %s %d","Daf Jomi: %s %d"},
      {""}, {""},
#line 138 "strings_hu.gperf"
      {"Leil Selichot","Szlichot"},
#line 122 "strings_hu.gperf"
      {"Chanukah: 5 Candles","Chánuká: 5 gyertya"},
      {""},
#line 208 "strings_hu.gperf"
      {"Yom Yerushalayim","Jom Jerusáláim"},
      {""}, {""},
#line 189 "strings_hu.gperf"
      {"Sukkot II","Szukkot II"},
      {""}, {""}, {""},
#line 115 "strings_hu.gperf"
      {"Asara B'Tevet","Ászárá böTévét"},
#line 121 "strings_hu.gperf"
      {"Chanukah: 4 Candles","Chánuká: 4 gyertya"},
      {""}, {""},
#line 35 "strings_hu.gperf"
      {"Horayot","Horájot"},
#line 191 "strings_hu.gperf"
      {"Sukkot III (CH''M)","Szukkot III (CH''M)"},
#line 120 "strings_hu.gperf"
      {"Chanukah: 3 Candles","Chánuká: 3 gyertya"},
      {""}, {""}, {""},
#line 20 "strings_hu.gperf"
      {"Chagigah","Chágigá"},
#line 119 "strings_hu.gperf"
      {"Chanukah: 2 Candles","Chánuká: 2 gyertya"},
      {""}, {""},
#line 111 "strings_hu.gperf"
      {"Vayikra","Vájikrá"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 209 "strings_hu.gperf"
      {"Yom HaAliyah","Jom háAlija"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 75 "strings_hu.gperf"
      {"Ha'Azinu","Háázinu"},
#line 106 "strings_hu.gperf"
      {"Vayeilech","Vájélech"},
      {""}, {""}, {""},
#line 55 "strings_hu.gperf"
      {"yahrtzeit file read error. Skipping line %s","Jorcájt fájl-olvasási hiba. Sor kihagyása %s"},
      {""}, {""}, {""},
#line 34 "strings_hu.gperf"
      {"Avodah Zarah","Ávodá Zárá"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 78 "strings_hu.gperf"
      {"Ki Teitzei","Ki Técé"},
      {""}, {""}, {""},
#line 204 "strings_hu.gperf"
      {"Yom HaAtzma'ut","Jom háÁcmáut"},
      {""}, {""},
#line 192 "strings_hu.gperf"
      {"Sukkot IV (CH''M)","Szukkot IV (CH''M)"},
      {""}, {""}, {""},
#line 113 "strings_hu.gperf"
      {"Vezot Haberakhah","Vözot háböráchá"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 65 "strings_hu.gperf"
      {"Beha'alotcha","Beháálotchá"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 195 "strings_hu.gperf"
      {"Sukkot VII (Hoshana Raba)","Szukkot VII (Hosáná rábá)"},
      {""}, {""},
#line 104 "strings_hu.gperf"
      {"Vayakhel","Vájákhél"},
      {""},
#line 116 "strings_hu.gperf"
      {"Candle lighting","Gyertyagyújtás"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 190 "strings_hu.gperf"
      {"Sukkot II (CH''M)","Szukkot II (CH''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 193 "strings_hu.gperf"
      {"Sukkot V (CH''M)","Szukkot V (CH''M)"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 110 "strings_hu.gperf"
      {"Vayigash","Vájigás"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 205 "strings_hu.gperf"
      {"Yom HaShoah","Jom háSoá"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 194 "strings_hu.gperf"
      {"Sukkot VI (CH''M)","Szukkot VI (CH''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 61 "strings_hu.gperf"
      {"Achrei Mot","Ácháré Mot"}
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
