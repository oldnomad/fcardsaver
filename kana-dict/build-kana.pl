#!/usr/bin/perl -w
use Text::CSV_XS;
use Unicode::Normalize;

use constant DEFAULT_DESTDIR => '../build/dict';
use constant KANA_FILE => 'kana.tsv';

my %YOON = (
    "ya" => "\N{U+3083}",
    "yu" => "\N{U+3085}",
    "yo" => "\N{U+3087}",
    "wa" => "\N{U+308E}",
    "YA" => "\N{U+30E3}",
    "YU" => "\N{U+30E5}",
    "YO" => "\N{U+30E7}",
    "WA" => "\N{U+30EE}"
);
my @MARK_SYM = (
    "",
    "\N{U+3099}", # Dakuten
    "\N{U+309A}"  # Handakuten
);


my $KANA_MAP;
my %KANA_LISTS;
my $TSV = Text::CSV_XS->new({
    binary => 1,
    sep_char => "\t",
    always_quote => 1,
    empty_is_undef => 1,
    auto_diag => 2
}) or die "".Text::CSV_XS->error_diag();

my $DEST_DIR = DEFAULT_DESTDIR;
if ($#ARGV >= 0)
{
    $DEST_DIR = $ARGV[0];
}

sub read_kana_map {
    open my $fh, "<:encoding(utf8)", KANA_FILE
        or die "$!";
    $KANA_MAP = $TSV->getline_all($fh);
    shift @$KANA_MAP;
    close $fh;
}

sub append_list($$$) {
    my ($lname, $kana, $romaji) = @_;
    my $fh;
    unless (exists($KANA_LISTS{$lname}))
    {
        my $lfile = $DEST_DIR."/".$lname.".tsv";
	printf STDERR "Creating file '%s'\n", $lfile;
        open $fh, ">:encoding(utf8)", $DEST_DIR."/".$lname.".tsv"
            or die "$!";
        my $hname = (split(/-/, $lname))[0].".header";
        open my $fhh, "<:encoding(utf8)", $hname
            or die "$!";
        while (<$fhh>)
        {
            print $fh $_;
        }
        close $fhh;
        $KANA_LISTS{$lname} = $fh;
    }
    else
    {
        $fh = $KANA_LISTS{$lname};
    }
    $TSV->print($fh, [ $kana, $romaji ]);
    print $fh "\n";
}

sub append_marked($$$$$) {
    my ($hk, $hm, $kk, $km, $rj) = @_;
    $hk = NFKC($hk.$hm);
    $kk = NFKC($kk.$km);
    my $ko = $rj =~ s/^&//;
    my $syll = "";
    $syll = "-ext" unless $hm eq '' && $km eq '';
    $syll = "-alien" if $rj =~ s/^!//;
    append_list "hiragana".$syll, $hk, $rj unless $ko;
    append_list "katakana".$syll, $kk, uc $rj;
}

printf STDERR "Reading kana map\n";
read_kana_map();
printf STDERR "Building lists\n";
foreach my $row (@$KANA_MAP)
{
    if (defined($row->[2]))
    {
        my (@rj) = split(/\Q|\E/, $row->[2]);
        for (my $i = 0; $i <= $#rj && $i <= $#MARK_SYM; ++$i)
        {
            my $mark = $MARK_SYM[$i];
            append_marked $row->[0], $mark, $row->[1], $mark, $rj[$i];
        }
    }
    if (defined($row->[3]))
    {
        my (@rj) = split(/\Q|\E/, $row->[3]);
        for (my $i = 0; $i <= $#rj && $i <= $#MARK_SYM; ++$i)
        {
            my $mark = $MARK_SYM[$i];
            my $pref = $rj[$i];
            foreach (grep(/^y/, keys %YOON))
            {
                my $suf = substr($_, 1);
                append_marked $row->[0], $mark.$YOON{$_}, $row->[1], $mark.$YOON{uc $_}, $pref.$suf;
            }
        }
    }
    if (defined($row->[4]))
    {
        my (@rj) = split(/\Q|\E/, $row->[4]);
        for (my $i = 0; $i <= $#rj && $i <= $#MARK_SYM; ++$i)
        {
            my $mark = $MARK_SYM[$i];
            my $pref = $rj[$i];
            foreach (grep /^w/, keys(%YOON))
            {
                my $suf = substr($_, 1);
                append_marked $row->[0], $mark.$YOON{$_}, $row->[1], $mark.$YOON{uc $_}, $pref.$suf;
            }
        }
    }
}

printf STDERR "Closing lists\n";
foreach my $fh (values %KANA_LISTS)
{
    close $fh;
}
