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

import java.util.Calendar;
import java.util.Date;
import java.util.Locale;
import java.util.ResourceBundle;

/**
 * Adapted from HebDate Avrom Finkelstein's Java port of the Reingold C++ algorithm.
 * http://www.bayt.org/calendar/hebdate.html
 * 
 * The HebrewDate class allows one to maintain an instance of a Gregorian date along
 * with the corresponding hebrew date.<P>
 * This class can use the standard Java Date and Calendar classes for setting it, but does
 * not subclass these classes or use them internally to any extensive use. This class also does not
 * have a concept of a time (which the Date class does). If you are looking for
 * a class that implements a hebrew calendar version of the Calendar class, one is available
 * from <A HREF="http://oss.software.ibm.com/developerworks/opensource/icu4j/">developerWorks</A> by IBM.
 * <P>
 * The Java code which is contained in this class was translated from my C++
 * code. Some of that C++ code was translated or taken from other C/C++ code
 * in "Calendrical Calculations" by Nachum Dershowitz and
 * Edward M. Reingold, Software-- Practice &amp; Experience,
 * vol. 20, no. 9 (September, 1990), pp. 899- 928.
 * <P>
 * Available at <A HREF="http://emr.cs.uiuc.edu/~reingold/calendar.ps">http://emr.cs.uiuc.edu/~reingold/calendar.ps</A><BR>
 * Original C++ source: <A HREF="http://emr.cs.uiuc.edu/~reingold/calendar.C">http://emr.cs.uiuc.edu/~reingold/calendar.C</A>
 * <P>
 * @see java.util.Date
 * @see java.util.Calendar

 *  @author Nachum Dershowitz
 *  @author Edward M. Reingold
 *  @author Avrom Finkelstein
 *  @author Danny Sadinoff
 */
public class HebrewDate implements Comparable {
	/** sephardic english locale, i.e.  "16 Tevet" */
	public static final Locale SEPHARDIC_ENGLISH_LOCALE = new Locale("en", "IL");

	/** russian */
	public static final Locale RUSSIAN_LOCALE = new Locale("ru");

	/** Ashkenazis english locale, i.e.  "16 Teves" */
	public static final Locale ASHKENAZIS_ENGLISH_LOCALE = new Locale("en",
			"PL");

	/** Hebrew locale, i.e.  "??? ???" */
	public static final Locale HEBREW_LOCALE = new Locale("he");


	/** sephardic english locale, i.e.  "16 Tevet" */
	public static final Locale DEFAULT_ENGLISH_LOCALE = SEPHARDIC_ENGLISH_LOCALE;


	/** Current month (hebrew or gregorian) this object is set to. */
	public static final int CURRENT_MONTH = 0;

	/** Current date of month (hebrew or gregorian) this object is set to. */
	public static final int CURRENT_DATE = 0;

	/** Current year (hebrew or gregorian) this object is set to. */
	public static final int CURRENT_YEAR = 0;

	private static final int HEBREW_EPOCH = -1373429;

	private static final int JANUARY = 1;
	private static final int DECEMBER = 12;

	public static final int DOW_SUNDAY = 1;
	public static final int DOW_SATURDAY= 7;

	public static final int MONTH_NISAN = 1;
	public static final int MONTH_IYAR = 2;
	public static final int MONTH_SIVAN = 3;
	public static final int MONTH_TAMMUZ = 4;
	public static final int MONTH_AV = 5;
	public static final int MONTH_ELUL = 6;
	public static final int MONTH_TISHREI = 7;
	public static final int MONTH_CHESHVAN = 8;
	public static final int MONTH_KISLEV = 9;
	public static final int MONTH_TEVET = 10;
	public static final int MONTH_SHVAT = 11;
	public static final int MONTH_ADAR = 12;
	public static final int MONTH_ADARII = 13;

	private static final String[] hebrewMonthProp = { "hmonth.NISAN",
			"hmonth.IYAR", "hmonth.SIVAN", "hmonth.TAMMUZ", "hmonth.AV",
			"hmonth.ELUL", "hmonth.TISHREI", "hmonth.CHESHVAN",
			"hmonth.KISLEV", "hmonth.TEVET", "hmonth.SHVAT", "hmonth.ADAR",
			"hmonth.ADARII" };

	private static final String[] gregMonthProp = { "January", "February",
			"March", "April", "May", "June", "July", "August", "September",
			"October", "November", "December" };


	private ResourceBundle bundle = null;
	private Locale locale = null;

	protected int hebrewMonth;
	protected int hebrewDate;
	protected int hebrewYear;
	protected int gMonth;
	protected int gDayOfMonth;
	protected int gYear;

	/** 1-based.  Sun=1, Sat =7.  */
	protected int dayOfWeek;

	protected int absDate;

	// Any method with the mark "ND+ER" means that method was taken
	// from this source with minor modifications.

	/**
	 * Gets how many days are in a month. */
	// ND+ER
	public static int getLastDayOfMonth(final int month, int gYear) {
		switch (month) {
		case 2:
			if ((((gYear % 4) == 0) && ((gYear % 100) != 0))
					|| ((gYear % 400) == 0))
				return 29;
			else
				return 28;
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;
		default:
			return 31;
		}
	}

	/**
	 * Gets how many days are in the current month. */
	// ND+ER
	public int getLastDayOfMonth() {
		return getLastDayOfMonth(gMonth, gYear);
	}

	// ND+ER //
	// Computes the Gregorian date from the absolute date.  //
	private void absDateToDate() {
		// Search forward year by year from approximate year//
		gYear = absDate / 366;
		while (absDate >= dateToAbsDate(1, 1, gYear + 1))
			gYear++;
		// Search forward month by month from January//
		gMonth = 1;
		while (absDate > dateToAbsDate(gMonth, getLastDayOfMonth(), gYear))
			gMonth++;
		gDayOfMonth = absDate - dateToAbsDate(gMonth, 1, gYear) + 1;
	}

	/**
	 * Returns the absolute date (days on the gregorian since Januray 1, 1)
	 */
	public int getAbsDate() {
		return absDate;
	}

	// ND+ER //
	// Computes the absolute date from the Gregorian date. Default is current date.//
	private static int dateToAbsDate(int month, int date, int year) {

		for (int m = month - 1; m > 0; m--)
			// days in prior months this year //
			date = date + getLastDayOfMonth(m, year);
		return (date // days this year //
				+ 365 * (year - 1) // days in previous years ignoring leap days //
				+ (year - 1) / 4 // Julian leap days before this year... //
				- (year - 1) / 100 // ...minus prior century years... //
		+ (year - 1) / 400); // ...plus prior years divisible by 400 //
	}

	/*
	 * return the locale associated with this calendar.  
	 */
	public Locale getLocale() {
		return locale;
	}

	/**
	 * returns true if the year is an Hebrew leap year. */
	public static boolean isHebrewLeapYear(int year) {
		if ((((7 * year) + 1) % 19) < 7)
			return true;
		else
			return false;
	}

	/**
	 * returns true if the current hebrew year is a leap year. */
	public boolean isThisHebrewLeapYear() {
		return isHebrewLeapYear(this.hebrewYear);
	}

	/**
	 * returns the last month of the Hebrew year. That is, either 12 or 13. */
	public static int getLastMonthOfHebrewYear(int hYear) {
		if (isHebrewLeapYear(hYear))
			return 13;
		else
			return 12;
	}
	
	/**
	 * returns the last month of the Hebrew year. That is, either 12 or 13. */
	public int getLastMonthOfThisHebrewYear() {
		return getLastMonthOfHebrewYear(hebrewYear);
	}	

	// ND+ER //
	// Number of days elapsed from the Sunday prior to the start of the //
	// Hebrew calendar to the mean conjunction of Tishri of Hebrew year.//
	private static int getHebrewCalendarElapsedDays(int year) {

		int monthsElapsed = (235 * ((year - 1) / 19)) // Months in complete cycles so far.//
				+ (12 * ((year - 1) % 19)) // Regular months in this cycle.//
				+ (7 * ((year - 1) % 19) + 1) / 19; // Leap months this cycle//
		int partsElapsed = 204 + 793 * (monthsElapsed % 1080);
		int hoursElapsed = 5 + 12 * monthsElapsed + 793
				* (monthsElapsed / 1080) + partsElapsed / 1080;
		int conjunctionDay = 1 + 29 * monthsElapsed + hoursElapsed / 24;
		int conjunctionParts = 1080 * (hoursElapsed % 24) + partsElapsed % 1080;
		int alternativeDay;
		if ((conjunctionParts >= 19440) // If new moon is at or after midday,//
				|| (((conjunctionDay % 7) == 2) // ...or is on a Tuesday...//
						&& (conjunctionParts >= 9924) // at 9 hours, 204 parts or later...//
				&& !isHebrewLeapYear(year)) // ...of a common year,//
				|| (((conjunctionDay % 7) == 1) // ...or is on a Monday at...//
						&& (conjunctionParts >= 16789) // 15 hours, 589 parts or later...//
				&& (isHebrewLeapYear(year - 1))))// at the end of a leap year//
			// Then postpone Rosh HaShanah one day//
			alternativeDay = conjunctionDay + 1;
		else
			alternativeDay = conjunctionDay;
		if (((alternativeDay % 7) == 0)// If Rosh HaShanah would occur on Sunday,//
				|| ((alternativeDay % 7) == 3) // or Wednesday,//
				|| ((alternativeDay % 7) == 5)) // or Friday//
			// Then postpone it one (more) day//
			return (1 + alternativeDay);
		else
			return alternativeDay;
	}

	// ND+ER //
	// Number of days in Hebrew year.//
	private int getDaysInThisHebrewYear() {
		return getDaysInHebrewYear(hebrewYear);

	}
	
	private static int getDaysInHebrewYear(int hYear) {
		return ((getHebrewCalendarElapsedDays(hYear + 1)) - (getHebrewCalendarElapsedDays(hYear)));
	}	

	// ND+ER //
	// True if Heshvan is long in Hebrew year. //
	protected  static boolean isCheshvanLong(int hYear) {
		if ((getDaysInHebrewYear(hYear) % 10) == 5)
			return true;
		else
			return false;
	}

	// ND+ER //
	// True if Kislev is short in Hebrew year.//
	protected static boolean isKislevShort(int hYear) {
		if ((getDaysInHebrewYear(hYear) % 10) == 3)
			return true;
		else
			return false;
	}

	/**
	 * Returns last day of a hebrew month. */
	public  static int getLastDayOfHebrewMonth(int hMonth, int hYear) {
		if ((hMonth == 2) || (hMonth == 4) || (hMonth == 6)
				|| ((hMonth == 8) && !(isCheshvanLong(hYear)))
				|| ((hMonth == 9) && isKislevShort(hYear)) || (hMonth == 10)
				|| ((hMonth == 12) && !(isHebrewLeapYear(hYear))) || (hMonth == 13))
			return 29;
		else
			return 30;
	}

	/**
	 * Returns last day of hebrew month.  */
	public int getLastDayOfHebrewMonth() {
		return getLastDayOfHebrewMonth(hebrewMonth,hebrewYear);
	}

	// ND+ER //
	// Computes the Hebrew date from the absolute date. //
	private void absDateToHebrewDate() {
		hebrewYear = (absDate + HEBREW_EPOCH) / 366; // Approximation from below.//
		// Search forward for year from the approximation.//
		while (absDate >= hebrewDateToAbsDate(7, 1, hebrewYear + 1))
			hebrewYear++;

		// Search forward for month from either Tishri or Nisan.//
		if (absDate < hebrewDateToAbsDate(1, 1, hebrewYear))
			hebrewMonth = 7;//Start at Tishri //
		else
			hebrewMonth = 1;//Start at Nisan //

		while (absDate > hebrewDateToAbsDate(hebrewMonth,
				getLastDayOfHebrewMonth(), hebrewYear))
			hebrewMonth++;

		// Calculate the day by subtraction.//
		hebrewDate = absDate - hebrewDateToAbsDate(hebrewMonth, 1, hebrewYear)
				+ 1;
	}

	// ND+ER //
	// Computes the absolute date of Hebrew date. Default is current hebrew date. //
	private static int hebrewDateToAbsDate(int hMonth, int hDayOfMonth, int hYear) {
		int m;

		// Before Tishri, so add days in prior months//
		if (hMonth < 7) {
			// this year before and after Nisan.//
			for (m = 7; m <= getLastMonthOfHebrewYear(hYear); m++)
				hDayOfMonth = hDayOfMonth + getLastDayOfHebrewMonth(m,hYear);

			for (m = 1; m < hMonth; m++)
				hDayOfMonth = hDayOfMonth + getLastDayOfHebrewMonth(m,hYear);
		}

		// Add days in prior months this year//
		else {
			for (m = 7; m < hMonth; m++)
				hDayOfMonth = hDayOfMonth + getLastDayOfHebrewMonth(m,hYear);
		}

		return (hDayOfMonth + getHebrewCalendarElapsedDays(hYear) // Days in prior years.//
		+ HEBREW_EPOCH); // Days elapsed before absolute date 1.//
	}

	/** Inits date based on gregorian date (month, day, year) */
	public HebrewDate(int gMonth, int gDayOfMonth, int gYear, Locale loc)
			throws HebrewDateException {
		// put in nicer message in exception
		if (gMonth == CURRENT_MONTH || gDayOfMonth == CURRENT_DATE
				|| gYear == CURRENT_YEAR)
			throw new HebrewDateException("Illegal value in constructor.");
		setDate(gMonth, gDayOfMonth, gYear);
		setLocale(loc);
	}

	/**
	 * Initializes a date based on the current system date.
	 */
	public HebrewDate() {
		this(DEFAULT_ENGLISH_LOCALE);
	}

	/**
	 * Initializes a date based on the current system date.
	 */
	public HebrewDate(Locale loc) {
		resetDate();
		setLocale(loc);
	}

	/**
	 * Initializes a date based on java.util.Date object.
	 */
	public HebrewDate(Date date, Locale loc) {
		Calendar cal = Calendar.getInstance();
		cal.setTime(date);
		setDate(cal);
		setLocale(loc);
	}

	/**
	 * Initializes a date based on java.util.Calendar object.
	 */
	public HebrewDate(Calendar cal, Locale loc) {
		setDate(cal);
		setLocale(loc);
	}

	/**
	 * 	set the locale which determines how to render the holiday data.
	 * @param loc
	 * @throws IllegalArgumentException if the locale is unknown
	 */
	public void setLocale(final Locale loc) throws IllegalArgumentException {
		this.locale = loc;
		bundle = ResourceBundle.getBundle("net.sf.hebcal.Calendar", loc);
	}

	/**
	 * Sets the date based on a java.util.Calendar object. Modifies the hebrew date as well.
	 */
	public void setDate(Calendar cal) {
		this.gMonth = cal.get(Calendar.MONTH) + 1;
		this.gDayOfMonth = cal.get(Calendar.DATE);
		this.gYear = cal.get(Calendar.YEAR);

		// init the hebrew date //
		absDate = dateToAbsDate(gMonth, gDayOfMonth, gYear);
		absDateToHebrewDate();

		// set day of week //
		dayOfWeek = Math.abs(absDate % 7) + 1;
	}

	/** Sets the Gregorian Date, and updates the hebrew date accordingly */
	public void setDate(final int gMonth, final int gDayOfMonth, final int gYear)
			throws HebrewDateException {
		// precond should be 1->12 anyways, but just in case... //
		if (gMonth > 12 || gMonth < 0)
			throw new HebrewDateException("Illegal value for month= " + gMonth);
		if (gDayOfMonth < 0)
			throw new HebrewDateException("Illegal value for date= " + gDayOfMonth);
		// make sure date is a valid date for the given month, if not, set to last day of month //
		if (gDayOfMonth > getLastDayOfMonth(gMonth, gYear))
			throw new IllegalArgumentException(gDayOfMonth + " >  lastDayOfMonth("
					+ gMonth + ")");
		// date= getLastDayOfMonth(month);
		if (gYear < 0)
			throw new HebrewDateException("Illegal value for year= " + gYear);

		// init  month, date, year //
		if (gMonth != CURRENT_MONTH)
			this.gMonth = gMonth;
		if (gDayOfMonth != CURRENT_DATE)
			this.gDayOfMonth = gDayOfMonth;
		if (gYear != CURRENT_YEAR)
			this.gYear = gYear;

		// init the hebrew date //
		absDate = dateToAbsDate(this.gMonth, this.gDayOfMonth, this.gYear);
		absDateToHebrewDate();

		// set day of week //
		dayOfWeek = Math.abs(absDate % 7) + 1;
	}

	/** Sets the Hebrew Date, and updates the gregorian date accordingly */
	public void setHebrewDate(final int newHMonth, final int newHDayOfMonth,final int newHYear)
			throws HebrewDateException {
		if (newHMonth < 1 || newHMonth > getLastMonthOfHebrewYear(newHYear))
			throw new HebrewDateException(
					"Illegal value for the hebrew month= " + newHMonth + " in year "+newHYear);
		if (newHDayOfMonth < 0)
			throw new HebrewDateException("Illegal value for hebrew date= "
					+ newHDayOfMonth);
		// make sure date is valid for this month, otherwise, set to last day of month //
		if (newHDayOfMonth > getLastDayOfHebrewMonth(newHMonth, newHYear))
			throw new IllegalArgumentException(newHDayOfMonth +" > last day of Hebrew month ["+getLastDayOfHebrewMonth(newHMonth,newHYear));
//			newHDayOfMonth = getLastDayOfHebrewMonth(newHMonth);

		if (newHYear < 0)
			throw new HebrewDateException("Illegal value for the hebrew year= "
					+ newHYear);

		if (newHMonth != CURRENT_MONTH)
			hebrewMonth = newHMonth;
		if (newHDayOfMonth != CURRENT_DATE)
			hebrewDate = newHDayOfMonth;
		if (newHYear != CURRENT_YEAR)
			hebrewYear = newHYear;

		// reset gregorian date //
		absDate = hebrewDateToAbsDate(hebrewMonth, hebrewDate, hebrewYear);
		absDateToDate();

		// reset day of week //
		dayOfWeek = Math.abs(absDate % 7) + 1;
	}

	/**
	 * Returns this object's date as a java.util.Date object. <B>Note</B>: This class does not
	 * have a concept of time.
	 * @return The date as a java.util.Date object.
	 */
	public Date getTime() {
		Calendar cal = Calendar.getInstance();
		cal.set(gYear, gMonth - 1, gDayOfMonth);
		return cal.getTime();
	}

	/** Resets this date to the current system date. */
	public void resetDate() {
		Calendar cal = Calendar.getInstance();
		setDate(cal);
	}

	/**
	 * accepts Numeric input and emits the hebrew-formatted date, i.e.
	 *     כ"ב באייר תשמ"א
	 * @param gMonth
	 * @param dayOfWeek
	 * @param gYear
	 * @param bundle
	 * @return
	 */
	public String formatHebrewDate_Hebrew() {
		String hMonth = getHebrewMonthAsString();
		String hYear = HebrewUtil.formatHebrewNumber(getHebrewYear() % 1000,
				true);
		String hDay = HebrewUtil.formatHebrewNumber(getHebrewDate(), true);
		return hDay + " ב" + hMonth + " " + hYear;
	}

	/** Returns a string containing the hebrew date in the form, "Month day, year" <BR>
	 * e.g. "Teves 23, 5760" */
	public String formatHebrewDate_English() { //FIX consider using an actual formatter for this...
		return getHebrewMonthAsString() + " " + hebrewDate + ", " + hebrewYear;
	}

	/** Returns a string containing the hebrew date in the form, "Month day, year" <BR>
	 * e.g. "Teves 23, 5760" */
	public String formatHebrewDate_Russian() { //FIX consider using an actual formatter for this...
		return getHebrewMonthAsString() + " " + hebrewDate + ", " + hebrewYear;
	}

	/** Returns a string containing the Gregorian date in the form, "Month day, year" <BR>
	 * e.g. "January 1, 2000".<P>
	 * For more "standard" date processing, use getTime
	 * and then the java.text.DateFormat class. */
	public String formatGregorianDate_English() {
		return getGregMonthAsString() + " " + gDayOfMonth + ", " + gYear;
	}

	/** Returns a string containing the hebrew date in the form, "Month day, year"
	 * e.g. "Teves 23, 5760" */
	public String toString() {
		return formatHebrewDate_English();
	}

	/** Rolls the date forward by 1. It modifies both the gregorian and hebrew dates
	 *  accordingly */
	public void forward() {
		// Change gregorian date //
		if (gDayOfMonth == getLastDayOfMonth()) {
			// if last day of year //
			if (gMonth == DECEMBER) {
				gYear++;
				gMonth = JANUARY;
				gDayOfMonth = 1;
			} else {
				gMonth++;
				gDayOfMonth = 1;
			}
		}
		// if not last day of month //
		else
			gDayOfMonth++;

		// Change Hebrew Date //
		if (hebrewDate == getLastDayOfHebrewMonth()) {
			// if it last day of elul (i.e. last day of hebrew year)//
			if (hebrewMonth == 6) {
				hebrewYear++;
				hebrewMonth++;
				hebrewDate = 1;
			}
			// if it is the last day of Adar, or Adar II as case may be //
			else if (hebrewMonth == getLastMonthOfThisHebrewYear()) {
				hebrewMonth = 1;
				hebrewDate = 1;
			} else {
				hebrewMonth++;
				hebrewDate = 1;
			}
		}
		// if not last date of month //
		else
			hebrewDate++;

		// if last day of week, loop back to sunday //
		if (dayOfWeek == 7)
			dayOfWeek = 1;
		else
			dayOfWeek++;

		// increment the absolute date //
		absDate++;
	}

	/** Rolls the date back by 1. It modifies both the gregorian and hebrew dates
	 *  accordingly */
	public void back() {
		// Change gregorian date //
		// if first day of month //
		if (gDayOfMonth == 1) {
			// if first day of year //
			if (gMonth == 1) {
				gMonth = 12;
				gYear--;
			} else
				gMonth--;

			// change to last day of previous month //
			gDayOfMonth = getLastDayOfMonth();
		} else
			gDayOfMonth--;

		// change hebrew date //
		// if first day of hebrew month //
		if (hebrewDate == 1) {
			// if nissan //
			if (hebrewMonth == 1)
				hebrewMonth = getLastMonthOfThisHebrewYear();
			// if rosh hashana //
			else if (hebrewMonth == 7) {
				hebrewYear--;
				hebrewMonth--;
			} else
				hebrewMonth--;

			hebrewDate = getLastDayOfHebrewMonth();
		} else
			hebrewDate--;

		// if first day of week, loop back to saturday //
		if (dayOfWeek == 1)
			dayOfWeek = 7;
		else
			dayOfWeek--;

		// change the absolute date //
		absDate--;
	}

	/** Compares two dates to see if they are equal */
	public boolean equals(Object object) {
		HebrewDate hebDate = (HebrewDate) object;
		if (absDate != hebDate.getAbsDate())
			return false;
		else
			return true;
	}

	/** Compares two dates as per the compareTo() method in the Comparable interface.
	 * Returns a value less than 0 if this date is "less than" (before) the date,
	 * greater than 0 if this date is "greater than" (after) the date, or 0 if they are
	 * equal. */
	public int compareTo(Object o) {
		HebrewDate hebDate = (HebrewDate) o;
		if (absDate < hebDate.getAbsDate())
			return -1;
		else if (absDate > hebDate.getAbsDate())
			return 1;
		else
			return 0;
	}

	/** returns a string  of the current hebrew month, localized with the calendar bundle*/
	public String getHebrewMonthAsString() {
		// if it is a leap year and 12th month //
		if (isThisHebrewLeapYear() && hebrewMonth == 12)
			return bundle.getString("hmonth.ADARI");
		else
			return bundle.getString(hebrewMonthProp[hebrewMonth - 1]);
	}

	/** returns a string  of the current month */
	public String getGregMonthAsString() {
		return bundle.getString(gregMonthProp[gMonth - 1]);
	}

	/**
	 * Returns the month (between 1-12).
	 */
	public int getGregorianMonth() {
		return gMonth;
	}

	/**
	 * Returns the date within the month.
	 */
	public int getGregorianDayOfMonth() {
		return gDayOfMonth;
	}

	/**
	 * Returns the year.
	 */
	public int getGregorianYear() {
		return gYear;
	}

	/**
	 * Returns the hebrew month (1-12 or 13).
	 */
	public int getHebrewMonth() {
		return hebrewMonth;
	}

	/**
	 * Returns the hebrew date of the month.
	 */
	public int getHebrewDate() {
		return hebrewDate;
	}

	/**
	 * Returns the hebrew year.
	 */
	public int getHebrewYear() {
		return hebrewYear;
	}

	/**
	 * Returns the day of the week as a number between 1-7.
	 */
	public int getDayOfWeek() {
		return dayOfWeek;
	}

	/** sets the month. */
	public void setMonth(int month) throws HebrewDateException {
		setDate(month, CURRENT_DATE, CURRENT_YEAR);
	}

	/** sets the year. */
	public void setYear(int year) throws HebrewDateException {
		setDate(CURRENT_MONTH, CURRENT_DATE, year);
	}

	/** sets the date of the month. */
	public void setDate(int date) throws HebrewDateException {
		setDate(CURRENT_MONTH, date, CURRENT_YEAR);
	}

	/** sets the hebrew month. */
	public void setHebrewMonth(int month) throws HebrewDateException {
		setHebrewDate(month, CURRENT_DATE, CURRENT_YEAR);
	}

	/** sets the hebrew year. */
	public void setHebrewYear(int year) throws HebrewDateException {
		setDate(CURRENT_MONTH, CURRENT_DATE, year);
	}

	/** sets the hebrew date of month. */
	public void setHebrewDate(int date) throws HebrewDateException {
		setHebrewDate(CURRENT_MONTH, date, CURRENT_YEAR);
	}

	/** Create a copy of this date. */
	public Object clone() {
		try {
			return new HebrewDate(gMonth, gDayOfMonth, gYear, locale);
		} catch (HebrewDateException e) {
			e.printStackTrace();
			return null;
		}
	}
}
