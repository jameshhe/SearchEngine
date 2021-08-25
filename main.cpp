#define CATCH_CONFIG_RUNNER
#include "parser.h"
#include "catch.hpp"
#include "userinterface.h"

using namespace std;

int runCatchTests(int argc,char *argv[]){
    return Catch::Session().run();
}




int main(int argc, char *argv[])
{

    if(argc == 1){
        return runCatchTests(argc, argv);
    }

    UserInterface ui(argv[1]);

    return 0;
}
