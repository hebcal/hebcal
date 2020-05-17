/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf strings_fr.gperf  */
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

#line 4 "strings_fr.gperf"

#include <string.h>
#line 7 "strings_fr.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 200
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 53
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
        0,  10, 150,  40,   5,  15,  95,   0, 466, 115,
      105,  10, 466, 466, 466, 466, 466,  10,   5,  35,
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
hebcal_lookup_fr (register const char *str, register size_t len)
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 158 "strings_fr.gperf"
      {"Elul","Eloul"},
#line 149 "strings_fr.gperf"
      {"Purim","Pourim"},
      {""},
#line 68 "strings_fr.gperf"
      {"Bo","Bo"},
#line 59 "strings_fr.gperf"
      {"Parashat","Parachah"},
      {""}, {""},
#line 138 "strings_fr.gperf"
      {"Pesach","Pessah̲"},
#line 97 "strings_fr.gperf"
      {"Terumah","Teroumah"},
#line 66 "strings_fr.gperf"
      {"Bereshit","Berechit"},
      {""},
#line 165 "strings_fr.gperf"
      {"Tevet","Tévet"},
      {""},
#line 90 "strings_fr.gperf"
      {"Pinchas","Pinh̲as"},
#line 9 "strings_fr.gperf"
      {"Berachot","Berakhot"},
#line 100 "strings_fr.gperf"
      {"Tzav","Tzav"},
#line 30 "strings_fr.gperf"
      {"Baba Batra","Baba Batra"},
#line 150 "strings_fr.gperf"
      {"Purim Katan","Pourim Katan"},
#line 41 "strings_fr.gperf"
      {"Temurah","Temourah"},
#line 12 "strings_fr.gperf"
      {"Pesachim","Pessah̲im"},
      {""},
#line 45 "strings_fr.gperf"
      {"Tamid","Tamid"},
      {""}, {""},
#line 62 "strings_fr.gperf"
      {"Bamidbar","Bamidbar"},
#line 73 "strings_fr.gperf"
      {"Emor","Emor"},
#line 28 "strings_fr.gperf"
      {"Baba Kamma","Baba Kamma"},
      {""},
#line 23 "strings_fr.gperf"
      {"Nedarim","Nedarim"},
#line 139 "strings_fr.gperf"
      {"Pesach I","Pessah̲ I"},
      {""}, {""},
#line 47 "strings_fr.gperf"
      {"Niddah","Niddah"},
      {""},
#line 51 "strings_fr.gperf"
      {"Error in input file.  Skipping line %s","Erreur dans le fichier d'entrée. Ignore la ligne %s"},
      {""},
#line 134 "strings_fr.gperf"
      {"Erev Yom Kippur","Erev Yom Kippour"},
#line 133 "strings_fr.gperf"
      {"Erev Tish'a B'Av","Erev Tich'ah beAv"},
#line 42 "strings_fr.gperf"
      {"Keritot","Keritot"},
#line 87 "strings_fr.gperf"
      {"Nitzavim","Nitzavim"},
#line 184 "strings_fr.gperf"
      {"Sigd","Sigd"},
#line 88 "strings_fr.gperf"
      {"Noach","Noah̲"},
#line 79 "strings_fr.gperf"
      {"Korach","Korah̲"},
#line 76 "strings_fr.gperf"
      {"Ki Tavo","Ki Tavo"},
#line 131 "strings_fr.gperf"
      {"Erev Simchat Torah","Erev Simh̲at Torah"},
      {""},
#line 161 "strings_fr.gperf"
      {"Nisan","Nissan"},
      {""},
#line 55 "strings_fr.gperf"
      {"Error in yahrtzeit file.  Skipping line %s","Erreur dans le fichier des yahrzeit. Ignore la ligne %s"},
      {""}, {""},
#line 25 "strings_fr.gperf"
      {"Sotah","Sotah"},
      {""},
#line 33 "strings_fr.gperf"
      {"Shevuot","Chevou'ot"},
#line 75 "strings_fr.gperf"
      {"Kedoshim","Kedochim"},
      {""},
#line 63 "strings_fr.gperf"
      {"Bechukotai","Beh̲oukotaï"},
#line 11 "strings_fr.gperf"
      {"Eruvin","Erouvin"},
#line 179 "strings_fr.gperf"
      {"Shavuot","Chavou'ot"},
#line 201 "strings_fr.gperf"
      {"Tzom Gedaliah","Tzom Guedalyah"},
#line 67 "strings_fr.gperf"
      {"Beshalach","Bechalah̲"},
#line 26 "strings_fr.gperf"
      {"Gitin","Guitin"},
      {""},
#line 144 "strings_fr.gperf"
      {"Pesach Sheni","Pessah̲ Cheni"},
#line 13 "strings_fr.gperf"
      {"Shekalim","Chekalim"},
      {""},
#line 128 "strings_fr.gperf"
      {"Erev Purim","Erev Pourim"},
      {""},
#line 95 "strings_fr.gperf"
      {"Shoftim","Choftim"},
#line 183 "strings_fr.gperf"
      {"Shushan Purim","Chouchan Pourim"},
#line 140 "strings_fr.gperf"
      {"Pesach II","Pessah̲ II"},
#line 163 "strings_fr.gperf"
      {"Sivan","Sivan"},
#line 17 "strings_fr.gperf"
      {"Taanit","Ta'anit"},
#line 22 "strings_fr.gperf"
      {"Ketubot","Ketoubot"},
      {""}, {""},
#line 199 "strings_fr.gperf"
      {"Tu BiShvat","Tou biChvat"},
#line 94 "strings_fr.gperf"
      {"Shmini","Chemini"},
#line 71 "strings_fr.gperf"
      {"Devarim","Devarim"},
#line 142 "strings_fr.gperf"
      {"Pesach III (CH''M)","Pessah̲ III (H̲''M)"},
      {""},
#line 82 "strings_fr.gperf"
      {"Matot","Mattot"},
      {""},
#line 130 "strings_fr.gperf"
      {"Erev Shavuot","Erev Chavou'ot"},
#line 185 "strings_fr.gperf"
      {"Simchat Torah","Simh̲at Torah"},
#line 27 "strings_fr.gperf"
      {"Kiddushin","Kiddouchin"},
#line 46 "strings_fr.gperf"
      {"Midot","Midot"},
#line 160 "strings_fr.gperf"
      {"Kislev","Kislav"},
#line 10 "strings_fr.gperf"
      {"Shabbat","Chabbat"},
#line 37 "strings_fr.gperf"
      {"Menachot","Menah̲ot"},
      {""},
#line 86 "strings_fr.gperf"
      {"Nasso","Nasso"},
#line 44 "strings_fr.gperf"
      {"Kinnim","Kinnim"},
#line 78 "strings_fr.gperf"
      {"Ki Tisa","Ki Tissa"},
#line 52 "strings_fr.gperf"
      {"Numeric hebrew month in input file.  Skipping line %s","Mois hébraïque numérique dans le fichier d'entrée. Ignore la ligne %s"},
#line 85 "strings_fr.gperf"
      {"Mishpatim","Michpatim"},
#line 19 "strings_fr.gperf"
      {"Moed Katan","Mo'ed Katan"},
#line 43 "strings_fr.gperf"
      {"Meilah","Me'ilah"},
#line 89 "strings_fr.gperf"
      {"Pekudei","Pekoudé"},
#line 177 "strings_fr.gperf"
      {"Shabbat Shuva","Chabbat Chouvah"},
#line 180 "strings_fr.gperf"
      {"Shavuot I","Chavou'ot I"},
#line 181 "strings_fr.gperf"
      {"Shavuot II","Chavou'ot II"},
#line 162 "strings_fr.gperf"
      {"Sh'vat","Chevat"},
#line 92 "strings_fr.gperf"
      {"Sh'lach","Chelah̲ Lekha"},
#line 174 "strings_fr.gperf"
      {"Shabbat Parah","Chabbat Parah"},
#line 196 "strings_fr.gperf"
      {"Ta'anit Esther","Ta'anit Esther"},
#line 101 "strings_fr.gperf"
      {"Vaera","Vaéra"},
#line 176 "strings_fr.gperf"
      {"Shabbat Shekalim","Chabbat Chekalim"},
#line 198 "strings_fr.gperf"
      {"Tu B'Av","Tou beAv"},
      {""}, {""},
#line 65 "strings_fr.gperf"
      {"Behar","Behar"},
#line 70 "strings_fr.gperf"
      {"Chukat","H̲oukat"},
#line 129 "strings_fr.gperf"
      {"Erev Rosh Hashana","Erev Roch Hachanah"},
#line 39 "strings_fr.gperf"
      {"Bechorot","Bekhorot"},
      {""},
#line 102 "strings_fr.gperf"
      {"Vaetchanan","Vaèth̲anan"},
#line 93 "strings_fr.gperf"
      {"Shemot","Chemot"},
#line 38 "strings_fr.gperf"
      {"Chullin","H̲oulin"},
#line 98 "strings_fr.gperf"
      {"Tetzaveh","Tétzaveh"},
      {""},
#line 200 "strings_fr.gperf"
      {"Tu B'Shvat","Tou biChvat"},
#line 127 "strings_fr.gperf"
      {"Erev Pesach","Erev Pessah̲"},
#line 21 "strings_fr.gperf"
      {"Yevamot","Yevamot"},
#line 157 "strings_fr.gperf"
      {"Cheshvan","H̲echvan"},
#line 14 "strings_fr.gperf"
      {"Yoma","Yoma"},
#line 72 "strings_fr.gperf"
      {"Eikev","Ekev"},
      {""}, {""},
#line 36 "strings_fr.gperf"
      {"Zevachim","Zevah̲im"},
#line 31 "strings_fr.gperf"
      {"Sanhedrin","Sanhédrin"},
#line 159 "strings_fr.gperf"
      {"Iyyar","Iyar"},
      {""}, {""},
#line 56 "strings_fr.gperf"
      {"Non-numeric month in yahrtzeit file. Skipping line %s","Mois non numérique dans le fichier des yahrzeit. Ignore la ligne %s"},
#line 50 "strings_fr.gperf"
      {"input file read error. Skipping line %s","erreur de lecture du fichier d'entrée. Ignore la ligne %s"},
#line 81 "strings_fr.gperf"
      {"Masei","Massé"},
#line 15 "strings_fr.gperf"
      {"Sukkah","Soukkah"},
#line 143 "strings_fr.gperf"
      {"Pesach IV (CH''M)","Pessah̲ IV (H̲''M)"},
      {""},
#line 178 "strings_fr.gperf"
      {"Shabbat Zachor","Chabbat Zakhor"},
#line 175 "strings_fr.gperf"
      {"Shabbat Rosh Chodesh","Chabbat Roch H̲odech"},
      {""}, {""}, {""}, {""},
#line 147 "strings_fr.gperf"
      {"Pesach VII","Pessah̲ VII"},
      {""},
#line 156 "strings_fr.gperf"
      {"Av","Av"},
#line 49 "strings_fr.gperf"
      {"Unable to allocate memory for holiday.","Impossible d'allouer de la mémoire pour la fête."},
      {""},
#line 171 "strings_fr.gperf"
      {"Shabbat HaGadol","Chabbat haGadol"},
#line 99 "strings_fr.gperf"
      {"Toldot","Toledot"},
#line 83 "strings_fr.gperf"
      {"Metzora","Metzora"},
      {""}, {""},
#line 61 "strings_fr.gperf"
      {"Balak","Balak"},
      {""},
#line 172 "strings_fr.gperf"
      {"Shabbat Machar Chodesh","Chabbat Mah̲ar H̲odech"},
      {""},
#line 153 "strings_fr.gperf"
      {"Adar","Adar"},
#line 173 "strings_fr.gperf"
      {"Shabbat Nachamu","Chabbat Nah̲amou"},
#line 154 "strings_fr.gperf"
      {"Adar I","Adar I"},
#line 155 "strings_fr.gperf"
      {"Adar II","Adar II"},
      {""},
#line 53 "strings_fr.gperf"
      {"Date out of range in input file. Skipping line %s","Date hors de portée dans le fichier. Ignore la ligne %s"},
#line 80 "strings_fr.gperf"
      {"Lech-Lecha","Lekh Lekha"},
#line 148 "strings_fr.gperf"
      {"Pesach VIII","Pessah̲ VIII"},
#line 166 "strings_fr.gperf"
      {"Rosh Hashana","Roch Hachanah"},
#line 57 "strings_fr.gperf"
      {"Date out of range in yahrtzeit file. Skipping line %s","Date hors de portée dans le fichier des Yahrzeit. Ignore la ligne %s"},
#line 167 "strings_fr.gperf"
      {"Rosh Hashana I","Roch Hachanah I"},
#line 168 "strings_fr.gperf"
      {"Rosh Hashana II","Roch Hachanah II"},
#line 84 "strings_fr.gperf"
      {"Miketz","Miketz"},
#line 40 "strings_fr.gperf"
      {"Arachin","Arah̲in"},
      {""},
#line 169 "strings_fr.gperf"
      {"Shabbat Chazon","Chabbat H̲azon"},
#line 136 "strings_fr.gperf"
      {"Lag BaOmer","Lag ba'Omer"},
#line 132 "strings_fr.gperf"
      {"Erev Sukkot","Erev Soukkot"},
#line 141 "strings_fr.gperf"
      {"Pesach II (CH''M)","Pessah̲ II (H̲''M)"},
      {""}, {""},
#line 164 "strings_fr.gperf"
      {"Tamuz","Tammouz"},
      {""},
#line 16 "strings_fr.gperf"
      {"Beitzah","Bétzah"},
      {""}, {""},
#line 113 "strings_fr.gperf"
      {"Yitro","Yitro"},
#line 58 "strings_fr.gperf"
      {"improper sedra year type calculated.","mauvais calcul de type d'année de sidra."},
#line 152 "strings_fr.gperf"
      {"Rosh Chodesh","Roch H̲odech"},
      {""}, {""},
#line 151 "strings_fr.gperf"
      {"Rosh Chodesh %s","Roch H̲odech %s"},
#line 29 "strings_fr.gperf"
      {"Baba Metzia","Baba Métzia"},
      {""}, {""}, {""}, {""},
#line 126 "strings_fr.gperf"
      {"Days of the Omer","Jours de l'Omer"},
      {""},
#line 18 "strings_fr.gperf"
      {"Megillah","Meguilah"},
      {""}, {""},
#line 186 "strings_fr.gperf"
      {"Sukkot","Soukkot"},
      {""},
#line 135 "strings_fr.gperf"
      {"Havdalah","Havdalah"},
      {""},
#line 24 "strings_fr.gperf"
      {"Nazir","Nazir"},
#line 106 "strings_fr.gperf"
      {"Vayera","Vayéra"},
      {""}, {""}, {""},
#line 206 "strings_fr.gperf"
      {"Yom Kippur","Yom Kippour"},
      {""},
#line 104 "strings_fr.gperf"
      {"Vayechi","Vayeh̲i"},
#line 107 "strings_fr.gperf"
      {"Vayeshev","Vayéchev"},
      {""},
#line 91 "strings_fr.gperf"
      {"Re'eh","Reéh"},
#line 145 "strings_fr.gperf"
      {"Pesach V (CH''M)","Pessah̲ V (H̲''M)"},
      {""},
#line 108 "strings_fr.gperf"
      {"Vayetzei","Vayétzé"},
      {""}, {""},
#line 197 "strings_fr.gperf"
      {"Tish'a B'Av","Tich'ah beAv"},
#line 170 "strings_fr.gperf"
      {"Shabbat HaChodesh","Chabbat haH̲odech"},
#line 187 "strings_fr.gperf"
      {"Sukkot I","Soukkot I"},
      {""},
#line 111 "strings_fr.gperf"
      {"Vayishlach","Vayichlah̲"},
#line 69 "strings_fr.gperf"
      {"Chayei Sara","H̲ayé Sarah"},
      {""},
#line 116 "strings_fr.gperf"
      {"Chanukah","H̲anoukah"},
      {""}, {""},
#line 202 "strings_fr.gperf"
      {"Tzom Tammuz","Tzom Tammouz"},
      {""}, {""}, {""}, {""},
#line 195 "strings_fr.gperf"
      {"Ta'anit Bechorot","Ta'anit Bekhorot"},
      {""},
#line 205 "strings_fr.gperf"
      {"Yom HaZikaron","Yom haZikaron"},
#line 182 "strings_fr.gperf"
      {"Shmini Atzeret","Chemini Atzéret"},
      {""},
#line 32 "strings_fr.gperf"
      {"Makkot","Makot"},
#line 125 "strings_fr.gperf"
      {"Chanukah: 8th Day","H̲anoukah: 8ème jour"},
#line 117 "strings_fr.gperf"
      {"Chanukah: 1 Candle","H̲anoukah: 1 Bougie"},
#line 124 "strings_fr.gperf"
      {"Chanukah: 8 Candles","H̲anoukah: 8 Bougies"},
#line 48 "strings_fr.gperf"
      {"Daf Yomi: %s %d","Daf Yomi: %s %d"},
#line 96 "strings_fr.gperf"
      {"Tazria","Tazri'a"},
      {""},
#line 114 "strings_fr.gperf"
      {"Asara B'Tevet","Assarah beTévet"},
#line 123 "strings_fr.gperf"
      {"Chanukah: 7 Candles","H̲anoukah: 7 Bougies"},
      {""},
#line 207 "strings_fr.gperf"
      {"Yom Yerushalayim","Yom Yerouchalayim"},
#line 146 "strings_fr.gperf"
      {"Pesach VI (CH''M)","Pessah̲ VI (H̲''M)"},
      {""},
#line 122 "strings_fr.gperf"
      {"Chanukah: 6 Candles","H̲anoukah: 6 Bougies"},
      {""}, {""}, {""},
#line 137 "strings_fr.gperf"
      {"Leil Selichot","Lel Selih̲ot"},
#line 121 "strings_fr.gperf"
      {"Chanukah: 5 Candles","H̲anoukah: 5 Bougies"},
      {""}, {""}, {""}, {""},
#line 188 "strings_fr.gperf"
      {"Sukkot II","Soukkot II"},
      {""}, {""}, {""}, {""},
#line 120 "strings_fr.gperf"
      {"Chanukah: 4 Candles","H̲anoukah: 4 Bougies"},
      {""}, {""},
#line 35 "strings_fr.gperf"
      {"Horayot","Horayot"},
#line 190 "strings_fr.gperf"
      {"Sukkot III (CH''M)","Soukkot III (H̲''M)"},
#line 119 "strings_fr.gperf"
      {"Chanukah: 3 Candles","H̲anoukah: 3 Bougies"},
      {""}, {""}, {""},
#line 20 "strings_fr.gperf"
      {"Chagigah","H̲aguigah"},
#line 118 "strings_fr.gperf"
      {"Chanukah: 2 Candles","H̲anoukah: 2 Bougies"},
      {""}, {""},
#line 110 "strings_fr.gperf"
      {"Vayikra","Vayikra"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 208 "strings_fr.gperf"
      {"Yom HaAliyah","Yom ha'Alyah"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 34 "strings_fr.gperf"
      {"Avodah Zarah","Avodah Zarah"},
#line 74 "strings_fr.gperf"
      {"Ha'Azinu","Ha'Azinou"},
#line 105 "strings_fr.gperf"
      {"Vayeilech","Vayelekh"},
      {""}, {""}, {""},
#line 54 "strings_fr.gperf"
      {"yahrtzeit file read error. Skipping line %s","Erreur de lecture du fichier des yahrzeit. Ignore la ligne %s"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 77 "strings_fr.gperf"
      {"Ki Teitzei","Ki Tétzé"},
      {""}, {""}, {""},
#line 203 "strings_fr.gperf"
      {"Yom HaAtzma'ut","Yom ha'Atzmaout"},
      {""}, {""},
#line 191 "strings_fr.gperf"
      {"Sukkot IV (CH''M)","Soukkot IV (H̲''M)"},
      {""}, {""}, {""},
#line 112 "strings_fr.gperf"
      {"Vezot Haberakhah","Vezot Haberakhah"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""},
#line 64 "strings_fr.gperf"
      {"Beha'alotcha","Beha'alotkha"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 194 "strings_fr.gperf"
      {"Sukkot VII (Hoshana Raba)","Soukkot VII (Hochanah Rabbah)"},
      {""}, {""},
#line 103 "strings_fr.gperf"
      {"Vayakhel","Vayak'hel"},
      {""},
#line 115 "strings_fr.gperf"
      {"Candle lighting","Allumage des bougies"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 189 "strings_fr.gperf"
      {"Sukkot II (CH''M)","Soukkot II (H̲''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""},
#line 192 "strings_fr.gperf"
      {"Sukkot V (CH''M)","Soukkot V (H̲''M)"},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 109 "strings_fr.gperf"
      {"Vayigash","Vayigach"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 204 "strings_fr.gperf"
      {"Yom HaShoah","Yom haShoah"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 193 "strings_fr.gperf"
      {"Sukkot VI (CH''M)","Soukkot VI (H̲''M)"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 60 "strings_fr.gperf"
      {"Achrei Mot","Ah̲aré Mot"}
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
