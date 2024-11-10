#!/usr/bin/perl

# Delete the Makefile.
unlink 'Makefile.generated' or die "Failed to delete Makefile.generated: $!";

# Intentional error.
open(my $file, '<', 'nonexistent_file.txt') or die "Failed to open file: $!";