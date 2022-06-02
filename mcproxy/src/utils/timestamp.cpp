
#include "include/utils/timestamp.hpp"

Timestamp Timestamp::get()
{
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    return Timestamp(ts);
}

Timestamp& Timestamp::operator+= (const Timestamp& rhs)
{
    m_ts.tv_sec += rhs.m_ts.tv_sec;
    m_ts.tv_nsec += rhs.m_ts.tv_nsec;
    if (m_ts.tv_nsec >= 1000000000) {
        m_ts.tv_nsec -= 1000000000;
        m_ts.tv_sec++;
    }
    return *this;
}

Timestamp operator+ (const Timestamp& lhs, const Timestamp& rhs)
{
    Timestamp ret = lhs;
    ret += rhs;
    return ret;
}

Timestamp& Timestamp::operator-= (const Timestamp& rhs)
{
    if (m_ts.tv_nsec < rhs.m_ts.tv_nsec) {
        m_ts.tv_nsec += 1000000000;
        m_ts.tv_sec--;
    }
    m_ts.tv_sec -= rhs.m_ts.tv_sec;
    m_ts.tv_nsec -= rhs.m_ts.tv_nsec;
    return *this;
}

Timestamp operator- (const Timestamp& lhs, const Timestamp& rhs)
{
    Timestamp ret = lhs;
    ret -= rhs;
    return ret;
}

