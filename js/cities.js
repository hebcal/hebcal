var c = require('./common'), greg = require('./greg');

module.exports = function(HDate) {
	var e = {}; // exports

	// name, latdeg, latmin, longdeg, longmin, TZ, DST_scheme
	var cities = {
	"Ashdod": [ 31, 48, 34, 38, -2, DST_ISRAEL ],
	"Atlanta": [ 33, 45, -84, -23, 5, DST_USOFA ],
	"Austin": [ 30, 16, -97, -45, 6, DST_USOFA ],
	"Baghdad": [ 33, 14, 44, 22, -3, DST_EU ],
	"Beer Sheva": [ 31, 15, 34, 47, -2, DST_ISRAEL ],
	"Berlin": [ 52, 31, 13, 24, -1, DST_EU ],
	"Baltimore": [ 39, 17, -76, -36, 5, DST_USOFA ],
	"Bogota": [ 4, 36, -74, -5, 5, DST_NONE ],
	"Boston": [ 42, 20, -71, -4, 5, DST_USOFA ],
	"Buenos Aires": [ -34, -37, -58, -24, 3, DST_NONE ],
	"Buffalo": [ 42, 53, -78, -52, 5, DST_USOFA ],
	"Chicago": [ 41, 50, -87, -45, 6, DST_USOFA ],
	"Cincinnati": [ 39, 6, -84, -31, 5, DST_USOFA ],
	"Cleveland": [ 41, 30, -81, -41, 5, DST_USOFA ],
	"Dallas": [ 32, 47, -96, -48, 6, DST_USOFA ],
	"Denver": [ 39, 44, -104, -59, 7, DST_USOFA ],
	"Detroit": [ 42, 20, -83, -2, 5, DST_USOFA ],
	"Eilat": [ 29, 33, 34, 57, -2, DST_ISRAEL ],
	"Gibraltar": [ 36, 8, -5, -21, -1, DST_EU ],
	"Haifa": [ 32, 49, 34, 59, -2, DST_ISRAEL ],
	"Hawaii": [ 19, 30, -155, -30, 10, DST_NONE ],
	"Houston": [ 29, 46, -95, -22, 6, DST_USOFA ],
	"Jerusalem": [ 31, 47, 35, 14, -2, DST_ISRAEL ],
	"Johannesburg": [ -26, -10, 28, 2, -2, DST_NONE ],
	"Kiev": [ 50, 28, 30, 29, -2, DST_EU ],
	"La Paz": [ -16, -30, -68, -9, 4, DST_NONE ],
	"Livingston": [ 40, 17, -74, -18, 5, DST_USOFA ],
	"London": [ 51, 30, 0, -10, 0, DST_EU ],
	"Los Angeles": [ 34, 4, -118, -15, 8, DST_USOFA ],
	"Miami": [ 25, 46, -80, -12, 5, DST_USOFA ],
	"Melbourne": [ -37, -52, 145, 8, -10, DST_AUNZ ],
	"Mexico City": [ 19, 24, -99, -9, 6, DST_MX ],
	"Montreal": [ 45, 30, -73, -36, 5, DST_USOFA ],
	"Moscow": [ 55, 45, 37, 42, -3, DST_EU ],
	"New York": [ 40, 43, -74, -1, 5, DST_USOFA ],
	"Omaha": [ 41, 16, -95, -56, 6, DST_USOFA ],
	"Ottawa": [ 45, 42, -75, -71, 5, DST_USOFA ],
	"Panama City": [ 8, 58, -79, -32, 5, DST_NONE ],
	"Paris": [ 48, 52, 2, 20, -1, DST_EU ],
	"Petach Tikvah": [ 32, 5, 34, 53, -2, DST_ISRAEL ],
	"Philadelphia": [ 39, 57, -75, -10, 5, DST_USOFA ],
	"Phoenix": [ 33, 27, -112, -4, 7, DST_NONE ],
	"Pittsburgh": [ 40, 26, -80, 0, 5, DST_USOFA ],
	"Saint Louis": [ 38, 38, -90, -12, 6, DST_USOFA ],
	"Saint Petersburg": [ 59, 53, 30, 15, -3, DST_EU ],
	"San Francisco": [ 37, 47, -122, -25, 8, DST_USOFA ],
	"Seattle": [ 47, 36, -122, -20, 8, DST_USOFA ],
	"Sydney": [ -33, -55, 151, 17, -10, DST_AUNZ ],
	"Tel Aviv": [ 32, 5, 34, 46, -2, DST_ISRAEL ],
	"Tiberias": [ 32, 58, 35, 32, -2, DST_ISRAEL ],
	"Toronto": [ 43, 38, -79, -24, 5, DST_USOFA ],
	"Vancouver": [ 49, 16, -123, -7, 8, DST_USOFA ],
	"White Plains": [ 41, 2, -73, -45, 5, DST_USOFA ],
	"Washington DC": [ 38, 55, -77, 0, 5, DST_USOFA ]
};

	function DST_ISRAEL(date) {
		date = date || new Date();
		var gregYr = date.getFullYear(), tempDt = new Date(gregYr,4,1), inDST = false;
			
		if (gregYr >= 2005) {
			/* Starting in 2005, DST begins in Israel on the
				 last Friday before April 2nd, at 2:00 a.m. */
			/* http://www.gov.il/FirstGov/NewsEng/SavingTime_news.htm */
			tempDt.setDate(1);
			tempDt.setMonth(4);
			inDST = c.day_on_or_before(c.days.FRI, greg.greg2abs(tempDt)) < greg.greg2abs(date);
		} else {
			/* daylight savings time begins in Israel on the */
			/* first Sunday after Passover ends on Nisan 21: */
			tempDt = new HDate(28, 'nisan', gregYr + 3760);
			inDST = c.day_on_or_before(c.days.SUN, HDate.hebrew2abs(tempDt)) < greg.greg2abs(date);
		}
					
		if (gregYr >= 2013) {
			/*
				IDT will end on the last Sunday of October
				http://en.wikipedia.org/wiki/Israel_Summer_Time#2013.E2.80.93Present
			*/
			tempDt.setDate(1);
			tempDt.setMonth(11);
			/*  the sunday before the first of november */
			inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) > greg.greg2abs(date);
		} else {      
			/* daylight savings time ends in Israel on the Sunday Selichot */
			/* begins: */
			tempDt = new HDate(1, 'tishrei', gregYr + 3761); // next year
			inDST = inDST && c.day_on_or_before(c.days.SUN, HDate.hebrew2abs(tempDt) - 3) > greg.greg2abs(date);
		}
		return 60 * inDST;
	}

	function DST_USOFA(date) {
		date = date || new Date();
		var gregYr = date.getFullYear(), tempDt = new Date(gregYr,4,1), inDST = false;

		
		// FIX: this needs more work for historical DST, obviously
		if (gregYr >= 1966 && gregYr < 1987) {
			// the last sunday in April
			tempDt.setDate(30);
			tempDt.setMonth(4);
			inDST = c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt)) < greg.greg2abs(date);

			/*  the sunday before the first of november */
			tempDt.setDate(1);
			tempDt.setMonth(11);
			inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) > greg.greg2abs(date);
		} else if (gregYr >= 1987 && gregYr < 2007) {
				/*  the first sunday in April */
			tempDt.setDate(1);
			tempDt.setMonth(4);
			inDST = c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) + 6) < greg.greg2abs(date);


			/*  the sunday before the first of november */
			tempDt.setDate(1);
			tempDt.setMonth(11);
			inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) > greg.greg2abs(date);
		} else if (gregYr >= 2007) {
			/*  Second Sunday in March */
			tempDt.setDate(1);
			tempDt.setMonth(3);
			inDST = c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) + 13) < greg.greg2abs(date);


			/*  first sunday of november */
			tempDt.setDate(1);
			tempDt.setMonth(11);
			inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) + 6) > greg.greg2abs(date);
		}
		return 60 * inDST;
	}

	function DST_MX(date) {
		date = date || new Date();
		var gregYr = date.getFullYear(), tempDt = new Date(gregYr,4,1), inDST = false;

		/* the first sunday in April */
		inDST = c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) + 6) < greg.greg2abs(date);

		/*  the sunday before the first of november */
		tempDt.setDate(1);
		tempDt.setMonth(11);
		inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) > greg.greg2abs(date);

		return 60 * inDST;
	}

	function DST_EU(date) {
		date = date || new Date();
		var gregYr = date.getFullYear(), tempDt = new Date(gregYr,4,1), inDST = false;

        /* The EU version of Daylight Saving Time runs from the last
         * Sunday in March through the last Sunday in October.
         * it starts at 1am instead of 2am, but that's not relevant here.
         * http://webexhibits.org/daylightsaving/g.html
         */
        inDST = c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) < greg.greg2abs(date);
        /*  the sunday before the first of november */
		tempDt.setDate(1);
		tempDt.setMonth(11);
        inDST = inDST && c.day_on_or_before(c.days.SUN, greg.greg2abs(tempDt) - 1) > greg.greg2abs(date);
        
        return 60 * inDST;
	}

	function DST_AUNZ(date) {
		// Australasia is the inverse of European Union
		return 60 - DST_EU(date);
	}

	function DST_NONE(date) {
		return 0;
	}

	e.DST_SCHEMES = {
		DST_ISRAEL: DST_ISRAEL,
		DST_USOFA: DST_USOFA,
		DST_EU: DST_EU,
		DST_AUNZ: DST_AUNZ,
		DST_MX: DST_MX,
		DST_NONE: DST_NONE
	};

	e.getCity = function getCity(city) {
		return cities[city] || [ 0, 0, 0, 0, 0, 0 ];
	};

	e.listCities = function listCities() {
		return Object.keys(cities);
	};

	e.addCity = function addCity(city, info) {
		cities[city] = info;
	};

	e.getTime = function getTime(cityInfo) {
		var now = new Date();
		return {
			h: now.getUTCHours() + cityInfo[4] + cityInfo[5](now),
			m: now.getUTCMinutes(),
			s: now.getUTCSeconds()
		};
	};

	e.getLocation = function getLocation(cityInfo) {
		return {
			lat: (cityInfo[0] * 60 + cityInfo[1]) / 60,
			long: (cityInfo[2] * 60 + cityInfo[3]) / 60
		};
	};

	e.nearest = function nearest(lat, long) {
		if (Array.isArray(lat)) {
			lat = (lat[0] * 60 + lat[1]) / 60;
		}
		if (Array.isArray(long)) {
			long = (long[0] * 60 + long[1]) / 60;
		}
		if (typeof lat != 'number') {
			throw new TypeError('incorrect lat type passed to nearest()');
		}
		if (typeof long != 'number') {
			throw new TypeError('incorrect long type passed to nearest()');
		}

		return e.listCities().map(function(city){
			var i = e.getLocation(e.getCity(city));
			return {
				name: city,
				dist: Math.sqrt( Math.pow(Math.abs(i.lat - lat), 2) + Math.pow(Math.abs(i.long - long), 2) )
			};
		}).reduce(function(close,city){
			return close.dist < city.dist ? close : city;
		}).name;
	};

	return e;
};
