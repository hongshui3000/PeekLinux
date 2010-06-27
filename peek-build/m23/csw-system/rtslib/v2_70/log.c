/****************************************************************************/
/*  log    v2.54                                                            */
/*  Copyright (c) 1995-2004 Texas Instruments Incorporated                  */
/****************************************************************************/
#include <math.h>
#include <values.h>
#include <errno.h>

/****************************************************************************/
/*  LOG() - natural log							    */
/*									    */
/*  Based on the algorithm from "Software Manual for the Elementary         */
/*  Functions", Cody and Waite, Prentice Hall 1980, chapter 5.              */
/*									    */
/*  N = exponent x							    */
/*  f = mantissa x, 0.5 <= f < 1					    */
/*  if f < sqrt(0.5), znum = f - 0.5, zden = znum * 0.5 + 0.5		    */
/*  if f > sqrt(0.5), znum = f - 1, zden = f * 0.5 + 0.5		    */
/*  z = znum / zden							    */
/*  w = z * z								    */
/*  R = polynomial expression						    */
/*									    */
/*  result = R + N * ln(2)						    */
/****************************************************************************/
double log(double x)
{
    double a, b, f, r, w, z, znum;
    int n;

    /************************************************************************/
    /* check for errors in domain and range                                 */
    /************************************************************************/
    if (x <= 0) { errno = (x == 0) ? ERANGE : EDOM; return (-HUGE_VAL); }

    /************************************************************************/
    /* f = mantissa(x), n = exponent(x)            			    */
    /************************************************************************/
    f = frexp(x, &n);

    /************************************************************************/
    /* for numbers <= sqrt(0.5)                                             */
    /************************************************************************/
    if (f <= SQRTHALF)
    {
	--n;
	znum = f - 0.5;
	z    = znum / (znum * 0.5 + 0.5);
    }

    /************************************************************************/
    /* for numbers > sqrt(0.5)                                              */
    /************************************************************************/
    else
    {
	znum = (f - 0.5) - 0.5;
	z    = znum / (f * 0.5 + 0.5);
    }

    /************************************************************************/
    /* determine polynomial expression                                      */
    /************************************************************************/
    w = z * z;

#if BITS<=24
    a = A0;
    b = w + B0;
#elif BITS>=25 && BITS<=32
    a = A1 * w + A0;
    b = w + B0;
#elif BITS>=33 && BITS<=48
    a = (A2 * w + A1) * w + A0;
    b = (w + B1) * w + B0;
#else
    a = (A2 * w + A1) * w + A0;
    b = ((w + B2) * w + B1) * w + B0;
#endif

    /************************************************************************/
    /* calculate the natural log of (mant x) / 2                            */
    /************************************************************************/
    r = z + z * w * (a / b);

    /************************************************************************/
    /* ln(x) = ln (mant x) + 2 * (exp x) (but more mathematically stable)   */
    /************************************************************************/
    return ((n * C4 + r) + n * C3);
}