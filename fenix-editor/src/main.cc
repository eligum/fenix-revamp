#include <fenix/utils/random.hh>
#include <fenix/utils/timer.hh>
#include <spdlog/spdlog.h>

using u32 = fenix::u32;

int main()
{
    auto timer = fenix::Timer{};
    u32 amount = 20;

    spdlog::set_level(spdlog::level::trace);

    fenix::Random::Init();

    for (u32 i = 0; i < amount; ++i)
    {
        u32 value = fenix::Random::GenerateUInt(1, 100);
        spdlog::trace("{}", value);
    }

    spdlog::info("Generated {} random numbers in range [1, 100]", amount);
    spdlog::info("main() body took: {} s\n", timer.Elapsed());

    spdlog::info("Support for floats {:04.2f}", 1.23456);
}
