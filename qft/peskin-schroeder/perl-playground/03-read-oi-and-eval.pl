#!/usr/bin/perl

my $code = '';

# Read lines from an input file...
while (<>) {
    # ^\s*: Matches any leading whitespace at the beginning of the line.
	# %:: Matches the literal %: at the start of the line, possibly indicating a special command or instruction.
	# (.*): Captures everything following %: on that line into $1.
    if (/^\s*%:(.*)/) {
        my ($line) = ($1);
        $code .= "$line\n";

        print "code is now:\n'$code'\n";
    }
    # ^\s*: Matches optional whitespace at the beginning of the line.
	# %.: Matches a % followed by any character (.).
	# (\d+(\.\d+)?): Matches a number, which could have a decimal point (\.) and digits after it.
    elsif (/^\s*%.(\d+(\.\d+)?)/) {
        my ($num) = $1;

        print "read numerical input: $num\n";
    }
    else {
        # Capture the current line directly
        my $input = $_;


        if ($code ne '') {
            # Execute perl code - not shell code.
            eval($code);
            die $@ if $@;
            $code = '';
        }

        if (/^\s*\\begin{frame}/ || /^\s*\\pause\b/) {
            print "found a frame or a pause command\n";
            print "input: $input\n";
        }
    }
}