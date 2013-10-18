/*
 * This file is part of mcproxy.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * written by Sebastian Woelke, in cooperation with:
 * INET group, Hamburg University of Applied Sciences,
 * Website: http://mcproxy.realmv6.org/
 */


#include "include/hamcast_logging.h"
#include "include/proxy/sender.hpp"
#include "include/proxy/message_format.hpp" //source
#include "include/proxy/timers_values.hpp"

#include <iostream>
sender::sender(int addr_family)
{
    HC_LOG_TRACE("");
    m_addr_family = addr_family;

    if (m_addr_family == AF_INET) {
        if (!m_sock.create_raw_ipv4_socket()) {
            throw "failed to create raw ipv4 socket";
        }
    } else if (m_addr_family == AF_INET6) {
        if (!m_sock.create_raw_ipv6_socket()) {
            throw "failed to create raw ipv6 socket";
        }
    } else {
        HC_LOG_ERROR("wrong addr_family: " << m_addr_family);
        throw "wrong addr_family";
    }

    if (!m_sock.set_loop_back(false)) {
        throw "failed to set loop back";
    }
}

bool sender::send_general_query(const timers_values& tv, group_mem_protocol gmp) const
{
    using namespace std;
    HC_LOG_TRACE("");

    cout << "!!--ACTION: send general query" << endl;
    cout << "max response time: " << tv.get_query_response_interval().count() << "msec" << endl;
    cout << "qrv: " << tv.get_robustness_variable() << endl;
    cout << "qqi: " << tv.get_query_interval().count() << "sec" << endl;
    cout << "protocol: " << get_group_mem_protocol_name(gmp) << endl;
    cout << endl;
    return true;
}

bool sender::send_mc_addr_specific_query(const timers_values& tv, const addr_storage& gaddr, bool s_flag, group_mem_protocol gmp) const
{
    using namespace std;
    HC_LOG_TRACE("");

    cout << "!!--ACTION: send multicast address specific query" << endl;
    cout << "max response time: " << tv.get_query_response_interval().count() << "msec" << endl;
    cout << "group address: " << gaddr << endl;
    cout << "s-flag: " << (s_flag? "true": "false") << endl;
    cout << "qrv: " << tv.get_robustness_variable() << endl;
    cout << "qqi: " << tv.get_query_interval().count() << "sec" << endl;
    cout << "protocol: " << get_group_mem_protocol_name(gmp) << endl;
    cout << endl;
    return true;
}

bool sender::send_mc_addr_and_src_specific_query(const timers_values& tv, const addr_storage& gaddr, source_list<source>& slist, group_mem_protocol gmp) const
{
    using namespace std;
    HC_LOG_TRACE("");
    source_list<source> list_lower;
    source_list<source> list_higher;
    for (auto & e : slist) {
        if (e.retransmission_count > 0) {
            e.retransmission_count--;

            if (e.shared_source_timer->is_remaining_time_greater_than(tv.get_last_listener_query_time())) {
                list_higher.insert(e);
            } else {
                list_lower.insert(e);
            }
        }
    }

    if (!list_lower.empty() || !list_higher.empty() ) {
        cout << "!!--ACTION: send one or two multicast address and source specific queries" << endl;
        cout << "max response time: " << tv.get_query_response_interval().count() << "msec" << endl;
        cout << "group address: " << gaddr << endl;
        cout << "qrv: " << tv.get_robustness_variable() << endl;
        cout << "qqi: " << tv.get_query_interval().count() << "sec" << endl;
        cout << "protocol: " << get_group_mem_protocol_name(gmp) << endl;
        cout << "source list with without S-flag: " << list_lower << endl;
        cout << "source list with with S-flag: " << list_higher << endl;
        cout << endl;
        return true;
    } else {
        return false;
    }
}

sender::~sender()
{
    HC_LOG_TRACE("");

}
