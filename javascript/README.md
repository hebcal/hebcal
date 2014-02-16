# Hebcal JavaScript

Original C written by Danny Sadinoff, with portions by Michael J. Radwin.
2014 JavaScript port by Eyal Schachter (age 15).

## Description

Hebcal JS is a JavaScript API to aid working with the Hebrew calendar. It is available for both browser and Node.JS.

It can convert between Hebrew and Gregorian dates, get weekly sedra, holidays, and halachik times (_zemanim_).

You can include it in a client-side (browser) script with the `client.min.js` script in this directory. It exposes a global variable, `Hebcal`. For Node, install `hebcal` from NPM...

```sh
$ npm install hebcal
```

...and include Hebcal with `var Hebcal = require('hebcal');`

Hebcal JS is currently at version 2.0. It is approximately equivalent to Hebcal C 3.14.
I (Eyal) did not write Hebcal JS 1.0. 2.0 is a nearly-complete rewrite of it.

## Contributors

Hebcal JS was ported from C by Eyal Schachter.

The original C and JavaScript code was written by Danny Sadinoff, with contributions by Michael J. Radwin.

## License

Copyright (C) 1994-2011 Danny Sadinoff
Portions Copyright (c) 2011 Michael J. Radwin. All Rights Reserved.

Hebcal JavaScript is distributed under the GNU Public License version 3. The original Hebcal C is under the GPLv2. The program and its source code may be freely distributed. For details, see the file COPYING in the distribution.

Remember, the GPL means you can't use this in a closed-source program!

If you are going to use this program, please drop me a line. I'd like to know who you are, what version you're using, and how you're using Hebcal, and anything else you'd like to tell me, so that I can adjust the program to meet users' needs.

# API

This is where every property and method is listed.

## Information on certain standards

### Descriptions

Sometimes a "description array" will be mentioned. This is an array with between 1 and 3 properties: `[Sfardit pronounciation[, Ashkenazis pronounciation[, Hebrew spelling]]]`.

Methods that fetch a value from this array expect one of the following strings: "a" for Ashkenazis, "h", for Hebrew, and anything else for Sfardit.

### Month names

Hebcal is not too particular about month names, and only checks up to the second letter. That means that, despite being totally incorrect, "cheese" will return the same value as "cheshvan". Month names work in both Hebrew and English.

Nisan, Iyyar, Sivan, Tamuz, Av, Elul, Tishrei, Cheshvan, Kislev, Tevet, Shvat, Adar (1, 2).
ניסו, אייר, סיון, תמוז, אב, אלול, תשרי, חשון, כסלב, טבת, שבט, אדר (1, 2).

### Sun times

Hebcal JS uses the [SunCalc](https://github.com/mourner/suncalc) JavaScript library to calculate sun times. They do not provide an accuracy claim, but I have found their times to be up to 3 minutes off from the accepted times. Do not rely on Zemanim until the last moment!

### Finding location in browser

Both scripts run on the exact same code, except for one thing. In the browser, Hebcal will attempt to set `Hebcal.defaultLocation` to the user's location using the [geolocation](https://developer.mozilla.org/en-US/docs/Web/API/Geolocation) API. The `Hebcal.onready` event is fired once the location has been obtained, or failed to be obtained.

This will cause any webpages with the Hebcal script to request the user's permission to view their location. If they deny it, the default location will be `[0, 0]`.

To turn of this feature, use the `client.noloc.min.js` script instead of the usual `client.min.js` script. The unminified version is `client.noloc.js`.

## `Hebcal`

The global variable (if running in the browser) `Hebcal` is the entrypoint to Hebcal. It is a constructor returning an object represtenting a Hebrew year, and also has other useful properties.

```
new Hebcal([year[, month]]);
```

`year`, if provided, should be an `int` representing a Hebrew year, for example, `5774`. If it is falsey (includes `null`, `undefined`, `0`, `false`, etc), it defaults to the current year. If it is not falsy, and not a number, Hebcal will throw an error.

`month`, if provided, supports multiple types:
* String -- The name of a single month.
* Number -- The number of a single month (Nisan is 1, Tishrei 7).
* Array -- An array of either of the above two, or mixed. Basically, anything that can be passed to `Hebcal.Month` (see below).
If not provided, it defaults to all of the months in the given `year`.
If it is a different type, Hebcal will throw an error.

To get the full current year:

```js
var year = new Hebcal();
```

### Properties on `Hebcal` instances

#### `Hebcal.prototype.year`

Number. The year represented in the `Hebcal`.

#### `Hebcal.prototype.months`

Array of `Hebcal.Month`s. Is not intended to be used directly, but rather with the `getMonth()` and `map()` methods.

#### `Hebcal.prototype.holidays`

Array of holidays, as `Hebcal.holidays.Event`s, for the represented year. Can be used directly.

#### `Hebcal.prototype.length`

Number of days in the year.

#### `Hebcal.prototype.il`

Boolean get/set. Whether the current location is in Israel or not. This is relevant with regards to holidays.
This gets set by `Hebcal.defaultCity`, and `year.setCity()`. It's possible that you may have to set it yourself in certain circumstances.

#### `Hebcal.prototype.lat` and `long`

Number get/sets. The latitude and longitude of the current location. This is relevant with regards to getting times for dates.
Default: 0.

#### `Hebcal.prototype.setCity(city)`

```js
year.setCity('Jerusalem');
```

Sets the lat/long to the given city's location. Returns the `Hebcal` it was called upon. This allows for easy method chaining.

For more information regarding cities, see `Hebcal.cities` below.

#### `Hebcal.prototype.setLocation(latitude, longitude)`

```js
year.setLocation(40.716, -74.016); // coords of New York
```

Sets the lat/long to the given location. Returns the `Hebcal` it was called upon.

#### `Hebcal.prototype.isLeapYear()`

```js
var isleap = year.isLeapYear();
```

Takes 0 arguments, returns a boolean.

#### `Hebcal.prototype.next()`

```js
var nextYear = year.next();
```

Takes 0 arguments, returns `Hebcal` representing the following year.

#### `Hebcal.prototype.prev()`

```js
var prevYear = year.prev();
```

Takes 0 arguments, returns `Hebcal` representing the preceding year.

#### `Hebcal.prototype.getMonth(month)`

```js
var tishrei = year.getMonth(7);
var adar2 = year.getMonth(-1);
var shvat = year.getMonth('shvat');
var elul = year.getMonth('אלול');
var nextNisan = year.getMonth(13); // in a non-leap year
```

Takes a single argument, the month to get. Returns `Hebcal.Month` representing the requested month.

If the month is a positive number, it is a month index. Remember, Nisan is 1, Tishrei 7.
If the month is a negative number, it is a month index from the end of the year.
If the month is a string, it can be a month name in Hebrew or English.

#### `Hebcal.prototype.map(mapFunc[, thisArg])`

A shortcut for `year.months.map(mapFunc, [thisArg])`.

#### `Hebcal.prototype.addHoliday(event)`

Add a holiday to the list of holidays. Throws an error if `event` is not a `Hebcal.holidays.Event`. Returns the `Hebcal` it was called upon.

```js
year.addHoliday(new Hebcal.holidays.Event(new Date(2014,0,1), 'New Years Day', Hebcal.holidays.masks.USER_EVENT));
```

See `Hebcal.holidays` for more information on creating an event.

#### `Hebcal.prototype.find()`

A "magical" method that allows finding dates in the year. It supports several types of arguments, and is extensible. Returns an array of `Hebcal.HDate`s.

If called with a single argument:

* `Hebcal.HDate`: Find that Hebrew date in the year. This is useful, because getting a date specifically from the year, and not recreating it, gives it a few additional properties, such as a pre-set location, and a reference to the containing year.
* `Date`: Find that Gregorian date in the year.
* `Array`: An array of acceptible arguments to `year.find()`. If an item is an array, it is treated as passing multiple arguments to `find()`.
* `String`: A description of a date. See `find.strings` below.

If called with two arguments: the first represents a day, or list of days, in a month; and the second represents a month, or list of months.
The day is anything that can be passed to `Hebcal.Month.prototype.find()` (see below).
If the month is a:

* `Hebcal.Month`: Search for the day in that month. This is usually used only internally; it's more user-friendly to use one of the other types.
* `Number`: A number to be given to `Hebcal.prototype.getMonth()`.
* `String`: A month name.
* `Array`: An array of any of the above, mixed as you please.

##### `Hebcal.prototype.find.strings`

This is technically a function for internal use, but it also serves as the extension point for passing strings. Simply set a property on it, and that can be used as a string to `Hebcal.prototype.find()`.
To let the passed string be more user-friendly, the following conversions are done: convert to lowercase; replace spaces with underscores. This means that "rosh chodesh", "Rosh Chodesh", and "ROSH_CHODESH" all evaluate to "rosh_chodesh".

The following properties are defined by default:

* `rosh_chodesh` (all roshei chodashim in the year)
* `holidays` (days with a holiday defined in `year.holidays`)
* `omer` (days in sefirat ha'omer)
* `pesach`
* `sukkot` (aliased as `sukkos`, `succot`, `succos`)
* `shavuot` (aliased as `shavuos`)
* `rosh_hashana` (aliased as `rosh_hashanah`)
* `today`
* `yesterday`
* `tomorrow`

To extend the list, add a property of `Hebcal.prototype.find.strings`. The name should be all lowercase, and spaces replaced with underscores, as described above. The property itself should be a function taking a no arguments. Within the function, `this` refers to the object that `find()` is being called on. It should return an Array. It is perfectly acceptible to return the result of another `find()` call.

## `Hebcal.addZeman(zeman, func)`

An alias of `Hebcal.HDate.addZeman`. See there.

## `Hebcal.cities`

Get locations for several cities around the world.

### `Hebcal.cities.getCity(cityName)`

Returns an array describing the location of a city, or an array of `0`s if the city wasn't found. This array is intended to be passed to `Hebcal.cities.getLocation()`.

`[latdeg, latmins, longdeg, longmins, Israel]`

### `Hebcal.cities.getLocation(cityInfo)`

Is intended to receive the return value of `getCity()`. Returns an object containing 2 properties: `lat` and `long`, with the numbers in degrees, including minutes.

An end-user should rarely need either of these; they are mainly used by `Hebcal.defaultCity` and `Hebcal.setCity()` and the methods they call.

### `Hebcal.cities.listCities()`

Return an array containing the names of all supported cities.

### `Hebcal.cities.addCity(city, info)`

Add a city to the list of cities. The first argument should be the name, the second an array of the kind that `getCity()` returns.
This method does not return anything.

### `Hebcal.cities.nearest(lat, long)`

Return the name of the city nearest to the given coordinates.

The coordinates can be given either as numbers, or as arrays of `[deg, mins]`.

## `Hebcal.defaultCity`

String get/set. The nearest city to `Hebcal.defaultLocation`.

## `Hebcal.defaultLocation`

Array of coordinates that are used as the default location when creating a new instance of `Hebcal` or `Hebcal.HDate`.

`[latdeg, longdeg]`

## `Hebcal.holidays`

A holiday/event interface.

### `Hebcal.holidays.masks`

An object containing bitmasks for use when creating a `Hebcal.holidays.Event`.

```js
masks.USER_EVENT          = 1;
masks.LIGHT_CANDLES       = 2;
masks.YOM_TOV_ENDS        = 4;
masks.CHUL_ONLY           = 8; // chutz l'aretz (Diaspora)
masks.IL_ONLY             = 16; // b'aretz (Israel)
masks.LIGHT_CANDLES_TZEIS = 32;
```

### `Hebcal.holidays.IGNORE_YEAR`

A constant to be passed to both `Date` and `Hebcal.HDate` constructors. It means run this holiday every year, not just one specific year. It is automatically applied to Gregorian dates.

### `new Hebcal.holidays.Event(date, desc[, masks])`

A constructor for Events. This is meant to be added to a holidays list.

`date` can be either a Gregorian `Date` object or a `Hebcal.HDate` object. If Gregorian, `Hebcal.holidays.IGNORE_YEAR` is assumed.

`desc` can be a description array, or string. A description array looks like: `[Sfardit pronounciation[, Ashkenazis pronounciation[, Hebrew spelling]]]`. If the Sfardit and Ashkenazis are the same, pass `null` for Ashkenazis.

`masks` should be a bitmask of the items in `Hebcal.holidays.masks`. I.e. `Hebcal.holidays.masks.IL_ONLY | Hebcal.holidays.masks.YOM_TOV_ENDS`.

#### `Hebcal.holidays.Event.prototype.date`

A `Hebcal.HDate` representing the date passed to the constructor.

#### `Hebcal.holidays.Event.prototype.desc`

An Array, either the array passed as `desc`, or the string passed, wrapped in an array.

#### `Hebcal.holidays.Event.prototype.IGNORE_YEAR`

Boolean, whether or not we ignore the year of the date. Always true for Gregorian dates.

#### `Hebcal.holidays.Event.prototype.USER_EVENT`
#### `Hebcal.holidays.Event.prototype.LIGHT_CANDLES`
#### `Hebcal.holidays.Event.prototype.YOM_TOV_ENDS`
#### `Hebcal.holidays.Event.prototype.CHUL_ONLY`
#### `Hebcal.holidays.Event.prototype.IL_ONLY`
#### `Hebcal.holidays.Event.prototype.LIGHT_CANDLES_TZEIS`

Booleans, whether or not the bitmasks for those values were provided.

#### `Hebcal.holidays.Event.prototype.is([date[, il]])`

Returns a Boolean, whether or not the event applies for a given date.

`date`, if provided, can be a `Hebcal.HDate` or a Gregorian `Date`. If undefined, defaults to the current date.

`il`, if provided, should be a Boolean whether or not you are in Israel. If undefined, defaults to the value of `Hebcal.holidays.Event.isIL`.

#### `Hebcal.holidays.Event.prototype.masks()`

Returns a number representing the original bitmask passed to the constructor.

#### `Hebcal.holidays.Event.prototype.getDesc(o)`

Returns a string of the description. `o` can be one of the standard language strings.

#### `Hebcal.holidays.Event.prototype.candleLighting()`

Return a `Date` object representing the candle-lighting time for an event, if applicable. Otherwise, returns null.

The number of minutes before sunset can be customized with the `Hebcal.holidays.Event.candleLighting` property.

#### `Hebcal.holidays.Event.prototype.havdalah()`

Return a `Date` object representing the havdalah time for an event, if applicable. Otherwise, returns null.

The number of minutes after sunset can be customized with the `Hebcal.holidays.Event.havdalah` property.

#### `Hebcal.holidays.Event.isIL`

Boolean, are we in Israel? Is not automatically set. Default `false`.

#### `Hebcal.holidays.Event.candleLighting`

The number of minutes before sunset to light candles. Defaults to 18.

#### `Hebcal.holidays.Event.havdalah`

The number of minutes after sunset to make havdalah. Defaults to 50.

### `Hebcal.holidays.standards`

Array of standard holidays that do not change based on years. Is used internally, and exported only incidentally.

### `Hebcal.holidays.getHolidaysForYear(year)`

Returns an array of all holidays for the year, including variable ones. Is used internally, and exported only incidentally.

## `Hebcal.candleLighting`

An alias of `Hebcal.holidays.Event.candleLighting`.

## `Hebcal.havdalah`

An alias of `Hebcal.holidays.Event.havdalah`.

## `Hebcal.gematriya(num | string[, limit])`

Return a gematriya string from a number, or a number from a string. Is imperfect above 1000.

The `limit` parameter can be a number, which is how many digits to truncate the original number to. Digits are removed from the beginning.

```js
Hebcal.gematriya(5774, 3) === 'תשע"ד' // first converted to 774
Hebcal.gematriya(5774) === 'התשע"ד'
Hebcal.gematriya(5) === "ה'"
Hebcal.gematriya('התשע"ד') === 5774 // the ה before a high number means 5000
Hebcal.gematriya('ה"ד') === 9 // just adds them up
Hebcal.gematriya(1234) === 'תתררל"ד'
```

## `Hebcal.range(start, end[, step])`

Return an array containing numbers between `start` and `end`, at an interval of `step`. Can be useful with `find()`, for fetching a bunch of days.

`step` defaults to 1 if not provided.

```js
Hebcal.range(1, 10) // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
Hebcal.range(1, 10, 2) // [1, 3, 5, 7, 9]
Hebcal.range(15, 1, 3) // [15, 12, 9, 6, 3]
Hebcal.range(1, 2, 0.25) // [1, 1.25, 1.5, 1.75, 2]
```

## `new Hebcal.Month(month, year)`

This constructor contains information on a Hebrew month. It is not intended to be directly constructed, but is fetched by `Hebcal.prototype.getMonth()`, so is useful to the end-user.

```js
var month = new Hebcal.Month(7, 5774); // Tishrei
var month = new Hebcal.Month('Tishrei', 5774); // Tishrei
```

`month` can be a month name or number; `year` must be a number. An error is thrown if one of these conditions is not met.

### `Hebcal.Month.prototype.month`

A number representing the `month` argument to the constructor.

### `Hebcal.Month.prototype.year`

A number representing the `year` argument to the constructor.

### `Hebcal.Month.prototype.days`

An array of days in the month. Each day is an instance of `Hebcal.HDate`.

### `Hebcal.Month.prototype.length`

Number; the length of the month. Equivalent to the length of the `days` array.

### `Hebcal.Month.prototype.il`

Boolean get/set. Whether or not the location is in Israel. Is usually set by the parent year.

### `Hebcal.Month.prototype.lat`, `Hebcal.Month.prototype.long`

Number get/set. Latitude/Longitude of the location. Is usually set by the parent year.

### `Hebcal.Month.prototype.__year`

Internal, non-enumerable. A reference to the containing year. Only set if the month was created by a `Hebcal()`.

### `Hebcal.Month.prototype.isLeapYear()`

Returns a Boolean, whether or not the year is a leap year.

### `Hebcal.Month.prototype.prev()`

Returns a `Hebcal.Month` representing the previous month.

### `Hebcal.Month.prototype.next()`

Returns a `Hebcal.Month` representing the following month.

### `Hebcal.Month.prototype.getDay(day)`

Returns a `Hebcal.HDate` at a 1-based index in the month. Negative indices are from the end of the month.

### `Hebcal.Month.prototype.getYearObject()`

Returns `this.__year` internal value.

### `Hebcal.Month.prototype.getName(o)`

Returns a string with the name of the month. Takes a standard options string.

### `Hebcal.Month.prototype.rosh_chodesh()`

Returns an array containing `Hebcal.HDate`s for that month's Rosh Chodesh. Returns an array whether Rosh Chodesh is one or two days.

### `Hebcal.Month.prototype.map(mapFunc[, thisArg])`

A shortcut for `month.days.map(mapFunc, [thisArg])`.

### `Hebcal.Month.prototype.find(day)`

A method similar to `Hebcal.prototype.find()` (see there), but specifically for a single month.

If `day` is a number, it is a 1-based index in the month. String, `Hebcal.HDate`, `Date`, and Array rules are the same as for `Hebcal.prototype.find()`.

Default strings are:

* `rosh_chodesh`: Same as `Hebcal.Month.prototype.rosh_chodesh()`
* `shabbat_mevarchim` (aliased as `shabbos_mevarchim`): The last Shabbat in the month.

Strings are extensible in the same manner: Add a property of `Hebcal.Month.prototype.find.strings`. Name conversion rules and function context apply the same as with `Hebcal.prototype.find()`.

`find()` returns an array of `Hebcal.HDate`s.

This method is sometimes called by `Hebcal.prototype.find()`.

## `new Hebcal.HDate([day[, month[, year]]])`

An `HDate` represents a Hebrew date.

`HDate` supports multiple types.

If 0 arguments are passed, or only one is and it is undefined, return an `HDate` object representing the current date.

```js
var day = new Hebcal.HDate();`
```

If 1 argument is passed:

* Gregorian `Date`: An `HDate` object representing that date. If the date is after sunset, it is the next day.
* `HDate`: Clone the object.
* String: Split the string by whitespace, and parse that.
* Number: Number of days since December 31, 1 BCE.

```js
var day = new Hebcal.HDate(new Date(2014, 0, 1));
var day = new Hebcal.HDate('1 Tishrei 5774');
var day = new Hebcal.HDate('15 Adar2 5774');
```

If 2: An `HDate` with the given `day` and `month`, and the current year.

```js
var day = new Hebcal.HDate(15, 1); // Pesach
var day = new Hebcal.HDate("ו'", "סיון");
```

If 3: Numbers, or a month name for the second argument. Days and months are 1 based. If any values are out of range, e.g. the 31st of Nisan, convert them to proper values, i.e. 1st of Iyyar.

```js
var day = new Hebcal.HDate(7, 2, 5770);
var day = new Hebcal.HDate(40, 'elul', 5773); // 11 Tishrei 5774
```

`Hebcal.HDate` will throw an error if bad arguments are passed.

The location will be set to `Hebcal.defaultLocation`.

### `Hebcal.HDate.prototype.year`

Number. The year passed to the constructor, possibly fixed up. It is preferred to get this value with `Hebcal.HDate.prototype.getFullYear()`.

### `Hebcal.HDate.prototype.month`

Number. The month passed to the constructor, possibly fixed up. It is preferred to get this value with `Hebcal.HDate.prototype.getMonth()`.

### `Hebcal.HDate.prototype.day`

Number. The day passed to the constructor, possibly fixed up. It is preferred to get this value with `Hebcal.HDate.prototype.getDate()`.

### `Hebcal.HDate.prototype.lat`
### `Hebcal.HDate.prototype.long`

Numbers. The latitude/longitude of the location. These default to the values of `Hebcal.defaultLocation`.

### `Hebcal.HDate.prototype.il`

Boolean. Whether the current location is in Israel or not. This is relevant with regards to holidays.
This gets set by `Hebcal.defaultCity`, and `day.setCity()`. It's possible that you may have to set it yourself in certain circumstances.

### `Hebcal.HDate.prototype.__month`

Internal, non-enumerable. A reference to the containing month. Only set if the day was created by a `Hebcal.Month()`.

### `Hebcal.HDate.prototype.getFullYear()`

Number. The year of the date.

### `Hebcal.HDate.prototype.getMonth()`

Number. The month of the date. 1 is Nisan, 7 is Tishrei, 13 is Adar B.

### `Hebcal.HDate.prototype.getTishreiMonth()`

Number. The Tishrei-based month of the date. 1 is Tishrei, 7 is Nisan, 13 is Elul in a leap year.

### `Hebcal.HDate.prototype.daysInMonth()`

Number. The number of days in the month.

### `Hebcal.HDate.prototype.getDate()`

Number. The day of the month of the date.

### `Hebcal.HDate.prototype.getDay()`

Number. The day of the week of the date. 0 is Sunday, as with `Date.prototype.getDay()`.

### `Hebcal.HDate.prototype.setFullYear(year)`

Sets the year of the date. Returns the object it was called upon.

### `Hebcal.HDate.prototype.setMonth(month)`

Sets the month of the date. Returns the object it was called upon.

### `Hebcal.HDate.prototype.setTishreiMonth(month)`

Sets the Tishrei-based month of the date. Returns the object it was called upon.

### `Hebcal.HDate.prototype.setDate(date)`

Sets the day of the month of the date. Returns the object it was called upon.

### `Hebcal.HDate.prototype.greg()`

Returns a `Date` object with the Gregorian date of the Hebrew date. The time is midnight.

### `Hebcal.HDate.prototype.gregEve()`

Returns a `Date` object with the Gregorian date of the start of the Hebrew date, the previous evening. The time is sunset.

### `Hebcal.HDate.prototype.abs()`

Return a number representing the number of days since December 31, 1 BCE, an "absolute date".

### `Hebcal.HDate.prototype.toString(o)`

Return a string with the current day, month, and year. Takes a standard language options string.

```js
new Hebcal.HDate().toString() // 16 Adar I 5774
new Hebcal.HDate(1,7,5769).toString() // 1 Tishrei 5769
new Hebcal.HDate(15,1,5773).toString('h') // ט"ו ניסן התשע"ג
```

### `Hebcal.HDate.prototype.getMonthName(o)`

Return a string of the current month name. Takes a standard language options string.

### `Hebcal.HDate.prototype.getSedra(o)`

Return an array of strings with the parshiot read on Shabbat, or a special event that takes precedence. Takes a standard language options string. Ashkenazis names for parshiot are not currently supported.

```js
new Hebcal.HDate(24,12,5773).getSedra() // [ 'Vayakhel', 'Pekudei' ]
new Hebcal.HDate(18,1,5774).getSedra() // [ 'Chol hamoed Pesach' ]
new Hebcal.HDate(12,6,5774).getSedra() // [ 'Ki Tavo' ]
new Hebcal.HDate(15,7,5774).getSedra('a') // [ 'Chol hamoed Succos' ]
new Hebcal.HDate(19,12,5773).getSedra() // [ 'Ki Tisa' ]
new Hebcal.HDate(19,12,5773).getSedra('a') // [ 'Ki Tisa' ]
new Hebcal.HDate(8,8,5774).getSedra('h') // [ 'לך-לך' ]
```

### `Hebcal.HDate.prototype.setCity(city)`

Set the location to the coordinates of a given city. For more about cities, see `Hebcal.cities`. Returns the object it was called upon.

### `Hebcal.HDate.prototype.setLocation(lat, long)`

Set the location to the given coordinates. Returns the object it was called upon.

`lat` and `long` can be numbers, the coordinates in degrees, or arrays, `[deg, mins]`.

### `Hebcal.HDate.prototype.sunrise()`

Returns a `Date` with the sunrise time for the day. This is calculated based on the location. Please read the SunCalc disclaimer.

### `Hebcal.HDate.prototype.sunset()`

Returns a `Date` with the sunset time for the day. This is calculated based on the location. Please read the SunCalc disclaimer.

### `Hebcal.HDate.prototype.hour()`

Returns the number of milliseconds in a halachik hour (sha'a zmaniya). This is intended mostly for internal use.

### `Hebcal.HDate.prototype.hourMins()`

Returns the number of minutes in a halachik hour (sha'a zmaniya). This is more useful for most people than milliseconds.

### `Hebcal.HDate.prototype.nightHour()`

Returns the number of milliseconds in a halachik hour for the night. This is intended mostly for internal use.

### `Hebcal.HDate.prototype.nightHourMins()`

Returns the number of minutes in a halachik hour for the night. This is more useful for most people than milliseconds.

### `Hebcal.HDate.prototype.getZemanim()`

Returns an object containing halachik times (zemanim) for the day. Each property is a `Date` object. By default, these are:

* `chatzot` (calculated as sunrise + six hours)
* `chatzot_night` (calculated as sunset + six hours)
* `alot_hashacher` (calculated as when the sun is at -16.1 degrees, as per [MyZmanim.com](http://www.myzmanim.com/read/degrees.aspx))
* `misheyakir` (calculated as when the sun is at -11.5 degrees, as per [MyZmanim.com](http://www.myzmanim.com/read/sources.aspx))
* `misheyakir_machmir` (calculated as when the sun is at -10.2 degrees, as per [MyZmanim.com](http://www.myzmanim.com/read/sources.aspx))
* `sof_zman_shma` (calculated as sunrise + 3 hours, according to the Gra and Baal HaTanya)
* `sof_zman_tfilla` (calculated as sunrise + 4 hours, according to the Gra and Baal HaTanya)
* `mincha_gedola` (calculated as sunrise + 6.5 hours)
* `mincha_ketana` (calculated as sunrise + 9.5 hours)
* `plag_hamincha` (calculated as sunrise + 10.75 hours)
* `tzeit` (calculated as when the sun is at -8.5 degrees, as per [MyZmanim.com](http://www.myzmanim.com/read/sources.aspx))

You can add a zeman to be returned with `Hebcal.HDate.addZeman()` (aliased as `Hebcal.addZeman()`).

### `Hebcal.HDate.prototype.next()`

Return tomorrow. Or rather, an `HDate` representing the next day.

If part of a `Hebcal()` year, return the day from the year. Otherwise, return a new object. (The difference is mainly with location, and back-reference to the month/year).

### `Hebcal.HDate.prototype.prev()`

Return yesterday. Or rather, an `HDate` representing the previous day.

If part of a `Hebcal()` year, return the day from the year. Otherwise, return a new object. (The difference is mainly with location, and back-reference to the month/year).

### `Hebcal.HDate.prototype.isSameDate(otherHDate)`

Returns a Boolean whether or not the two dates represent the same day.

### `Hebcal.HDate.prototype.before(dayNumber)`

Returns an `HDate` representing the last day `dayNumber` before the current date. Sunday is 0, Saturday 6.

```js
new Hebcal.HDate(new Date('Wednesday February 19, 2014')).before(6).greg() // Sat Feb 15 2014
```

### `Hebcal.HDate.prototype.nearest(dayNumber)`

Returns an `HDate` representing the nearest `dayNumber` to the current date. Sunday is 0, Saturday 6.

```js
new Hebcal.HDate(new Date('Wednesday February 19, 2014')).nearest(6).greg() // Sat Feb 22 2014
new Hebcal.HDate(new Date('Tuesday February 18, 2014')).nearest(6).greg() // Sat Feb 15 2014
```

### `Hebcal.HDate.prototype.onOrAfter(dayNumber)`

Returns an `HDate` representing the a `dayNumber` on or after the current date. Sunday is 0, Saturday 6.

```js
new Hebcal.HDate(new Date('Wednesday February 19, 2014')).onOrAfter(6).greg() // Sat Feb 22 2014
new Hebcal.HDate(new Date('Saturday February 22, 2014')).onOrAfter(6).greg() // Sat Feb 22 2014
new Hebcal.HDate(new Date('Sunday February 23, 2014')).onOrAfter(6).greg() // Sat Mar 01 2014
```

### `Hebcal.HDate.prototype.after(dayNumber)`

Returns an `HDate` representing the the first `dayNumber` after the current date. Sunday is 0, Saturday 6.

```js
new Hebcal.HDate(new Date('Wednesday February 19, 2014')).after(6).greg() // Sat Feb 22 2014
new Hebcal.HDate(new Date('Saturday February 22, 2014')).after(6).greg() // Sat Mar 01 2014
new Hebcal.HDate(new Date('Sunday February 23, 2014')).after(6).greg() // Sat Mar 01 2014
```

### `Hebcal.HDate.prototype.getMonthObject()`

Return `this.__month` internal.

### `Hebcal.HDate.prototype.getYearObject()`

Call `getYearObject()` of `this.getMonthObject()`.

### `Hebcal.HDate.prototype.holidays()`

Return an array containing `Hebcal.holidays.Event`s applying to the current day.

### `Hebcal.HDate.prototype.omer()`

Return a number representing the day of Sefirat Omer. Returns 0 if not during Sefirah.

```js
new Hebcal.HDate(20, 'iyyar').omer() // 35
new Hebcal.HDate(16, 'nisan').omer() // 1
new Hebcal.HDate(15, 'nisan').omer() // 0
```

### `Hebcal.HDate.prototype.dafyomi(o)`

Return a string containing the day's Daf Yomi. Takes a standard language options string.

```js
new Hebcal.HDate(15, 'nisan', 5774).dafyomi() // Beitzah 16
new Hebcal.HDate(15, 'elul', 5772).dafyomi('a') // Berachos 30
```

### `Hebcal.HDate.defaultLocation`

An alias of `Hebcal.defaultLocation`.

### `Hebcal.HDate.defaultCity`

An alias of `Hebcal.defaultCity`.

### `Hebcal.HDate.hebrew2abs(hdate)`

Return an absolute date from an `HDate`. Equivalent to `Hebcal.HDate.prototype.abs()`.

### `Hebcal.HDate.abs2hebrew(absdate)`

Return an `HDate` from absolute date. Equivalent to `new Hebcal.HDate(absdate)`.

### `Hebcal.HDate.addZeman(zeman, func)`

Add a zeman to the object of times returned by `Hebcal.HDate.prototype.getZemanim()`. This function is aliased as `Hebcal.addZeman()`.

`zeman` should be the name of the zeman; `func` should be a function taking one parameter, the `HDate` to get the zeman for. `func` should return a `Date` object with the time.

## `Hebcal.ready` (client-side only)

Boolean; whether the geolocating code has completed or failed yet. If using the noloc script, it is always true.

## `Hebcal.onready()` (client-side only)

Set this to a function to be executed when geolocation has finished. Is called immediately upon being set in a no-loc environment.

This function is liable to be buggy.

## Finale

That's it! You now know everything there is to know about Hebcal JavaScript! Good luck with whatever you choose to do with it!

If you have any other requests for features, drop me an issue or even a pull request.