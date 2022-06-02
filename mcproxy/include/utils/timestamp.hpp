#ifndef TIMESTAMP_HPP
#define TIMESTAMP_HPP

#include <time.h>
#include <iostream>
#include <cstdio>

class Timestamp
{
private:
    struct timespec m_ts;

public:
    Timestamp(time_t secs = 0, long nsecs = 0) : m_ts({secs, nsecs}) {}
    Timestamp(const struct timespec& ts) : m_ts(ts) {}

    static Timestamp get();
    inline time_t get_secs() const { return m_ts.tv_sec; }
    inline long get_nsecs() const { return m_ts.tv_nsec; }

    Timestamp& operator+=(const Timestamp& rhs);
    Timestamp& operator-=(const Timestamp& rhs);

    friend std::ostream& operator<<(std::ostream& stream, const Timestamp& t);
    friend bool operator== (const Timestamp& lhs, const Timestamp& rhs);
    friend bool operator< (const Timestamp& lhs, const Timestamp& rhs);
};

Timestamp operator+ (const Timestamp& lhs, const Timestamp& rhs);
Timestamp operator- (const Timestamp& lhs, const Timestamp& rhs);

inline std::ostream& operator<<(std::ostream& stream, const Timestamp& t)
{
    char str[64];
    std::snprintf(str, sizeof(str), "%lu.%09lu", (unsigned long)t.m_ts.tv_sec, (unsigned long)t.m_ts.tv_nsec);
    return stream << str;
}

inline bool operator== (const Timestamp& lhs, const Timestamp& rhs)
{
    return (lhs.m_ts.tv_sec == rhs.m_ts.tv_sec) && (lhs.m_ts.tv_nsec == rhs.m_ts.tv_nsec);
}

inline bool operator< (const Timestamp& lhs, const Timestamp& rhs)
{
    if (lhs.m_ts.tv_sec < rhs.m_ts.tv_sec)
        return true;
    if (lhs.m_ts.tv_sec > rhs.m_ts.tv_sec)
        return false;
    return (lhs.m_ts.tv_nsec < rhs.m_ts.tv_nsec) ? true : false;
}

inline bool operator<= (const Timestamp& lhs, const Timestamp& rhs)
{
    return (lhs == rhs) || (lhs < rhs);
}

inline bool operator> (const Timestamp& lhs, const Timestamp& rhs)
{
    return rhs < lhs;
}

inline bool operator>= (const Timestamp& lhs, const Timestamp& rhs)
{
    return rhs <= lhs;
}

#endif // TIMESTAMP_HPP
