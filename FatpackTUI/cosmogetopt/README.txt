GETOPT(3)         Cosmopolitan Library Functions Manual              GETOPT(3)

𝐍𝐀𝐌𝐄
     𝗴𝗲𝘁𝗼𝗽𝘁 — get option character from command line argument list

𝐒𝐘𝐍𝐎𝐏𝐒𝐈𝐒
     #𝗶𝗻𝗰𝗹𝘂𝗱𝗲 <𝘂𝗻𝗶𝘀𝘁𝗱.𝗵>

     e̲x̲t̲e̲r̲n̲ c̲h̲a̲r̲ *̲o̲p̲t̲a̲r̲g̲;̲
     e̲x̲t̲e̲r̲n̲ i̲n̲t̲ o̲p̲t̲e̲r̲r̲;̲
     e̲x̲t̲e̲r̲n̲ i̲n̲t̲ o̲p̲t̲i̲n̲d̲;̲
     e̲x̲t̲e̲r̲n̲ i̲n̲t̲ o̲p̲t̲o̲p̲t̲;̲
     e̲x̲t̲e̲r̲n̲ i̲n̲t̲ o̲p̲t̲r̲e̲s̲e̲t̲;̲

     i̲n̲t̲
     𝗴𝗲𝘁𝗼𝗽𝘁(i̲n̲t̲ a̲r̲g̲c̲, c̲h̲a̲r̲ *̲ c̲o̲n̲s̲t̲ *̲a̲r̲g̲v̲, c̲o̲n̲s̲t̲ c̲h̲a̲r̲ *̲o̲p̲t̲s̲t̲r̲i̲n̲g̲);

𝐃𝐄𝐒𝐂𝐑𝐈𝐏𝐓𝐈𝐎𝐍
     The 𝗴𝗲𝘁𝗼𝗽𝘁() function incrementally parses a command line argument
     list a̲r̲g̲v̲ and returns the next k̲n̲o̲w̲n̲ option character.  An option
     character is k̲n̲o̲w̲n̲ if it has been specified in the string of
     accepted option characters, o̲p̲t̲s̲t̲r̲i̲n̲g̲.

     The option string o̲p̲t̲s̲t̲r̲i̲n̲g̲ may contain the following elements:
     individual characters, characters followed by a colon, and charac‐
     ters followed by two colons.  A character followed by a single
     colon indicates that an argument is to follow the option on the
     command line.  Two colons indicates that the argument is optional -
     this is an extension not covered by POSIX.  For example, an option
     string "x" recognizes an option -𝘅, and an option string "x:" rec‐
     ognizes an option and argument -𝘅 a̲r̲g̲u̲m̲e̲n̲t̲.  It does not matter to
     𝗴𝗲𝘁𝗼𝗽𝘁() if a following argument has leading whitespace; except in
     the case where the argument is optional, denoted with two colons,
     no leading whitespace is permitted.

     On return from 𝗴𝗲𝘁𝗼𝗽𝘁(), o̲p̲t̲a̲r̲g̲ points to an option argument, if it
     is anticipated, and the variable o̲p̲t̲i̲n̲d̲ contains the index to the
     next a̲r̲g̲v̲ argument for a subsequent call to 𝗴𝗲𝘁𝗼𝗽𝘁().

     The variables o̲p̲t̲e̲r̲r̲ and o̲p̲t̲i̲n̲d̲ are both initialized to 1.  The
     o̲p̲t̲i̲n̲d̲ variable may be set to another value larger than 0 before a
     set of calls to 𝗴𝗲𝘁𝗼𝗽𝘁() in order to skip over more or less a̲r̲g̲v̲
     entries.  An o̲p̲t̲i̲n̲d̲ value of 0 is reserved for compatibility with
     GNU 𝗴𝗲𝘁𝗼𝗽𝘁().

     In order to use 𝗴𝗲𝘁𝗼𝗽𝘁() to evaluate multiple sets of arguments, or
     to evaluate a single set of arguments multiple times, the variable
     o̲p̲t̲r̲e̲s̲e̲t̲ must be set to 1 before the second and each additional set
     of calls to 𝗴𝗲𝘁𝗼𝗽𝘁(), and the variable o̲p̲t̲i̲n̲d̲ must be reinitial‐
     ized.

     The 𝗴𝗲𝘁𝗼𝗽𝘁() function returns -1 when the argument list is
     exhausted.  The interpretation of options in the argument list may
     be cancelled by the option ‘--’ (double dash) which causes 𝗴𝗲𝘁𝗼𝗽𝘁()
     to signal the end of argument processing and return -1.  When all
     options have been processed (i.e., up to the first non-option argu‐
     ment), 𝗴𝗲𝘁𝗼𝗽𝘁() returns -1.

𝐑𝐄𝐓𝐔𝐑𝐍 𝐕𝐀𝐋𝐔𝐄𝐒
     The 𝗴𝗲𝘁𝗼𝗽𝘁() function returns the next known option character in
     o̲p̲t̲s̲t̲r̲i̲n̲g̲.  If 𝗴𝗲𝘁𝗼𝗽𝘁() encounters a character not found in
     o̲p̲t̲s̲t̲r̲i̲n̲g̲ or if it detects a missing option argument, it returns
     ‘?’ (question mark).  If o̲p̲t̲s̲t̲r̲i̲n̲g̲ has a leading ‘:’ then a missing
     option argument causes ‘:’ to be returned instead of ‘?’.  In
     either case, the variable o̲p̲t̲o̲p̲t̲ is set to the character that
     caused the error.  The 𝗴𝗲𝘁𝗼𝗽𝘁() function returns -1 when the argu‐
     ment list is exhausted.

𝐄𝐗𝐀𝐌𝐏𝐋𝐄𝐒
     The following code accepts the options -𝗯 and -𝗳 a̲r̲g̲u̲m̲e̲n̲t̲ and
     adjusts a̲r̲g̲c̲ and a̲r̲g̲v̲ after option argument processing has com‐
     pleted.

           int bflag, ch, fd;

           bflag = 0;
           while ((ch = getopt(argc, argv, "bf:")) != -1) {
                   switch (ch) {
                   case 'b':
                           bflag = 1;
                           break;
                   case 'f':
                           if ((fd = open(optarg, O_RDONLY, 0)) == -1)
                                   err(1, "%s", optarg);
                           break;
                   default:
                           usage();
                   }
           }
           argc -= optind;
           argv += optind;

𝐃𝐈𝐀𝐆𝐍𝐎𝐒𝐓𝐈𝐂𝐒
     If the 𝗴𝗲𝘁𝗼𝗽𝘁() function encounters a character not found in the
     string o̲p̲t̲s̲t̲r̲i̲n̲g̲ or detects a missing option argument, it writes an
     error message to s̲t̲d̲e̲r̲r̲ and returns ‘?’.  Setting o̲p̲t̲e̲r̲r̲ to a zero
     will disable these error messages.  If o̲p̲t̲s̲t̲r̲i̲n̲g̲ has a leading ‘:’
     then a missing option argument causes a ‘:’ to be returned in addi‐
     tion to suppressing any error messages.

     Option arguments are allowed to begin with ‘-’; this is reasonable
     but reduces the amount of error checking possible.

𝐒𝐄𝐄 𝐀𝐋𝐒𝐎
     getopt(1), getopt_long(3), getsubopt(3)

𝐒𝐓𝐀𝐍𝐃𝐀𝐑𝐃𝐒
     The 𝗴𝗲𝘁𝗼𝗽𝘁() function implements a superset of the functionality
     specified by IEEE Std 1003.1 (“POSIX.1”).

     The following extensions are supported:

     ·   The o̲p̲t̲r̲e̲s̲e̲t̲ variable was added to make it possible to call the
         𝗴𝗲𝘁𝗼𝗽𝘁() function multiple times.

     ·   If the o̲p̲t̲i̲n̲d̲ variable is set to 0, 𝗴𝗲𝘁𝗼𝗽𝘁() will behave as if
         the o̲p̲t̲r̲e̲s̲e̲t̲ variable has been set.  This is for compatibility
         with GNU 𝗴𝗲𝘁𝗼𝗽𝘁().  New code should use o̲p̲t̲r̲e̲s̲e̲t̲ instead.

     ·   If the first character of o̲p̲t̲s̲t̲r̲i̲n̲g̲ is a plus sign (‘+’), it
         will be ignored.  This is for compatibility with GNU 𝗴𝗲𝘁𝗼𝗽𝘁().

     ·   If the first character of o̲p̲t̲s̲t̲r̲i̲n̲g̲ is a dash (‘-’), non-
         options will be returned as arguments to the option character
         ‘\1’.  This is for compatibility with GNU 𝗴𝗲𝘁𝗼𝗽𝘁().

     ·   A single dash (‘-’) may be specified as a character in
         o̲p̲t̲s̲t̲r̲i̲n̲g̲, however it should n̲e̲v̲e̲r̲ have an argument associated
         with it.  This allows 𝗴𝗲𝘁𝗼𝗽𝘁() to be used with programs that
         expect ‘-’ as an option flag.  This practice is wrong, and
         should not be used in any current development.  It is provided
         for backward compatibility o̲n̲l̲y̲.  Care should be taken not to
         use ‘-’ as the first character in o̲p̲t̲s̲t̲r̲i̲n̲g̲ to avoid a semantic
         conflict with GNU 𝗴𝗲𝘁𝗼𝗽𝘁() semantics (see above).  By default,
         a single dash causes 𝗴𝗲𝘁𝗼𝗽𝘁() to return -1.

     Historic BSD versions of 𝗴𝗲𝘁𝗼𝗽𝘁() set o̲p̲t̲o̲p̲t̲ to the last option
     character processed.  However, this conflicts with IEEE Std 1003.1
     (“POSIX.1”) which stipulates that o̲p̲t̲o̲p̲t̲ be set to the last charac‐
     ter that caused an error.

𝐇𝐈𝐒𝐓𝐎𝐑𝐘
     The 𝗴𝗲𝘁𝗼𝗽𝘁() function appeared in 4.3BSD.

𝐁𝐔𝐆𝐒
     The 𝗴𝗲𝘁𝗼𝗽𝘁() function was once specified to return EOF instead of
     -1.  This was changed by IEEE Std 1003.2-1992 (“POSIX.2”) to decou‐
     ple 𝗴𝗲𝘁𝗼𝗽𝘁() from <s̲t̲d̲i̲o̲.̲h̲>.

     It is possible to handle digits as option letters.  This allows
     𝗴𝗲𝘁𝗼𝗽𝘁() to be used with programs that expect a number (“-3”) as an
     option.  This practice is wrong, and should not be used in any cur‐
     rent development.  It is provided for backward compatibility o̲n̲l̲y̲.
     The following code fragment works in most cases and can handle
     mixed number and letter arguments.

           int aflag = 0, bflag = 0, ch, lastch = '\0';
           int length = -1, newarg = 1, prevoptind = 1;

           while ((ch = getopt(argc, argv, "0123456789ab")) != -1) {
                   switch (ch) {
                   case '0': case '1': case '2': case '3': case '4':
                   case '5': case '6': case '7': case '8': case '9':
                           if (newarg || !isdigit(lastch))
                                   length = 0;
                           else if (length > INT_MAX / 10)
                                   usage();
                           length = (length * 10) + (ch - '0');
                           break;
                   case 'a':
                           aflag = 1;
                           break;
                   case 'b':
                           bflag = 1;
                           break;
                   default:
                           usage();
                   }
                   lastch = ch;
                   newarg = optind != prevoptind;
                   prevoptind = optind;
           }

COSMOPOLITAN                    January 4, 2016                            BSD
