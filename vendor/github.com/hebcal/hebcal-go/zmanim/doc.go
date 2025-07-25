/*
Hebcal's zmanim package calculates halachic times.
  - chatzotNight: Midnight – Chatzot.
    Sunset plus 6 halachic hours
  - alotHaShachar: Dawn – Alot haShachar.
    Sun is 16.1° below the horizon in the morning
  - misheyakir: Earliest talis & tefillin – Misheyakir.
    Sun is 11.5° below the horizon in the morning
  - misheyakirMachmir: Earliest talis & tefillin – Misheyakir Machmir.
    Sun is 10.2° below the horizon in the morning
  - dawn: Civil dawn.
    Sun is 6° below the horizon in the morning
  - sunrise: Sunrise.
    Upper edge of the Sun appears over the eastern horizon in the morning (0.833° above horizon)
  - sofZmanShma: Latest Shema (Gra).
    Sunrise plus 3 halachic hours, according to the Gra
  - sofZmanShmaMGA: Latest Shema (MGA).
    Sunrise plus 3 halachic hours, according to Magen Avraham
  - sofZmanTfilla: Latest Shacharit (Gra).
    Sunrise plus 4 halachic hours, according to the Gra
  - sofZmanTfillaMGA: Latest Shacharit (MGA).
    Sunrise plus 4 halachic hours, according to Magen Avraham
  - chatzot: Midday – Chatzot.
    Sunrise plus 6 halachic hours
  - minchaGedola: Earliest Mincha – Mincha Gedola.
    Sunrise plus 6.5 halachic hours
  - minchaKetana: Preferable earliest time to recite Minchah – Mincha Ketana.
    Sunrise plus 9.5 halachic hours
  - plagHaMincha: Plag haMincha.
    Sunrise plus 10.75 halachic hours
  - sunset: Sunset.
    When the upper edge of the Sun disappears below the horizon (0.833° below horizon).
  - dusk: Civil dusk.
    Sun is 6° below the horizon in the evening
  - Tzeit (Nightfall) by solar degrees below the horizon. Common values include:
    7.083° for when 3 medium stars are observable in the night sky with the naked eye;
    8.5° for when 3 small stars are observable in the night sky with the naked eye.
  - Tzeit (Nightfall) by fixed minutes after sunset.
    Common values include 42 minutes: when 3 medium stars are observable
    in the night sky with the naked eye;
    50 minutes: when 3 small stars are observable in the night sky with the naked eye;
    72 minutes: when 3 small stars are observable in the night sky with the naked eye.
*/
package zmanim
