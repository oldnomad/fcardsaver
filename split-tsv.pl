#!/usr/bin/perl -w
#
# This script splits a TSV (tab-sepatared) flashcard file into several files
# using the last column in each line to determine which file should this line
# belong to.
#
# Any #FCARD# comments before the first tab-separated line are collected for
# further use; all other comments and empty lines are skipped.
#
# The last column in each tab-separated line is interpreted as a "mode". It
# is further split by dashes ("-") into "mode list".
#
# Script is called with 3 arguments: source file name, destination directory
# name, and configuration file name.
#
# Configuration file contains equals-separated definitions of various
# parameters, as listed below. Any line starting with hash ("#") is ignored.
#
# Following parameters are used:
#
#    normalize = {yes|no}
#
#       If specified and equals to "yes" (case-insensitive), each line is
#       normalized to Unicode NFKC form.
#
#    filename = <format>
#
#       Specifies sprintf-like format for creating output file name.
#       Parameters passed to sprintf are (in order): lowercase "mode" string,
#       all strings in "mode list" in their native case.
#
#       If this parameter is not specified, "%s.tsv" is used, meaning that
#       the resulting file name is "<mode>.tsv", in lowercase.
#
#    header:<mode> = {<name>|*}
#    header:<mode-elem-0> = {<name>|*}
#    header = {<name>|*}
#
#       Specifies header (#FCARD# comments) to use for this mode. Parameters
#       are checked in following order:
#
#          - "header:" with lowercase "mode" string.
#          - "header:" with lowercase initial element of "mode list".
#          - Default "header".
#
#       For example, if mode is "Kanji-G1", the script will look for
#       parameters "header:kanji-g1", "header:kanji" and "header", in that
#       order.
#
#       The value of this parameter, if specified and not "*", is used with
#       suffix ".header" as a file name of the header file to use. All lines
#       from this file will be included in the beginning of the output file.
#
#       If header name is not specified or is "*", old header collected from
#       the input file will be used.
#
use Text::CSV_XS;
use Unicode::Normalize;
use warnings;

my $SRC = shift @ARGV;
my $DST = shift @ARGV;
my $CFG = shift @ARGV;
my @OLDHEADER;
my %FILES;
my %CONFIG;

open CONF, '<:encoding(utf8)', $CFG or die "$!";
while (<CONF>)
{
    chomp;
    next if m/^#/;
    s/^\s*(.*)\s*$/$1/;
    next unless $_ ne '';
    my ($var, $val) = split(/\s*=\s*/, $_, 2);
    $CONFIG{$var} = $val;
}
close CONF;

my $TSV = Text::CSV_XS->new({
    binary => 1,
    sep_char => "\t",
    always_quote => 1,
    empty_is_undef => 1,
    auto_diag => 2
}) or die "".Text::CSV_XS->error_diag();

printf STDERR "Splitting file %s into directory %s\n", $SRC, $DST;
open SOURCE, '<:encoding(utf8)', $SRC or die "$!";
my $got_line = 0;
while (<SOURCE>)
{
    chomp;
    if (m/^#FCARD#/)
    {
        push @OLDHEADER, $_ unless $got_line;
        next;
    }
    next if m/^#/;
    next if m/^\s*$/;
    $_ = NFKC($_) if defined($CONFIG{'normalize'}) && lc $CONFIG{'normalize'} eq 'yes';
    $TSV->parse($_) or die "".$TSV->error_diag;
    my @line = $TSV->fields;
    $got_line = 1;
    my $file;
    my $mode = $line[$#line];
    my $mode_l = lc $mode;
    my @mode_list = split('-', $mode);
    if (exists($FILES{$mode_l}))
    {
        $file = $FILES{$mode_l};
    }
    else
    {
        my $fnformat = $CONFIG{'filename'} || '%s.tsv';
        my $filename = $DST.'/'.sprintf($fnformat, $mode_l, @mode_list);
        printf STDERR "Creating file %s\n", $filename;
        open $file, '>:encoding(utf8)', $filename or die "$!";
        printf $file "# %s\n", $mode;
        my $hdr = $CONFIG{'header:'.$mode_l} ||
            $CONFIG{'header:'.lc $mode_list[0]} ||
            $CONFIG{'header'} || '*';
        printf STDERR "Using header '%s'\n", $hdr;
        if ($hdr eq '*')
        {
            printf $file "%s\n", $_ foreach (@OLDHEADER);
        }
        else
        {
            open HEADER, '<', $hdr.".header" or die "$!";
            while (<HEADER>)
            {
                chomp;
                printf $file "%s\n", $_;
            }
            close HEADER;
        }
        $FILES{lc $mode} = $file;
    }
    $TSV->print($file, \@line);
    print $file "\n";
}
foreach (values %FILES)
{
    close $_ if defined $_;
}
close SOURCE;
