#include "gtest/gtest.h"
#include "benchmark/benchmark.h"
 
int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
 
    constexpr int tests_run_okay{ 0 };
    constexpr int tests_run_with_error{ 1 };
 
    if(RUN_ALL_TESTS() == tests_run_okay)
    {
        ::benchmark::Initialize(&argc, argv);
        if (::benchmark::ReportUnrecognizedArguments(argc, argv))
        {
            return tests_run_with_error;
        }
        ::benchmark::RunSpecifiedBenchmarks();
    }else{
        return tests_run_with_error;
    }
}
