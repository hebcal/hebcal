package sunrise

import "math"

// DefaultPlaces specifies the default precision for rounding.
const DefaultPlaces = 5

// Round takes the provided float and rounds it to the specified number of
// decimal places. This function is adapted from user korya on GitHub
// (https://gist.github.com/DavidVaini/10308388#gistcomment-1391788).
func Round(f float64, places int) float64 {
	shift := math.Pow(10, float64(places))
	return math.Floor(f*shift+.5) / shift
}
