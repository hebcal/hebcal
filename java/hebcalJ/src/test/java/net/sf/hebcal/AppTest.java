package net.sf.hebcal;

import junit.framework.Test;
import junit.framework.TestCase;
import junit.framework.TestSuite;

/**
 * Unit test for simple App.
 */
public class AppTest 
    extends TestCase
{
    /**
     * Create the test case
     *
     * @param testName name of the test case
     */
    public AppTest( String testName )
    {
        super( testName );
    }

    /**
     * @return the suite of tests being tested
     */
    public static Test suite()
    {
        return new TestSuite( AppTest.class );
    }

    /**
     * prove that we can print something out.
     */
    public void testApp()
        throws Exception
    {
        HebrewDate hd = new HebrewDate(12,31,2011,HebrewDate.ASHKENAZIS_ENGLISH_LOCALE);
        hd.setHebrewDate(9,30,5767);

        System.out.println(hd.formatHebrewDate_Hebrew());
    }
}
