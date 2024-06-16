// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#ifndef SPDLOG_HEADER_ONLY
    #include <spdlog/details/ansicolors.h>
#endif

namespace spdlog {
namespace details {

SPDLOG_INLINE ansicolors::ansicolors() {
    colors_.at(level::trace) = to_string_(white);
    colors_.at(level::debug) = to_string_(cyan);
    colors_.at(level::info) = to_string_(green);
    colors_.at(level::warn) = to_string_(yellow_bold);
    colors_.at(level::err) = to_string_(red_bold);
    colors_.at(level::critical) = to_string_(bold_on_red);
    colors_.at(level::off) = to_string_(reset);
}

SPDLOG_INLINE void ansicolors::set_color(level::level_enum color_level, string_view_t color) {
    colors_.at(static_cast<size_t>(color_level)) = to_string_(color);
}

SPDLOG_INLINE std::vector<string_view_t> ansicolors::ranges(
    const details::log_msg &msg, const memory_buf_t &formatted_msg) const {
    std::vector<string_view_t> result{};
    if (msg.color_range_end > msg.color_range_start) {
        // before color range
        if (msg.color_range_start > 0) {
            result.push_back(string_view_t{formatted_msg.data(), msg.color_range_start});
        }
        // in color range
        result.push_back(string_view_t{colors_.at(static_cast<size_t>(msg.level))});
        result.push_back(string_view_t{formatted_msg.data() + msg.color_range_start,
                                       msg.color_range_end - msg.color_range_start});
        result.push_back(reset);
        // after color range
        if (msg.color_range_end < formatted_msg.size()) {
            result.push_back(string_view_t{formatted_msg.data() + msg.color_range_end,
                                           formatted_msg.size() - msg.color_range_end});
        }
    } else  // no color
    {
        result.push_back(string_view_t{formatted_msg.data(), formatted_msg.size()});
    }
    return result;
}

SPDLOG_INLINE std::string ansicolors::to_string_(const string_view_t &sv) {
    return std::string(sv.data(), sv.size());
}

}  // namespace details
}  // namespace spdlog
