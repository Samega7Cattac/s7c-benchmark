#include "s7c_benchmark.hpp"
#include <iostream>

Timer::Timer(const std::string& name, bool enable) : name(name), running(enable)
{
#ifdef S7C_BECNHMARK_DEBUG
    debug("created");
#endif
    if (running) start();
}

double Timer::tick()
{
    if (!running)
    {
#ifdef S7C_BECNHMARK_DEBUG
        debug("tried to tick while stop!");
#endif
        return 0;
    }
    double value = std::chrono::duration<double, std::nano>(std::chrono::high_resolution_clock::now() - startT).count();
    sumT += value;
#ifdef S7C_BECNHMARK_DEBUG
    debug("tick");
#endif
    return value;
}

void Timer::start(bool do_reset)
{
#ifdef S7C_BECNHMARK_DEBUG
    debug("start");
#endif
    if (do_reset) reset();
    running = true;
    startT = std::chrono::high_resolution_clock::now();
}

void Timer::stop()
{
    if (!running) return;
    tick();
    running = false;
#ifdef S7C_BECNHMARK_DEBUG
    debug("stop");
#endif
}

void Timer::reset()
{
    sumT = 0.0;
    start();
#ifdef S7C_BECNHMARK_DEBUG
    debug("reset");
#endif
}

void Timer::print()
{
    tick();
    std::cout << std::fixed << name << " in " << sumT << std::endl;
}

void Timer::print_tick()
{
    std::cout << std::fixed << name << " ticked in " << tick() << std::endl;
}

double Timer::get_sum() const { return sumT; }
std::string Timer::get_name() const { return name; }

#ifdef S7C_BECNHMARK_DEBUG
void Timer::debug(std::string msg)
{
    std::cout << "[" << name << "] " << msg << std::endl;
}
#endif

Timer::~Timer()
{
    if (running) stop();
}
