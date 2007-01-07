package avrom.util;

import com.web_tomorrow.utils.suntimes.*;

import java.util.*;

/**
* Concrete subclass of SunRiseSet.<P>
* This uses the classes written by <A HREF="http://www.kevinboone.com/">Kevin Boone</A>.
*/
public class DefaultSunRiseSet extends SunRiseSet
{

	public Calendar getSunriseTime()
	{
		try
		{
			Time time= SunTimes.getSunriseTimeUTC(date.get(Calendar.YEAR), 
					date.get(Calendar.MONTH)+ 1, 
					date.get(Calendar.DAY_OF_MONTH), 
					longitude, latitude, SunTimes.CIVIL_ZENITH);

			return getDateFromTime(time);
		}
		catch (SunTimesException e)
		{
			e.printStackTrace();
		}
		return null;
	}

	private Calendar getDateFromTime(Time time)
	{
		Calendar cal= (Calendar) date.clone();
		cal.set(Calendar.HOUR_OF_DAY, time.getHour());
		cal.set(Calendar.MINUTE, time.getMinute());
		cal.set(Calendar.SECOND, time.getSecond());

		return cal;
	}
	public Calendar getSunsetTime()
	{
		try
		{
			Time time= SunTimes.getSunsetTimeUTC(date.get(Calendar.YEAR), 
					date.get(Calendar.MONTH)+ 1, 
					date.get(Calendar.DAY_OF_MONTH), longitude, latitude, SunTimes.ZENITH);
			return getDateFromTime(time);
		}
		catch (SunTimesException e)
		{
			e.printStackTrace();
		}

		return null;
	}
}