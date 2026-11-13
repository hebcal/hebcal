package zmanim

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
// Derived from original C version, Copyright (C) 1994-2004 Danny Sadinoff
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

// IANA time zone identifiers referenced by classicCities, defined as named
// constants so each literal appears exactly once.
const (
	tzAfricaCairo                 = "Africa/Cairo"
	tzAfricaCasablanca            = "Africa/Casablanca"
	tzAfricaJohannesburg          = "Africa/Johannesburg"
	tzAfricaLagos                 = "Africa/Lagos"
	tzAfricaTunis                 = "Africa/Tunis"
	tzAmericaAnchorage            = "America/Anchorage"
	tzAmericaArgentinaBuenosAires = "America/Argentina/Buenos_Aires"
	tzAmericaArgentinaCordoba     = "America/Argentina/Cordoba"
	tzAmericaBogota               = "America/Bogota"
	tzAmericaBoise                = "America/Boise"
	tzAmericaCaracas              = "America/Caracas"
	tzAmericaChicago              = "America/Chicago"
	tzAmericaCuracao              = "America/Curacao"
	tzAmericaDenver               = "America/Denver"
	tzAmericaDetroit              = "America/Detroit"
	tzAmericaEdmonton             = "America/Edmonton"
	tzAmericaElSalvador           = "America/El_Salvador"
	tzAmericaHalifax              = "America/Halifax"
	tzAmericaIndianaIndianapolis  = "America/Indiana/Indianapolis"
	tzAmericaLaPaz                = "America/La_Paz"
	tzAmericaLima                 = "America/Lima"
	tzAmericaLosAngeles           = "America/Los_Angeles"
	tzAmericaMexicoCity           = "America/Mexico_City"
	tzAmericaMontevideo           = "America/Montevideo"
	tzAmericaNewYork              = "America/New_York"
	tzAmericaPanama               = "America/Panama"
	tzAmericaPhoenix              = "America/Phoenix"
	tzAmericaPuertoRico           = "America/Puerto_Rico"
	tzAmericaRegina               = "America/Regina"
	tzAmericaSantiago             = "America/Santiago"
	tzAmericaSaoPaulo             = "America/Sao_Paulo"
	tzAmericaTijuana              = "America/Tijuana"
	tzAmericaToronto              = "America/Toronto"
	tzAmericaVancouver            = "America/Vancouver"
	tzAmericaWinnipeg             = "America/Winnipeg"
	tzAsiaAlmaty                  = "Asia/Almaty"
	tzAsiaBaghdad                 = "Asia/Baghdad"
	tzAsiaBaku                    = "Asia/Baku"
	tzAsiaBangkok                 = "Asia/Bangkok"
	tzAsiaDhaka                   = "Asia/Dhaka"
	tzAsiaDubai                   = "Asia/Dubai"
	tzAsiaHongKong                = "Asia/Hong_Kong"
	tzAsiaIrkutsk                 = "Asia/Irkutsk"
	tzAsiaJerusalem               = "Asia/Jerusalem"
	tzAsiaKarachi                 = "Asia/Karachi"
	tzAsiaKathmandu               = "Asia/Kathmandu"
	tzAsiaKolkata                 = "Asia/Kolkata"
	tzAsiaManila                  = "Asia/Manila"
	tzAsiaShanghai                = "Asia/Shanghai"
	tzAsiaSingapore               = "Asia/Singapore"
	tzAsiaTashkent                = "Asia/Tashkent"
	tzAsiaTehran                  = "Asia/Tehran"
	tzAsiaTokyo                   = "Asia/Tokyo"
	tzAsiaVladivostok             = "Asia/Vladivostok"
	tzAustraliaAdelaide           = "Australia/Adelaide"
	tzAustraliaBrisbane           = "Australia/Brisbane"
	tzAustraliaMelbourne          = "Australia/Melbourne"
	tzAustraliaPerth              = "Australia/Perth"
	tzAustraliaSydney             = "Australia/Sydney"
	tzEuropeAmsterdam             = "Europe/Amsterdam"
	tzEuropeAthens                = "Europe/Athens"
	tzEuropeBerlin                = "Europe/Berlin"
	tzEuropeBrussels              = "Europe/Brussels"
	tzEuropeBucharest             = "Europe/Bucharest"
	tzEuropeBudapest              = "Europe/Budapest"
	tzEuropeChisinau              = "Europe/Chisinau"
	tzEuropeCopenhagen            = "Europe/Copenhagen"
	tzEuropeDublin                = "Europe/Dublin"
	tzEuropeGibraltar             = "Europe/Gibraltar"
	tzEuropeHelsinki              = "Europe/Helsinki"
	tzEuropeIstanbul              = "Europe/Istanbul"
	tzEuropeKaliningrad           = "Europe/Kaliningrad"
	tzEuropeKiev                  = "Europe/Kiev"
	tzEuropeLondon                = "Europe/London"
	tzEuropeMadrid                = "Europe/Madrid"
	tzEuropeMinsk                 = "Europe/Minsk"
	tzEuropeMoscow                = "Europe/Moscow"
	tzEuropeParis                 = "Europe/Paris"
	tzEuropePrague                = "Europe/Prague"
	tzEuropeRiga                  = "Europe/Riga"
	tzEuropeRome                  = "Europe/Rome"
	tzEuropeStockholm             = "Europe/Stockholm"
	tzEuropeVienna                = "Europe/Vienna"
	tzEuropeVolgograd             = "Europe/Volgograd"
	tzEuropeWarsaw                = "Europe/Warsaw"
	tzEuropeZurich                = "Europe/Zurich"
	tzPacificAuckland             = "Pacific/Auckland"
	tzPacificHonolulu             = "Pacific/Honolulu"
)

// classicCities is the database of well-known cities used by LookupCity and
// AllCities, each with its country code, coordinates, elevation in meters, and
// time zone.
var classicCities = []Location{
	{"Abuja", "NG", 9.05785, 7.49508, 476, tzAfricaLagos, 0},
	{"Acre", "IL", 32.92814, 35.07647, 8, tzAsiaJerusalem, 0},
	{"Adelaide", "AU", -34.92866, 138.59863, 61, tzAustraliaAdelaide, 0},
	{"Albany", "US", 42.65258, -73.75623, 57, tzAmericaNewYork, 0},
	{"Albuquerque", "US", 35.08449, -106.65114, 1516, tzAmericaDenver, 0},
	{"Almaty", "KZ", 43.25, 76.91667, 795, tzAsiaAlmaty, 0},
	{"Amsterdam", "NL", 52.37403, 4.88969, 12, tzEuropeAmsterdam, 0},
	{"Anaheim", "US", 33.83529, -117.9145, 53, tzAmericaLosAngeles, 0},
	{"Anchorage", "US", 61.21806, -149.90028, 31, tzAmericaAnchorage, 0},
	{"Arad", "IL", 31.25882, 35.21282, 611, tzAsiaJerusalem, 0},
	{"Arlington TX", "US", 32.73569, -97.10807, 190, tzAmericaChicago, 0},
	{"Ashdod", "IL", 31.79213, 34.64966, 27, tzAsiaJerusalem, 0},
	{"Ashkelon", "IL", 31.66926, 34.57149, 42, tzAsiaJerusalem, 0},
	{"Ashqelon", "IL", 31.66926, 34.57149, 42, tzAsiaJerusalem, 0},
	{"Athens", "GR", 37.98376, 23.72784, 86, tzEuropeAthens, 0},
	{"Atlanta", "US", 33.749, -84.38798, 336, tzAmericaNewYork, 0},
	{"Auckland", "NZ", -36.84853, 174.76349, 43, tzPacificAuckland, 0},
	{"Aurora", "US", 39.72943, -104.83192, 1651, tzAmericaDenver, 0},
	{"Austin", "US", 30.26715, -97.74306, 165, tzAmericaChicago, 0},
	{"Baghdad", "IQ", 33.34058, 44.40088, 41, tzAsiaBaghdad, 0},
	{"Bakersfield", "US", 35.37329, -119.01871, 129, tzAmericaLosAngeles, 0},
	{"Baku", "AZ", 40.37767, 49.89201, 0, tzAsiaBaku, 0},
	{"Baltimore", "US", 39.29038, -76.61219, 35, tzAmericaNewYork, 0},
	{"Bangkok", "TH", 13.75398, 100.50144, 11, tzAsiaBangkok, 0},
	{"Barcelona", "ES", 41.38879, 2.15899, 47, tzEuropeMadrid, 0},
	{"Basel", "CH", 47.55839, 7.57327, 279, tzEuropeZurich, 0},
	{"Bat Yam", "IL", 32.02379, 34.75185, 34, tzAsiaJerusalem, 0},
	{"Baton Rouge|LA", "US", 30.44332, -91.18747, 20, tzAmericaChicago, 0},
	{"Beer Sheva", "IL", 31.25181, 34.7913, 285, tzAsiaJerusalem, 0},
	{"Beersheba", "IL", 31.25181, 34.7913, 286, tzAsiaJerusalem, 0},
	{"Beijing", "CN", 39.9075, 116.39723, 49, tzAsiaShanghai, 0},
	{"Berlin", "DE", 52.52437, 13.41053, 43, tzEuropeBerlin, 0},
	{"Bet Shemesh", "IL", 31.73072, 34.99293, 285, tzAsiaJerusalem, 0},
	{"Birmingham", "GB", 52.48142, -1.89983, 150, tzEuropeLondon, 0},
	{"Birobidzhan", "RU", 48.79284, 132.92386, 85, tzAsiaVladivostok, 0},
	{"Bnei Brak", "IL", 32.08074, 34.8338, 50, tzAsiaJerusalem, 0},
	{"Bogota", "CO", 4.60971, -74.08175, 2582, tzAmericaBogota, 0},
	{"Boise", "US", 43.6135, -116.20345, 835, tzAmericaBoise, 0},
	{"Bolzano", "IT", 46.49067, 11.33982, 257, tzEuropeRome, 0},
	{"Boston", "US", 42.35843, -71.05977, 38, tzAmericaNewYork, 0},
	{"Bozen", "IT", 46.49067, 11.33982, 257, tzEuropeRome, 0},
	{"Brisbane", "AU", -27.46794, 153.02809, 40, tzAustraliaBrisbane, 0},
	{"Brussels", "BE", 50.85045, 4.34878, 28, tzEuropeBrussels, 0},
	{"Bucharest", "RO", 44.43225, 26.10626, 83, tzEuropeBucharest, 0},
	{"Budapest", "HU", 47.49801, 19.03991, 104, tzEuropeBudapest, 0},
	{"Buenos Aires", "AR", -34.61315, -58.37723, 31, tzAmericaArgentinaBuenosAires, 0},
	{"Buffalo", "US", 42.88645, -78.87837, 191, tzAmericaNewYork, 0},
	{"Burlington", "US", 44.47588, -73.21207, 62, tzAmericaNewYork, 0},
	{"Cairo", "EG", 30.06263, 31.24967, 25, tzAfricaCairo, 0},
	{"Calgary", "CA", 51.05011, -114.08529, 1043, tzAmericaEdmonton, 0},
	{"Cape Town", "ZA", -33.92584, 18.42322, 26, tzAfricaJohannesburg, 0},
	{"Caracas", "VE", 10.48801, -66.87919, 893, tzAmericaCaracas, 0},
	{"Casablanca", "MA", 33.58831, -7.61138, 26, tzAfricaCasablanca, 0},
	{"Chandler", "US", 33.30616, -111.84125, 368, tzAmericaPhoenix, 0},
	{"Chapel Hill", "US", 35.9132, -79.05584, 149, tzAmericaNewYork, 0},
	{"Charlotte", "US", 35.22709, -80.84313, 279, tzAmericaNewYork, 0},
	{"Chicago", "US", 41.85003, -87.65005, 180, tzAmericaChicago, 0},
	{"Chisinau", "MD", 47.00556, 28.8575, 53, tzEuropeChisinau, 0},
	{"Chula Vista", "US", 32.64005, -117.0842, 21, tzAmericaLosAngeles, 0},
	{"Cincinnati", "US", 39.162, -84.45689, 267, tzAmericaNewYork, 0},
	{"Cleveland", "US", 41.4995, -81.69541, 204, tzAmericaNewYork, 0},
	{"Colorado Springs", "US", 38.83388, -104.82136, 1837, tzAmericaDenver, 0},
	{"Columbus", "US", 39.96118, -82.99879, 241, tzAmericaNewYork, 0},
	{"Copenhagen", "DK", 55.67594, 12.56553, 14, tzEuropeCopenhagen, 0},
	{"Corpus Christi", "US", 27.80058, -97.39638, 9, tzAmericaChicago, 0},
	{"Dallas", "US", 32.78306, -96.80667, 139, tzAmericaChicago, 0},
	{"Delhi", "IN", 28.65195, 77.23149, 228, tzAsiaKolkata, 0},
	{"Denver", "US", 39.73915, -104.9847, 1636, tzAmericaDenver, 0},
	{"Des Moines", "US", 41.60054, -93.60911, 265, tzAmericaChicago, 0},
	{"Detroit", "US", 42.33143, -83.04575, 192, tzAmericaDetroit, 0},
	{"Dhaka", "BD", 23.7104, 90.40744, 23, tzAsiaDhaka, 0},
	{"Dimona", "IL", 31.07079, 35.03269, 551, tzAsiaJerusalem, 0},
	{"Dnipro", "UA", 48.46664, 35.04066, 61, tzEuropeKiev, 0},
	{"Dortmund", "DE", 51.51494, 7.466, 99, tzEuropeBerlin, 0},
	{"Dresden", "DE", 51.05089, 13.73832, 117, tzEuropeBerlin, 0},
	{"Dubai", "AE", 25.07725, 55.30927, 24, tzAsiaDubai, 0},
	{"Dublin", "IE", 53.33306, -6.24889, 17, tzEuropeDublin, 0},
	{"Dundee", "GB", 56.46913, -2.97489, 77, tzEuropeLondon, 0},
	{"Durban", "ZA", -29.8579, 31.0292, 18, tzAfricaJohannesburg, 0},
	{"Durham", "US", 35.99403, -78.89862, 122, tzAmericaNewYork, 0},
	{"Dusseldorf", "DE", 51.22172, 6.77616, 46, tzEuropeBerlin, 0},
	{"Edmonton", "CA", 53.55014, -113.46871, 610, tzAmericaEdmonton, 0},
	{"Eilat", "IL", 29.55805, 34.94821, 63, tzAsiaJerusalem, 0},
	{"El Paso", "US", 31.75872, -106.48693, 1136, tzAmericaDenver, 0},
	{"Far Rockaway", "US", 40.60538, -73.75513, 6, tzAmericaNewYork, 0},
	{"Fort Wayne", "US", 41.1306, -85.12886, 250, tzAmericaIndianaIndianapolis, 0},
	{"Fort Worth", "US", 32.72541, -97.32085, 203, tzAmericaChicago, 0},
	{"Frankfurt", "DE", 50.11552, 8.68417, 114, tzEuropeBerlin, 0},
	{"Fremont", "US", 37.54827, -121.98857, 18, tzAmericaLosAngeles, 0},
	{"Fresno", "US", 36.74773, -119.77237, 100, tzAmericaLosAngeles, 0},
	{"Gibraltar", "GI", 36.14474, -5.35257, 11, tzEuropeGibraltar, 0},
	{"Glasgow", "GB", 55.86515, -4.25763, 41, tzEuropeLondon, 0},
	{"Great Neck", "US", 40.80066, -73.72846, 38, tzAmericaNewYork, 0},
	{"Greenlawn", "US", 40.86899, -73.36512, 71, tzAmericaNewYork, 0},
	{"Greensboro", "US", 36.07264, -79.79198, 259, tzAmericaNewYork, 0},
	{"Grenoble", "FR", 45.17869, 5.71479, 223, tzEuropeParis, 0},
	{"Guadalajara", "MX", 20.66682, -103.39182, 1598, tzAmericaMexicoCity, 0},
	{"Guangzhou", "CN", 23.11667, 113.25, 15, tzAsiaShanghai, 0},
	{"Hadera", "IL", 32.44192, 34.9039, 12, tzAsiaJerusalem, 0},
	{"Haifa", "IL", 32.81841, 34.9885, 40, tzAsiaJerusalem, 0},
	{"Halifax", "CA", 44.6464, -63.57291, 28, tzAmericaHalifax, 0},
	{"Hamburg", "DE", 53.55073, 9.99302, 11, tzEuropeBerlin, 0},
	{"Hamilton", "CA", 43.25011, -79.84963, 94, tzAmericaToronto, 0},
	{"Hartford", "US", 41.76371, -72.68509, 25, tzAmericaNewYork, 0},
	{"Hawaii", "US", 21.30694, -157.85833, 18, tzPacificHonolulu, 0},
	{"Helsinki", "FI", 60.16952, 24.93545, 26, tzEuropeHelsinki, 0},
	{"Henderson", "US", 36.0397, -114.98194, 573, tzAmericaLosAngeles, 0},
	{"Herzliya", "IL", 32.16627, 34.82536, 28, tzAsiaJerusalem, 0},
	{"Holon", "IL", 32.01034, 34.77918, 31, tzAsiaJerusalem, 0},
	{"Hong Kong", "HK", 22.27832, 114.17469, 59, tzAsiaHongKong, 0},
	{"Honolulu", "US", 21.30694, -157.85833, 17, tzPacificHonolulu, 0},
	{"Houston", "US", 29.76328, -95.36327, 30, tzAmericaChicago, 0},
	{"Indianapolis", "US", 39.76838, -86.15804, 241, tzAmericaIndianaIndianapolis, 0},
	{"Irkutsk", "RU", 52.29778, 104.29639, 428, tzAsiaIrkutsk, 0},
	{"Irvine", "US", 33.66946, -117.82311, 22, tzAmericaLosAngeles, 0},
	{"Irving", "US", 32.81402, -96.94889, 151, tzAmericaChicago, 0},
	{"Istanbul", "TR", 41.01384, 28.94966, 32, tzEuropeIstanbul, 0},
	{"Jacksonville", "US", 30.33218, -81.65565, 10, tzAmericaNewYork, 0},
	{"Jersey City", "US", 40.72816, -74.07764, 11, tzAmericaNewYork, 0},
	{"Jerusalem", "IL", 31.76904, 35.21633, 786, tzAsiaJerusalem, 0},
	{"Johannesburg", "ZA", -26.20227, 28.04363, 1767, tzAfricaJohannesburg, 0},
	{"Kaifeng", "CN", 34.7986, 114.30742, 75, tzAsiaShanghai, 0},
	{"Kaliningrad", "RU", 54.70649, 20.51095, 2, tzEuropeKaliningrad, 0},
	{"Kansas City", "US", 39.09973, -94.57857, 285, tzAmericaChicago, 0},
	{"Karachi", "PK", 24.8608, 67.0104, 8, tzAsiaKarachi, 0},
	{"Kathmandu", "NP", 27.70169, 85.3206, 1296, tzAsiaKathmandu, 0},
	{"Kazan", "RU", 55.78874, 49.12214, 67, tzEuropeMoscow, 0},
	{"Kfar Saba", "IL", 32.175, 34.90694, 56, tzAsiaJerusalem, 0},
	{"Kharkiv", "UA", 49.98081, 36.25272, 113, tzEuropeKiev, 0},
	{"Kiev", "UA", 50.45466, 30.5238, 187, tzEuropeKiev, 0},
	{"Kiryas Joel", "US", 41.34204, -74.16792, 213, tzAmericaNewYork, 0},
	{"Kiryat Gat", "IL", 31.60998, 34.76422, 133, tzAsiaJerusalem, 0},
	{"Kyiv", "UA", 50.45466, 30.5238, 184, tzEuropeKiev, 0},
	{"Kyoto", "JP", 35.02107, 135.75385, 50, tzAsiaTokyo, 0},
	{"La Paz", "BO", -16.5, -68.15, 3782, tzAmericaLaPaz, 0},
	{"Lagos", "NG", 6.45407, 3.39467, 10, tzAfricaLagos, 0},
	{"Lakewood", "US", 40.09789, -74.21764, 24, tzAmericaNewYork, 0},
	{"Las Vegas", "US", 36.17497, -115.13722, 613, tzAmericaLosAngeles, 0},
	{"Leeds", "GB", 53.79648, -1.54785, 47, tzEuropeLondon, 0},
	{"Leipzig", "DE", 51.33962, 12.37129, 115, tzEuropeBerlin, 0},
	{"Lexington KY", "US", 37.98869, -84.47772, 299, tzAmericaNewYork, 0},
	{"Lima", "PE", -12.04318, -77.02824, 153, tzAmericaLima, 0},
	{"Lincoln", "US", 40.8, -96.66696, 366, tzAmericaChicago, 0},
	{"Livingston", "US", 40.79593, -74.31487, 98, tzAmericaNewYork, 0},
	{"Llandudno", "GB", 53.32498, -3.83148, 11, tzEuropeLondon, 0},
	{"Lod", "IL", 31.9467, 34.8903, 68, tzAsiaJerusalem, 0},
	{"London", "GB", 51.50853, -0.12574, 25, tzEuropeLondon, 0},
	{"Long Beach", "US", 33.76696, -118.18923, 28, tzAmericaLosAngeles, 0},
	{"Los Angeles", "US", 34.05223, -118.24368, 96, tzAmericaLosAngeles, 0},
	{"Lyon", "FR", 45.74846, 4.84671, 173, tzEuropeParis, 0},
	{"Madison", "US", 43.07305, -89.40123, 272, tzAmericaChicago, 0},
	{"Madrid", "ES", 40.4165, -3.70256, 666, tzEuropeMadrid, 0},
	{"Manchester", "GB", 53.48095, -2.23743, 50, tzEuropeLondon, 0},
	{"Manila", "PH", 14.6042, 120.9822, 13, tzAsiaManila, 0},
	{"Marseilles", "FR", 43.29695, 5.38107, 28, tzEuropeParis, 0},
	{"Medzhybizh", "UA", 49.43658, 27.40907, 290, tzEuropeKiev, 0},
	{"Melbourne", "AU", -37.814, 144.96332, 25, tzAustraliaMelbourne, 0},
	{"Memphis", "US", 35.14953, -90.04898, 84, tzAmericaChicago, 0},
	{"Mercer Island", "US", 47.57065, -122.22207, 104, tzAmericaLosAngeles, 0},
	{"Mesa", "US", 33.42227, -111.82264, 380, tzAmericaPhoenix, 0},
	{"Mexico City", "MX", 19.42847, -99.12766, 2240, tzAmericaMexicoCity, 0},
	{"Miami", "US", 25.77427, -80.19366, 25, tzAmericaNewYork, 0},
	{"Milan", "IT", 45.46427, 9.18951, 127, tzEuropeRome, 0},
	{"Milwaukee", "US", 43.0389, -87.90647, 199, tzAmericaChicago, 0},
	{"Minneapolis", "US", 44.97997, -93.26384, 262, tzAmericaChicago, 0},
	{"Minsk", "BY", 53.9, 27.56667, 222, tzEuropeMinsk, 0},
	{"Mississauga", "CA", 43.5789, -79.6583, 160, tzAmericaToronto, 0},
	{"Mitzpe Ramon", "IL", 30.60944, 34.80111, 855, tzAsiaJerusalem, 0},
	{"Modiin", "IL", 31.89825, 35.01051, 240, tzAsiaJerusalem, 0},
	{"Montevideo", "UY", -34.90328, -56.18816, 33, tzAmericaMontevideo, 0},
	{"Montreal", "CA", 45.50884, -73.58781, 216, tzAmericaToronto, 0},
	{"Moscow", "RU", 55.75222, 37.61556, 144, tzEuropeMoscow, 0},
	{"Mumbai", "IN", 19.07283, 72.88261, 8, tzAsiaKolkata, 0},
	{"Munich", "DE", 48.13743, 11.57549, 525, tzEuropeBerlin, 0},
	{"Nashville", "US", 36.16589, -86.78444, 169, tzAmericaChicago, 0},
	{"Nazareth", "IL", 32.69925, 35.30483, 356, tzAsiaJerusalem, 0},
	{"Netanya", "IL", 32.33291, 34.85992, 38, tzAsiaJerusalem, 0},
	{"New Haven", "US", 41.30815, -72.92816, 18, tzAmericaNewYork, 0},
	{"New Orleans", "US", 29.95465, -90.07507, 18, tzAmericaChicago, 0},
	{"New York", "US", 40.71427, -74.00597, 57, tzAmericaNewYork, 0},
	{"Newark", "US", 40.73566, -74.17237, 21, tzAmericaNewYork, 0},
	{"Newton", "US", 42.33704, -71.20922, 37, tzAmericaNewYork, 0},
	{"Nice", "FR", 43.70313, 7.26608, 19, tzEuropeParis, 0},
	{"Norfolk", "US", 36.84681, -76.28522, 13, tzAmericaNewYork, 0},
	{"Oakland", "US", 37.80437, -122.2708, 23, tzAmericaLosAngeles, 0},
	{"Odessa", "UA", 46.48572, 30.74383, 36, tzEuropeKiev, 0},
	{"Oklahoma City", "US", 35.46756, -97.51643, 394, tzAmericaChicago, 0},
	{"Omaha", "US", 41.25861, -95.93779, 315, tzAmericaChicago, 0},
	{"Orlando", "US", 28.53834, -81.37924, 53, tzAmericaNewYork, 0},
	{"Osaka", "JP", 34.69374, 135.50218, 17, tzAsiaTokyo, 0},
	{"Ottawa", "CA", 45.41117, -75.69812, 71, tzAmericaToronto, 0},
	{"Panama City", "PA", 8.9936, -79.51973, 17, tzAmericaPanama, 0},
	{"Paris", "FR", 48.85341, 2.3488, 42, tzEuropeParis, 0},
	{"Passaic", "US", 40.85677, -74.12848, 38, tzAmericaNewYork, 0},
	{"Pawtucket", "US", 41.87871, -71.38256, 0, tzAmericaNewYork, 0},
	{"Perth", "AU", -31.95224, 115.8614, 30, tzAustraliaPerth, 0},
	{"Petach Tikvah", "IL", 32.08707, 34.88747, 54, tzAsiaJerusalem, 0},
	{"Philadelphia", "US", 39.95233, -75.16379, 8, tzAmericaNewYork, 0},
	{"Phoenix", "US", 33.44838, -112.07404, 366, tzAmericaPhoenix, 0},
	{"Pittsburgh", "US", 40.44062, -79.99589, 239, tzAmericaNewYork, 0},
	{"Plano", "US", 33.01984, -96.69889, 207, tzAmericaChicago, 0},
	{"Portland", "US", 45.52345, -122.67621, 15, tzAmericaLosAngeles, 0},
	{"Porto Alegre", "BR", -30.03283, -51.23019, 46, tzAmericaSaoPaulo, 0},
	{"Poway", "US", 32.96282, -117.03586, 156, tzAmericaLosAngeles, 0},
	{"Prague", "CZ", 50.08804, 14.42076, 201, tzEuropePrague, 0},
	{"Princeton", "US", 40.34872, -74.65905, 72, tzAmericaNewYork, 0},
	{"Providence", "US", 41.82399, -71.41283, 0, tzAmericaNewYork, 0},
	{"Ra'anana", "IL", 32.1836, 34.87386, 49, tzAsiaJerusalem, 0},
	{"Raleigh", "US", 35.7721, -78.63861, 99, tzAmericaNewYork, 0},
	{"Ramat Gan", "IL", 32.08227, 34.81065, 56, tzAsiaJerusalem, 0},
	{"Ramla", "IL", 31.92923, 34.86563, 84, tzAsiaJerusalem, 0},
	{"Regina", "CA", 50.45008, -104.6178, 577, tzAmericaRegina, 0},
	{"Reno", "US", 39.52963, -119.8138, 1378, tzAmericaLosAngeles, 0},
	{"Richmond Hill", "CA", 43.87111, -79.43725, 233, tzAmericaToronto, 0},
	{"Richmond", "US", 37.55376, -77.46026, 67, tzAmericaNewYork, 0},
	{"Riga", "LV", 56.946, 24.10589, 6, tzEuropeRiga, 0},
	{"Rio de Janeiro", "BR", -22.90642, -43.18223, 12, tzAmericaSaoPaulo, 0},
	{"Rishon LeZiyyon", "IL", 31.97102, 34.78939, 56, tzAsiaJerusalem, 0},
	{"Riverside", "US", 33.95335, -117.39616, 256, tzAmericaLosAngeles, 0},
	{"Rochester", "US", 43.15478, -77.61556, 157, tzAmericaNewYork, 0},
	{"Rome", "IT", 41.89193, 12.51133, 53, tzEuropeRome, 0},
	{"Rosario", "AR", -32.94682, -60.63932, 38, tzAmericaArgentinaCordoba, 0},
	{"Rotterdam", "NL", 51.9225, 4.47917, 11, tzEuropeAmsterdam, 0},
	{"Sacramento", "US", 38.58157, -121.4944, 16, tzAmericaLosAngeles, 0},
	{"Safed", "IL", 32.96465, 35.496, 779, tzAsiaJerusalem, 0},
	{"Saint Louis", "US", 38.62727, -90.19789, 149, tzAmericaChicago, 0},
	{"Saint Paul", "US", 44.94441, -93.09327, 243, tzAmericaChicago, 0},
	{"Saint Petersburg", "RU", 59.93863, 30.31413, 11, tzEuropeMoscow, 0},
	{"Salzburg", "AT", 47.79941, 13.04399, 435, tzEuropeVienna, 0},
	{"San Antonio", "US", 29.42412, -98.49363, 205, tzAmericaChicago, 0},
	{"San Diego", "US", 32.71533, -117.15726, 20, tzAmericaLosAngeles, 0},
	{"San Francisco", "US", 37.77493, -122.41942, 28, tzAmericaLosAngeles, 0},
	{"San Jose", "US", 37.33939, -121.89496, 23, tzAmericaLosAngeles, 0},
	{"San Juan", "PR", 18.46633, -66.10572, 9, tzAmericaPuertoRico, 0},
	{"San Salvador", "SV", 13.68935, -89.18718, 651, tzAmericaElSalvador, 0},
	{"Santa Ana", "US", 33.74557, -117.86783, 40, tzAmericaLosAngeles, 0},
	{"Santiago", "CL", -33.45694, -70.64827, 555, tzAmericaSantiago, 0},
	{"Sao Paulo", "BR", -23.5475, -46.63611, 769, tzAmericaSaoPaulo, 0},
	{"Saskatoon", "CA", 52.13238, -106.66892, 485, tzAmericaRegina, 0},
	{"Scottsdale", "US", 33.50921, -111.89903, 382, tzAmericaPhoenix, 0},
	{"Sderot", "IL", 31.525, 34.59693, 90, tzAsiaJerusalem, 0},
	{"Seattle", "US", 47.60621, -122.33207, 56, tzAmericaLosAngeles, 0},
	{"Shanghai", "CN", 31.22222, 121.45806, 13, tzAsiaShanghai, 0},
	{"Shenzhen", "CN", 22.54554, 114.0683, 4, tzAsiaShanghai, 0},
	{"Singapore", "SG", 1.28967, 103.85007, 20, tzAsiaSingapore, 0},
	{"Spokane", "US", 47.65966, -117.42908, 539, tzAmericaLosAngeles, 0},
	{"Stanford", "US", 37.42411, -122.16608, 31, tzAmericaLosAngeles, 0},
	{"Stockholm", "SE", 59.32938, 18.06871, 19, tzEuropeStockholm, 0},
	{"Stockton", "US", 37.9577, -121.29078, 6, tzAmericaLosAngeles, 0},
	{"Strasbourg", "FR", 48.58392, 7.74553, 146, tzEuropeParis, 0},
	{"Stuttgart", "DE", 48.78232, 9.17702, 253, tzEuropeBerlin, 0},
	{"Sudbury", "US", 42.38343, -71.41617, 64, tzAmericaNewYork, 0},
	{"Sydney", "AU", -33.86785, 151.20732, 58, tzAustraliaSydney, 0},
	{"Tacoma", "US", 47.25288, -122.44429, 74, tzAmericaLosAngeles, 0},
	{"Tampa", "US", 27.94752, -82.45843, 38, tzAmericaNewYork, 0},
	{"Tashkent", "UZ", 41.26465, 69.21627, 423, tzAsiaTashkent, 0},
	{"Teaneck", "US", 40.8976, -74.01597, 42, tzAmericaNewYork, 0},
	{"Tehran", "IR", 35.69439, 51.42151, 1179, tzAsiaTehran, 0},
	{"Tel Aviv", "IL", 32.08088, 34.78057, 15, tzAsiaJerusalem, 0},
	{"The Hague", "NL", 52.07667, 4.29861, 5, tzEuropeAmsterdam, 0},
	{"Tianjin", "CN", 39.14222, 117.17667, 10, tzAsiaShanghai, 0},
	{"Tiberias", "IL", 32.79221, 35.53124, 0, tzAsiaJerusalem, 0},
	{"Tijuana", "MX", 32.5027, -117.00371, 101, tzAmericaTijuana, 0},
	{"Tokyo", "JP", 35.6895, 139.69171, 42, tzAsiaTokyo, 0},
	{"Toledo", "US", 41.66394, -83.55521, 189, tzAmericaNewYork, 0},
	{"Toronto", "CA", 43.70011, -79.4163, 175, tzAmericaToronto, 0},
	{"Toulouse", "FR", 43.60426, 1.44367, 153, tzEuropeParis, 0},
	{"Tucson", "US", 32.22174, -110.92648, 758, tzAmericaPhoenix, 0},
	{"Tulsa", "US", 36.15398, -95.99277, 228, tzAmericaChicago, 0},
	{"Tunis", "TN", 36.81897, 10.16579, 23, tzAfricaTunis, 0},
	{"Uman", "UA", 48.7501, 30.21944, 219, tzEuropeKiev, 0},
	{"Vancouver", "CA", 49.24966, -123.11934, 70, tzAmericaVancouver, 0},
	{"Vaughan", "CA", 43.8361, -79.49827, 218, tzAmericaToronto, 0},
	{"Venice", "IT", 45.43713, 12.33265, 6, tzEuropeRome, 0},
	{"Vienna", "AT", 48.20849, 16.37208, 191, tzEuropeVienna, 0},
	{"Virginia Beach", "US", 36.85293, -75.97799, 4, tzAmericaNewYork, 0},
	{"Volgograd", "RU", 48.71939, 44.50183, 66, tzEuropeVolgograd, 0},
	{"Warsaw", "PL", 52.22977, 21.01178, 113, tzEuropeWarsaw, 0},
	{"Washington DC", "US", 38.89511, -77.03637, 6, tzAmericaNewYork, 0},
	{"Wellington", "NZ", -41.28664, 174.77557, 32, tzPacificAuckland, 0},
	{"White Plains", "US", 41.03399, -73.76291, 82, tzAmericaNewYork, 0},
	{"Wichita", "US", 37.69224, -97.33754, 402, tzAmericaChicago, 0},
	{"Willemstad", "CW", 12.1084, -68.93354, 2, tzAmericaCuracao, 0},
	{"Windsor", "CA", 42.30008, -83.01654, 190, tzAmericaToronto, 0},
	{"Winnipeg", "CA", 49.8844, -97.14704, 242, tzAmericaWinnipeg, 0},
	{"Woodmere", "US", 40.63205, -73.71263, 11, tzAmericaNewYork, 0},
	{"Worcester", "US", 42.26259, -71.80229, 164, tzAmericaNewYork, 0},
}
