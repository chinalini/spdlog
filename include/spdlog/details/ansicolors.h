// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/common.h>

#include <spdlog/details/log_msg.h>

#include <array>
#include <vector>

namespace spdlog {
namespace details {

class SPDLOG_API ansicolors {
public:
    explicit ansicolors();
    ansicolors(const ansicolors&) = delete;
    ansicolors& operator=(const ansicolors&) = delete;

    void set_color(level::level_enum color_level, string_view_t color);

    std::vector<string_view_t> ranges(const details::log_msg& msg,
                                      const memory_buf_t& formatted_msg) const;

    // Formatting codes
    static constexpr const char* reset = "\033[m";
    static constexpr const char* bold = "\033[1m";
    static constexpr const char* dark = "\033[2m";
    static constexpr const char* underline = "\033[4m";
    static constexpr const char* blink = "\033[5m";
    static constexpr const char* reverse = "\033[7m";
    static constexpr const char* concealed = "\033[8m";
    static constexpr const char* clear_line = "\033[K";

    // Foreground colors
    static constexpr const char* black = "\033[30m";
    static constexpr const char* red = "\033[31m";
    static constexpr const char* green = "\033[32m";
    static constexpr const char* yellow = "\033[33m";
    static constexpr const char* blue = "\033[34m";
    static constexpr const char* magenta = "\033[35m";
    static constexpr const char* cyan = "\033[36m";
    static constexpr const char* white = "\033[37m";

    /// Background colors
    static constexpr const char* on_black = "\033[40m";
    static constexpr const char* on_red = "\033[41m";
    static constexpr const char* on_green = "\033[42m";
    static constexpr const char* on_yellow = "\033[43m";
    static constexpr const char* on_blue = "\033[44m";
    static constexpr const char* on_magenta = "\033[45m";
    static constexpr const char* on_cyan = "\033[46m";
    static constexpr const char* on_white = "\033[47m";

    /// Bold colors
    static constexpr const char* yellow_bold = "\033[33m\033[1m";
    static constexpr const char* red_bold = "\033[31m\033[1m";
    static constexpr const char* bold_on_red = "\033[1m\033[41m";

private:
    std::array<std::string, level::n_levels> colors_;
    static std::string to_string_(const string_view_t& sv);
};

}  // namespace details
}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
    #include "ansicolors-inl.h"
#endif
