package avrom.util;

import java.util.*;
import java.text.*;

import net.sf.hebcal.HebrewDate;
import net.sf.hebcal.JewishHolidaysCalendar;

/** Calculates candlelighting times and havdala times for a given location.
* <P>
* Candlelighting and havdala times are calculated based on a given hebrew date and location.
* If the hebrew date is not a date on which candlelighting or havdala occurs, it will not
* return a time.
* <P>
* This class uses the HebrewDate class or its subclass RegularHebrewDate to determine the times.
* If a HebrewDate object is used, it assumes that you are using the non Israel holiday scheme.
* Otherwise, if you are using a RegularHebrewDate object, it will use whatever scheme it is
* set to.
* <P>
* Locations are determined using a Location. A Location is basically a city or town with it's
* name, longitude and latitude, and time zone. To facilitate multiple locations, this class can also use
* a properties file of which each property will be loaded into a Location object. The properties
* in the property file must have the form:
* <P><CODE>
* Key= Name: latitude: longitude: time zone id<BR>
* e.g. <BR>
* Toronto=Toronto, Ontario:43.65:-79.38:EST
* </CODE><P>
* @see avrom.util.Location
* @see net.sf.hebcal.HebrewDate
*/
public class CandleLighting
{
	private int CANDLE_OFFSET= 18;
	private int HAVDALA_OFFSET= 60;
	private final Location defaultLocale= new Location("Toronto", "Toronto, Ontario" , 43.65, -79.38, TimeZone.getTimeZone("EST"));
	private Location currentLocale= defaultLocale;
	private HebrewDate hebrewDate= new HebrewDate();
	private LocationList locales= new LocationList();

	private DateFormat df= new SimpleDateFormat("h:mm a");
	private SunRiseSet sunset= SunRiseSet.getInstance();
	/** Initialize candlelighting with a hebrew date (locale is defaulted to Toronto.) */
	public CandleLighting(HebrewDate date)
	{
		hebrewDate= date;
		setCurrentTimeZone();
	}
	/** Defaults to current system date and default locale (Toronto). */
	public CandleLighting()
	{
		setCurrentTimeZone();
	}
	/** Initialize with a hebrew date and a city locale. */
	public CandleLighting(HebrewDate date, Location locale)
	{
		this(date);
		setCurrentLocale(locale);
	}
	/** Initialize with the location of the properties file. */
	public CandleLighting(String localeFile) throws Exception
	{
		this();
		setProperties(localeFile);
	}
	/** Initialize with the location of the properties file and a hebrew date. */
	public CandleLighting(HebrewDate date, String localeFile) throws Exception
	{
		this(date);
		setProperties(localeFile);
	}
	/** Private method to make sure the date formatter is set to the current time zone */
	private void setCurrentTimeZone()
	{
		df.setTimeZone(currentLocale.getTimeZone());
	}

	/** Sets the offset (in minutes) before sunset for candlelighting times.<BR>
	* The default value is 18.
	*/
	public void setCandleOffset(int offset)
	{
		if (offset < 0)
			throw new IllegalArgumentException("Candlelighting offset cannot be less than zero. (Recommended value: 18)");

		CANDLE_OFFSET= offset;
	}
	/** Gets the offset (in minutes) before sunset for candlelighting times.<BR>
	* The default value is 18.
	*/
	public int getCandleOffset()
	{
		return CANDLE_OFFSET;
	}
	/** Sets the offset (in minutes) after sunset for havdala times.<BR>
	* The default value is 60.<BR>
	* "Recommended" values: 40- 72 minutes.
	*/
	public void setHavdalaOffset(int offset)
	{
		if (offset < 0)
			throw new IllegalArgumentException("Havdala time offset cannot be less than zero. (Recommended values: 40-72)");

		HAVDALA_OFFSET= offset;
	}
	/** Gets the offset (in minutes) after sunset for havdala times.<BR>
	* The default value is 60.
	*/
	public int getHavdalaOffset()
	{
		return HAVDALA_OFFSET;
	}
	/** Gets the Location that this object is currently set to. */
	public Location getCurrentLocale()
	{
		return currentLocale;
	}
	/** Sets the current City Locale. */
	public void setCurrentLocale(Location locale)
	{
		currentLocale= locale;
		setCurrentTimeZone();
	}
	/**
    * Sets the current city locale based on a key. This key must match
	* a key in the list of city locales. If this locale is not found in the
	* list, no change is made (i.e. stays at current settings)
	*/
	public void setCurrentLocaleKey(String localeKey)
	{
		Location temp= currentLocale;
		currentLocale= locales.getLocation(localeKey);
		if (currentLocale == null)
			currentLocale= temp;
		setCurrentTimeZone();
	}
    /**
     * Returns the key of the currently set locale.
     * @return The key of the currently set locale.
     */
	public String getCurrentLocaleKey()
	{
        return currentLocale.getKey();
    }
	/** Resets the current Location to the default locale (Toronto, Ontario, Canada) */
	public void resetLocale()
	{
		currentLocale= defaultLocale;
		setCurrentTimeZone();
	}
	/** Gets the hebrew date that this is set to.
	* Default is system date. */
	public HebrewDate getHebrewDate()
	{
		return hebrewDate;
	}
	/** Sets the hebrew date for which we calculate the candlelighting/havdala times.
	* Default is today's date. */
	public void setHebrewDate(HebrewDate date)
	{
		hebrewDate= date;
	}
	/** Returns the candlelighting time as a string in the format "hh:mm xm"
	* or an empty string if there is no candlelighting time for this date */
	public String getCandleLighting()
	{
		Date time= getCandleLightingAsDate();
		if (time == null)
			return "";
		else
			return df.format(time);

	}
	/** Returns the candlelighting time as a java.util.Date or null if there
	* is no candlelighting time for this date.
	*/
	public Date getCandleLightingAsDate()
	{
		if (!isCandleLightingDay())
			return null;

		Calendar time= getSunset();
		// if Yom Tov falls on Motzai Shabbos, you cannot light candles
		// until after Shabbos is over. Also, the same applies for second day Yom Tov,
		// except if it falls out on Friday night.
		if (hebrewDate.getDayOfWeek() == 7 || (isSecondDay() && hebrewDate.getDayOfWeek() != 6))
			time.add(Calendar.MINUTE, HAVDALA_OFFSET);
		else
			time.add(Calendar.MINUTE, - CANDLE_OFFSET);

		//time.set(time.SECOND, 0);

		return time.getTime();
	}
	/** lets you know if it is second day Yom Tov or not. */
	private boolean isSecondDay()
	{
		boolean israeli= false;
		if (hebrewDate instanceof JewishHolidaysCalendar)
		{
			JewishHolidaysCalendar d= (JewishHolidaysCalendar) hebrewDate;
			israeli= d.isIsraeliCalendar();
		}
		int hebrewMonth= hebrewDate.getHebrewMonth();
		int date= hebrewDate.getHebrewDate();

		// only second day Yom Tov in Israel is Rosh Hashana
		if (israeli && hebrewMonth== 7 && date== 1)
			return true;
		else if (!israeli)
		{
			switch (hebrewMonth)
			{
				case 7:
					// Rosh Hashana, Sukos, and Shimi Atezres
					if (date== 1 || date== 15 || date== 22)
						return true;
				break;
				case 1:
					// 2nd and last days pesach
					if (date== 15 || date== 21)
						return true;
				break;
				case 3:
					// shavuos
					if (date== 6)
						return true;
				break;
				default:
					return false;
			}
		}
		return false;


	}
	/** Private method to get the sunset time as a Calendar object. It is
	* set to the GMT time zone */
	private Calendar getSunset()
	{
		sunset.setDateAndCoordinates(hebrewDate.getTime(), currentLocale.getLongitude(), currentLocale.getLatitude());
		Calendar time= sunset.getSunsetTime();
		return time;
	}
	/** Returns the havdala time as a string or an empty string if there
	* is no havdala time for this date */
	public String getHavdala()
	{
		Date time= getHavdalaAsDate();
		if (time == null)
			return "";
		else
			return df.format(time);
	}
	/** Returns the havdala time as a java.util.Date
	* or null if there is no havdala time for this date.
	*/
	public Date getHavdalaAsDate()
	{
		if (!isHavdalaDay())
			return null;

		Calendar time= getSunset();
		/*int offset= HAVDALA_OFFSET;
		// if there are some seconds, round up to next minute
		if (time.get(time.SECOND) > 0)
			offset++;
		time.set(time.SECOND, 0);	*/
		time.add(Calendar.MINUTE, HAVDALA_OFFSET);
		return time.getTime();
	}
	/** Checks to see if the day is a candlelighting day.<P>
	* This would be either Friday or erev Yom Tov (1st or 2nd day). */
	public boolean isCandleLightingDay()
	{
		// if its a Friday, then it always is a candlelighting day
		if (hebrewDate.getDayOfWeek() == 6)
			return true;

		boolean israeli= false;

		if (hebrewDate instanceof JewishHolidaysCalendar)
		{
			JewishHolidaysCalendar date= (JewishHolidaysCalendar) hebrewDate;
			israeli= date.isIsraeliCalendar();
		}
		int hebrewMonth= hebrewDate.getHebrewMonth();
		int date= hebrewDate.getHebrewDate();

		switch (hebrewMonth)
		{
			case 6:
				// if erev Rosh Hashana
				if (date == 29)
					return true;
			break;
			case 7:
				// if second day Rosh Hashana, erev Yom Kippur, erev Sukos,
				// or erev Shmini Atzeres
				if (date == 1 || date == 9 || date == 14 || date == 21)
					return true;
				// if outside Israel, if second day sukos and Shimini Atezres
				if (!israeli)
				{
					if (date == 15 || date == 22)
						return true;
				}
			break;
			case 1:
				// if erev Pesach or erev 7th day Pesach
				if (date == 14 || date == 20)
					return true;
				// if outside Israel, if second day Pesach or erev 8th day pesach
				if (!israeli)
				{
					if (date == 15 || date == 21)
						return true;
				}
			break;
			case 3:
				// if erev Shavuos (and for 2nd day outside israel)
				if (date == 5 || (!israeli && date == 6))
					return true;
			break;
			default:
				return false;
		}
		return false;
	}
	/** Checks to see if the day is a havdala day.<P>
	* This would be either Sat. night (except for a Yom Tov night)
	* or a Motzai Yom Tov (except for Fri. night). */
	public boolean isHavdalaDay()
	{
		// if today is friday, return false, even if end of Yom Tov
		if (hebrewDate.getDayOfWeek() == 6)
			return false;
		// if today is a candlelighting day, then there is no havdala
		if (isCandleLightingDay())
			return false;
		boolean israeli= false;

		if (hebrewDate instanceof JewishHolidaysCalendar)
		{
			JewishHolidaysCalendar date= (JewishHolidaysCalendar) hebrewDate;
			israeli= date.isIsraeliCalendar();
		}
		int hebrewMonth= hebrewDate.getHebrewMonth();
		int date= hebrewDate.getHebrewDate();

		// if today is sat. night, return true unless it is yom tov
		if (hebrewDate.getDayOfWeek() == 7)
		{
			switch(hebrewMonth)
			{
				case 7:
					// if Rosh Hashana 1, Sukos 1, or Shimini Atzres
					if (date == 1 || (!israeli && date == 15) || (!israeli && date== 21))
						return false;
					else
						return true;
				case 1:
					// if Pesach 1 or 7
					if ((!israeli && date == 15) || (!israeli && date == 21))
						return false;
					else
						return true;
				case 3:
					// if Shavuos 1
					if ((!israeli && date == 6))
						return false;
					else
						return true;
				default:
					return true;
			}

		}
		// detect whether Motzai Yom Tov on non-Sat. night or otherwise return false
		else
		{
			switch(hebrewMonth)
			{
				case 7:
					// if Rosh Hashana 2, Yom Kippur, Sukos, or Shimini Atzres
					if (date == 2 || date == 10  || (israeli && (date == 15 || date== 22)) || (!israeli && (date == 16 || date== 23)))
						return true;
					else
						return false;
				case 1:
					// if Pesach 1 or 7 (2 or 8 outside israel)
					if ((israeli && (date == 15 || date == 21)) || (!israeli && (date == 16 || date == 22)))
						return true;
					else
						return false;
				case 3:
					// if Shavuos 1
					if ((israeli && date == 6) || (!israeli && date == 7))
						return true;
					else
						return false;
				default:
					return false;
			}
		}
	}
	/** Load properties from a property file that contains city locale info.
	* The form of the property should be:<BR>
	* Key= Name: latitude: longitude: time zone id<BR>
	* e.g. <BR>
	* Toronto=Toronto, Ontario:43.65:-79.38:EST */
	public void setProperties(String filename)
	{
		locales.setProperties(filename);
	}
	/** Sets the list of city locales. Overrides the current list. */
	public void setLocationList(LocationList list)
	{
		locales= list;
	}
	/** Returns the city locale list. */
	public LocationList getLocationList()
	{
		return locales;
	}
	/** Adds a city locale to the list of city locales. If this locale's key
	* matches a key already in the list, it overrides it */
	public void addLocation(Location locale)
	{
		locales.addLocation(locale);
	}
	/*public static void main (String[] args)
	{
		try
		{
			HebrewDate hebrewDate= new HebrewDate();
			hebrewDate.setDate(1);

			CandleLighting lighting= new CandleLighting(hebrewDate);
			lighting.setProperties("cities.properties");
            String[] keys= lighting.getLocationList().getLocationKeys();
            for (int i=0; i < keys.length; i++)
                System.out.println(keys[i]);

			lighting.setCurrentLocaleKey("Denver");

			System.out.println("Candlelighting/Havadala Times for: "+lighting.getCurrentLocale().getName());


			for (int i=0; i < hebrewDate.getLastDayOfMonth(); i++)
			{
				String candle= lighting.getCandleLighting();
				String havdala= lighting.getHavdala();

				if (candle.length() != 0)
					System.out.println("Candlelighting time for " + hebrewDate + ' ' + candle);
				if (havdala.length() != 0)
					System.out.println("Havdala time for " + hebrewDate + ' ' + havdala);

				hebrewDate.forward();
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
		}
	}*/
}