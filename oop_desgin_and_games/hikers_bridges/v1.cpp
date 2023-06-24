//#define WINDOWS_GETOPT 1

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#if defined(WINDOWS_GETOPT) // getopt()
#include  "getoptwin.h"
#else   // used for linux
#include <unistd.h>
#endif
#include "hikers.h"
#include <yaml-cpp/yaml.h>
#include "timer.h"

using namespace std;

#ifdef WINDOWS_GETOPT
// Forward decals for getopt()
extern char* optarg;
extern int optind, opterr, optopt;
#endif

/**
 * Use the following sites to validate YAML files:
 * http://yaml-online-parser.appspot.com/
 * http://www.yamllint.com/
 *
 */

int parseConfigYamlFile(const std::string fileName, std::vector<long double> &bridgesLengths,
                        std::vector<std::vector<long double>> &hikerSpeed) {
    YAML::Node map = YAML::LoadFile(fileName);
    const YAML::Node& bridges = map["hikerbridges"];
    
    for (YAML::const_iterator it = bridges.begin(); it != bridges.end(); ++it) {
        const YAML::Node& bridge = *it;
        long double length = bridge["bridge"]["length"].as<long double>();
        DBGVAR(cout, length);
        bridgesLengths.push_back(length);

        std::vector<long double> vec = bridge["bridge"]["hikers"].as<std::vector<long double>>();
        DBGVAR(cout, vec[0]);
        hikerSpeed.push_back(vec);
    }

    return 0;
}

void printUsage() {
    using namespace std;
    cerr << "Bridges and Hikers: Usage:"
         << " to parse and generate input YAML file\n"
         << "For each bridge we calculate minumum crossing time\n\n"
         << "At each each bridge we also show the total crossing time"
         << " so far.\n"
         << "All times are minutes and all lengths are in feet\n\n";

    cerr << "./v1 <input file name (YAML format)>\n"
         << "Example to parse file:\n\n"
         << "\t\t./v1 myFirst.yaml\n\n";


    cerr << "./v1 -b <n# bridges> -k <max n# hikers> -l <max bridge len>\n"
         << " -s <max hikers speed> -f <file name> -r <rand seed>, \n"
         << " to generate a YAML file with these properties"
         << " (using random numbers)\n";
    cerr << "Example to generate file:\n\n"
         << "\t\t./v1 -b 5 -k 4 -l 500 -s 26 -f myFirst.yaml -r 12345\n\n";

    
    
    cerr << "Example to get help:\n\n";
    cerr << "\t\t./v1 or ./v1 -help or ./v1 -h --> to generate this help message\n\n";
    cerr << "Note: At the end of each run we print how much time used to run the program\n\n";
    return;
}

int main(int argc, char* argv[]) {
    // set the time to measure the performance between two solutiuons
    Timer tmr;
   
    std::string fileName = "config1.yaml";  // default infile
    
    // handle the command arguments
    if (argc < 2) {
        // Tell the user how to run the program
        printUsage();
        return 1;
    }
    // if we are going parse YAML file
    if (argc == 2) {
        std::string args = argv[1];
        if ((args == "-h") || (args == "-help")) {
            printUsage();
            return 0;
        }

        fileName = std::string(argv[1]);
        DBGVAR(cout, fileName);
        
        // vector of vectors for hikers speeds
        std::vector<std::vector<long double>> hSpeeds;
        // vector to keep the length of each bridge
        std::vector<long double> bridgesLengths;

        // Parse the YAML file:
        parseConfigYamlFile(fileName, bridgesLengths, hSpeeds);

        // Let's evaulate the bridges
        std::vector<long double> tmp = hSpeeds[0];
        Bridge B(hSpeeds, bridgesLengths);

    } else { // argc > 2 
        int opt = 0;
        // This is the default output file name
        std::string outFileName = "ConfigDef.yaml";
        // define a default config file parameters
        configGenFile cfg;
        cfg.fileName = outFileName;
        // procss the command line:
        while ((opt = getopt(argc, argv, ":b:k:l:s:f:h")) != -1) {
            switch (opt) {
            case 'b':
                cfg.numBridges = atoi(optarg);
                break;

            case 'k':
                cfg.maxNumHikers = atoi(optarg);
                break;

            case 'l':
                cfg.maxBridgeLen = atoi(optarg);
                break;

            case 's':
                cfg.maxHikerSpeed = atoi(optarg);
                break;

            case 'f':
                outFileName = std::string(optarg);
                break;

            case 'h':
                printUsage();
                return 1;

            default:
            case '?':
                std::cerr << "Unknown option: "<< optopt << std::endl;
                break;
            }
        }
        // put the file name
        cfg.fileName = std::move(outFileName);
        DBGVAR(std::cout, cfg.fileName);

        // create the file:
        if (generateYamlFile(cfg))
            cout << "Sucess, we created file: " << cfg.fileName << endl;
        else
            cout << "Error, could not created file: " << cfg.fileName << endl;
    }
    
    return 0;
}
