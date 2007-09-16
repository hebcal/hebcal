// $Header$

// good test birthday data: 2000-3-7

/** calculate the bar/bat mitzvah
   date from the given jewish date */
function bMitzvah( hBirthDate, maleNotFemale ){
    assert("bmitzvah start:"+hBirthDate,hBirthDate instanceof HebcalHebrewDate);
    var years ;
    if(maleNotFemale){
        years = 13;
    }
    else{
        years = 12;
    }

    var bmYear = hBirthDate.hYear + years;
    var bmMonth;
    if( hBirthDate.hMonth < HebcalHebrewDate.ADAR_I){
        bmMonth = hBirthDate.hMonth;
    } else if( ! LEAP_YR_HEB( bmYear )){ 
        //born in some Adar, non-leap b-mitzvah year
        bmMonth = HebcalHebrewDate.ADAR_I;
    } else if( LEAP_YR_HEB (hBirthDate.hYear ) 
               && hBirthDate.hMonth == HebcalHebrewDate.ADAR_I ){
        //born in AdarI, leap b-mitzvah year
        bmMonth = HebcalHebrewDate.ADAR_I;

    } else{
        //born in Adar or AdarII, leap b-mitzvah year
        bmMonth = HebcalHebrewDate.ADAR_II;
    }

    var bmDay;
    if( 30 !=  hBirthDate.hDay )
        bmDay = hBirthDate.hDay; 
    else if (30 ==  max_days_in_heb_month( bmMonth, bmYear ))
        bmDay = hBirthDate.hDay; 
    else { //29-day bar-mitzvah month, birthday on 30th.  roll.
        bmDay = 1;
        bmMonth++;              // only get here in adar if bmMonth is adar I.
        if( bmMonth ==  HebcalHebrewDate.ADAR_II && !LEAP_YR_HEB( bmYear ) )
            bmMonth = HebcalHebrewDate.NISAN;
    }

    return new HebcalHebrewDate(bmYear, bmMonth, bmDay);
}
