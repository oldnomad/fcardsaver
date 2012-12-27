#!/usr/bin/perl -w
use warnings;

my $SRC = shift @ARGV;
my $DST = shift @ARGV;
my @MODES = @ARGV;
my @HEADER;
my $GOT_LINE = 0;
my %FILES;

printf STDERR "Splitting file %s into %s, modes %s\n", $SRC, $DST, join(', ', @MODES);
$FILES{$_} = undef foreach (@MODES);
open SOURCE, '<', $SRC or die "$!";
while (<SOURCE>)
{
    chomp;
    if (m/^#FCARD#/)
    {
        push @HEADER, $_ unless $GOT_LINE;
        next;
    }
    next if m/^#/;
    next unless m/\t"Kanji-([^"]*)"$/;
    my $mode = $1;
    my $line = $_;
    $GOT_LINE = 1;
    next unless exists $FILES{$mode};
    my $file = $FILES{$mode};
    unless (defined $file)
    {
        my $filename = sprintf($DST, $mode);
        printf STDERR "Creating file %s\n", $filename;
        open $file, '>', $filename or die "$!";
        printf $file "# Kanji %s\n", $mode;
        printf $file "%s\n", $_ foreach (@HEADER);
        $FILES{$mode} = $file;
    }
    printf $file "%s\n", $line;
}
foreach (values %FILES)
{
    close $_ if defined $_;
}
close SOURCE;
