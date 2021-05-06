#!/usr/bin/env perl

=pod

toICS.pl

converts hebcal -g output to ICS.


usage: hebcal -Y ~/yahrtzeit.txt 2022 -h -g | perl toICS.pl -p Yahrtzeit
 hebcal takes as input the gregorian dates in  yahrtzeit.txt  stored as; 
    MM DD YYYY description...
  hebcal's output converts those to gregorian (presumably future) dates like so:

2022-04-18 Joe Blow
2022-04-25 Jane Doe
...

this script parses that and outputs simple all-day events in iCalendar format 
(aka .ics, .ical etc)

=cut

use strict;
use feature ':5.10';

say "BEGIN:VCALENDAR\nVERSION:1.0";
my $prefix = '';
if($ARGV[0] eq '-p' ){
    shift;
    $prefix = shift;
    $prefix .=": ";
}

while(<>){
    chomp;

    my ($dtiso, $name)  = split (/ /,$_, 2);
    $dtiso=~s/-//g;

    say "BEGIN:VEVENT";

    say "DTSTART;VALUE=DATE:$dtiso";
    say "DTEND;VALUE=DATE:$dtiso";

    say "SUMMARY:$prefix$name";
    say "DESCRIPTION:$prefix$name";

    say "END:VEVENT" ;
    
}
say "END:VCALENDAR";
