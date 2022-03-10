#include "add_algos.h"

#include <stdlib.h>
#include "chain_utils.h"
#include "bin_utils.h"

unsigned find_optimal_k(unsigned n)
{
	return (bin_length(bin_length(n) - 1) - 1)
		- 2 * (bin_length(bin_length(bin_length(n) - 1) - 1) - 1);
}


unsigned* brouwer(unsigned n, unsigned k)
{
	unsigned d = upow(2, k);

	// converting n to the base of 2^k
	unsigned* rarr = malloc(sizeof(unsigned));
	unsigned q = n / d; rarr[0] = n % d;
	unsigned sqmax = rarr[0]; // maximum number b in starting sequence 1, ..., b
	size_t qln; for(qln = 1; q >= d; ++qln)
	{
		rarr = realloc(rarr, sizeof(unsigned) * (qln + 1));
		rarr[qln] = q % d;
		q /= d;
		if(rarr[qln] > sqmax)
			sqmax = rarr[qln];
	}
	sqmax = q > sqmax ? q : sqmax;

	size_t chain_ln = sqmax + (qln * (k + 1)) + 1;
	unsigned* chain = malloc(sizeof(unsigned) * chain_ln);

	// starting sequence 1, 2, 3, ..., 2^k - 1
	for(unsigned i = 1; i <= sqmax; ++i)
		chain[i-1] = i;

	// adding digits of base 2^k
	chain[sqmax - 1] = q;
	for(size_t i = 0; i < qln; ++i)
	{ // i - digit number
		for(size_t j = 0; j < k; ++j)
		{ // j - doubling number
			chain[sqmax + i * (k + 1) + j] = chain[sqmax + i * (k + 1) + j - 1] * 2;
		}
		chain[sqmax + i * (k + 1) + k] = chain[sqmax + i * (k + 1) + k - 1] + rarr[qln - 1 - i];
	}
	chain[sqmax - 1] = sqmax;

	chain[chain_ln - 1] = 0;
	free(rarr);
	return optimize_chain(chain);
}

unsigned* yao(unsigned n, unsigned k)
{
	unsigned d = upow(2, k);
	unsigned bl = bin_length(n);

	// [jk <= lambda(n) <= (j+1)k] => ceil(lambda / k) = j
	// base chain is 1, ..., 2^jk
	size_t chain_ln = ( (bl - 1) / k + !!((bl - 1) % k) ) * k;
	unsigned* chain = malloc(sizeof(unsigned) * chain_ln);

	for(size_t i = 0; i < chain_ln; ++i)
		chain[i] = upow(2, i);

	// converting n to the base of 2^k
	unsigned* rarr = malloc(sizeof(unsigned));
	unsigned q = n / d; rarr[0] = n % d;
	size_t qln; for(qln = 1; q >= d; ++qln)
	{
		rarr = realloc(rarr, sizeof(unsigned) * (qln + 1));
		rarr[qln] = q % d;
		q /= d;
	}

	++qln;
	rarr = realloc(rarr, sizeof(unsigned) * qln);
	rarr[qln - 1] = q;

	unsigned n_brk = 0; // n broken down to sum of z*d(z) from z = 1 to 2^k - 1
	for(unsigned z = 1; z <= d - 1; ++z){
		unsigned dz = 0;
		for(size_t i = 0; i < qln; ++i)
			if(rarr[i] == z)
				dz += upow(d, i);
		if(dz == 0)
			continue;

		// creating a chain for d(z)
		unsigned* dz_chain = brouwer(dz, 1);
		size_t prev_chain_ln = chain_ln;
		chain_ln += chain_length(dz_chain);
		chain = realloc(chain, sizeof(unsigned) * chain_ln);
		for(size_t i = 0; dz_chain[i]; ++i)
			chain[prev_chain_ln + i] = dz_chain[i];

		// creating a chain for z * d(z)
		unsigned* zdz_chain = brouwer(z, 1);

		prev_chain_ln = chain_ln;
		chain_ln += chain_length(zdz_chain) + 1;
		chain = realloc(chain, sizeof(unsigned) * chain_ln);
		for(size_t i = 0; zdz_chain[i]; ++i)
			chain[prev_chain_ln + i] = zdz_chain[i] * dz;
		n_brk += z * dz;
		chain[chain_ln - 1] = n_brk;

		free(dz_chain); free(zdz_chain);
	}

	chain = realloc(chain, sizeof(unsigned) * (chain_ln + 2));
	chain[chain_ln] = n;
	chain[chain_ln + 1] = 0;

	free(rarr);
	return sort_chain(optimize_chain(chain));
}
