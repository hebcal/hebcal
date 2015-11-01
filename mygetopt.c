/* optarg - parse command-line arguments - changed to Optarg for portability */
/* Author: AT&T */

/** NOTE: This is slated for destruction.  we're going to go to GNU-style args soon */


#include <stdio.h>
#include <string.h>

#define ERR(S, C) if(Opterr){\
		       char errbuf[3];\
		       errbuf[0] = C; errbuf[1] = '\n'; errbuf[2] = '\0';\
		       fprintf(stderr,"%s", argv[0]);\
		       fprintf(stderr, "%s", S);\
		       fprintf(stderr, "%s", errbuf);}

int Opterr = 1;			/* getopt prints errors if this is on */
int Optind = 1;			/* token pointer */
int Optopt;			/* moption character passed back to user */
char *Optarg;			/* flag argument (or value) */


/* return moption character,  EOF if no more or ? if problem */
int 
Getopt (argc, argv, mopts, reset)
     int argc;
     char **argv;
     char *mopts;		/* moption string */
     int reset;
{
  static int sp = 1;		/* character index in current token */
  register char *cp;		/* pointer into current token */

  if (reset)
    {
      sp = 1;
      Opterr = 1;
      Optind = 1;
      Optopt = 0;
      Optarg = NULL;
      return 0;
    }

  if (sp == 1)
  {
      /* check for more flag-like tokens */
      if (Optind >= argc ||
          argv[Optind][0] != '-' || argv[Optind][1] == '\0')
          return (EOF);
      else if (strcmp (argv[Optind], "--") == 0)
      {
          Optind++;
          return (EOF);
      }
  }
  Optopt = argv[Optind][sp];
  if (Optopt == ':' || (cp = strchr (mopts, Optopt)) == 0)
  {
      ERR (": illegal option -- ", Optopt);
      /* if no chars left in this token, move to next token */
      if (argv[Optind][++sp] == '\0')
	{
	  Optind++;
	  sp = 1;
	}
      return ('?');
    }

  if (*++cp == ':')		/* if a value is expected, get it */
    {
      if (argv[Optind][sp + 1] != '\0')
	/* flag value is rest of current token */
	Optarg = &argv[Optind++][sp + 1];
      else if (++Optind >= argc)
	{
	  ERR (": option requires an argument -- ", Optopt);
	  sp = 1;
	  return ('?');
	}
      else
	/* flag value is next token */
	Optarg = argv[Optind++];
      sp = 1;
    }
  else
    {
      /* set up to look at next char in token, next time */
      if (argv[Optind][++sp] == '\0')
	{
	  /* no more in current token, so setup next token */
	  sp = 1;
	  Optind++;
	}
      Optarg = 0;
    }
  return (Optopt);		/* return the current flag character found */
}
