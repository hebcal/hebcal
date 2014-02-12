var c = require('./common'), greg = require('./greg');

var shas = [
	// sname, aname, hname, blatt
	[ "Berachot",       "Berachos",         "ברכות",         64  ],
	[ "Shabbat",        "Shabbos",          "שבת",          157 ],
	[ "Eruvin",         "Eruvin",           "ערובין",         105 ],
	[ "Pesachim",       "Pesachim",         "פסחים",         121 ],
	[ "Shekalim",       "Shekalim",         "שקלים",         22  ],
	[ "Yoma",           "Yoma",             "יומא",           88  ],
	[ "Sukkah",         "Sukkah",           "סוכה",          56  ],
	[ "Beitzah",        "Beitzah",          "ביצה",          40  ],
	[ "Rosh Hashana",   "Rosh Hashana",     "ראש השנה",      35  ],
	[ "Taanit",         "Taanis",           "תענית",          31  ],
	[ "Megillah",       "Megillah",         "מגילה",          32  ],
	[ "Moed Katan",     "Moed Katan",       "מועד קטן",       29  ],
	[ "Chagigah",       "Chagigah",         "חגיגה",          27  ],
	[ "Yevamot",        "Yevamos",          "יבמות",          122 ],
	[ "Ketubot",        "Kesubos",          "כתובות",         112 ],
	[ "Nedarim",        "Nedarim",          "נדרים",          91  ],
	[ "Nazir",          "Nazir",            "נזיר",           66  ],
	[ "Sotah",          "Sotah",            "סוטה",          49  ],
	[ "Gitin",          "Gitin",            "גיטין",           90  ],
	[ "Kiddushin",      "Kiddushin",        "קידושין",         82  ],
	[ "Baba Kamma",     "Baba Kamma",       "בבא קמא",      119 ],
	[ "Baba Metzia",    "Baba Metzia",      "בבא מציעא",     119 ],
	[ "Baba Batra",     "Baba Basra",       "בבא בתרא",     176 ],
	[ "Sanhedrin",      "Sanhedrin",        "סנהדרין",        113 ],
	[ "Makkot",         "Makkos",           "מכות",          24  ],
	[ "Shevuot",        "Shevuos",          "שבועות",        49  ],
	[ "Avodah Zarah",   "Avodah Zarah",     "עבודה זרה",     76  ],
	[ "Horayot",        "Horayos",          "הוריות",         14  ],
	[ "Zevachim",       "Zevachim",         "זבכים",         120 ],
	[ "Menachot",       "Menachos",         "מנחות",         110 ],
	[ "Chullin",        "Chullin",          "חולין",          142 ],
	[ "Bechorot",       "Bechoros",         "בכורות",         61  ],
	[ "Arachin",        "Arachin",          "ערכין",          34  ],
	[ "Temurah",        "Temurah",          "תמורה",         34  ],
	[ "Keritot",        "Kerisos",          "כריתות",         28  ],
	[ "Meilah",         "Meilah",           "מעילה",         22  ],
	[ "Kinnim",         "Kinnim",           "כינים",          4   ],
	[ "Tamid",          "Tamid",            "תמיד",          10  ],
	[ "Midot",          "Midos",            "מידות",          4   ],
	[ "Niddah",         "Niddah",           "נידה",           73  ]
].map(function(m){
	return {name: m.slice(0,3), blatt: m[3]};
});

function dafyomi(gregdate) {
	var dafcnt = 40, cno, dno, osday, nsday, total, count, j, cday, blatt, tmp_date;

	if (!(gregdate instanceof Date)) {
		throw new TypeError('non-date given to dafyomi');
	}

	osday = greg.greg2abs(new Date(1923, 8, 11));
	nsday = greg.greg2abs(new Date(1975, 5, 24));
	cday = greg.greg2abs(gregdate);

	if (cday < osday) { // no cycle; dy didn't start yet
		return {name: [], blatt: 0};
	}
	if (cday >= nsday) { // "new" cycle
		cno = 8 + ( (cday - nsday) / 2711 );
		dno = (cday - nsday) % 2711;
	} else { // old cycle
		cno = 1 + ( (cday - osday) / 2702 );
		dno = (cday - osday) / 2702;
	}

	// Find the daf taking note that the cycle changed slightly after cycle 7.

	total = blatt = 0;
	count = -1;

	// Fix Shekalim for old cycles
	if (cno <= 7) {
		shas[4].blatt = 13;
	} else {
		shas[4].blatt = 22;
	}

	// Find the daf
	j = 0;
	while (j < dafcnt) {
		count++;
		total = total + shas[j].blatt - 1;
		if (dno < total) {
			blatt = (shas[j].blatt + 1) - (total - dno);
			// fiddle with the weird ones near the end
			switch (count) {
				case 36:
					blatt = blatt + 21;
					break;
				case 37:
					blatt = blatt + 24;
					break;
				case 38:
					blatt = blatt + 33;
					break;
				default:
					break;
			}
			// Bailout
			j = 1 + dafcnt;
		}
		j++;
	}

	return {name: shas[count].name, blatt: blatt};
}
exports.dafyomi = dafyomi;

function dafname(daf, o) {
	return c.LANGUAGE(daf.name, o) + ' ' + (o === 'h' ? c.gematriya(daf.blatt) : daf.blatt);
}
exports.dafname = dafname;