/****************************************************************************/
/*  frexp  v2.54                                                            */
/*  Copyright (c) 1995-2004 Texas Instruments Incorporated                  */
/****************************************************************************/
#include <math.h>

double frexp(double value, int *exp)
{
     unsigned long *ptr = (unsigned long *)&value;

     /***********************************************************************/
     /* IF THE VALUE IS ZERO, THE RESULTS ARE ZERO                          */ 
     /***********************************************************************/ 
     if (!*ptr) { *exp = 0; return value; }

     /***********************************************************************/
     /* EXTRACT THE EXPONENT, SUBTRACT THE BIAS, THEN ADD 1                 */
     /***********************************************************************/
     *exp = ((*ptr >> 20) & 0x7FF) - 0x3FE;

     /***********************************************************************/
     /* SET THE EXPONENT OF THE FRACTION TO -1                              */
     /***********************************************************************/
     *ptr = (*ptr & 0x800FFFFF) | (0x3FE << 20);

     return (value);
}