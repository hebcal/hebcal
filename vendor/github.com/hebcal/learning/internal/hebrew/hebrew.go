// Package hebrew holds Hebrew-rendering helpers shared by the schedule
// packages, mirroring the TypeScript src/common.ts.
package hebrew

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
	"strings"

	"github.com/hebcal/gematriya"
)

var stripPunct = strings.NewReplacer("׳", "", "״", "")

// GematriyaNN formats a number as Hebrew letters without the geresh /
// gershayim punctuation marks, matching the TypeScript gematriyaNN().
func GematriyaNN(n int) string {
	return stripPunct.Replace(gematriya.Gematriya(n))
}
