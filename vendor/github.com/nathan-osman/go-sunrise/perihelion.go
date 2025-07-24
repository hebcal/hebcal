package sunrise

// ArgumentOfPerihelion calculates the argument of periapsis for the earth on
// the given Julian day.
func ArgumentOfPerihelion(d float64) float64 {
	return 102.93005 + 0.3179526*(d-2451545)/36525
}
