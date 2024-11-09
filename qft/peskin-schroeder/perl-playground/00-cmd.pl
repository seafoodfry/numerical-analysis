#!/usr/bin/perl

use Getopt::Long;

my $opt_frame_div = 4;
my $opt_frame_rate = 24;
my $opt_links_dir = 'links';
my $opt_plot_options = '';
my $opt_prefix = 'tmp/';

my $own_var;
my $num;
my $verbose = '';

my $result = GetOptions(
        "frame-div=s" => \$opt_frame_div,
        "frame-rate=s" => \$opt_frame_rate,
        "links-dir=s" => \$opt_links_dir,
        "plot-options=s" => \$opt_plot_options,
        "prefix=s" => \$opt_prefix,
        "own-var=s" => \$own_var,
        "num=i" => \$num,
        "verbose" => \$verbose,
);

# Check if GetOptions succeeded
if (!$result) {
    print STDERR "Usage: ./animate.pl [options] < slides.tex\n";
    exit(1);
}

# Print the values of the variables
print "frame-div: $opt_frame_div\n";
print "frame-rate: $opt_frame_rate\n";
print "links-dir: $opt_links_dir\n";
print "plot-options: $opt_plot_options\n";
print "prefix: $opt_prefix\n";

print "own-var: $own_var\n";
print "num: $num\n";
print "verbose: $verbose\n";
