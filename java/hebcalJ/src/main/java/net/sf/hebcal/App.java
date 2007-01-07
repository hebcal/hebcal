/*
   $Header$
   Hebcal - A Jewish Calendar Generator
   Copyright (C) 1994-2004  Danny Sadinoff

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

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.Calendar;

/**
 * Hello world!
 *  @revision $Header$
 *
 */
public class App 
{
    public static void main( String[] args ) throws Exception
    {
        PrintWriter out = new PrintWriter (new BufferedWriter(new OutputStreamWriter(
                                                                                     new FileOutputStream(args[0]),
                                                                                     "UTF-8")));
        out.println("<html><head><META http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"></head><pre dir=\"rtl\">");
        out.println("\u05e0\u05d9\u05e1\u05df<br>");
        out.println("אבגדהוזחטי");
        out.println("אבגדהוזחטי");

        Date startDate = new Date();
        if( args.length > 1)
            {
                int gyear = Integer.parseInt(args[1]);
                Calendar cal = new GregorianCalendar(gyear,0,1);

                startDate  = cal.getTime();
            }
        
        JewishHolidaysCalendar hd = new JewishHolidaysCalendar(startDate,HebrewDate.SEPHARDIC_ENGLISH_LOCALE);
        hd.setIsraeliCalendar(true);
        for(int startGYear = hd.getGregorianYear();
            hd.getGregorianYear() == startGYear;
            hd.forward())

        {
        	out.println();
        	out.println(hd.formatGregorianDate_English());
                out.println(hd.formatHebrewDate_Russian());
                out.println(hd.formatHebrewDate_Hebrew());
        	out.println(hd. formatHebrewDate_English());
        	
        	JewishCalendarEvent[] holidays = hd.getEvents();
        	for( int j = 0; j< holidays.length;j++)
        	{
        		out.println(holidays[j].toString());
        		out.println(holidays[j].getLocalizedString());        		
        	}
        }
        out.flush();
    }
}
