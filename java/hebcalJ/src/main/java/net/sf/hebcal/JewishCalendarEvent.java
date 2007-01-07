/*
   $Header$
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2006  Danny Sadinoff

   http://sourceforge.net/projects/hebcal

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   Danny Sadinoff can be reached at 

   danny@sadinoff.com
 */
package net.sf.hebcal;

import java.util.BitSet;
import java.util.ResourceBundle;

public class JewishCalendarEvent
{
	final String key;
	private final ResourceBundle bundle;
	final BitSet flags;
	private final boolean formatted ;
	
	// bitset offsets
	public static final int USER_EVENT	 = 0;
	public static final int LIGHT_CANDLES	 = 1;
	public static final int YOM_TOV_ENDS	 = 2;
    public static final int YOM_TOV	 = 3;
    public static final int FAST_DAY	 = 4;
    public static final int ROSH_CHODESH	 = 5;
    public static final int OMER		 = 6;
    public static final int SEDRA		 = 7;
    public static final int CANDLELIGHTING_TIME	 = 8;
    public static final int HAVDALAH_TIME	 = 9;
    public static final int HEBREW_DATE	 = 10;
    public static final int CHOL_HAMOED	 = 11;
    public static final int CHANUKAH	 = 12;
    public static final int SPECIAL_SHABBAT = 13;
    public static final int HOLIDAY_NON_YOM_TOV	 = 14;
    public static final int YAHRTZEIT	 = 15;
    public static final int SATURDAY_PARSHA = 16; 
    public static final int PURIM = 17; 
    public static final int SHUSHAN_PURIM = 18; 
    public static final int TAANIT_BECHOROT = 19; 
	private static final String[] bitsetElements = 
	{
		"user-event",
		"light-candles",
		"yom-tov-ends",
		"yom-tov",
		"fast-day",
		"rosh-chodesh",
		"omer",
		"sedra",
		"candlelighting",
		"havdalah",
		"hebrew-date",
		"chol-hamoed",
		"chanukah",
		"special-shabbat",
		"holiday-non-yom-tov",
		"yahrtzeit",
		"saturday-parsha",
		"purim",
		"shushan-purim",
		"taanit-bechorot",
	};
	
	
	JewishCalendarEvent(final String preformattedMessage,
			int[] flagsArray)
	{
		this.key = preformattedMessage;
		this.bundle = null;
		this.formatted = true;
		this.flags = new BitSet();
		if (null != flagsArray)
		{
			for( int i =0; i< flagsArray.length;i++)
				flags.set(flagsArray[i]);
		}
	}

			
	JewishCalendarEvent(final String key, 
			final ResourceBundle bundle,
			int[] flagsArray)
	{
		this.key = key;
		this.bundle = bundle;
		this.flags = new BitSet();
		this.formatted = false;
		if (null != flagsArray)
		{
			for( int i =0; i< flagsArray.length;i++)
				flags.set(flagsArray[i]);
		}
	}
	
	public String getLocalizedString()
	{
		if( formatted )
			return key;
		else
			return bundle.getString(key);
	}
	
	public String getKey()
	{
		return key;
	}
	
	public String toString()
	{
		StringBuffer buf= new StringBuffer("{Holiday:"+key+" flags=[");
		for (int i =0; i< bitsetElements.length;i++)
			if( flags.get(i))
				buf.append(bitsetElements[i]+",");
		buf.append("]}");
		return buf.toString();
	}
	
	public BitSet getFlags() {
		return (BitSet)flags.clone();
	}
	
}
