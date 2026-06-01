package nine29

// Code generated from @hebcal/learning tanakhNumChap.json; DO NOT EDIT manually.

// tanakhBook is one book of the Tanakh and its chapter count.
type tanakhBook struct {
	name    string
	numChap int
}

// tanakhBooks lists the 39 books of the Tanakh in 929 reading order,
// totaling 929 chapters.
var tanakhBooks = []tanakhBook{
	{name: "Genesis", numChap: 50},
	{name: "Exodus", numChap: 40},
	{name: "Leviticus", numChap: 27},
	{name: "Numbers", numChap: 36},
	{name: "Deuteronomy", numChap: 34},
	{name: "Joshua", numChap: 24},
	{name: "Judges", numChap: 21},
	{name: "I Samuel", numChap: 31},
	{name: "II Samuel", numChap: 24},
	{name: "I Kings", numChap: 22},
	{name: "II Kings", numChap: 25},
	{name: "Isaiah", numChap: 66},
	{name: "Jeremiah", numChap: 52},
	{name: "Ezekiel", numChap: 48},
	{name: "Hosea", numChap: 14},
	{name: "Joel", numChap: 4},
	{name: "Amos", numChap: 9},
	{name: "Obadiah", numChap: 1},
	{name: "Jonah", numChap: 4},
	{name: "Micah", numChap: 7},
	{name: "Nachum", numChap: 3},
	{name: "Habakkuk", numChap: 3},
	{name: "Zephaniah", numChap: 3},
	{name: "Haggai", numChap: 2},
	{name: "Zechariah", numChap: 14},
	{name: "Malachi", numChap: 3},
	{name: "Psalms", numChap: 150},
	{name: "Proverbs", numChap: 31},
	{name: "Job", numChap: 42},
	{name: "Song of Songs", numChap: 8},
	{name: "Ruth", numChap: 4},
	{name: "Lamentations", numChap: 5},
	{name: "Ecclesiastes", numChap: 12},
	{name: "Esther", numChap: 10},
	{name: "Daniel", numChap: 12},
	{name: "Ezra", numChap: 10},
	{name: "Nehemiah", numChap: 13},
	{name: "I Chronicles", numChap: 29},
	{name: "II Chronicles", numChap: 36},
}
