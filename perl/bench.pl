#!/usr/local/bin/perl -w
use strict;
require "jewish.pl";
require "greg.pl";
use Date;
use Benchmark;

use Date::Convert;


my $astroDate = convert Date::Convert::Astro (new Date::Convert::Gregorian(1972,1,6));
print "astrotime: $astroDate->{astro}\n";
my $today_hebcal = Hebcal::greg2abs( new Date(dd=>6, mm=>1, yy=>1972));
my ($c,$d);
$d = Date::Convert::Hebrew->convert($astroDate);
$c = Hebcal::abs2hebrew($today_hebcal);
my $q;
my $count = 365;
timethese(100,{ 'Date::Convert'  => sub { 
		    for (0..$count){
		       $astroDate->{astro}++;
		       $q = Date::Convert::Hebrew->convert($astroDate) 
		    }
		 } ,
		 'Hebcal'	   => sub { 
		    for (0..$count){ 
		       $c = Hebcal::nextHebDate($c);
		    }
		}});
