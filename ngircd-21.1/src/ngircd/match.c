/*
 * ngIRCd -- The Next Generation IRC Daemon
 * Copyright (c)2001-2012 Alexander Barton (alex@barton.de) and Contributors.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * Please read the file COPYING, README and AUTHORS for more information.
 */

#include "portab.h"

/**
 * @file
 * Wildcard pattern matching
 */

#include "imp.h"
#include <assert.h>
#include <string.h>

#include "exp.h"
#include "match.h"
#include "defines.h"
#include "tool.h"


/*
 * The pattern matching functions [Matche(), Matche_After_Star()] are based
 * on code of J. Kercheval. Version 1.1 has been released on 1991-03-12 as
 * "public domain": <http://c.snippets.org/snip_lister.php?fname=match.c>
 */


static int Matche PARAMS(( const char *p, const char *t ));
static int Matche_After_Star PARAMS(( const char *p, const char *t ));


#define MATCH_PATTERN	6	/**< bad pattern */
#define MATCH_LITERAL	5	/**< match failure on literal match */
#define MATCH_RANGE	4	/**< match failure on [..] construct */
#define MATCH_ABORT	3	/**< premature end of text string */
#define MATCH_END	2	/**< premature end of pattern string */
#define MATCH_VALID	1	/**< valid match */


/**
 * Match string with pattern.
 *
 * @param Pattern Pattern to match with
 * @param String Input string
 * @return true if pattern matches
 */
GLOBAL bool
Match( const char *Pattern, const char *String )
{
	if( Matche( Pattern, String ) == MATCH_VALID ) return true;
	else return false;
} /* Match */


/**
 * Match string with pattern case-insensitive.
 *
 * @param Pattern Pattern to match with
 * @param String Input string, at most COMMAND_LEN-1 characters long
 * @return true if pattern matches
 */
GLOBAL bool
MatchCaseInsensitive(const char *Pattern, const char *String)
{
	char haystack[COMMAND_LEN];

	strlcpy(haystack, String, sizeof(haystack));
	return Match(Pattern, ngt_LowerStr(haystack));
} /* MatchCaseInsensitive */


/**
 * Match string with pattern case-insensitive.
 *
 * @param pattern Pattern to match with
 * @param String Input string, at most COMMAND_LEN-1 characters long
 * @param Separator Character separating the individual patterns in the list
 * @return true if pattern matches
 */
GLOBAL bool
MatchCaseInsensitiveList(const char *Pattern, const char *String,
		     const char *Separator)
{
	char tmp_pattern[COMMAND_LEN], haystack[COMMAND_LEN], *ptr;

	strlcpy(tmp_pattern, Pattern, sizeof(tmp_pattern));
	strlcpy(haystack, String, sizeof(haystack));
	ngt_LowerStr(haystack);

	ptr = strtok(tmp_pattern, Separator);
	while (ptr) {
		ngt_TrimStr(ptr);
		if (Match(ptr, haystack))
			return true;
		ptr = strtok(NULL, Separator);
	}
	return false;
} /* MatchCaseInsensitive */


static int
Matche( const char *p, const char *t )
{
	register char range_start, range_end;
	bool invert;
	bool member_match;
	bool loop;

	for( ; *p; p++, t++ )
	{
		/* if this is the end of the text then this is the end of the match */
		if( ! *t )
		{
			return ( *p == '*' && *++p == '\0' ) ? MATCH_VALID : MATCH_ABORT;
		}

		/* determine and react to pattern type */
		switch( *p )
		{
			case '?':	/* single any character match */
				break;

			case '*':	/* multiple any character match */
				return Matche_After_Star( p, t );

			case '[':	/* [..] construct, single member/exclusion character match */
				/* move to beginning of range */
				p++;

				/* check if this is a member match or exclusion match */
				invert = false;
				if( *p == '!' || *p == '^' )
				{
					invert = true;
					p++;
				}

				/* if closing bracket here or at range start then we have a malformed pattern */
				if ( *p == ']' ) return MATCH_PATTERN;

				member_match = false;
				loop = true;

				while( loop )
				{
					/* if end of construct then loop is done */
					if( *p == ']' )
					{
						loop = false;
						continue;
					}

					/* matching a '!', '^', '-', '\' or a ']' */
					if( *p == '\\' ) range_start = range_end = *++p;
					else  range_start = range_end = *p;

					/* if end of pattern then bad pattern (Missing ']') */
					if( ! *p ) return MATCH_PATTERN;

					/* check for range bar */
					if( *++p == '-' )
					{
						/* get the range end */
						range_end = *++p;

						/* if end of pattern or construct then bad pattern */
						if( range_end == '\0' || range_end == ']' ) return MATCH_PATTERN;

						/* special character range end */
						if( range_end == '\\' )
						{
							range_end = *++p;

							/* if end of text then we have a bad pattern */
							if ( ! range_end ) return MATCH_PATTERN;
						}

						/* move just beyond this range */
						p++;
					}

					/* if the text character is in range then match found. make sure the range
					 * letters have the proper relationship to one another before comparison */
					if( range_start < range_end )
					{
						if( *t >= range_start && *t <= range_end )
						{
							member_match = true;
							loop = false;
						}
					}
					else
					{
						if( *t >= range_end && *t <= range_start )
						{
							member_match = true;
							loop = false;
						}
					}
				}

				/* if there was a match in an exclusion set then no match */
				/* if there was no match in a member set then no match */
				if(( invert && member_match ) || ! ( invert || member_match )) return MATCH_RANGE;

				/* if this is not an exclusion then skip the rest of the [...]
				 * construct that already matched. */
				if( member_match )
				{
					while( *p != ']' )
					{
						/* bad pattern (Missing ']') */
						if( ! *p ) return MATCH_PATTERN;

						/* skip exact match */
						if( *p == '\\' )
						{
							p++;

							/* if end of text then we have a bad pattern */
							if( ! *p ) return MATCH_PATTERN;
						}

						/* move to next pattern char */
						p++;
					}
				}
				break;
			case '\\':	/* next character is quoted and must match exactly */
				/* move pattern pointer to quoted char and fall through */
				p++;

				/* if end of text then we have a bad pattern */
				if( ! *p ) return MATCH_PATTERN;

				/* must match this character exactly */
			default:
				if( *p != *t ) return MATCH_LITERAL;
		}
	}
	/* if end of text not reached then the pattern fails */

	if( *t ) return MATCH_END;
	else return MATCH_VALID;
} /* Matche */


static int
Matche_After_Star( const char *p, const char *t )
{
	register int nextp, match = 0;

	/* pass over existing ? and * in pattern */
	while( *p == '?' || *p == '*' )
	{
		/* take one char for each ? and + */
		if (*p == '?')
		{
			/* if end of text then no match */
			if( ! *t++ ) return MATCH_ABORT;
		}

		/* move to next char in pattern */
		p++;
	}

	/* if end of pattern we have matched regardless of text left */
	if( ! *p ) return MATCH_VALID;

	/* get the next character to match which must be a literal or '[' */
	nextp = *p;
	if( nextp == '\\' )
	{
		nextp = p[1];

		/* if end of text then we have a bad pattern */
		if( ! nextp ) return MATCH_PATTERN;
	}

	/* Continue until we run out of text or definite result seen */
	do
	{
		/* a precondition for matching is that the next character
		 * in the pattern match the next character in the text or that
		 * the next pattern char is the beginning of a range.  Increment
		 * text pointer as we go here */
		if( nextp == *t || nextp == '[' ) match = Matche( p, t );

		/* if the end of text is reached then no match */
		if( ! *t++ ) match = MATCH_ABORT;
	} while( match != MATCH_VALID && match != MATCH_ABORT && match != MATCH_PATTERN );

	/* return result */
	return match;
} /* Matche_After_Star */


/* -eof- */
