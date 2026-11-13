// The Hebrew strings in this file were copied byte for byte, by script, from
// @hebcal/core 6.16.0 (dist/esm/omer.js). Do not retype them: nikud order
// matters, and a retyped string can look identical while its bytes differ.

package omer

var tens = []string{
	"",
	"עֲשָׂרָה",
	"עֶשְׂרִים",
	"שְׁלוֹשִׁים",
	"אַרְבָּעִים",
}

var ones = []string{
	"",
	"אֶחָד",
	"שְׁנַיִם",
	"שְׁלוֹשָׁה",
	"אַרְבָּעָה",
	"חֲמִשָּׁה",
	"שִׁשָּׁה",
	"שִׁבְעָה",
	"שְׁמוֹנָה",
	"תִּשְׁעָה",
}

const (
	shnei     = "שְׁנֵי"
	yamim     = "יָמִים"
	shavuot   = "שָׁבוּעוֹת"
	yom       = "יוֹם"
	asar      = "עָשָׂר"
	hayom     = "הַיּוֹם"
	achad     = "אַחַד"
	shneim    = "שְׁנֵים"
	vavShuruk = "וּ"
	vavSheva  = "וְ"
	shehem    = "שֶׁהֵם"
	shavua    = "שָׁבֽוּעַ"
	vavPatach = "וַ"
	laOmer    = "לָעֽוֹמֶר"
)

// sefirotHe names the day within the week.
var sefirotHe = []string{
	"",
	"חֶֽסֶד",
	"גְּבוּרָה",
	"תִּפְאֶֽרֶת",
	"נֶּֽצַח",
	"הוֹד",
	"יְּסוֹד",
	"מַלְכוּת",
}

// sefirotHePfx names the week, with its "within" prefix already attached.
var sefirotHePfx = []string{
	"",
	"שֶׁבְּחֶֽסֶד",
	"שֶׁבִּגְבוּרָה",
	"שֶׁבְּתִפְאֶֽרֶת",
	"שֶׁבְּנֶֽצַח",
	"שֶׁבְּהוֹד",
	"שֶׁבִּיְסוֹד",
	"שֶׁבְּמַלְכוּת",
}
