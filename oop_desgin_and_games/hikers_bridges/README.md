# Hikers_Bridges
A simple problem to teach how to use YAML and performance measurement: you have groups of hikers going through multiple bridges through the night.
Each hiker has speed measured in feet/minute. Each bridge has length in feet. 

To describe the problem, we need to use YAML file.

We use https://github.com/jbeder/yaml-cpp/ to create a YAML file parser to describe the problem and then we test our solution.
The actual get the minimum cross time per bridge and total cross time for all bridges.

Note: 
	Please install yaml-cpp library to be able to compile this tool.

We have two ways to evaluate the solutions:
    1- Use priority queue to keep track of the fastest hiker.
    2- Just use STL std::max_element() to get the fastest hiker.

We used a generic Timer (included in the src) to measure the performance.
For the same file, we see that method #2 is better in terms of time (63 msec compared to 288 msec for the priority queue).

We also created a tool to generate these YAML files.

Here how to use it:

 - To generate a YAML file with these properties (using random numbers)
	./v1 -b <n# bridges> -k <max n# hikers> -l <max bridge len> -s <max hikers speed> -f <file name> -r <rand seed>,
 
Example to generate file:
	./v1 -b 5 -k 4 -l 500 -s 26 -f myFirst.yaml -r 12345

Example to parse file:
	./v1 myFirst.yaml

To get help: 
	./v1 or ./v1 -help or ./v1 -h --> to generate this help message

Note: At the end of each run we print how much time used to run the program
