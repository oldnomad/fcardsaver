#!/usr/bin/perl -w
use Text::CSV_XS;
use Unicode::Normalize;
use warnings;

my $SRC = shift @ARGV;
my $DST = shift @ARGV;
my %FILES;

my $TSV = Text::CSV_XS->new({
    binary => 1,
    sep_char => "\t",
    always_quote => 1,
    empty_is_undef => 1,
    auto_diag => 2
}) or die "".Text::CSV_XS->error_diag();
#binmode STDIN, ":utf8";

printf STDERR "Splitting file %s into %s\n", $SRC, $DST;
open SOURCE, '<:encoding(utf8)', $SRC or die "$!";
while (my $line = $TSV->getline(SOURCE))
{
    my $file;
    my $mode = $line->[2];
    $mode .= "-".$line->[3] if defined($line->[3]) && $line->[3] ne '';
    if (exists($FILES{$mode}))
    {
        $file = $FILES{$mode};
    }
    else
    {
        my $filename = sprintf($DST, lc($mode));
        printf STDERR "Creating file %s\n", $filename;
        open $file, '>:encoding(utf8)', $filename or die "$!";
        printf $file "# %s\n", $mode;
        my $hdr;
        open $hdr, '<', lc($line->[2]).".header" or die "$!";
        while (<$hdr>)
        {
            chomp;
            printf $file "%s\n", $_;
        }
        close $hdr;
        $FILES{$mode} = $file;
    }
    splice @$line, 3;
    $line->[0] = NFKC($line->[0]);
    $line->[2] = $mode;
    $TSV->print($file, $line);
    print $file "\n";
}
foreach (values %FILES)
{
    close $_ if defined $_;
}
close SOURCE;
