#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include <sys/time.h>
#include <map>
#include <sys/resource.h>
#include <fcntl.h>    /* For O_RDWR */
using namespace std;

#define LINE_LENGTH 100 // Max # of characters in an input line
#define MAX_ARGS 7 // Max number of arguments to a command
#define MAX_LENGTH 20 // Max # of characters in an argument
#define MAX_PT_ENTRIES 32 // Max entries in the Process Table