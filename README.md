**add_chains** is a simple CLI program that outputs formatted additive chains using Brouwer's and Yao's algorithms.

For compiling the program, use
```make```
For cleaning up the binaries, use
```make clean```

Program should compile under most POSIX-compatible environments.

You can get help by simply running the program without arguments:
```
Usage: ./add_chains [options] <list of numbers>
Options:
-b              use Brouwer's algorithm (default)
-y              use Yao's algorithm
-k <value>      uses k in algorithms (default is optimal)
-f <format>     sets output formatting:
                %s - additive chain separated by spaces
                %c - additive chain separated by commas
                %n - length of additive chain
                %k - value of k
```
