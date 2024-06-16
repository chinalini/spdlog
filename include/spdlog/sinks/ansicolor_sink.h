// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <array>
#include <memory>
#include <mutex>
#include <spdlog/details/ansicolors.h>
#include <spdlog/details/console_globals.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/sinks/sink.h>
#include <string>

namespace spdlog {
namespace sinks {

/**
 * This sink prefixes the output with an ANSI escape sequence color code
 * depending on the severity
 * of the message.
 * If no color terminal detected, omit the escape codes.
 */

template <typename ConsoleMutex>
class ansicolor_sink : public sink {
public:
    using mutex_t = typename ConsoleMutex::mutex_t;
    ansicolor_sink(FILE *target_file, color_mode mode);
    ~ansicolor_sink() override = default;

    ansicolor_sink(const ansicolor_sink &other) = delete;
    ansicolor_sink(ansicolor_sink &&other) = delete;

    ansicolor_sink &operator=(const ansicolor_sink &other) = delete;
    ansicolor_sink &operator=(ansicolor_sink &&other) = delete;

    void set_color(level::level_enum color_level, string_view_t color);
    void set_color_mode(color_mode mode);
    bool should_color();

    void log(const details::log_msg &msg) override;
    void flush() override;
    void set_pattern(const std::string &pattern) final;
    void set_formatter(std::unique_ptr<spdlog::formatter> sink_formatter) override;

    // Formatting codes
    const string_view_t reset = details::ansicolors::reset;
    const string_view_t bold = details::ansicolors::bold;
    const string_view_t dark = details::ansicolors::dark;
    const string_view_t underline = details::ansicolors::underline;
    const string_view_t blink = details::ansicolors::blink;
    const string_view_t reverse = details::ansicolors::reverse;
    const string_view_t concealed = details::ansicolors::concealed;
    const string_view_t clear_line = details::ansicolors::clear_line;

    // Foreground colors
    const string_view_t black = details::ansicolors::black;
    const string_view_t red = details::ansicolors::red;
    const string_view_t green = details::ansicolors::green;
    const string_view_t yellow = details::ansicolors::yellow;
    const string_view_t blue = details::ansicolors::blue;
    const string_view_t magenta = details::ansicolors::magenta;
    const string_view_t cyan = details::ansicolors::cyan;
    const string_view_t white = details::ansicolors::white;

    /// Background colors
    const string_view_t on_black = details::ansicolors::on_black;
    const string_view_t on_red = details::ansicolors::on_red;
    const string_view_t on_green = details::ansicolors::on_green;
    const string_view_t on_yellow = details::ansicolors::on_yellow;
    const string_view_t on_blue = details::ansicolors::on_blue;
    const string_view_t on_magenta = details::ansicolors::on_magenta;
    const string_view_t on_cyan = details::ansicolors::on_cyan;
    const string_view_t on_white = details::ansicolors::on_white;

    /// Bold colors
    const string_view_t yellow_bold = details::ansicolors::yellow_bold;
    const string_view_t red_bold = details::ansicolors::red_bold;
    const string_view_t bold_on_red = details::ansicolors::bold_on_red;

private:
    FILE *target_file_;
    mutex_t &mutex_;
    details::ansicolors colors_;
    bool should_do_colors_;
    std::unique_ptr<spdlog::formatter> formatter_;
    void print_view_(const string_view_t &sv);
    void print_range_(const memory_buf_t &formatted, size_t start, size_t end);
};

template <typename ConsoleMutex>
class ansicolor_stdout_sink : public ansicolor_sink<ConsoleMutex> {
public:
    explicit ansicolor_stdout_sink(color_mode mode = color_mode::automatic);
};

template <typename ConsoleMutex>
class ansicolor_stderr_sink : public ansicolor_sink<ConsoleMutex> {
public:
    explicit ansicolor_stderr_sink(color_mode mode = color_mode::automatic);
};

using ansicolor_stdout_sink_mt = ansicolor_stdout_sink<details::console_mutex>;
using ansicolor_stdout_sink_st = ansicolor_stdout_sink<details::console_nullmutex>;

using ansicolor_stderr_sink_mt = ansicolor_stderr_sink<details::console_mutex>;
using ansicolor_stderr_sink_st = ansicolor_stderr_sink<details::console_nullmutex>;

}  // namespace sinks
}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
    #include "ansicolor_sink-inl.h"
#endif
