// Package sefaria builds sefaria.org reference URLs shared by the schedule
// packages, mirroring the TypeScript src/common.ts sefariaUrl helper.
package sefaria

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

import "strings"

// URL builds a Sefaria URL of the form
// https://www.sefaria.org/{book}.{chapter}?lang=bi . Spaces in book are
// converted to underscores automatically, matching sefariaUrl() in the
// TypeScript @hebcal/learning package.
func URL(book, chapter string) string {
	slug := strings.ReplaceAll(book, " ", "_")
	return "https://www.sefaria.org/" + slug + "." + chapter + "?lang=bi"
}

// EncodeURIComponent percent-encodes s the same way JavaScript's
// encodeURIComponent() does: every byte is escaped except for the
// unreserved set A-Z a-z 0-9 and - _ . ! ~ * ' ( ). It is used to build
// Mishneh Torah reference URLs whose section names contain characters such
// as commas.
func EncodeURIComponent(s string) string {
	const upperhex = "0123456789ABCDEF"
	var b strings.Builder
	for i := 0; i < len(s); i++ {
		c := s[i]
		if isUnreserved(c) {
			b.WriteByte(c)
		} else {
			b.WriteByte('%')
			b.WriteByte(upperhex[c>>4])
			b.WriteByte(upperhex[c&0x0f])
		}
	}
	return b.String()
}

func isUnreserved(c byte) bool {
	switch {
	case c >= 'A' && c <= 'Z':
		return true
	case c >= 'a' && c <= 'z':
		return true
	case c >= '0' && c <= '9':
		return true
	}
	switch c {
	case '-', '_', '.', '!', '~', '*', '\'', '(', ')':
		return true
	}
	return false
}
