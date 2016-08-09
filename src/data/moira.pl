#!/usr/athena/bin/perl

# By Piotr Mitros
# Copyright (c) 2001. Piotr F. Mitros. GNU GPL. http://www.fsf.org/

sub check_ip {
    my $name=0;
    my $ip=shift;
    my $line;
    my $room="000";
    my $owner="unknown";
    my $human="guest";

    open PIPE, "nslookup ".$ip."|";
    
    while(<PIPE>) {
	if(/Name: *([^.]*)./) {
	    $name=$1;
	}
    }
    
    close PIPE;
    
    if($name) {
	open FILE, ">tmpcmd";
	print FILE "4\n1\n".$name."\nq\n" ;
	close FILE;
	
	open PIPE, "cat tmpcmd | moira -nomenu|";
	
	while($line=<PIPE>) {
	    if($line =~ /Location: *([^ ]*)[ ]*/) {
		$room=$1;
		$room =~ s/NW61-//g;
		chomp($room);
	    }
	    if($line =~ /Owner: *([^ ]* ?[^ ]*)[ ]*/) {
		$owner=$1;
		$owner =~ s/USER //g;
		$owner =~ s/ //g;
	    }
	}
	close PIPE;

	if(($owner ne "NONE") && ($owner ne "unknown")) {
	    open PIPE, "finger ".$owner.'|';
	
	    while($line=<PIPE>) {
		if($line =~ /real life:[^A-Za-z]*([^ ]*)[ ]*/) {
		    if(!($line =~ /\?\?\?/)) {
			$human=$1;
			chomp($human);
		    }
		}
	    }
	    close PIPE;
	}

	$ip.":".$name.":".$room.":".$owner.":".$human."\n";
    } else {
	0;
    }
}

my $i, $j, $res;

open RES, ">moira.res";

for $j (0 .. 1) {
    for $i (1 .. 255) {
	$res=check_ip("18.243.".$j.".".$i);
	if($res) {
	    print RES $res;
	    print $res;
#	    Moira will throttle us if we don't wait a bit. This is
#	    more than we need, so be it. 
	    system 'sleep 1';
	}
    }
}

close RES;
