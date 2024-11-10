#!/usr/bin/perl

my %make_rules_seen;

my $dir = 'tmp';
my $frame_counter = 0;
my @frame_links;


# Open the generated makefile.
open(my $makefile, '>', 'Makefile.generated') or die;

sub add_make_rule
{
    my ($dsts, $srcs, @commands) = @_;
    my $dst = join(' ', @$dsts);
    my $src = join(' ', @$srcs);

    return if $make_rules_seen{$dst}++;

    print $makefile "$dst : $src\n";
    print $makefile "\t$_\n" for @commands;
    print $makefile "\n";
}

sub add_frame
{
    my ($path) = @_;
    my $fn_link = sprintf("$dir/frame-%06d.png", $frame_counter++);
    push @frame_links, [$fn_link, $path];
}


# Add a rule via the subroutine.
add_make_rule(['output.o'], ['input.c', 'header.h'], ('gcc -c input.c -o output.o', 'python --version'));


# Manually add a target.
add_frame('./path/file');
add_frame('./path/another_file');

print $makefile "\n.PHONY: run\n";
print $makefile "run:\n";
print $makefile "\tln -sf ../$_->[1] $_->[0]\n" for @frame_links;

# Print the contents of %make_rules_seen.
print "Contents of %make_rules_seen:\n";
foreach my $key (keys %make_rules_seen) {
    print "\t$key => $make_rules_seen{$key}\n";
}

print STDOUT "\n";

# Print the contents of @frame_links for debugging.
print "Contents of \@frame_links:\n";
foreach my $frame (@frame_links) {
    my ($fn_link, $path) = @$frame;  # Dereference the array reference.
    print "\tLink: $fn_link, Path: $path\n";
}

# CLose the generated makefile.
close($makefile) or die "Failed to close Makefile.generated: $!";