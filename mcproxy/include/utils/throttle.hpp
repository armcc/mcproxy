#ifndef THROTTLE_HPP
#define THROTTLE_HPP

#include "include/utils/timestamp.hpp" 

class Throttle
{
private:
    const unsigned long m_pps;
    const unsigned long m_full;
    unsigned long m_reserve;
    Timestamp m_last_ts;
    const Timestamp m_hold_duration;
    Timestamp m_hold_expire;

public:
    Throttle(unsigned long pps = 0, Timestamp hold_duration = 0)
      : m_pps(pps)
      , m_full(pps * 1000)
      , m_reserve(pps * 1000)
      , m_last_ts(0)
      , m_hold_duration(hold_duration)
      , m_hold_expire(0) {
    }

    bool get();
};

#endif // THROTTLE_HPP
