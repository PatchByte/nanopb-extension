#include "main.h"
#include "tests.h"

#include <iostream>
#include <unordered_map>

#include <test.pb.h>

static std::unordered_map<std::string, tests::TestFunction_t*> smTestFunctionMap = 
{
    { "TestStringNormal", tests::TestStringNormal },
    { "TestSubmessageNormal", tests::TestSubmessageNormal },
    { "TestRepeatedNormal", tests::TestRepeatedNormal },
    { "TestBytesNormal", tests::TestBytesNormal }
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