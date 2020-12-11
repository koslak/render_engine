#include "benchmark/benchmark.h"
 
class Widget
{
public:
    Widget() = default;
 
    int get_value() const noexcept
    {
        // Really dummy code here:
        return 1500 * 400;
    }
};
 
static void BM_SomeFunction(benchmark::State& state)
{
    Widget object;
 
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        object.get_value();
    }
}
 
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
