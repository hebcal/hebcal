package event

import "strings"

// SmartApostrophe converts straight apostrophes to U+2019, the same
// substitution applied to event titles ("Sh'vat" => "Sh’vat").
func SmartApostrophe(s string) string {
	return strings.ReplaceAll(s, "'", "’")
}
