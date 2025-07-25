// Hebcal's nachyomi package calculates the Nach Yomi, a daily
// regimen of learning the books of Nevi'im (Prophets) and
// Ketuvim (Writings).
package nachyomi

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

import (
	"errors"
	"time"

	"github.com/hebcal/greg"
	"github.com/hebcal/hdate"
	"github.com/hebcal/hebcal-go/dafyomi"
)

// Nach Yomi takes slightly longer than two years,
// with each of the 742 days representing a chapter;
// beginning with Joshua 1, on November 1, 2007,
// the cycle will conclude on November 11, 2009 with
// II Chronicles 36
var shas = []dafyomi.Daf{
	// Nevi'im
	{Name: "Joshua", Blatt: 24},
	{Name: "Judges", Blatt: 21},
	{Name: "I Samuel", Blatt: 31},
	{Name: "II Samuel", Blatt: 24},
	{Name: "I Kings", Blatt: 22},
	{Name: "II Kings", Blatt: 25},
	{Name: "Isaiah", Blatt: 66},
	{Name: "Jeremiah", Blatt: 52},
	{Name: "Ezekiel", Blatt: 48},
	{Name: "Hosea", Blatt: 14},
	{Name: "Joel", Blatt: 4},
	{Name: "Amos", Blatt: 9},
	{Name: "Obadiah", Blatt: 1},
	{Name: "Jonah", Blatt: 4},
	{Name: "Micah", Blatt: 7},
	{Name: "Nachum", Blatt: 3},
	{Name: "Habakkuk", Blatt: 3},
	{Name: "Zephaniah", Blatt: 3},
	{Name: "Haggai", Blatt: 2},
	{Name: "Zechariah", Blatt: 14},
	{Name: "Malachi", Blatt: 3},
	// Ketuvim
	{Name: "Psalms", Blatt: 150},
	{Name: "Proverbs", Blatt: 31},
	{Name: "Job", Blatt: 42},
	{Name: "Song of Songs", Blatt: 8},
	{Name: "Ruth", Blatt: 4},
	{Name: "Lamentations", Blatt: 5},
	{Name: "Ecclesiastes", Blatt: 12},
	{Name: "Esther", Blatt: 10},
	{Name: "Daniel", Blatt: 12},
	{Name: "Ezra", Blatt: 10},
	{Name: "Nehemiah", Blatt: 13},
	{Name: "I Chronicles", Blatt: 29},
	{Name: "II Chronicles", Blatt: 36},
}

// NachYomiStart is the R.D. number of the start of the
// Nach Yomi cycle, corresponding to 1 November 2007.
var NachYomiStart = greg.ToRD(2007, time.November, 1)

const numChapters = 742

// NachYomiIndex is an index by day number of the entire
// Nach Yomi cycle.
type NachYomiIndex []dafyomi.Daf

// MakeIndex initializes the index for Mishna Yomi.
func MakeIndex() NachYomiIndex {
	days := make(NachYomiIndex, numChapters)
	i := 0
	for j := 0; j < len(shas); j++ {
		masechet := shas[j]
		for k := 1; k <= masechet.Blatt; k++ {
			days[i] = dafyomi.Daf{Name: masechet.Name, Blatt: k}
			i++
		}
	}
	return days
}

// NachYomiIndex.Lookup calculates the Nach Yomi for given date.
//
// Returns an error if the date is before Nach Yomi cycle began
// (20 May 1947).
func (idx NachYomiIndex) Lookup(hd hdate.HDate) (dafyomi.Daf, error) {
	abs := hd.Abs()
	if abs < NachYomiStart {
		return dafyomi.Daf{}, errors.New("before Nach Yomi cycle began")
	}
	dayNum := (abs - NachYomiStart) % numChapters
	return idx[dayNum], nil
}
