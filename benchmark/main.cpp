#include "pcap_parser.h"

#include <benchmark/benchmark.h>

static void BM_SomeFunction(benchmark::State& state) {
    for (auto _ : state) {
        const std::string in = "../../sample.pcap";
        int bound = state.range(0);

        PcapParser parser(in, bound, OutputFromat::disabled);
        parser.parse();
    }
}

// Register the function as a benchmark
BENCHMARK(BM_SomeFunction)->Arg(1000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SomeFunction)->Arg(10000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SomeFunction)->Arg(100000)->Unit(benchmark::kMicrosecond);
BENCHMARK(BM_SomeFunction)->Arg(1000000)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
