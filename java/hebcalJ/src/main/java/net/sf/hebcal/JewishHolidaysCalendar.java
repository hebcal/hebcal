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

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Collection;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.Locale;
import java.util.Map;
import java.util.MissingResourceException;
import java.util.ResourceBundle;

/**
 * The RegularHebrewDate class is a subclass of the HebrewDate class that adds
 * the following functionality to it.
 * <P>
 * <UL>
 * <LI>Getting the corresponding jewish holiday or fast day. (no "modern"
 * holidays)
 * <LI>Getting the regular parsha (Torah or sedra) reading for Shabbos.
 * <LI>Getting the current Omer count.
 * </UL>
 * In addition, This class has settings for Ashkenazi or Sephardi pronunciations
 * and whether the parsha and holiday scheme follows the Israel scheme or
 * outside Israel scheme.
 * <P>
 * The methods used to obtain the parsha were derived from the source code of
 * hebcal by Danny Sadinoff and JCal for the Mac by Frank Yellin. Both based
 * their code on routines by Nachum Dershowitz and Edward M. Reingold.
 * 
 * hebcal web page is at <A
 * HREF="http://www.sadinoff.com/hebcal/">www.sadinoff.com/hebcal</A>
 *  @author Nachum Dershowitz
 *  @author Edward M. Reingold
 *  @author Avrom Finkelstein
 *  @author Danny Sadinoff
 */
public class JewishHolidaysCalendar extends HebrewDate {
	private Locale locale;

	private ResourceBundle holidayBundle = null;

	private ResourceBundle parshiotBundle = null;

	private boolean israeliCalendar = false;

	// XXX: this wants to be a formatter.
	private static Map OMER_MAP = new HashMap();
	static {
		try {
                    //			System.out.println(Arrays.asList(JewishHolidaysCalendar.class
                    //					.getDeclaredMethods()));
			OMER_MAP.put("en", JewishHolidaysCalendar.class.getDeclaredMethod(
					"formatOmer_short_EN", new Class[] { int.class }));
			OMER_MAP.put("iw", JewishHolidaysCalendar.class.getDeclaredMethod(
					"formatOmer_short_HE", new Class[] { int.class }));
			OMER_MAP.put("ru", JewishHolidaysCalendar.class.getDeclaredMethod(
					"formatOmer_short_EN", new Class[] { int.class }));
		} catch (NoSuchMethodException e) {
			e.printStackTrace();
		}
	}

	private static final String[] parshiotKeys = { "Bereshit", "Noach",
			"LechLecha", "Vayera", "ChayeiSara", "Toldot", "Vayetzei",
			"Vayishlach", "Vayeshev", "Miketz", "Vayigash", "Vayechi",
			"Shemot", "Vaera", "Bo", "Beshalach", "Yitro", "Mishpatim",
			"Terumah", "Tetzaveh", "KiTisa", "Vayakhel", "Pekudei", "Vayikra",
			"Tzav", "Shmini", "Tazria", "Metzora", "AchreiMot", "Kedoshim",
			"Emor", "Behar", "Bechukotai", "Bamidbar", "Nasso", "Behaalotcha",
			"Shlach", "Korach", "Chukat", "Balak", "Pinchas", "Matot", "Masei",
			"Devarim", "Vaetchanan", "Eikev", "Reeh", "Shoftim", "KiTeitzei",
			"KiTavo", "Nitzavim", "Vayeilech", "Haazinu",

			"VayakhelPekudei", "TazriaMetzora", "AchreiMotKedoshim",
			"BeharBechukotai", "ChukatBalak", "MatotMasei",
			"NitzavimVayeilech", };

	// These indices were originaly included in the emacs 19 distribution.
	// These arrays determine the correct indices into the parsha names
	// -1 means no parsha that week, values > 52 means it is a double parsha

	private static final int[] Sat_short = { -1, 52, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 53, 23, 24,
			-1, 25, 54, 55, 30, 56, 33, 34, 35, 36, 37, 38, 39, 40, 58, 43, 44,
			45, 46, 47, 48, 49, 50 };

	private static final int[] Sat_long = { -1, 52, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 53, 23, 24,
			-1, 25, 54, 55, 30, 56, 33, 34, 35, 36, 37, 38, 39, 40, 58, 43, 44,
			45, 46, 47, 48, 49, 59 };

	private static final int[] Mon_short = { 51, 52, -1, 0, 1, 2, 3, 4, 5, 6,
			7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 53, 23, 24,
			-1, 25, 54, 55, 30, 56, 33, 34, 35, 36, 37, 38, 39, 40, 58, 43, 44,
			45, 46, 47, 48, 49, 59 };

	private static final int[] Mon_long = // split //
	{ 51, 52, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17,
			18, 19, 20, 53, 23, 24, -1, 25, 54, 55, 30, 56, 33, -1, 34, 35, 36,
			37, 57, 40, 58, 43, 44, 45, 46, 47, 48, 49, 59 };

	private static final int[] Thu_normal = { 52, -1, -1, 0, 1, 2, 3, 4, 5, 6,
			7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 53, 23, 24,
			-1, -1, 25, 54, 55, 30, 56, 33, 34, 35, 36, 37, 38, 39, 40, 58, 43,
			44, 45, 46, 47, 48, 49, 50 };

	private static final int[] Thu_normal_Israel = { 52, -1, -1, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 53, 23,
			24, -1, 25, 54, 55, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 58,
			43, 44, 45, 46, 47, 48, 49, 50 };

	private static final int[] Thu_long = { 52, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7,
			8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
			-1, 25, 54, 55, 30, 56, 33, 34, 35, 36, 37, 38, 39, 40, 58, 43, 44,
			45, 46, 47, 48, 49, 50 };

	private static final int[] Sat_short_leap = { -1, 52, -1, -1, 0, 1, 2, 3,
			4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
			22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
			38, 39, 40, 58, 43, 44, 45, 46, 47, 48, 49, 59 };

	private static final int[] Sat_long_leap = { -1, 52, -1, -1, 0, 1, 2, 3, 4,
			5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
			23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37,
			57, 40, 58, 43, 44, 45, 46, 47, 48, 49, 59 };

	private static final int[] Mon_short_leap = { 51, 52, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, -1, 34, 35, 36, 37, 57,
			40, 58, 43, 44, 45, 46, 47, 48, 49, 59 };

	private static final int[] Mon_short_leap_Israel = { 51, 52, -1, 0, 1, 2,
			3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
			21, 22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36,
			37, 38, 39, 40, 58, 43, 44, 45, 46, 47, 48, 49, 59 };

	private static final int[] Mon_long_leap = { 51, 52, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, -1, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
			39, 40, 58, 43, 44, 45, 46, 47, 48, 49, 50 };

	private static final int[] Mon_long_leap_Israel = { 51, 52, -1, 0, 1, 2, 3,
			4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
			22, 23, 24, 25, 26, 27, -1, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37,
			38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

	private static final int[] Thu_short_leap = { 52, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50 };

	private static final int[] Thu_long_leap = { 52, -1, -1, 0, 1, 2, 3, 4, 5,
			6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23,
			24, 25, 26, 27, 28, -1, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
			40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 59 };

	/**
	 * Set the default Locale for the holidays.
	 * 
	 * @param defaultLocale
	 *            the default locale for the holidays to be output
	 * @throws MissingResourceException
	 *             if the locale is unknown
	 */
	public void setHolidayLocale(Locale defaultLocale)
			throws MissingResourceException {
		locale = defaultLocale;
		final String className = JewishHolidaysCalendar.class.getName();
		// ASSUME: this class is not in the default package.
		final String packageName = className.substring(0, className
				.lastIndexOf('.') + 1);
		holidayBundle = ResourceBundle.getBundle(className, locale);
		parshiotBundle = ResourceBundle.getBundle(packageName + "Parshiot",
				locale);
	}

	/** Initializes based on gregorian date */
	public JewishHolidaysCalendar(int gMonth, int gDayOfMonth, int gYear,
			boolean israeli, Locale defaultLocale) throws HebrewDateException {
		super(gMonth, gDayOfMonth, gYear, defaultLocale);
		this.israeliCalendar = israeli;
		setHolidayLocale(defaultLocale);
	}

	/**
	 * Initializes based on a month, date, year. Assume non-israeli calendar.
	 */
	public JewishHolidaysCalendar(int gMonth, int gDayOfMonth, int gYear,
			Locale loc) throws HebrewDateException {
		super(gMonth, gDayOfMonth, gYear, loc);
		setHolidayLocale(loc);
	}

	/**
	 * Initializes a date based on the current system date.
	 */
	public JewishHolidaysCalendar() {
		super();
		setHolidayLocale(getLocale());
	}

	/**
	 * Initializes a date based on java.util.Date object.
	 */

	public JewishHolidaysCalendar(Date date, Locale loc) {
		super(date, loc);
		setHolidayLocale(loc);
	}

	/**
	 * Initializes a date based on java.util.Calendar object.
	 */

	public JewishHolidaysCalendar(Calendar cal, Locale loc) {
		super(cal, loc);
		setHolidayLocale(loc);
	}

	/**
	 * Sets whether to use Israel parsha and holiday scheme or not. Default is
	 * "false".
	 */
	public void setIsraeliCalendar(boolean israeli) {
		this.israeliCalendar = israeli;
	}

	/**
	 * Gets whether Israel parsha and holiday scheme is used or not. Default is
	 * "false".
	 */
	public boolean isIsraeliCalendar() {
		return israeliCalendar;
	}

	private static String formatOmer_short_HE(int oDay) {
		return "יוס " + HebrewUtil.formatHebrewNumber(oDay, true) + " לעומר";
	}

	private static String formatOmer_short_EN(int oDay) {
		return "Omer: " + oDay;
	}

	/**
	 * Returns the holiday the current day, or an empty array if none.
	 * 
	 * @return An array of Holiday Objects
	 */
	public JewishCalendarEvent[] getEvents() {
/*		BitSet bsLightCandles = new BitSet();
		bsLightCandles.set(JewishCalendarEvent.CANDLELIGHTING_TIME);

		BitSet bsYomTov = new BitSet();
		bsYomTov.set(JewishCalendarEvent.YOM_TOV);
*/
		List retList = new ArrayList();
		JewishCalendarEvent parsha = getShabbatParsha();
		if (null != parsha)
			retList.add(parsha);
		// check by month (starts from Nissan)
		switch (hebrewMonth) {
		case MONTH_NISAN:
			if (hebrewDate == 14)
				retList.add(new JewishCalendarEvent("ErevPesach",
						holidayBundle,
						new int[] { JewishCalendarEvent.LIGHT_CANDLES }));

			if (hebrewDate == 15
					|| hebrewDate == 21
					|| (!israeliCalendar && (hebrewDate == 16 || hebrewDate == 22)))
				retList.add(new JewishCalendarEvent("Pesach", holidayBundle,
						new int[] { JewishCalendarEvent.YOM_TOV }));

			if (hebrewDate >= 17 && hebrewDate <= 20
					|| (hebrewDate == 16 && israeliCalendar))
				retList.add(new JewishCalendarEvent("CholHamoedPesach",
						holidayBundle,
						new int[] { JewishCalendarEvent.CHOL_HAMOED }));
			break;

		case MONTH_IYAR:
			if (hebrewDate == 14)
				retList.add(new JewishCalendarEvent("PesachSheni",
						holidayBundle,
						new int[] { JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));
			break;
		case MONTH_SIVAN:
			if (hebrewDate == 5) {
				retList.add(new JewishCalendarEvent("ErevShavuot",
						holidayBundle,
						new int[] { JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));

			}
			if (hebrewDate == 6 || (hebrewDate == 7 && !israeliCalendar)) {
				retList.add(new JewishCalendarEvent("Shavuot", holidayBundle,
						new int[] { JewishCalendarEvent.YOM_TOV }));
			}
			break;
		case MONTH_TAMMUZ:
			// push off the fast day if it falls on shabbos//
			if ((hebrewDate == 17 && dayOfWeek != 7)
					|| (hebrewDate == 18 && dayOfWeek == 1))
				retList.add(new JewishCalendarEvent("TzomTammuz",
						holidayBundle, new int[] {JewishCalendarEvent.FAST_DAY}));
			break;
		case MONTH_AV:
			// if tisha b'av falls on shabbos, push off until sunday//
			if ((dayOfWeek == 1 && hebrewDate == 10)
					|| (dayOfWeek != 7 && hebrewDate == 9))
				retList.add(new JewishCalendarEvent("TishaBAv", holidayBundle,
						new int[] {JewishCalendarEvent.FAST_DAY}));
			if (hebrewDate == 15)
				retList.add(new JewishCalendarEvent("TuBAv", holidayBundle,
						new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));
			break;
		case MONTH_ELUL:
			if (hebrewDate == 29)
				retList.add(new JewishCalendarEvent("ErevRoshHashanah",
						holidayBundle, new int[] {}));
			break;
		case MONTH_TISHREI:
			if (hebrewDate == 1 || hebrewDate == 2)
				retList.add(new JewishCalendarEvent("RoshHashanah",
						holidayBundle, new int[] {JewishCalendarEvent.YOM_TOV}));
			// push off tzom gedalia if it falls on Shabbos //
			if ((hebrewDate == 3 && dayOfWeek != 7)
					|| (hebrewDate == 4 && dayOfWeek == 1))
				retList.add(new JewishCalendarEvent("TzomGedalia",
						holidayBundle, new int[] {JewishCalendarEvent.FAST_DAY}));
			if (hebrewDate == 9)
				retList.add(new JewishCalendarEvent("ErevYomKippur",
						holidayBundle, new int[] {}));
			if (hebrewDate == 10)
				retList.add(new JewishCalendarEvent("YomKippur", holidayBundle,
						new int[] {JewishCalendarEvent.YOM_TOV}));
			if (hebrewDate == 14) {
				/*
				 * if (ashkenaz) retList.add( new Holiday( "Erev Sukkos",
				 * holidayBundle, new int[] {})); else
				 */
				retList.add(new JewishCalendarEvent("ErevSukkot",
						holidayBundle, new int[] {}));
			}
			if (hebrewDate == 15 || (hebrewDate == 16 && !israeliCalendar)) {
				retList.add(new JewishCalendarEvent("Sukkot", holidayBundle,
						new int[] {JewishCalendarEvent.YOM_TOV}));
			}
			if (hebrewDate >= 17 && hebrewDate <= 20
					|| (hebrewDate == 16 && israeliCalendar)) {
				retList.add(new JewishCalendarEvent("CholHamoedSukkot",
						holidayBundle, new int[] {JewishCalendarEvent.CHOL_HAMOED}));
			}
			if (hebrewDate == 21)
				retList.add(new JewishCalendarEvent("HoshanaRabah",
						holidayBundle, new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));
			if (hebrewDate == 22) {
				retList.add(new JewishCalendarEvent("ShminiAtzeret",
						holidayBundle, new int[] {JewishCalendarEvent.YOM_TOV}));
			}
			if (hebrewDate == 23 && !israeliCalendar) {
				retList.add(new JewishCalendarEvent("SimchatTorah",
						holidayBundle, new int[] {JewishCalendarEvent.YOM_TOV}));
			}
			break;
		case MONTH_TEVET:
			if (hebrewDate == 24)
				retList.add(new JewishCalendarEvent("ErevChanukah",
						holidayBundle, new int[] {}));
			if (hebrewDate >= 25)
				retList.add(new JewishCalendarEvent("Chanukah", holidayBundle,
						new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));

			break;
		case MONTH_KISLEV:
			if (hebrewDate == 1 || hebrewDate == 2
					|| (hebrewDate == 3 && isKislevShort(hebrewYear)))
				retList.add(new JewishCalendarEvent("Chanukah", holidayBundle,
						new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));
			if (hebrewDate == 10) {
				retList.add(new JewishCalendarEvent("TzomTevet", holidayBundle,
						new int[] {JewishCalendarEvent.FAST_DAY}));
			}
			break;
		case MONTH_SHVAT:
			if (hebrewDate == 15)
				retList.add(new JewishCalendarEvent("TuBShvat", holidayBundle,
						new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));
			break;
		case MONTH_ADAR:
			if (!isThisHebrewLeapYear()) {
				// if 13th adar falls on fri or shabbos, push back to thursday
				if (((hebrewDate == 11 || hebrewDate == 12) && dayOfWeek == 5)
						|| (hebrewDate == 13 && !(dayOfWeek == 6 || dayOfWeek == 7))) {
					retList.add(new JewishCalendarEvent("TaanitEsther",
							holidayBundle, new int[]{JewishCalendarEvent.TAANIT_BECHOROT,JewishCalendarEvent.FAST_DAY}));
				}
				if (hebrewDate == 14)
					retList.add(new JewishCalendarEvent("Purim", holidayBundle,
							new int[] { JewishCalendarEvent.PURIM,
									JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));
				if (hebrewDate == 15)
					retList.add(new JewishCalendarEvent("ShushanPurim",
							holidayBundle, new int[] {
									JewishCalendarEvent.SHUSHAN_PURIM,
									JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));
			}
			// else if a leap year //
			else {
				if (hebrewDate == 14)
					retList.add(new JewishCalendarEvent("PurimKatan",
							holidayBundle, new int[] {JewishCalendarEvent.HOLIDAY_NON_YOM_TOV}));
			}
			break;
		case MONTH_ADARII:
			// if 13th adar falls on fri or shabbos, push back to thursday
			if (((hebrewDate == 11 || hebrewDate == 12) && dayOfWeek == 5)
					|| (hebrewDate == 13 && !(dayOfWeek == 6 || dayOfWeek == 7))) {
				/*
				 * if (ashkenaz) retList.add( new Holiday( "Ta'anis Esther",
				 * holidayBundle, new int[] {})); else
				 */
				retList.add(new JewishCalendarEvent("TaanitEsther",
						holidayBundle, new int[]{JewishCalendarEvent.TAANIT_BECHOROT,JewishCalendarEvent.FAST_DAY}));
			}
			if (hebrewDate == 14)
				retList.add(new JewishCalendarEvent("Purim", holidayBundle,
						new int[] { JewishCalendarEvent.PURIM,
								JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));
			if (hebrewDate == 15)
				retList.add(new JewishCalendarEvent("ShushanPurim",
						holidayBundle, new int[] {
								JewishCalendarEvent.SHUSHAN_PURIM,
								JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));
			break;
		}
		retList.addAll(getOmerHoliday());
		return (JewishCalendarEvent[]) retList
				.toArray(new JewishCalendarEvent[] {});
	}

	/**
	 * returns the int value of the omer day or zero if the day is not in the
	 * omer
	 * 
	 * @return The omer count as an int or zero.
	 */
	public int getOmer() {
		int omer = 0;

		// if nissan and second day of pesach and on //
		if (hebrewMonth == MONTH_NISAN && hebrewDate >= 16)
			omer = hebrewDate - 15;
		// if iyar //
		if (hebrewMonth == MONTH_IYAR)
			omer = hebrewDate + 15;
		// if sivan and before shavuos //
		if (hebrewMonth == MONTH_SIVAN && hebrewDate < 6)
			omer = hebrewDate + 44;

		return omer;
	}

	/**
	 * returns a collection of Holiday Objects corresponding to the omer count.
	 * 
	 * @return
	 */
	public Collection getOmerHoliday() {
		int omer = getOmer();
		List retList = new LinkedList();

		// if not omer day //
		if (omer == 0)
			return Collections.EMPTY_LIST;

		String lang = holidayBundle.getLocale().getLanguage();
		try {
			String omerString = (String) ((Method) OMER_MAP.get(lang)).invoke(
					null, new Object[] { new Integer(omer) });
			retList.add(new JewishCalendarEvent(omerString,
					new int[] { JewishCalendarEvent.OMER }));
		} catch (Exception e) {
			e.printStackTrace();
		}
		if (omer == 33)
			retList.add(new JewishCalendarEvent("LagBOmer", holidayBundle,
					new int[] { JewishCalendarEvent.HOLIDAY_NON_YOM_TOV }));

		return retList;
	}

	/** returns the shabbat parsha */
	public JewishCalendarEvent getShabbatParsha() {

		// if today is not shabbos, then there is no normal parsha reading
		if (getDayOfWeek() != DOW_SATURDAY)
			return null;

		// kvia= whether a hebrew year is short/regular/long (0/1/2)
		// roshHashana= Rosh Hashana of this hebrew year
		// roshHashanaDay= day of week Rosh Hashana was on this year
		// week= current week in hebrew calendar from Rosh Hashana
		// array= the correct index array for this hebrew year
		// index= the index number of the parsha name
		int kvia;
		int roshHashanaDay;
		int week;
		int[] array = null;
		int index;
		// create a clone of this date
		HebrewDate roshHashana = (HebrewDate) this.clone();
		try {
			// set it to Rosh Hashana of this year
			roshHashana.setHebrewDate(7, 1, CURRENT_YEAR);
		} catch (HebrewDateException e) {
			e.printStackTrace();
		}

		// get day Rosh Hashana was on
		roshHashanaDay = roshHashana.getDayOfWeek();

		// week is the week since the first Shabbos on or after Rosh Hashana
		week = (((absDate - roshHashana.getAbsDate()) - (7 - roshHashanaDay)) / 7);

		// get kvia
		if (isCheshvanLong(hebrewYear) && !isKislevShort(hebrewYear))
			kvia = 2;
		else if (!isCheshvanLong(hebrewYear) && isKislevShort(hebrewYear))
			kvia = 0;
		else
			kvia = 1;

		// determine appropriate array
		if (!isThisHebrewLeapYear()) {
			switch (roshHashanaDay) {
			case 7: // RH was on a Saturday
				if (kvia == 0)
					array = Sat_short;
				else if (kvia == 2)
					array = Sat_long;
				break;
			case 2: // RH was on a Monday
				if (kvia == 0)
					array = Mon_short;
				else if (kvia == 2)
					array = israeliCalendar ? Mon_short : Mon_long;
				break;
			case 3: // RH was on a Tuesday
				if (kvia == 1)
					array = israeliCalendar ? Mon_short : Mon_long;
				break;
			case 5: // RH was on a Thursday
				if (kvia == 1)
					array = israeliCalendar ? Thu_normal_Israel : Thu_normal;
				else if (kvia == 2)
					array = Thu_long;
				break;
			}
		}

		// if leap year //
		else {
			switch (roshHashanaDay) {
			case 7: // RH was on a Sat
				if (kvia == 0)
					array = Sat_short_leap;
				else if (kvia == 2)
					array = israeliCalendar ? Sat_short_leap : Sat_long_leap;
				break;
			case 2: // RH was on a Mon
				if (kvia == 0)
					array = israeliCalendar ? Mon_short_leap_Israel
							: Mon_short_leap;
				else if (kvia == 2)
					array = israeliCalendar ? Mon_long_leap_Israel
							: Mon_long_leap;
				break;
			case 3: // RH was on a Tue
				if (kvia == 1)
					array = israeliCalendar ? Mon_long_leap_Israel
							: Mon_long_leap;
				break;
			case 5: // RH was on a Thu
				if (kvia == 0)
					array = Thu_short_leap;
				else if (kvia == 2)
					array = Thu_long_leap;
				break;
			}
		}
		// if something goes wrong
		if (array == null)
			throw new NullPointerException(
					"Was not able to set the index array to any of the known types.");

		// get index from array
		index = array[week];

		// If no Parsha this week
		if (index == -1)
			return null;

		// parsha this week
		return new JewishCalendarEvent(parshiotKeys[index], parshiotBundle,
				new int[] { JewishCalendarEvent.SATURDAY_PARSHA });
	}

	/** Create a copy of this date. */
	public Object clone() {
		try {
			return new JewishHolidaysCalendar(gMonth, gDayOfMonth, gYear,
					israeliCalendar, locale);
		} catch (HebrewDateException e) {
			e.printStackTrace();
			return null;
		}

	}
	/*
	 * public static void main(String[] args) { try { HebrewDate date= new
	 * HebrewDate(); date.setHebrewDate(7, 10, 5660);
	 * 
	 * for (int i=1900; i < 2000; i++) {
	 * 
	 * if (date.getDayOfWeek() == 7) {
	 * System.out.println(date.getDateAsString()); }
	 * date.setHebrewDate(HebrewDate.CURRENT_MONTH, HebrewDate.CURRENT_DATE,
	 * date.getHebrewYear() + 1); } } catch (Exception e) { e.printStackTrace(); } }
	 */
}
