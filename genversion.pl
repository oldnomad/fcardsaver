#!/usr/bin/perl -w
use warnings;

open CHGLOG, '<', 'debian/changelog' or die "$!";
my $line = readline(CHGLOG);
close CHGLOG;
die "No changelog preamble\n" unless defined($line);
die "Cannot match a version value\n" unless $line =~ m/.* \(([-._:+0-9a-z]+)\) .*/;
my $ver = $1;
die "Empty version\n" if $ver eq '';
printf "#define VERSION_STRING \"%s\"\n", $ver;
my @parts = split(/\./, $ver);
my @verl;
foreach my $v (@parts)
{
    last if $#verl >= 3;
    last unless $v =~ m/^(\d+)([a-z]?)$/;
    push @verl, $1;
    if ($2 ne '')
    {
        last if $#verl >= 3;
        push @verl, (ord($2) - ord('a') + 1);
    }
}
push @verl, 0 while $#verl < 3;
printf "#define VERSION_LIST %s\n", join(', ', @verl);
