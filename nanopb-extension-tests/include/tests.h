#pragma once

namespace tests
{

    typedef bool(TestFunction_t)();

    TestFunction_t TestStringNormal;
    TestFunction_t TestSubmessageNormal;
    TestFunction_t TestRepeatedNormal;
    TestFunction_t TestRepeatedCustom;
    TestFunction_t TestBytesNormal;

}