package dafyomi

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2026 Michael J. Radwin
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

import (
	"strconv"
	"strings"

	"github.com/hebcal/learning/internal/sefaria"
)

// dafYomiSefaria maps a tractate name to its Sefaria spelling where the two
// differ. Tractates not listed keep their own name.
var dafYomiSefaria = map[string]string{
	"Berachot":     "Berakhot",
	"Rosh Hashana": "Rosh Hashanah",
	"Gitin":        "Gittin",
	"Baba Kamma":   "Bava Kamma",
	"Baba Metzia":  "Bava Metzia",
	"Baba Batra":   "Bava Batra",
	"Bechorot":     "Bekhorot",
	"Arachin":      "Arakhin",
	"Midot":        "Middot",
	"Shekalim":     "Jerusalem_Talmud_Shekalim",
}

// URL returns a link to sefaria.org or dafyomi.org for the given Talmud
// page, matching the DafPageEvent.url() logic in @hebcal/learning. It is
// shared by the Daf Yomi and Daf-a-Week schedules.
func URL(daf Daf) string {
	tractate := daf.Name
	blatt := daf.Blatt
	if tractate == "Kinnim" || tractate == "Midot" {
		return "https://www.dafyomi.org/index.php?masechta=meilah&daf=" +
			strconv.Itoa(blatt) + "a"
	}
	if tractate == "Shekalim" {
		aEntry := shekalimDafYomiMap[strconv.Itoa(blatt)+"a"]
		bEntry := shekalimDafYomiMap[strconv.Itoa(blatt)+"b"]
		aStart := strings.SplitN(aEntry, "-", 2)[0]
		bEnd := bEntry
		if i := strings.Index(bEntry, "-"); i >= 0 {
			bEnd = bEntry[i+1:]
		}
		ref := strings.ReplaceAll(aStart+"-"+bEnd, ":", ".")
		return sefaria.URL("Jerusalem Talmud Shekalim", ref)
	}
	name0 := tractate
	if alt, ok := dafYomiSefaria[tractate]; ok {
		name0 = alt
	}
	return sefaria.URL(name0, strconv.Itoa(blatt)+"a")
}
