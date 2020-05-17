/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf strings_ru.gperf  */
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

#line 4 "strings_ru.gperf"

#include <string.h>
#line 7 "strings_ru.gperf"
struct event_title { char *name; char *dest; };

#define TOTAL_KEYWORDS 190
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
hash (register const char *str, register size_t len)
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
        5, 110,  45,   0,   0,  80,   5,  40, 140,  10,
        5,  35,  70,   0,   0,   5,  10, 165, 461, 125,
       10,   0, 461, 461, 461, 461, 461,  30,  10,  15,
        0,  35,  65,   0,  80,   5,  80,   0,  75,  55,
       65, 100,   0,   5,   0,   0,  20,   5,   0,  60,
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
hebcal_lookup_ru (register const char *str, register size_t len)
{
  static struct event_title wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 175 "strings_ru.gperf"
      {"Sigd","Сигд"},
      {""},
#line 177 "strings_ru.gperf"
      {"Sukkot","Суккот"},
#line 189 "strings_ru.gperf"
      {"Tu B'Av","Пятнадцатое Ава"},
#line 178 "strings_ru.gperf"
      {"Sukkot I","Суккот I"},
#line 179 "strings_ru.gperf"
      {"Sukkot II","Суккот II"},
#line 156 "strings_ru.gperf"
      {"Tevet","Тевет"},
#line 129 "strings_ru.gperf"
      {"Pesach","Пэсах"},
      {""},
#line 130 "strings_ru.gperf"
      {"Pesach I","Пэсах I"},
#line 131 "strings_ru.gperf"
      {"Pesach II","Пэсах II"},
      {""},
#line 15 "strings_ru.gperf"
      {"Sukkah","Сукка"},
#line 180 "strings_ru.gperf"
      {"Sukkot II (CH''M)","Второй день Суккота (Холь Амоэд)"},
#line 181 "strings_ru.gperf"
      {"Sukkot III (CH''M)","Третий день Суккота (Холь Амоэд)"},
      {""},
#line 191 "strings_ru.gperf"
      {"Tu B'Shvat","Пятнадцатое Швата"},
      {""},
#line 132 "strings_ru.gperf"
      {"Pesach II (CH''M)","Второй день Пэсах, Холь Амоэд"},
#line 133 "strings_ru.gperf"
      {"Pesach III (CH''M)","Третий день Пэсах, Холь Амоэд"},
#line 149 "strings_ru.gperf"
      {"Elul","Элул"},
#line 138 "strings_ru.gperf"
      {"Pesach VII","Седьмой день Пэсах"},
#line 139 "strings_ru.gperf"
      {"Pesach VIII","Восьмой день Пэсах"},
#line 184 "strings_ru.gperf"
      {"Sukkot VI (CH''M)","Шестой день Суккота (Холь Амоэд)"},
      {""}, {""},
#line 25 "strings_ru.gperf"
      {"Sotah","Сота"},
      {""},
#line 137 "strings_ru.gperf"
      {"Pesach VI (CH''M)","Шестой день Пэсах, Холь Амоэд"},
      {""},
#line 91 "strings_ru.gperf"
      {"Tzav","Цав"},
#line 185 "strings_ru.gperf"
      {"Sukkot VII (Hoshana Raba)","Седьмой день Суккота (Ошана Раба)"},
      {""},
#line 170 "strings_ru.gperf"
      {"Shavuot","Шавуот"},
      {""},
#line 171 "strings_ru.gperf"
      {"Shavuot I","Шавуот I"},
#line 172 "strings_ru.gperf"
      {"Shavuot II","Шавуот II"},
#line 84 "strings_ru.gperf"
      {"Shemot","Шмот"},
#line 33 "strings_ru.gperf"
      {"Shevuot","Швуот"},
      {""}, {""}, {""},
#line 32 "strings_ru.gperf"
      {"Makkot","Макот"},
#line 80 "strings_ru.gperf"
      {"Pekudei","Пкудей"},
      {""}, {""},
#line 46 "strings_ru.gperf"
      {"Midot","Мидот"},
#line 75 "strings_ru.gperf"
      {"Miketz","Микец"},
#line 10 "strings_ru.gperf"
      {"Shabbat","Шаббат"},
#line 50 "strings_ru.gperf"
      {"Parashat","Глава"},
      {""},
#line 71 "strings_ru.gperf"
      {"Lech-Lecha","Лех-Леха"},
      {""},
#line 59 "strings_ru.gperf"
      {"Bo","Бо"},
#line 174 "strings_ru.gperf"
      {"Shushan Purim","Шушан Пурим"},
      {""},
#line 92 "strings_ru.gperf"
      {"Vaera","Ваера"},
#line 43 "strings_ru.gperf"
      {"Meilah","Меила"},
#line 41 "strings_ru.gperf"
      {"Temurah","Тмура"},
#line 168 "strings_ru.gperf"
      {"Shabbat Shuva","Шаббат Шува"},
      {""},
#line 166 "strings_ru.gperf"
      {"Shabbat Rosh Chodesh","Шаббат Новомесячья"},
#line 188 "strings_ru.gperf"
      {"Tish'a B'Av","Девятое Ава"},
#line 74 "strings_ru.gperf"
      {"Metzora","Мецора"},
#line 12 "strings_ru.gperf"
      {"Pesachim","Песахим"},
      {""},
#line 73 "strings_ru.gperf"
      {"Matot","Матот"},
      {""},
#line 88 "strings_ru.gperf"
      {"Terumah","Трума"},
#line 36 "strings_ru.gperf"
      {"Zevachim","Звахим"},
#line 169 "strings_ru.gperf"
      {"Shabbat Zachor","Шаббат Захор"},
#line 140 "strings_ru.gperf"
      {"Purim","Пурим"},
      {""},
#line 147 "strings_ru.gperf"
      {"Av","Ав"},
#line 176 "strings_ru.gperf"
      {"Simchat Torah","Симхат Тора"},
      {""},
#line 63 "strings_ru.gperf"
      {"Eikev","Эйкев"},
#line 90 "strings_ru.gperf"
      {"Toldot","Толдот"},
#line 81 "strings_ru.gperf"
      {"Pinchas","Пинхас"},
#line 9 "strings_ru.gperf"
      {"Berachot","Благословения"},
      {""},
#line 164 "strings_ru.gperf"
      {"Shabbat Nachamu","Шаббат Нахаму"},
#line 70 "strings_ru.gperf"
      {"Korach","Корах"},
#line 135 "strings_ru.gperf"
      {"Pesach Sheni","Второй Пэсах"},
      {""}, {""},
#line 150 "strings_ru.gperf"
      {"Iyyar","Ияр"},
      {""},
#line 42 "strings_ru.gperf"
      {"Keritot","Критот"},
#line 165 "strings_ru.gperf"
      {"Shabbat Parah","Шаббат Пара"},
#line 58 "strings_ru.gperf"
      {"Beshalach","Бешалах"},
#line 45 "strings_ru.gperf"
      {"Tamid","Тамид"},
#line 167 "strings_ru.gperf"
      {"Shabbat Shekalim","Шаббат Шкалим"},
#line 67 "strings_ru.gperf"
      {"Ki Tavo","Ки Таво"},
#line 39 "strings_ru.gperf"
      {"Bechorot","Бехорот"},
      {""},
#line 190 "strings_ru.gperf"
      {"Tu BiShvat","Пятнадцатое Швата"},
#line 97 "strings_ru.gperf"
      {"Vayera","Ваера"},
#line 95 "strings_ru.gperf"
      {"Vayechi","Ваехи"},
#line 57 "strings_ru.gperf"
      {"Bereshit","Берешит"},
      {""},
#line 154 "strings_ru.gperf"
      {"Sivan","Сиван"},
#line 11 "strings_ru.gperf"
      {"Eruvin","Эрувин"},
      {""},
#line 13 "strings_ru.gperf"
      {"Shekalim","Шкалим"},
#line 144 "strings_ru.gperf"
      {"Adar","Адар"},
#line 54 "strings_ru.gperf"
      {"Bechukotai","Бехукотай"},
#line 61 "strings_ru.gperf"
      {"Chukat","Хукат"},
#line 86 "strings_ru.gperf"
      {"Shoftim","Шофтим"},
#line 99 "strings_ru.gperf"
      {"Vayetzei","Ваеце"},
      {""},
#line 30 "strings_ru.gperf"
      {"Baba Batra","Баба Батра"},
#line 17 "strings_ru.gperf"
      {"Taanit","Пост"},
#line 40 "strings_ru.gperf"
      {"Arachin","Арахин"},
#line 89 "strings_ru.gperf"
      {"Tetzaveh","Тецаве"},
#line 64 "strings_ru.gperf"
      {"Emor","Эмор"},
      {""},
#line 124 "strings_ru.gperf"
      {"Erev Tish'a B'Av","Вечер Девятого Ава"},
#line 22 "strings_ru.gperf"
      {"Ketubot","Ктубот"},
#line 98 "strings_ru.gperf"
      {"Vayeshev","Ваешев"},
#line 76 "strings_ru.gperf"
      {"Mishpatim","Мишпатим"},
#line 72 "strings_ru.gperf"
      {"Masei","Масей"},
#line 153 "strings_ru.gperf"
      {"Sh'vat","Шват"},
#line 83 "strings_ru.gperf"
      {"Sh'lach","Шлах"},
      {""}, {""},
#line 93 "strings_ru.gperf"
      {"Vaetchanan","Ваитханан"},
#line 145 "strings_ru.gperf"
      {"Adar I","Адар I"},
#line 146 "strings_ru.gperf"
      {"Adar II","Адар II"},
#line 37 "strings_ru.gperf"
      {"Menachot","Менахот"},
      {""},
#line 19 "strings_ru.gperf"
      {"Moed Katan","Моэд Катан"},
#line 183 "strings_ru.gperf"
      {"Sukkot V (CH''M)","Пятый день Суккота (Холь Амоэд)"},
#line 120 "strings_ru.gperf"
      {"Erev Rosh Hashana","Вечер Рош-А-Шана"},
      {""}, {""},
#line 56 "strings_ru.gperf"
      {"Behar","Бе-ар"},
#line 136 "strings_ru.gperf"
      {"Pesach V (CH''M)","Пятый день Пэсах, Холь Амоэд"},
#line 62 "strings_ru.gperf"
      {"Devarim","Дварим"},
#line 126 "strings_ru.gperf"
      {"Havdalah","Авдала"},
      {""},
#line 77 "strings_ru.gperf"
      {"Nasso","Насо"},
      {""},
#line 23 "strings_ru.gperf"
      {"Nedarim","Недарим"},
#line 192 "strings_ru.gperf"
      {"Tzom Gedaliah","Пост Гедальи"},
      {""},
#line 104 "strings_ru.gperf"
      {"Yitro","Итро"},
#line 151 "strings_ru.gperf"
      {"Kislev","Кислев"},
#line 161 "strings_ru.gperf"
      {"Shabbat HaChodesh","Шаббат Месяца"},
#line 53 "strings_ru.gperf"
      {"Bamidbar","Бамидбар"},
      {""},
#line 102 "strings_ru.gperf"
      {"Vayishlach","Ваишлах"},
#line 47 "strings_ru.gperf"
      {"Niddah","Нида"},
#line 38 "strings_ru.gperf"
      {"Chullin","Хулин"},
      {""}, {""},
#line 119 "strings_ru.gperf"
      {"Erev Purim","Вечер Пурима"},
#line 85 "strings_ru.gperf"
      {"Shmini","Шмини"},
#line 35 "strings_ru.gperf"
      {"Horayot","Орайот"},
#line 66 "strings_ru.gperf"
      {"Kedoshim","Кдошим"},
      {""},
#line 125 "strings_ru.gperf"
      {"Erev Yom Kippur","Вечер Дня Раскаяния"},
      {""},
#line 69 "strings_ru.gperf"
      {"Ki Tisa","Ки Тиса"},
      {""}, {""},
#line 162 "strings_ru.gperf"
      {"Shabbat HaGadol","Большой Шаббат"},
#line 118 "strings_ru.gperf"
      {"Erev Pesach","Вечер Пэсаха"},
#line 157 "strings_ru.gperf"
      {"Rosh Hashana","Рош-А-Шана"},
#line 122 "strings_ru.gperf"
      {"Erev Simchat Torah","Канун Симхат Тора"},
#line 158 "strings_ru.gperf"
      {"Rosh Hashana I","Рош-А-Шана I"},
#line 159 "strings_ru.gperf"
      {"Rosh Hashana II","Рош-А-Шана II"},
#line 123 "strings_ru.gperf"
      {"Erev Sukkot","Вечер Суккота"},
#line 101 "strings_ru.gperf"
      {"Vayikra","Ваикра"},
      {""}, {""},
#line 79 "strings_ru.gperf"
      {"Noach","Ноах"},
#line 141 "strings_ru.gperf"
      {"Purim Katan","Маленький Пурим"},
#line 182 "strings_ru.gperf"
      {"Sukkot IV (CH''M)","Четвертый день Суккота (Холь Амоэд)"},
#line 18 "strings_ru.gperf"
      {"Megillah","Свиток"},
#line 14 "strings_ru.gperf"
      {"Yoma","Йома"},
#line 155 "strings_ru.gperf"
      {"Tamuz","Тамуз"},
#line 103 "strings_ru.gperf"
      {"Vezot Haberakhah","Везот Абраха"},
#line 134 "strings_ru.gperf"
      {"Pesach IV (CH''M)","Четвертый день Пэсах, Холь Амоэд"},
      {""}, {""},
#line 82 "strings_ru.gperf"
      {"Re'eh","Рэ-э"},
      {""},
#line 16 "strings_ru.gperf"
      {"Beitzah","Яйцо"},
      {""},
#line 27 "strings_ru.gperf"
      {"Kiddushin","Кидушин"},
#line 197 "strings_ru.gperf"
      {"Yom Kippur","День Раскаяния"},
#line 87 "strings_ru.gperf"
      {"Tazria","Тазриа"},
#line 21 "strings_ru.gperf"
      {"Yevamot","Евамот"},
      {""}, {""}, {""}, {""}, {""},
#line 148 "strings_ru.gperf"
      {"Cheshvan","Хешван"},
#line 96 "strings_ru.gperf"
      {"Vayeilech","Ваелех"},
      {""}, {""}, {""},
#line 107 "strings_ru.gperf"
      {"Chanukah","Ханука"},
      {""},
#line 52 "strings_ru.gperf"
      {"Balak","Балак"},
      {""}, {""}, {""}, {""},
#line 28 "strings_ru.gperf"
      {"Baba Kamma","Баба Кама"},
      {""},
#line 116 "strings_ru.gperf"
      {"Chanukah: 8th Day","Ханука: 8-ой День"},
#line 108 "strings_ru.gperf"
      {"Chanukah: 1 Candle","Ханука: 1 Свеча"},
#line 115 "strings_ru.gperf"
      {"Chanukah: 8 Candles","Ханука: 8 Свечей"},
      {""}, {""},
#line 163 "strings_ru.gperf"
      {"Shabbat Machar Chodesh","Шаббат Перед Новомесячьем"},
#line 49 "strings_ru.gperf"
      {"Daf Yomi","Страница Дня"},
#line 160 "strings_ru.gperf"
      {"Shabbat Chazon","Шаббат Хазон"},
#line 127 "strings_ru.gperf"
      {"Lag BaOmer","Лаг Ба Омер"},
      {""}, {""}, {""},
#line 114 "strings_ru.gperf"
      {"Chanukah: 7 Candles","Ханука: 7 Свечей"},
#line 48 "strings_ru.gperf"
      {"Daf Yomi: %s %d","Страница Дня: %s %d"},
#line 44 "strings_ru.gperf"
      {"Kinnim","Киним"},
      {""},
#line 78 "strings_ru.gperf"
      {"Nitzavim","Ницавим"},
#line 113 "strings_ru.gperf"
      {"Chanukah: 6 Candles","Ханука: 6 Свечей"},
#line 26 "strings_ru.gperf"
      {"Gitin","Гитин"},
#line 60 "strings_ru.gperf"
      {"Chayei Sara","Хаей Сара"},
#line 143 "strings_ru.gperf"
      {"Rosh Chodesh","Новый Месяц"},
      {""},
#line 112 "strings_ru.gperf"
      {"Chanukah: 5 Candles","Ханука: 5 Свечей"},
#line 142 "strings_ru.gperf"
      {"Rosh Chodesh %s","Новый Месяц %s"},
      {""}, {""},
#line 105 "strings_ru.gperf"
      {"Asara B'Tevet","Пост 10го Тевета"},
#line 31 "strings_ru.gperf"
      {"Sanhedrin","Сангедрин"},
#line 152 "strings_ru.gperf"
      {"Nisan","Нисан"},
      {""},
#line 34 "strings_ru.gperf"
      {"Avodah Zarah","Авода зара"},
      {""},
#line 187 "strings_ru.gperf"
      {"Ta'anit Esther","Пост Эстер"},
      {""}, {""}, {""},
#line 94 "strings_ru.gperf"
      {"Vayakhel","Ваякгель"},
#line 111 "strings_ru.gperf"
      {"Chanukah: 4 Candles","Ханука: 4 Свечи"},
#line 24 "strings_ru.gperf"
      {"Nazir","Назир"},
      {""}, {""}, {""},
#line 110 "strings_ru.gperf"
      {"Chanukah: 3 Candles","Ханука: 3 Свечи"},
      {""},
#line 198 "strings_ru.gperf"
      {"Yom Yerushalayim","День Иерусалима"},
      {""},
#line 100 "strings_ru.gperf"
      {"Vayigash","Ваигаш"},
#line 109 "strings_ru.gperf"
      {"Chanukah: 2 Candles","Ханука: 2 Свечи"},
      {""}, {""}, {""}, {""}, {""},
#line 51 "strings_ru.gperf"
      {"Achrei Mot","Ахарей Мот"},
#line 29 "strings_ru.gperf"
      {"Baba Metzia","Баба Мециа"},
#line 121 "strings_ru.gperf"
      {"Erev Shavuot","Вечер Шавуота"},
#line 196 "strings_ru.gperf"
      {"Yom HaZikaron","День Памяти"},
      {""}, {""}, {""}, {""},
#line 128 "strings_ru.gperf"
      {"Leil Selichot","Ночь слихот"},
      {""}, {""}, {""}, {""}, {""},
#line 173 "strings_ru.gperf"
      {"Shmini Atzeret","Шмини Ацерет"},
      {""}, {""}, {""},
#line 20 "strings_ru.gperf"
      {"Chagigah","Хагига"},
      {""}, {""}, {""}, {""},
#line 65 "strings_ru.gperf"
      {"Ha'Azinu","А-Азину"},
      {""}, {""},
#line 117 "strings_ru.gperf"
      {"Days of the Omer","Дней Омэра"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 68 "strings_ru.gperf"
      {"Ki Teitzei","Ки Теце"},
#line 186 "strings_ru.gperf"
      {"Ta'anit Bechorot","Пост первенцев"},
#line 55 "strings_ru.gperf"
      {"Beha'alotcha","Вегаалотха"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 195 "strings_ru.gperf"
      {"Yom HaShoah","День Катастрофы"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 193 "strings_ru.gperf"
      {"Tzom Tammuz","Пост Тамуза"},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 194 "strings_ru.gperf"
      {"Yom HaAtzma'ut","День Независимости Израиля"},
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
#line 106 "strings_ru.gperf"
      {"Candle lighting","Зажигание свечей"}
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
