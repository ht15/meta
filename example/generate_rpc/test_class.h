#pragma once
#include <core/serialize/decode.h>
#include <iostream>
#include <iomanip>
#include <core/macro.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
namespace test
{
class Meta(rpc) test_class
{
    Meta(rpc) void rpc_1(int a, std::string b)
    {
        return;
    }
    Meta(rpc) int rpc_2()
    {
        return 1;
    }
};
}
