#!/usr/bin/perl

$c = 0;
while (<>) {
    if ((/^>/) || /->/) {
	if (/^>/) {
	    print;
	}
	else {
	    s/.*->.*\t//g;
	    if ($c) {
		s/\-//g;
		print; 
		$c = 0;    
	    }
	    else {
		$c++;
	    }
	}
    }
}
