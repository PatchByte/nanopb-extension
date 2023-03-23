#include "main.h"
#include "tests.h"

#include <iostream>
#include <map>

#include <test.pb.h>

static std::map<std::string, tests::TestFunction_t*> smTestFunctionMap = 
{
    { "TestStringNormal", tests::TestStringNormal }
};

int main()
{

    for(auto testFunctionIterator : smTestFunctionMap)
    {
        if(testFunctionIterator.second() == false)
        {
            std::cerr << "Test \"" << testFunctionIterator.first << "\" has failed." << std::endl;
            return -1;
        }
    }

    return 0;
}