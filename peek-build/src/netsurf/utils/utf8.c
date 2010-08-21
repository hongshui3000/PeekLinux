/*
 * Copyright 2005 John M Bell <jmb202@ecs.soton.ac.uk>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * UTF-8 manipulation functions (implementation).
 */

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <iconv.h>
#include "errno.h"
#include <parserutils/charset/utf8.h>

#include "utils/config.h"
#include "utils/log.h"
#include "utils/utf8.h"

static utf8_convert_ret utf8_convert(const char *string, size_t len,
		const char *from, const char *to, char **result);

/**
 * Convert a UTF-8 multibyte sequence into a single UCS4 character
 *
 * Encoding of UCS values outside the UTF-16 plane has been removed from
 * RFC3629. This function conforms to RFC2279, however.
 *
 * \param s  The sequence to process
 * \param l  Length of sequence
 * \return   UCS4 character
 */
uint32_t utf8_to_ucs4(const char *s_in, size_t l)
{
	uint32_t ucs4;
	size_t len;
	parserutils_error perror;

	perror = parserutils_charset_utf8_to_ucs4((const uint8_t *) s_in, l, 
			&ucs4, &len);
	if (perror != PARSERUTILS_OK)
		ucs4 = 0xfffd;

	return ucs4;
}

/**
 * Convert a single UCS4 character into a UTF-8 multibyte sequence
 *
 * Encoding of UCS values outside the UTF-16 plane has been removed from
 * RFC3629. This function conforms to RFC2279, however.
 *
 * \param c  The character to process (0 <= c <= 0x7FFFFFFF)
 * \param s  Pointer to 6 byte long output buffer
 * \return   Length of multibyte sequence
 */
size_t utf8_from_ucs4(uint32_t c, char *s)
{
	uint8_t *in = (uint8_t *) s;
	size_t len = 6;
	parserutils_error perror;

	perror = parserutils_charset_utf8_from_ucs4(c, &in, &len);
	if (perror != PARSERUTILS_OK) {
		s[0] = 0xef;
		s[1] = 0xbf;
		s[2] = 0xbd;
		return 3;
	}

	return 6 - len;
}

/**
 * Calculate the length (in characters) of a NULL-terminated UTF-8 string
 *
 * \param s  The string
 * \return   Length of string
 */
size_t utf8_length(const char *s)
{
	return utf8_bounded_length(s, strlen(s));
}

/**
 * Calculated the length (in characters) of a bounded UTF-8 string
 *
 * \param s  The string
 * \param l  Maximum length of input (in bytes)
 * \return Length of string, in characters
 */
size_t utf8_bounded_length(const char *s, size_t l)
{
	size_t len;
	parserutils_error perror;

	perror = parserutils_charset_utf8_length((const uint8_t *) s, l, &len);
	if (perror != PARSERUTILS_OK)
		return 0;

	return len;
}

/**
 * Calculate the length (in bytes) of a UTF-8 character
 *
 * \param s  Pointer to start of character
 * \return Length of character, in bytes
 */
size_t utf8_char_byte_length(const char *s)
{
	size_t len;
	parserutils_error perror;

	perror = parserutils_charset_utf8_char_byte_length((const uint8_t *) s,
			&len);
	assert(perror == PARSERUTILS_OK);

	return len;
}

/**
 * Find previous legal UTF-8 char in string
 *
 * \param s  The string
 * \param o  Offset in the string to start at
 * \return Offset of first byte of previous legal character
 */
size_t utf8_prev(const char *s, size_t o)
{
	uint32_t prev;
	parserutils_error perror;

	perror = parserutils_charset_utf8_prev((const uint8_t *) s, o, &prev);
	assert(perror == PARSERUTILS_OK);

	return prev;
}

/**
 * Find next legal UTF-8 char in string
 *
 * \param s  The string
 * \param l  Maximum offset in string
 * \param o  Offset in the string to start at
 * \return Offset of first byte of next legal character
 */
size_t utf8_next(const char *s, size_t l, size_t o)
{
	uint32_t next;
	parserutils_error perror;

	perror = parserutils_charset_utf8_next((const uint8_t *) s, l, o, 
			&next);
	assert(perror == PARSERUTILS_OK);

	return next;
}

/* Cache of previous iconv conversion descriptor used by utf8_convert */
static struct {
	char from[32];	/**< Encoding name to convert from */
	char to[32];	/**< Encoding name to convert to */
	iconv_t cd;	/**< Iconv conversion descriptor */
} last_cd;

/**
 * Finalise the UTF-8 library
 */
void utf8_finalise(void)
{
	if (last_cd.cd != 0)
		iconv_close(last_cd.cd);

	/* paranoia follows */
	last_cd.from[0] = '\0';
	last_cd.to[0] = '\0';
	last_cd.cd = 0;
}

/**
 * Convert a UTF8 string into the named encoding
 *
 * \param string  The NULL-terminated string to convert
 * \param encname The encoding name (suitable for passing to iconv)
 * \param len     Length of input string to consider (in bytes), or 0
 * \param result  Pointer to location to store result (allocated on heap)
 * \return Appropriate utf8_convert_ret value
 */
utf8_convert_ret utf8_to_enc(const char *string, const char *encname,
		size_t len, char **result)
{
	return utf8_convert(string, len, "UTF-8", encname, result);
}

/**
 * Convert a string in the named encoding into a UTF-8 string
 *
 * \param string  The NULL-terminated string to convert
 * \param encname The encoding name (suitable for passing to iconv)
 * \param len     Length of input string to consider (in bytes), or 0
 * \param result  Pointer to location to store result (allocated on heap)
 * \return Appropriate utf8_convert_ret value
 */
utf8_convert_ret utf8_from_enc(const char *string, const char *encname,
		size_t len, char **result)
{
	return utf8_convert(string, len, encname, "UTF-8", result);
}

/**
 * Convert a string from one encoding to another
 *
 * \param string  The NULL-terminated string to convert
 * \param len     Length of input string to consider (in bytes), or 0
 * \param from    The encoding name to convert from
 * \param to      The encoding name to convert to
 * \param result  Pointer to location in which to store result
 * \return Appropriate utf8_convert_ret value
 */
utf8_convert_ret utf8_convert(const char *string, size_t len,
		const char *from, const char *to, char **result)
{
	iconv_t cd;
	char *temp, *out, *in;
	size_t slen, rlen;

	assert(string && from && to && result);

	if (string[0] == '\0') {
		/* On AmigaOS, iconv() returns an error if we pass an 
		 * empty string.  This prevents iconv() being called as 
		 * there is no conversion necessary anyway. */
		*result = strdup("");
		if (!(*result)) {
			*result = NULL;
			return UTF8_CONVERT_NOMEM;
		}

		return UTF8_CONVERT_OK;
	}

	if (strcasecmp(from, to) == 0) {
		/* conversion from an encoding to itself == strdup */
		slen = len ? len : strlen(string);
		*(result) = strndup(string, slen);
		if (!(*result)) {
			*(result) = NULL;
			return UTF8_CONVERT_NOMEM;
		}

		return UTF8_CONVERT_OK;
	}

	in = (char *)string;

	/* we cache the last used conversion descriptor,
	 * so check if we're trying to use it here */
	if (strncasecmp(last_cd.from, from, sizeof(last_cd.from)) == 0 &&
			strncasecmp(last_cd.to, to, sizeof(last_cd.to)) == 0) {
		cd = last_cd.cd;
	}
	else {
		/* no match, so create a new cd */
		cd = iconv_open(to, from);
		if (cd == (iconv_t)-1) {
			if (errno == EINVAL)
				return UTF8_CONVERT_BADENC;
			/* default to no memory */
			return UTF8_CONVERT_NOMEM;
		}

		/* close the last cd - we don't care if this fails */
		if (last_cd.cd)
			iconv_close(last_cd.cd);

		/* and copy the to/from/cd data into last_cd */
		strncpy(last_cd.from, from, sizeof(last_cd.from));
		strncpy(last_cd.to, to, sizeof(last_cd.to));
		last_cd.cd = cd;
	}

	slen = len ? len : strlen(string);
	/* Worst case = ACSII -> UCS4, so allocate an output buffer
	 * 4 times larger than the input buffer, and add 4 bytes at
	 * the end for the NULL terminator
	 */
	rlen = slen * 4 + 4;

	temp = out = malloc(rlen);
	if (!out)
		return UTF8_CONVERT_NOMEM;

	/* perform conversion */
	if (iconv(cd, &in, &slen, &out, &rlen) == (size_t)-1) {
		free(temp);
		/* clear the cached conversion descriptor as it's invalid */
		if (last_cd.cd)
			iconv_close(last_cd.cd);
		last_cd.from[0] = '\0';
		last_cd.to[0] = '\0';
		last_cd.cd = 0;
		/** \todo handle the various cases properly
		 * There are 3 possible error cases:
		 * a) Insufficiently large output buffer
		 * b) Invalid input byte sequence
		 * c) Incomplete input sequence */
		return UTF8_CONVERT_NOMEM;
	}

	*(result) = realloc(temp, out - temp + 4);
	if (!(*result)) {
		free(temp);
		*(result) = NULL; /* for sanity's sake */
		return UTF8_CONVERT_NOMEM;
	}

	/* NULL terminate - needs 4 characters as we may have
	 * converted to UTF-32 */
	memset((*result) + (out - temp), 0, 4);

	return UTF8_CONVERT_OK;
}
