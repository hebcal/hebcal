package avrom.util;

import java.util.Calendar;
import java.util.Date;
import java.util.TimeZone;

/**
* Allows one to compute the sunrise and sunset times for a given location.<P>
* This abstract class has a date, latitude, and longitude as its main
* properties. Subclasses of this class are expected to implement the sunrise
* and sunset computations based on these values. <P>
* The intention for this structure is that the subclass will esentially wrap around another
* third party class that will do the actual work. 
* <P>
* Sunrise and sunset times must be computed to the GMT (UTC) time.
*/
public abstract class SunRiseSet
{
	protected Calendar date;
	
	protected double longitude;
	protected double latitude;
	
	private TimeZone gmt= TimeZone.getTimeZone("GMT");
	
	/** Gets a concrete subclass of this class. */
	public static SunRiseSet getInstance()
	{
		return new DefaultSunRiseSet();
	}
	/** Sets the planetary coordinates. (+ is North and East, - is South and West) */
	public void setCoordinates(double longitude, double latitude)
	{
		this.longitude= longitude;
		this.latitude= latitude;
	}
	/** Set the date you wish to obtain sunrise/set times for using a Date object */
	public void setDate(Date date)
	{
		// ensure that this Calendar object is always set to GMT timezone
		this.date= Calendar.getInstance(gmt);
		this.date.setTime(date);
		/*Calendar cal= Calendar.getInstance();
		cal.setTime(date);
		setDate(cal);*/
	}
	/** Set the date you wish to obtain sunrise/set times for using a Calendar object */
	public void setDate(Calendar date)
	{
		this.date= date;
		// ensure that this Calendar object is always set to GMT timezone
		this.date.setTimeZone(gmt);
	}
	/** Set the date you wish to obtain sunrise/set times for using a Date object 
	* plus the longitude and latitude. */
	public void setDateAndCoordinates(Date date, double longitude, double latitude)
	{
		setDate(date);
		setCoordinates(longitude, latitude);
	}
	/** Set the date you wish to obtain sunrise/set times for using a Calendar object 
	* plus the longitude and latitude. */
	public void setDateAndCoordinates(Calendar date, double longitude, double latitude)
	{
		setDate(date);
		setCoordinates(longitude, latitude);
	}	
	/** Implementation returns a Calendar object set to the sunrise time. <P>
	* Calendar should be set to GMT time zone */
	public abstract Calendar getSunriseTime();
	/** Implementation returns a Calendar object set to the sunset time. <P>
	* Calendar should be set to GMT time zone */	
	public abstract Calendar getSunsetTime();

}