/*
	Hebcal - A Jewish Calendar Generator
	Copyright (C) 1994-2004  Danny Sadinoff
	Portions Copyright (c) 2002 Michael J. Radwin. All Rights Reserved.

	https://github.com/hebcal/hebcal

	Licensed GPLv3.

	You should have received a copy of the GNU General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Danny Sadinoff can be reached at 
	danny@sadinoff.com

	Michael Radwin has made significant contributions as a result of
	maintaining hebcal.com.

	The JavaScript code was completely rewritten in 2014 by Scimonster
 */
window.Hebcal = require('./hebcal');

if (navigator.geolocation) {
	Hebcal.ready = false;
	navigator.geolocation.getCurrentPosition(function(p){
		Hebcal.defaultLocation = [p.coords.latitude,p.coords.longitude];
		ready();
	}, ready);
} else {
	ready();
}

function ready() {
	Hebcal.ready = true;
	if (typeof Hebcal.onready === 'function') {
		Hebcal.onready();
	}
}