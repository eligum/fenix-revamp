#include <fenix/utils/random.hh>
#include <fenix/utils/timer.hh>
#include <fenix/core/log.hh>

using u32 = fenix::u32;

int main()
{
    auto timer = fenix::Timer{};
    u32 amount = 20;

    fenix::Log::Init();
    fenix::Random::Init();

    for (u32 i = 0; i < amount; ++i)
    {
        u32 value = fenix::Random::GenerateUInt(1, 100);
        LOG_TRACE("{}", value);
    }

    LOG_INFO("Generated {} random numbers in range [1, 100]", amount);
    LOG_INFO("main() body took: {} s", timer.Elapsed());
}
