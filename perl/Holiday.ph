#     $Header$
#     Hebcal - A Jewish Calendar Generator
#     Copyright (C) 1994-1998  Danny Sadinoff

#     This program is free software; you can redistribute it and/or
#     modify it under the terms of the GNU General Public License
#     as published by the Free Software Foundation; either version 2
#     of the License, or (at your option) any later version.

#     This program is distributed in the hope that it will be useful,
#     but WITHOUT ANY WARRANTY; without even the implied warranty of
#     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#     GNU General Public License for more details.

#     You should have received a copy of the GNU General Public License
#     along with this program; if not, write to the Free Software
#     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#     Danny Sadinoff can be reached at 
#     1 Cove La.
#     Great Neck, NY
#     11024

#     sadinoff@pobox.com
#     http://pobox.com/~sadinoff


package Holiday;


$USER_EVENT	 = 1;
$LIGHT_CANDLES	 = 1<<1;
$YOM_TOV_ENDS	 = 1<<2;
   
   # event designators
$YOM_TOV	 = 1<<3;
$FAST_DAY	 = 1<<4;
$ROSH_CHODESH	 = 1<<5;
$OMER		 = 1<<6;
$SEDRA		 = 1<<7;
$CANDLELIGHTING_TIME	 = 1<<8;
$HAVDALAH_TIME	 = 1<<9;
$HEBREW_DATE	 = 1<<10;
$CHOL_HAMOED	 = 1<<11;
$CHANUKAH	 = 1<<12;
$SPECIAL_SHABBAT = 1<<13;
$HOLIDAY_NON_YOM_TOV	 = 1<<14;
$YAHRTZEIT	 = 1<<15;
1;
