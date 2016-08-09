#!/usr/bin/perl

# By Piotr Mitros
# Copyright (c) 2001. Piotr F. Mitros. GNU GPL. http://www.fsf.org/

while(<>) {
    @f=split /:/;
    print $f[0].':'.$f[2]."\n";
}
    
