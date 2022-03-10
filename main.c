#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "add_algos.h"
#include "chain_utils.h"

struct {
	enum {
		ALG_BROUWER,
		ALG_YAO
	} algorithm;
	const char* format;
	unsigned k;	// 0 means [use optimal]
} main_config;

void print_help(char* arg0)
{
	printf( "Usage: %s [options] <list of numbers>\n"
		"Options:\n"
		"-b\t\tuse Brouwer's algorithm (default)\n"
		"-y\t\tuse Yao's algorithm\n"
		"-k <value>\tuses k in algorithms (default is optimal)\n"
		"-f <format>\tsets output formatting:\n"
		"\t\t%%s - additive chain separated by spaces\n"
		"\t\t%%c - additive chain separated by commas\n"
		"\t\t%%n - length of additive chain\n"
		"\t\t%%k - value of k\n"
		, arg0);
}

void print_chain_format(unsigned* chain)
{
	size_t cln = chain_length(chain);
	unsigned k = main_config.k ? main_config.k : find_optimal_k(chain[cln - 1]);
	k = k ? k : 1;

	const char* f = main_config.format;
	for(; *f; ++f)
	{
		if(*f == '%'){
			++f;
			switch(*f){
				default:
					fprintf(stderr, "Unknown format specifier %%%c\n", *f);
					return;
				case 's':
					for(size_t i = 0; chain[i]; ++i)
						printf(chain[i + 1] ? "%u " : "%u", chain[i]);
					break;
				case 'c':
					for(size_t i = 0; chain[i]; ++i)
						printf(chain[i + 1] ? "%u, " : "%u", chain[i]);
					break;
				case 'n':
					printf("%lu", cln);
					break;
				case 'k':
					printf("%u", k);
					break;
			}
		}
		else if(*f == '\\'){
			const char conv_syms[] =  {'n', 't', 'b', 'v', '\0'};
			const char conv_ascii[] = {'\n', '\t', '\b', '\v'};
			++f;

			size_t i = 0; for(; conv_syms[i] && conv_syms[i] != *f; ++i)
				;
			if(!conv_syms[i]){
				fprintf(stderr, "Unsupported ASCII escape sequence \\%c", *f);
				return;
			}
			putchar(conv_ascii[i]);
		}
		else
			putchar(*f);
	}
}

int main(int argc, char** argv)
{
	if(argc == 1){
		print_help(argv[0]);
		return -1;
	}

	main_config.algorithm = ALG_BROUWER;
	main_config.format = "%n | %s\\n";
	main_config.k = 0;

	int arg;
	while( (arg = getopt(argc, argv, "byf:k:")) != -1)
	{
		switch(arg)
		{
			case 'b':
				main_config.algorithm = ALG_BROUWER;
				break;
			case 'y':
				main_config.algorithm = ALG_YAO;
				break;
			case 'f':
				main_config.format = optarg;
				break;
			case 'k':
			{
				long long kraw = strtoll(optarg, NULL, 10);
				if(kraw == 0){
					fprintf(stderr, "%lld is zero or invalid; not setting k\n", kraw);
					break;
				}
				else if(kraw < 0){
					fprintf(stderr, "%lld is not positive; not setting k\n", kraw);
					break;
				}
				else if(kraw > UINT_MAX){
					fprintf(stderr, "%lld will overflow unsigned int; not setting k\n", kraw);
					break;
				}
				main_config.k = (unsigned)kraw;
			} break;

			case '?':
				if(optopt == 'f') return -2; // f requires an argument
				else return -3; // unknown option
		}
	}

	for(int i = optind; i < argc; ++i)
	{
		long long nraw = strtoll(argv[i], NULL, 10);
		if(nraw > UINT_MAX){
			fprintf(stderr, "%lld will overflow unsigned int, skipping\n", nraw);
			continue;
		}
		else if (nraw == 0){
			fprintf(stderr, "%lld is zero or invalid, skipping\n", nraw);
		}

		unsigned n = (unsigned)nraw;
		unsigned* chain;
		unsigned k = main_config.k ? main_config.k : find_optimal_k(n);
		k = k ? k : 1;
		switch(main_config.algorithm){
			case ALG_BROUWER: chain = brouwer(n, k); break;
			case ALG_YAO: chain = yao(n, k); break;
		}

		print_chain_format(chain);
		free(chain);
	}
}
