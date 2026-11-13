/*
Package hebcal provides functionality for calculating Jewish holidays,
candle-lighting and havdalah times, and fast start/end times.

Hebcal incorporates and uses several related packages:

  - hdate: converts between Hebrew and Gregorian dates.
    Also includes functions for calculating personal anniversaries
    (Yahrzeit, Birthday) according to the Hebrew calendar.
  - dailylearning: a registry for daily learning schedules (Daf Yomi,
    Mishna Yomi, ...). The schedules themselves are in the separate module
    github.com/hebcal/learning; import it to enable them.
  - event: the CalEvent interface and the shared event types.
  - greg: converts between Gregorian dates and R.D. (Rata Die)
    day numbers.
  - sedra: weekly Torah reading (Parashat HaShavua).
  - zmanim: calculates halachic times.
  - locales: translations and transliterations of Jewish holiday
    names into several languages.
*/
package hebcal
