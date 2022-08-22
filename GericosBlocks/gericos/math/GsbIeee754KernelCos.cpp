/*************************************************************************

 Copyright (c) 2022 by CNRS/LESIA

 This software is copyrighted by and is the sole property of CNRS/LESIA.
 All rights, title, ownership, or other interests in the software remain
 the property of CNRS/LESIA.
 This software may only be used in accordance with the corresponding
 license agreement. Any unauthorized use, duplication, transmission,
 distribution, or disclosure of this software is expressly forbidden.

 This Copyright notice may not be removed or modified without prior
 written consent of CNRS/LESIA.


 LESIA
 Observatoire de Meudon
 5 place Jules Janssen
 92195 Meudon
 http://www.lesia.obspm.fr/-Logiciels-embarques-.html
 *************************************************************************/

/************************************************************************

 GericosBlocks Component

 GsbIeee754KernelCos.cpp

 $Rev: 000 $

 ************************************************************************/

/************************************************************************
 *
 *  @file GsbIeee754KernelCos.cpp
 *  @brief Implementation of the GsbIeee754::kernelCos() function
 *
 *  The algorithm is based on the one developed by Sun Microsystems and re-used in the Basic Mathematical Library for Flight Software (E1356-MLFS).
 *  The code has been reworked in order to follow LESIA's Flight Software C++ Coding Standards.
 *
 *  Changes :
 * 		Variable names change.
 ************************************************************************/

/*
 * ====================================================
 * Copyright (C) 1993 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunPro, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <gericos/blocks/math/GsbIeee754.hpp>

/*
 * kernelCos(x, xTail)
 * kernel cos function on [-pi/4, pi/4], pi/4 ~ 0.785398164
 * Input x is assumed to be bounded by ~pi/4 in magnitude.
 * Input xTail is the tail of x.
 *
 * Algorithm
 *	1. Since cos(-x) = cos(x), we need only to consider positive x.
 *	2. if x < 2^-27 (hx < 0x3E400000), return 1 with inexact if x != 0.
 *	3. cos(x) is approximated by a polynomial of degree 14 on
 *	   [0,pi/4]
 *	   	cos(x) ~ 1 - x*x/2 + C1*x^4 + ... + C6*x^14
 *	   where the Remez error is :
 * 	|cos(x) - (1 - 0.5*x^2 + C1*x^4 + C2*x^6 + C3*x^8 + C4*x^10 + C5*x^12 + C6*x^14)| <= 2^-58
 *
 *	4. let remez = C1*x^4 + C2*x^6 + C3*x^8 + C4*x^10 + C5*x^12 + C6*x^14, then
 *	       cos(x) ~ 1 - x*x/2 + remez
 *	   since cos(x+xTail) ~ cos(x) - sin(x)*xTail
 *			          ~ cos(x) - x*xTail,
 *	   a correction term is necessary in cos(x) and hence
 *		cos(x+xTail) = 1 - ( x*x/2 - (remez - x*xTail))
 *	   For better accuracy, rearrange to
 *	       cos(x+xTail) ~ oneSubHalfTwo + (tmp + (remez - x*xTail))
 *	   where oneSubHalfTwo = 1 - x*x/2 and tmp is a tiny correction term
 *	   (1 - x*x/2 == oneSubHalfTwo + tmp exactly in infinite precision).
 *	   The exactness of oneSubHalfTwo + tmp in infinite precision depends on oneSubHalfTwo and tmp having the same precision as x.
 *     If they have extra precision due to compiler bugs, then the extra precision is only good provided it is retained in all terms of the final
 *	   expression for cos().  Retention happens in all cases tested under FreeBSD, so don't pessimize things by forcibly clipping any extra precision in oneSubHalfTwo.
 */
double GsbIeee754::kernelCos(double x, double xTail) {
	// Remez polynomial of degree 14
	static const double C1 = 4.16666666666666019037e-02;	 // 0x 3FA55555 5555554C
	static const double C2 = -1.38888888888741095749e-03;	 // 0x BF56C16C 16C15177
	static const double C3 = 2.48015872894767294178e-05;	 // 0x 3EFA01A0 19CB1590
	static const double C4 = -2.75573143513906633035e-07;	 // 0x BE927E4F 809C52AD
	static const double C5 = 2.08757232129817482790e-09;	 // 0x 3E21EE9E BDB4B1C4
	static const double C6 = -1.13596475577881948265e-11;	 // 0x BDA8FAE9 BE8838D4
	// 1.0
	static const double ONE = 1.0;
	// 0.5
	static const double HALF = 0.5;

	// xTwo = x*x
	double xTwo = x * x;
	// remez polynomial approximation : C1*x^4 + C2*x^6 + C3*x^8 + C4*x^10 + C5*x^12 + C6*x^14
	double remez = xTwo * (C1 + (xTwo * (C2 + (xTwo * (C3 + (xTwo * (C4 + (xTwo * (C5 + (xTwo * C6))))))))));
	// halfXTwo = (x*x)/2
	double halfXTwo = (HALF * xTwo);
	// oneSubHalfTwo = 1 - (x*x)/2
	double oneSubHalfTwo = (ONE - halfXTwo);

	return (oneSubHalfTwo + (((ONE - oneSubHalfTwo) - halfXTwo) + ((xTwo * remez) - (x * xTail))));
}
