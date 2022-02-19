#include "pcap_parser.h"

#include <benchmark/benchmark.h>

static void BM_SomeFunction(benchmark::State& state) {
    // Perform setup here
    for (auto _ : state) {
        const std::string in = "../../sample.pcap";
        const std::string out = "../../decoded.txt";
        int bound = 1000;

        PcapParser parser(in, out, bound);
        parser.parse();
    }
}

// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);

BENCHMARK_MAIN();
