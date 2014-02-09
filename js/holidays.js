// nothing here yet

var masks = exports.masks = {};
masks.USER_EVENT          = 1;
masks.LIGHT_CANDLES       = 2;
masks.YOM_TOV_ENDS        = 4;
masks.CHUL_ONLY           = 8; // chutz l'aretz (Diaspora)
masks.IL_ONLY             = 16; // b'aretz (Israel)
masks.LIGHT_CANDLES_TZEIS = 32;

var IGNORE_YEAR = exports.IGNORE_YEAR = 0;

function Event(e) {
	// body...
}