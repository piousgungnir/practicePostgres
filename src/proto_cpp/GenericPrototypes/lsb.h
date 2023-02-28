/* 
 * File:   lsb.h
 *
 */

#ifndef LSB_H
#define	LSB_H


struct lsb
{
	explicit lsb(double w = 1)
		: weight(w)
	{}

	double weight;
};

	
#endif	/* LSB_H */

