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
	{"Abuja", "NG", 9.05785, 7.49508, 476, tzAfricaLagos},
	{"Acre", "IL", 32.92814, 35.07647, 8, tzAsiaJerusalem},
	{"Adelaide", "AU", -34.92866, 138.59863, 61, tzAustraliaAdelaide},
	{"Albany", "US", 42.65258, -73.75623, 57, tzAmericaNewYork},
	{"Albuquerque", "US", 35.08449, -106.65114, 1516, tzAmericaDenver},
	{"Almaty", "KZ", 43.25, 76.91667, 795, tzAsiaAlmaty},
	{"Amsterdam", "NL", 52.37403, 4.88969, 12, tzEuropeAmsterdam},
	{"Anaheim", "US", 33.83529, -117.9145, 53, tzAmericaLosAngeles},
	{"Anchorage", "US", 61.21806, -149.90028, 31, tzAmericaAnchorage},
	{"Arad", "IL", 31.25882, 35.21282, 611, tzAsiaJerusalem},
	{"Arlington TX", "US", 32.73569, -97.10807, 190, tzAmericaChicago},
	{"Ashdod", "IL", 31.79213, 34.64966, 27, tzAsiaJerusalem},
	{"Ashkelon", "IL", 31.66926, 34.57149, 42, tzAsiaJerusalem},
	{"Ashqelon", "IL", 31.66926, 34.57149, 42, tzAsiaJerusalem},
	{"Athens", "GR", 37.98376, 23.72784, 86, tzEuropeAthens},
	{"Atlanta", "US", 33.749, -84.38798, 336, tzAmericaNewYork},
	{"Auckland", "NZ", -36.84853, 174.76349, 43, tzPacificAuckland},
	{"Aurora", "US", 39.72943, -104.83192, 1651, tzAmericaDenver},
	{"Austin", "US", 30.26715, -97.74306, 165, tzAmericaChicago},
	{"Baghdad", "IQ", 33.34058, 44.40088, 41, tzAsiaBaghdad},
	{"Bakersfield", "US", 35.37329, -119.01871, 129, tzAmericaLosAngeles},
	{"Baku", "AZ", 40.37767, 49.89201, 0, tzAsiaBaku},
	{"Baltimore", "US", 39.29038, -76.61219, 35, tzAmericaNewYork},
	{"Bangkok", "TH", 13.75398, 100.50144, 11, tzAsiaBangkok},
	{"Barcelona", "ES", 41.38879, 2.15899, 47, tzEuropeMadrid},
	{"Basel", "CH", 47.55839, 7.57327, 279, tzEuropeZurich},
	{"Bat Yam", "IL", 32.02379, 34.75185, 34, tzAsiaJerusalem},
	{"Baton Rouge|LA", "US", 30.44332, -91.18747, 20, tzAmericaChicago},
	{"Beer Sheva", "IL", 31.25181, 34.7913, 285, tzAsiaJerusalem},
	{"Beersheba", "IL", 31.25181, 34.7913, 286, tzAsiaJerusalem},
	{"Beijing", "CN", 39.9075, 116.39723, 49, tzAsiaShanghai},
	{"Berlin", "DE", 52.52437, 13.41053, 43, tzEuropeBerlin},
	{"Bet Shemesh", "IL", 31.73072, 34.99293, 285, tzAsiaJerusalem},
	{"Birmingham", "GB", 52.48142, -1.89983, 150, tzEuropeLondon},
	{"Birobidzhan", "RU", 48.79284, 132.92386, 85, tzAsiaVladivostok},
	{"Bnei Brak", "IL", 32.08074, 34.8338, 50, tzAsiaJerusalem},
	{"Bogota", "CO", 4.60971, -74.08175, 2582, tzAmericaBogota},
	{"Boise", "US", 43.6135, -116.20345, 835, tzAmericaBoise},
	{"Bolzano", "IT", 46.49067, 11.33982, 257, tzEuropeRome},
	{"Boston", "US", 42.35843, -71.05977, 38, tzAmericaNewYork},
	{"Bozen", "IT", 46.49067, 11.33982, 257, tzEuropeRome},
	{"Brisbane", "AU", -27.46794, 153.02809, 40, tzAustraliaBrisbane},
	{"Brussels", "BE", 50.85045, 4.34878, 28, tzEuropeBrussels},
	{"Bucharest", "RO", 44.43225, 26.10626, 83, tzEuropeBucharest},
	{"Budapest", "HU", 47.49801, 19.03991, 104, tzEuropeBudapest},
	{"Buenos Aires", "AR", -34.61315, -58.37723, 31, tzAmericaArgentinaBuenosAires},
	{"Buffalo", "US", 42.88645, -78.87837, 191, tzAmericaNewYork},
	{"Burlington", "US", 44.47588, -73.21207, 62, tzAmericaNewYork},
	{"Cairo", "EG", 30.06263, 31.24967, 25, tzAfricaCairo},
	{"Calgary", "CA", 51.05011, -114.08529, 1043, tzAmericaEdmonton},
	{"Cape Town", "ZA", -33.92584, 18.42322, 26, tzAfricaJohannesburg},
	{"Caracas", "VE", 10.48801, -66.87919, 893, tzAmericaCaracas},
	{"Casablanca", "MA", 33.58831, -7.61138, 26, tzAfricaCasablanca},
	{"Chandler", "US", 33.30616, -111.84125, 368, tzAmericaPhoenix},
	{"Chapel Hill", "US", 35.9132, -79.05584, 149, tzAmericaNewYork},
	{"Charlotte", "US", 35.22709, -80.84313, 279, tzAmericaNewYork},
	{"Chicago", "US", 41.85003, -87.65005, 180, tzAmericaChicago},
	{"Chisinau", "MD", 47.00556, 28.8575, 53, tzEuropeChisinau},
	{"Chula Vista", "US", 32.64005, -117.0842, 21, tzAmericaLosAngeles},
	{"Cincinnati", "US", 39.162, -84.45689, 267, tzAmericaNewYork},
	{"Cleveland", "US", 41.4995, -81.69541, 204, tzAmericaNewYork},
	{"Colorado Springs", "US", 38.83388, -104.82136, 1837, tzAmericaDenver},
	{"Columbus", "US", 39.96118, -82.99879, 241, tzAmericaNewYork},
	{"Copenhagen", "DK", 55.67594, 12.56553, 14, tzEuropeCopenhagen},
	{"Corpus Christi", "US", 27.80058, -97.39638, 9, tzAmericaChicago},
	{"Dallas", "US", 32.78306, -96.80667, 139, tzAmericaChicago},
	{"Delhi", "IN", 28.65195, 77.23149, 228, tzAsiaKolkata},
	{"Denver", "US", 39.73915, -104.9847, 1636, tzAmericaDenver},
	{"Des Moines", "US", 41.60054, -93.60911, 265, tzAmericaChicago},
	{"Detroit", "US", 42.33143, -83.04575, 192, tzAmericaDetroit},
	{"Dhaka", "BD", 23.7104, 90.40744, 23, tzAsiaDhaka},
	{"Dimona", "IL", 31.07079, 35.03269, 551, tzAsiaJerusalem},
	{"Dnipro", "UA", 48.46664, 35.04066, 61, tzEuropeKiev},
	{"Dortmund", "DE", 51.51494, 7.466, 99, tzEuropeBerlin},
	{"Dresden", "DE", 51.05089, 13.73832, 117, tzEuropeBerlin},
	{"Dubai", "AE", 25.07725, 55.30927, 24, tzAsiaDubai},
	{"Dublin", "IE", 53.33306, -6.24889, 17, tzEuropeDublin},
	{"Dundee", "GB", 56.46913, -2.97489, 77, tzEuropeLondon},
	{"Durban", "ZA", -29.8579, 31.0292, 18, tzAfricaJohannesburg},
	{"Durham", "US", 35.99403, -78.89862, 122, tzAmericaNewYork},
	{"Dusseldorf", "DE", 51.22172, 6.77616, 46, tzEuropeBerlin},
	{"Edmonton", "CA", 53.55014, -113.46871, 610, tzAmericaEdmonton},
	{"Eilat", "IL", 29.55805, 34.94821, 63, tzAsiaJerusalem},
	{"El Paso", "US", 31.75872, -106.48693, 1136, tzAmericaDenver},
	{"Far Rockaway", "US", 40.60538, -73.75513, 6, tzAmericaNewYork},
	{"Fort Wayne", "US", 41.1306, -85.12886, 250, tzAmericaIndianaIndianapolis},
	{"Fort Worth", "US", 32.72541, -97.32085, 203, tzAmericaChicago},
	{"Frankfurt", "DE", 50.11552, 8.68417, 114, tzEuropeBerlin},
	{"Fremont", "US", 37.54827, -121.98857, 18, tzAmericaLosAngeles},
	{"Fresno", "US", 36.74773, -119.77237, 100, tzAmericaLosAngeles},
	{"Gibraltar", "GI", 36.14474, -5.35257, 11, tzEuropeGibraltar},
	{"Glasgow", "GB", 55.86515, -4.25763, 41, tzEuropeLondon},
	{"Great Neck", "US", 40.80066, -73.72846, 38, tzAmericaNewYork},
	{"Greenlawn", "US", 40.86899, -73.36512, 71, tzAmericaNewYork},
	{"Greensboro", "US", 36.07264, -79.79198, 259, tzAmericaNewYork},
	{"Grenoble", "FR", 45.17869, 5.71479, 223, tzEuropeParis},
	{"Guadalajara", "MX", 20.66682, -103.39182, 1598, tzAmericaMexicoCity},
	{"Guangzhou", "CN", 23.11667, 113.25, 15, tzAsiaShanghai},
	{"Hadera", "IL", 32.44192, 34.9039, 12, tzAsiaJerusalem},
	{"Haifa", "IL", 32.81841, 34.9885, 40, tzAsiaJerusalem},
	{"Halifax", "CA", 44.6464, -63.57291, 28, tzAmericaHalifax},
	{"Hamburg", "DE", 53.55073, 9.99302, 11, tzEuropeBerlin},
	{"Hamilton", "CA", 43.25011, -79.84963, 94, tzAmericaToronto},
	{"Hartford", "US", 41.76371, -72.68509, 25, tzAmericaNewYork},
	{"Hawaii", "US", 21.30694, -157.85833, 18, tzPacificHonolulu},
	{"Helsinki", "FI", 60.16952, 24.93545, 26, tzEuropeHelsinki},
	{"Henderson", "US", 36.0397, -114.98194, 573, tzAmericaLosAngeles},
	{"Herzliya", "IL", 32.16627, 34.82536, 28, tzAsiaJerusalem},
	{"Holon", "IL", 32.01034, 34.77918, 31, tzAsiaJerusalem},
	{"Hong Kong", "HK", 22.27832, 114.17469, 59, tzAsiaHongKong},
	{"Honolulu", "US", 21.30694, -157.85833, 17, tzPacificHonolulu},
	{"Houston", "US", 29.76328, -95.36327, 30, tzAmericaChicago},
	{"Indianapolis", "US", 39.76838, -86.15804, 241, tzAmericaIndianaIndianapolis},
	{"Irkutsk", "RU", 52.29778, 104.29639, 428, tzAsiaIrkutsk},
	{"Irvine", "US", 33.66946, -117.82311, 22, tzAmericaLosAngeles},
	{"Irving", "US", 32.81402, -96.94889, 151, tzAmericaChicago},
	{"Istanbul", "TR", 41.01384, 28.94966, 32, tzEuropeIstanbul},
	{"Jacksonville", "US", 30.33218, -81.65565, 10, tzAmericaNewYork},
	{"Jersey City", "US", 40.72816, -74.07764, 11, tzAmericaNewYork},
	{"Jerusalem", "IL", 31.76904, 35.21633, 786, tzAsiaJerusalem},
	{"Johannesburg", "ZA", -26.20227, 28.04363, 1767, tzAfricaJohannesburg},
	{"Kaifeng", "CN", 34.7986, 114.30742, 75, tzAsiaShanghai},
	{"Kaliningrad", "RU", 54.70649, 20.51095, 2, tzEuropeKaliningrad},
	{"Kansas City", "US", 39.09973, -94.57857, 285, tzAmericaChicago},
	{"Karachi", "PK", 24.8608, 67.0104, 8, tzAsiaKarachi},
	{"Kathmandu", "NP", 27.70169, 85.3206, 1296, tzAsiaKathmandu},
	{"Kazan", "RU", 55.78874, 49.12214, 67, tzEuropeMoscow},
	{"Kfar Saba", "IL", 32.175, 34.90694, 56, tzAsiaJerusalem},
	{"Kharkiv", "UA", 49.98081, 36.25272, 113, tzEuropeKiev},
	{"Kiev", "UA", 50.45466, 30.5238, 187, tzEuropeKiev},
	{"Kiryas Joel", "US", 41.34204, -74.16792, 213, tzAmericaNewYork},
	{"Kiryat Gat", "IL", 31.60998, 34.76422, 133, tzAsiaJerusalem},
	{"Kyiv", "UA", 50.45466, 30.5238, 184, tzEuropeKiev},
	{"Kyoto", "JP", 35.02107, 135.75385, 50, tzAsiaTokyo},
	{"La Paz", "BO", -16.5, -68.15, 3782, tzAmericaLaPaz},
	{"Lagos", "NG", 6.45407, 3.39467, 10, tzAfricaLagos},
	{"Lakewood", "US", 40.09789, -74.21764, 24, tzAmericaNewYork},
	{"Las Vegas", "US", 36.17497, -115.13722, 613, tzAmericaLosAngeles},
	{"Leeds", "GB", 53.79648, -1.54785, 47, tzEuropeLondon},
	{"Leipzig", "DE", 51.33962, 12.37129, 115, tzEuropeBerlin},
	{"Lexington KY", "US", 37.98869, -84.47772, 299, tzAmericaNewYork},
	{"Lima", "PE", -12.04318, -77.02824, 153, tzAmericaLima},
	{"Lincoln", "US", 40.8, -96.66696, 366, tzAmericaChicago},
	{"Livingston", "US", 40.79593, -74.31487, 98, tzAmericaNewYork},
	{"Llandudno", "GB", 53.32498, -3.83148, 11, tzEuropeLondon},
	{"Lod", "IL", 31.9467, 34.8903, 68, tzAsiaJerusalem},
	{"London", "GB", 51.50853, -0.12574, 25, tzEuropeLondon},
	{"Long Beach", "US", 33.76696, -118.18923, 28, tzAmericaLosAngeles},
	{"Los Angeles", "US", 34.05223, -118.24368, 96, tzAmericaLosAngeles},
	{"Lyon", "FR", 45.74846, 4.84671, 173, tzEuropeParis},
	{"Madison", "US", 43.07305, -89.40123, 272, tzAmericaChicago},
	{"Madrid", "ES", 40.4165, -3.70256, 666, tzEuropeMadrid},
	{"Manchester", "GB", 53.48095, -2.23743, 50, tzEuropeLondon},
	{"Manila", "PH", 14.6042, 120.9822, 13, tzAsiaManila},
	{"Marseilles", "FR", 43.29695, 5.38107, 28, tzEuropeParis},
	{"Medzhybizh", "UA", 49.43658, 27.40907, 290, tzEuropeKiev},
	{"Melbourne", "AU", -37.814, 144.96332, 25, tzAustraliaMelbourne},
	{"Memphis", "US", 35.14953, -90.04898, 84, tzAmericaChicago},
	{"Mercer Island", "US", 47.57065, -122.22207, 104, tzAmericaLosAngeles},
	{"Mesa", "US", 33.42227, -111.82264, 380, tzAmericaPhoenix},
	{"Mexico City", "MX", 19.42847, -99.12766, 2240, tzAmericaMexicoCity},
	{"Miami", "US", 25.77427, -80.19366, 25, tzAmericaNewYork},
	{"Milan", "IT", 45.46427, 9.18951, 127, tzEuropeRome},
	{"Milwaukee", "US", 43.0389, -87.90647, 199, tzAmericaChicago},
	{"Minneapolis", "US", 44.97997, -93.26384, 262, tzAmericaChicago},
	{"Minsk", "BY", 53.9, 27.56667, 222, tzEuropeMinsk},
	{"Mississauga", "CA", 43.5789, -79.6583, 160, tzAmericaToronto},
	{"Mitzpe Ramon", "IL", 30.60944, 34.80111, 855, tzAsiaJerusalem},
	{"Modiin", "IL", 31.89825, 35.01051, 240, tzAsiaJerusalem},
	{"Montevideo", "UY", -34.90328, -56.18816, 33, tzAmericaMontevideo},
	{"Montreal", "CA", 45.50884, -73.58781, 216, tzAmericaToronto},
	{"Moscow", "RU", 55.75222, 37.61556, 144, tzEuropeMoscow},
	{"Mumbai", "IN", 19.07283, 72.88261, 8, tzAsiaKolkata},
	{"Munich", "DE", 48.13743, 11.57549, 525, tzEuropeBerlin},
	{"Nashville", "US", 36.16589, -86.78444, 169, tzAmericaChicago},
	{"Nazareth", "IL", 32.69925, 35.30483, 356, tzAsiaJerusalem},
	{"Netanya", "IL", 32.33291, 34.85992, 38, tzAsiaJerusalem},
	{"New Haven", "US", 41.30815, -72.92816, 18, tzAmericaNewYork},
	{"New Orleans", "US", 29.95465, -90.07507, 18, tzAmericaChicago},
	{"New York", "US", 40.71427, -74.00597, 57, tzAmericaNewYork},
	{"Newark", "US", 40.73566, -74.17237, 21, tzAmericaNewYork},
	{"Newton", "US", 42.33704, -71.20922, 37, tzAmericaNewYork},
	{"Nice", "FR", 43.70313, 7.26608, 19, tzEuropeParis},
	{"Norfolk", "US", 36.84681, -76.28522, 13, tzAmericaNewYork},
	{"Oakland", "US", 37.80437, -122.2708, 23, tzAmericaLosAngeles},
	{"Odessa", "UA", 46.48572, 30.74383, 36, tzEuropeKiev},
	{"Oklahoma City", "US", 35.46756, -97.51643, 394, tzAmericaChicago},
	{"Omaha", "US", 41.25861, -95.93779, 315, tzAmericaChicago},
	{"Orlando", "US", 28.53834, -81.37924, 53, tzAmericaNewYork},
	{"Osaka", "JP", 34.69374, 135.50218, 17, tzAsiaTokyo},
	{"Ottawa", "CA", 45.41117, -75.69812, 71, tzAmericaToronto},
	{"Panama City", "PA", 8.9936, -79.51973, 17, tzAmericaPanama},
	{"Paris", "FR", 48.85341, 2.3488, 42, tzEuropeParis},
	{"Passaic", "US", 40.85677, -74.12848, 38, tzAmericaNewYork},
	{"Pawtucket", "US", 41.87871, -71.38256, 0, tzAmericaNewYork},
	{"Perth", "AU", -31.95224, 115.8614, 30, tzAustraliaPerth},
	{"Petach Tikvah", "IL", 32.08707, 34.88747, 54, tzAsiaJerusalem},
	{"Philadelphia", "US", 39.95233, -75.16379, 8, tzAmericaNewYork},
	{"Phoenix", "US", 33.44838, -112.07404, 366, tzAmericaPhoenix},
	{"Pittsburgh", "US", 40.44062, -79.99589, 239, tzAmericaNewYork},
	{"Plano", "US", 33.01984, -96.69889, 207, tzAmericaChicago},
	{"Portland", "US", 45.52345, -122.67621, 15, tzAmericaLosAngeles},
	{"Porto Alegre", "BR", -30.03283, -51.23019, 46, tzAmericaSaoPaulo},
	{"Poway", "US", 32.96282, -117.03586, 156, tzAmericaLosAngeles},
	{"Prague", "CZ", 50.08804, 14.42076, 201, tzEuropePrague},
	{"Princeton", "US", 40.34872, -74.65905, 72, tzAmericaNewYork},
	{"Providence", "US", 41.82399, -71.41283, 0, tzAmericaNewYork},
	{"Ra'anana", "IL", 32.1836, 34.87386, 49, tzAsiaJerusalem},
	{"Raleigh", "US", 35.7721, -78.63861, 99, tzAmericaNewYork},
	{"Ramat Gan", "IL", 32.08227, 34.81065, 56, tzAsiaJerusalem},
	{"Ramla", "IL", 31.92923, 34.86563, 84, tzAsiaJerusalem},
	{"Regina", "CA", 50.45008, -104.6178, 577, tzAmericaRegina},
	{"Reno", "US", 39.52963, -119.8138, 1378, tzAmericaLosAngeles},
	{"Richmond Hill", "CA", 43.87111, -79.43725, 233, tzAmericaToronto},
	{"Richmond", "US", 37.55376, -77.46026, 67, tzAmericaNewYork},
	{"Riga", "LV", 56.946, 24.10589, 6, tzEuropeRiga},
	{"Rio de Janeiro", "BR", -22.90642, -43.18223, 12, tzAmericaSaoPaulo},
	{"Rishon LeZiyyon", "IL", 31.97102, 34.78939, 56, tzAsiaJerusalem},
	{"Riverside", "US", 33.95335, -117.39616, 256, tzAmericaLosAngeles},
	{"Rochester", "US", 43.15478, -77.61556, 157, tzAmericaNewYork},
	{"Rome", "IT", 41.89193, 12.51133, 53, tzEuropeRome},
	{"Rosario", "AR", -32.94682, -60.63932, 38, tzAmericaArgentinaCordoba},
	{"Rotterdam", "NL", 51.9225, 4.47917, 11, tzEuropeAmsterdam},
	{"Sacramento", "US", 38.58157, -121.4944, 16, tzAmericaLosAngeles},
	{"Safed", "IL", 32.96465, 35.496, 779, tzAsiaJerusalem},
	{"Saint Louis", "US", 38.62727, -90.19789, 149, tzAmericaChicago},
	{"Saint Paul", "US", 44.94441, -93.09327, 243, tzAmericaChicago},
	{"Saint Petersburg", "RU", 59.93863, 30.31413, 11, tzEuropeMoscow},
	{"Salzburg", "AT", 47.79941, 13.04399, 435, tzEuropeVienna},
	{"San Antonio", "US", 29.42412, -98.49363, 205, tzAmericaChicago},
	{"San Diego", "US", 32.71533, -117.15726, 20, tzAmericaLosAngeles},
	{"San Francisco", "US", 37.77493, -122.41942, 28, tzAmericaLosAngeles},
	{"San Jose", "US", 37.33939, -121.89496, 23, tzAmericaLosAngeles},
	{"San Juan", "PR", 18.46633, -66.10572, 9, tzAmericaPuertoRico},
	{"San Salvador", "SV", 13.68935, -89.18718, 651, tzAmericaElSalvador},
	{"Santa Ana", "US", 33.74557, -117.86783, 40, tzAmericaLosAngeles},
	{"Santiago", "CL", -33.45694, -70.64827, 555, tzAmericaSantiago},
	{"Sao Paulo", "BR", -23.5475, -46.63611, 769, tzAmericaSaoPaulo},
	{"Saskatoon", "CA", 52.13238, -106.66892, 485, tzAmericaRegina},
	{"Scottsdale", "US", 33.50921, -111.89903, 382, tzAmericaPhoenix},
	{"Sderot", "IL", 31.525, 34.59693, 90, tzAsiaJerusalem},
	{"Seattle", "US", 47.60621, -122.33207, 56, tzAmericaLosAngeles},
	{"Shanghai", "CN", 31.22222, 121.45806, 13, tzAsiaShanghai},
	{"Shenzhen", "CN", 22.54554, 114.0683, 4, tzAsiaShanghai},
	{"Singapore", "SG", 1.28967, 103.85007, 20, tzAsiaSingapore},
	{"Spokane", "US", 47.65966, -117.42908, 539, tzAmericaLosAngeles},
	{"Stanford", "US", 37.42411, -122.16608, 31, tzAmericaLosAngeles},
	{"Stockholm", "SE", 59.32938, 18.06871, 19, tzEuropeStockholm},
	{"Stockton", "US", 37.9577, -121.29078, 6, tzAmericaLosAngeles},
	{"Strasbourg", "FR", 48.58392, 7.74553, 146, tzEuropeParis},
	{"Stuttgart", "DE", 48.78232, 9.17702, 253, tzEuropeBerlin},
	{"Sudbury", "US", 42.38343, -71.41617, 64, tzAmericaNewYork},
	{"Sydney", "AU", -33.86785, 151.20732, 58, tzAustraliaSydney},
	{"Tacoma", "US", 47.25288, -122.44429, 74, tzAmericaLosAngeles},
	{"Tampa", "US", 27.94752, -82.45843, 38, tzAmericaNewYork},
	{"Tashkent", "UZ", 41.26465, 69.21627, 423, tzAsiaTashkent},
	{"Teaneck", "US", 40.8976, -74.01597, 42, tzAmericaNewYork},
	{"Tehran", "IR", 35.69439, 51.42151, 1179, tzAsiaTehran},
	{"Tel Aviv", "IL", 32.08088, 34.78057, 15, tzAsiaJerusalem},
	{"The Hague", "NL", 52.07667, 4.29861, 5, tzEuropeAmsterdam},
	{"Tianjin", "CN", 39.14222, 117.17667, 10, tzAsiaShanghai},
	{"Tiberias", "IL", 32.79221, 35.53124, 0, tzAsiaJerusalem},
	{"Tijuana", "MX", 32.5027, -117.00371, 101, tzAmericaTijuana},
	{"Tokyo", "JP", 35.6895, 139.69171, 42, tzAsiaTokyo},
	{"Toledo", "US", 41.66394, -83.55521, 189, tzAmericaNewYork},
	{"Toronto", "CA", 43.70011, -79.4163, 175, tzAmericaToronto},
	{"Toulouse", "FR", 43.60426, 1.44367, 153, tzEuropeParis},
	{"Tucson", "US", 32.22174, -110.92648, 758, tzAmericaPhoenix},
	{"Tulsa", "US", 36.15398, -95.99277, 228, tzAmericaChicago},
	{"Tunis", "TN", 36.81897, 10.16579, 23, tzAfricaTunis},
	{"Uman", "UA", 48.7501, 30.21944, 219, tzEuropeKiev},
	{"Vancouver", "CA", 49.24966, -123.11934, 70, tzAmericaVancouver},
	{"Vaughan", "CA", 43.8361, -79.49827, 218, tzAmericaToronto},
	{"Venice", "IT", 45.43713, 12.33265, 6, tzEuropeRome},
	{"Vienna", "AT", 48.20849, 16.37208, 191, tzEuropeVienna},
	{"Virginia Beach", "US", 36.85293, -75.97799, 4, tzAmericaNewYork},
	{"Volgograd", "RU", 48.71939, 44.50183, 66, tzEuropeVolgograd},
	{"Warsaw", "PL", 52.22977, 21.01178, 113, tzEuropeWarsaw},
	{"Washington DC", "US", 38.89511, -77.03637, 6, tzAmericaNewYork},
	{"Wellington", "NZ", -41.28664, 174.77557, 32, tzPacificAuckland},
	{"White Plains", "US", 41.03399, -73.76291, 82, tzAmericaNewYork},
	{"Wichita", "US", 37.69224, -97.33754, 402, tzAmericaChicago},
	{"Willemstad", "CW", 12.1084, -68.93354, 2, tzAmericaCuracao},
	{"Windsor", "CA", 42.30008, -83.01654, 190, tzAmericaToronto},
	{"Winnipeg", "CA", 49.8844, -97.14704, 242, tzAmericaWinnipeg},
	{"Woodmere", "US", 40.63205, -73.71263, 11, tzAmericaNewYork},
	{"Worcester", "US", 42.26259, -71.80229, 164, tzAmericaNewYork},
}
