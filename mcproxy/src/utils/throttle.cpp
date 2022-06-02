
#include "include/utils/throttle.hpp"
#include "include/utils/timestamp.hpp"

bool Throttle::get()
{
    if (m_pps == 0 || m_hold_duration == 0)
        return false;

    Timestamp now = Timestamp::get();
    if (now < m_hold_expire)
        return true;

    Timestamp delta = now - m_last_ts;
    m_last_ts = now;

    unsigned long ticks = (delta.get_secs() > 0) ? 1000 : delta.get_nsecs() / 1000000;

    if (ticks < 1000) {
        m_reserve += ticks * m_pps;
        if (m_reserve > m_full)
            m_reserve = m_full;
    } else {
        m_reserve = m_full;
    }

    if (m_reserve >= 1000) {
        m_reserve -= 1000;
        return false;
    } else {
        m_hold_expire = now + m_hold_duration;
        return true;
    }
}
