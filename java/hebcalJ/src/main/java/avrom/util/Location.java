package avrom.util;

import java.util.*;

/**
* This class defines a city, i.e. a named Location, and it's coordinates on the planet.<BR>
* It's essentially a lattitude, a longitude and a timezone.
* @see java.util.TimeZone
* @see LocationList
*/
public class Location
{
	private String name;
	private String key= "default";
	private double longitude;
	private double latitude;
	private TimeZone timeZone;

	public Location (String key, String name, double latitude, double longitude, TimeZone timeZone)
	{
		setKey(key);
		setName(name);
		setLatitude(latitude);
		setLongitude(longitude);
		setTimeZone(timeZone);
	}

	public Location()
	{
	}
	/** Gets the full name for this city.
	* @return The full name or the key if no name is defined.
	*/
	public String getName()
	{
		if (name == null)
			return key;
		else
			return name;
	}
	/** Gets the key for this city. */
	public String getKey()
	{
		return key;
	}
	/** Gets the longitude of this city (East is +/West is -). */
	public double getLongitude()
	{
		return longitude;
	}
	/** Gets the latitude of this city (North is +/South is -). */
	public double getLatitude()
	{
		return latitude;
	}
	/** Gets the TimeZone for this city.
     * @return The TimeZone that this city is set to, otherwise,
     * it returns the default TimeZone.
     */
	public TimeZone getTimeZone()
	{
		if (timeZone != null)
			return timeZone;
		else
			return TimeZone.getDefault();
	}
	/** Sets the full name for this city. */
	public void setName(String name)
	{
		this.name= name;
	}
	/** Sets the unique key for this city.<P>
	* Usually, this is a short form version of its name.
	*/
	public void setKey(String key)
	{
		this.key= key;
	}
	/** Sets the longitude of this city (East is +/West is -). */
	public void setLongitude(double longitude)
	{
		this.longitude= longitude;
	}
	/** Sets the latitude of this city (North is +/South is -). */
	public void setLatitude(double latitude)
	{
		this.latitude= latitude;
	}
	/** Sets the TimeZone for this city. */
	public void setTimeZone(TimeZone timeZone)
	{
		this.timeZone= timeZone;
	}
	/**
     * Sets the TimeZone using a String that is this time zone's id.
     * @param zoneID The TimeZone ID string.
     * @see java.util.TimeZone
     */
	public void setTimeZoneID(String zoneID)
	{
		this.timeZone= TimeZone.getTimeZone(zoneID);
	}
    /**
     * Gets the TimeZone's ID.
     * @return the Time Zone ID String.
     */
    public String getTimeZoneID()
    {
        return timeZone.getID();
    }

}