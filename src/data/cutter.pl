#!/usr/bin/perl
use strict;

# By Piotr Mitros
# Copyright (c) 2001. Piotr F. Mitros. GNU GPL. http://www.fsf.org/

my @xcuts=(0,23,53,62,67,75,105,110,115,140,145,150,180,188,194,203,233,256);
my @ycuts=(53,75,98,105,110,121);
my $i;
my $j;

print "Looping ".$#xcuts." ".$#ycuts;

for $j (1 .. $#ycuts) {
    print ".";
    for $i (1 .. $#xcuts) {
#	print '<td><img src=blank'.$i."x".$j.'.tif></td>';
	system 'convert -crop ' .
	    ($xcuts[$i]-$xcuts[$i-1]) . "x" .
	    ($ycuts[$j]-$ycuts[$j-1]) . "+" . 
            ($xcuts[$i-1]) . "+".
	    ($ycuts[$j-1]) . " bathwhole.tif blank".$i."x".$j.".gif\n";

	system 'convert -crop ' .
	    ($xcuts[$i]-$xcuts[$i-1]) . "x" .
	    ($ycuts[$j]-$ycuts[$j-1]) . "+" . 
            ($xcuts[$i-1]) . "+".
	    ($ycuts[$j-1]) . " bathpiped.tif pipe".$i."x".$j.".gif\n";
    }
#    print '</tr><tr>' . "\n";
    print "\n";
}

