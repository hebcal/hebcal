// Hebcal's mishnayomi package calculates the Mishna Yomi, a program
// of daily learning in which participants study two Mishnayot each
// day in order to finish the entire Mishnah in ~6 years.
package mishnayomi

import (
	"errors"
	"strconv"

	"github.com/hebcal/hdate"
)

// Hebcal - A Jewish Calendar Generator
// Copyright (c) 2022 Michael J. Radwin
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

// Mishna represents a single chapter and verse of Mishnah,
// such as Avodah Zarah 2:5
type Mishna struct {
	Tractate string // Tractate name (e..g Berachot)
	Chap     int    // Page number
	Verse    int    // Page number
}

// MishnaPair is a pair of Mishna Yomis read on the same day.
type MishnaPair []Mishna

// MishnaYomiIndex is an index by day number of the entire 6-year
// Mishna Yomi cycle. Each day is represented by a 2-element
// array of Mishna objects.
type MishnaYomiIndex []MishnaPair

var mishnayot = []struct {
	k string // tractate
	v []int  // chapter lengths
}{
	{k: "Berakhot", v: []int{5, 8, 6, 7, 5, 8, 5, 8, 5}},
	{k: "Peah", v: []int{6, 8, 8, 11, 8, 11, 8, 9}},
	{k: "Demai", v: []int{4, 5, 6, 7, 11, 12, 8}},
	{k: "Kilayim", v: []int{9, 11, 7, 9, 8, 9, 8, 6, 10}},
	{k: "Sheviit", v: []int{8, 10, 10, 10, 9, 6, 7, 11, 9, 9}},
	{k: "Terumot", v: []int{10, 6, 9, 13, 9, 6, 7, 12, 7, 12, 10}},
	{k: "Maasrot", v: []int{8, 8, 10, 6, 8}},
	{k: "Maaser Sheni", v: []int{7, 10, 13, 12, 15}},
	{k: "Challah", v: []int{9, 8, 10, 11}},
	{k: "Orlah", v: []int{9, 17, 9}},
	{k: "Bikkurim", v: []int{11, 11, 12, 5}},
	{k: "Shabbat", v: []int{11, 7, 6, 2, 4, 10, 4, 7, 7, 6, 6, 6, 7, 4, 3, 8, 8, 3, 6, 5, 3, 6, 5, 5}},
	{k: "Eruvin", v: []int{10, 6, 9, 11, 9, 10, 11, 11, 4, 15}},
	{k: "Pesachim", v: []int{7, 8, 8, 9, 10, 6, 13, 8, 11, 9}},
	{k: "Shekalim", v: []int{7, 5, 4, 9, 6, 6, 7, 8}},
	{k: "Yoma", v: []int{8, 7, 11, 6, 7, 8, 5, 9}},
	{k: "Sukkah", v: []int{11, 9, 15, 10, 8}},
	{k: "Beitzah", v: []int{10, 10, 8, 7, 7}},
	{k: "Rosh Hashanah", v: []int{9, 9, 8, 9}},
	{k: "Taanit", v: []int{7, 10, 9, 8}},
	{k: "Megillah", v: []int{11, 6, 6, 10}},
	{k: "Moed Katan", v: []int{10, 5, 9}},
	{k: "Chagigah", v: []int{8, 7, 8}},
	{k: "Yevamot", v: []int{4, 10, 10, 13, 6, 6, 6, 6, 6, 9, 7, 6, 13, 9, 10, 7}},
	{k: "Ketubot", v: []int{10, 10, 9, 12, 9, 7, 10, 8, 9, 6, 6, 4, 11}},
	{k: "Nedarim", v: []int{4, 5, 11, 8, 6, 10, 9, 7, 10, 8, 12}},
	{k: "Nazir", v: []int{7, 10, 7, 7, 7, 11, 4, 2, 5}},
	{k: "Sotah", v: []int{9, 6, 8, 5, 5, 4, 8, 7, 15}},
	{k: "Gittin", v: []int{6, 7, 8, 9, 9, 7, 9, 10, 10}},
	{k: "Kiddushin", v: []int{10, 10, 13, 14}},
	{k: "Bava Kamma", v: []int{4, 6, 11, 9, 7, 6, 7, 7, 12, 10}},
	{k: "Bava Metzia", v: []int{8, 11, 12, 12, 11, 8, 11, 9, 13, 6}},
	{k: "Bava Batra", v: []int{6, 14, 8, 9, 11, 8, 4, 8, 10, 8}},
	{k: "Sanhedrin", v: []int{6, 5, 8, 5, 5, 6, 11, 7, 6, 6, 6}},
	{k: "Makkot", v: []int{10, 8, 16}},
	{k: "Shevuot", v: []int{7, 5, 11, 13, 5, 7, 8, 6}},
	{k: "Eduyot", v: []int{14, 10, 12, 12, 7, 3, 9, 7}},
	{k: "Avodah Zarah", v: []int{9, 7, 10, 12, 12}},
	{k: "Avot", v: []int{18, 16, 18, 22, 23, 11}},
	{k: "Horayot", v: []int{5, 7, 8}},
	{k: "Zevachim", v: []int{4, 5, 6, 6, 8, 7, 6, 12, 7, 8, 8, 6, 8, 10}},
	{k: "Menachot", v: []int{4, 5, 7, 5, 9, 7, 6, 7, 9, 9, 9, 5, 11}},
	{k: "Chullin", v: []int{7, 10, 7, 7, 5, 7, 6, 6, 8, 4, 2, 5}},
	{k: "Bekhorot", v: []int{7, 9, 4, 10, 6, 12, 7, 10, 8}},
	{k: "Arakhin", v: []int{4, 6, 5, 4, 6, 5, 5, 7, 8}},
	{k: "Temurah", v: []int{6, 3, 5, 4, 6, 5, 6}},
	{k: "Keritot", v: []int{7, 6, 10, 3, 8, 9}},
	{k: "Meilah", v: []int{4, 9, 8, 6, 5, 6}},
	{k: "Tamid", v: []int{4, 5, 9, 3, 6, 3, 4}},
	{k: "Middot", v: []int{9, 6, 8, 7, 4}},
	{k: "Kinnim", v: []int{4, 5, 6}},
	{k: "Kelim", v: []int{9, 8, 8, 4, 11, 4, 6, 11, 8, 8, 9, 8, 8, 8, 6, 8, 17, 9, 10, 7, 3, 10, 5, 17, 9, 9, 12, 10, 8, 4}},
	{k: "Oholot", v: []int{8, 7, 7, 3, 7, 7, 6, 6, 16, 7, 9, 8, 6, 7, 10, 5, 5, 10}},
	{k: "Negaim", v: []int{6, 5, 8, 11, 5, 8, 5, 10, 3, 10, 12, 7, 12, 13}},
	{k: "Parah", v: []int{4, 5, 11, 4, 9, 5, 12, 11, 9, 6, 9, 11}},
	{k: "Tahorot", v: []int{9, 8, 8, 13, 9, 10, 9, 9, 9, 8}},
	{k: "Mikvaot", v: []int{8, 10, 4, 5, 6, 11, 7, 5, 7, 8}},
	{k: "Niddah", v: []int{7, 7, 7, 7, 9, 14, 5, 4, 11, 8}},
	{k: "Makhshirin", v: []int{6, 11, 8, 10, 11, 8}},
	{k: "Zavim", v: []int{6, 4, 3, 7, 12}},
	{k: "Tevul Yom", v: []int{5, 8, 6, 7}},
	{k: "Yadayim", v: []int{5, 4, 5, 8}},
	{k: "Oktzin", v: []int{6, 10, 12}},
}

// MishnaYomiStart is the R.D. number of the start of the Mishna Yomi cycle,
// corresponding to 20 May 1947.
const MishnaYomiStart = 710901

const numMishnayot = 4192
const numDays = numMishnayot / 2

// MakeIndex initializes the index for Mishna Yomi.
func MakeIndex() MishnaYomiIndex {
	tmp := make([]Mishna, numMishnayot)
	i := 0
	for _, tractate := range mishnayot {
		for chap := 1; chap <= len(tractate.v); chap++ {
			numv := tractate.v[chap-1]
			for verse := 1; verse <= numv; verse++ {
				tmp[i] = Mishna{Tractate: tractate.k, Chap: chap, Verse: verse}
				i++
			}
		}
	}
	days := make(MishnaYomiIndex, numDays)
	for j := 0; j < numDays; j++ {
		k := j * 2
		days[j] = MishnaPair{tmp[k], tmp[k+1]}
	}
	return days
}

// MYomiIndex.Lookup calculates the Mishna Yomi for given date.
//
// Returns an error if the date is before Mishna Yomi cycle began
// (20 May 1947).
func (idx MishnaYomiIndex) Lookup(hd hdate.HDate) (MishnaPair, error) {
	abs := hd.Abs()
	if abs < MishnaYomiStart {
		return []Mishna{}, errors.New("before Mishna Yomi cycle began")
	}
	dayNum := (abs - MishnaYomiStart) % numDays
	return idx[dayNum], nil
}

// Returns a string representation of the Mishna Yomi.
func (m Mishna) String() string {
	return m.Tractate + " " + strconv.Itoa(m.Chap) + ":" + strconv.Itoa(m.Verse)
}

// Returns a string representation of the Mishna Yomi.
func (pair MishnaPair) String() string {
	m1 := pair[0]
	m2 := pair[1]
	s := m1.String() + "-"
	sameTractate := m1.Tractate == pair[1].Tractate
	if !sameTractate {
		s += m2.Tractate + " "
	}
	if sameTractate && m2.Chap == m1.Chap {
		s += strconv.Itoa(m2.Verse)
	} else {
		s += strconv.Itoa(m2.Chap) + ":" + strconv.Itoa(m2.Verse)
	}
	return s
}
