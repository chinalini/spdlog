// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/details/ansicolors.h>
#include <spdlog/details/rotating_file.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/synchronous_factory.h>
#include <spdlog/sinks/base_sink.h>

#include <chrono>
#include <mutex>
#include <string>

namespace spdlog {
namespace sinks {

//
// Rotating file sink based on size, with ansi colors.
//
template <typename Mutex>
class ansicolor_rotating_file_sink final : public base_sink<Mutex> {
public:
    ansicolor_rotating_file_sink(filename_t base_filename,
                                 std::size_t max_size,
                                 std::size_t max_files,
                                 bool rotate_on_open = false,
                                 const file_event_handlers &event_handlers = {});

    void set_color(level::level_enum color_level, string_view_t color);

    static filename_t calc_filename(const filename_t &filename, std::size_t index);
    filename_t filename();

protected:
    void sink_it_(const details::log_msg &msg) override;
    void flush_() override;

private:
    details::ansicolors colors_;
    details::rotating_file file_;
};

using ansicolor_rotating_file_sink_mt = ansicolor_rotating_file_sink<std::mutex>;
using ansicolor_rotating_file_sink_st = ansicolor_rotating_file_sink<details::null_mutex>;

}  // namespace sinks

//
// factory functions
//

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> ansicolor_rotating_logger_mt(
    const std::string &logger_name,
    const filename_t &filename,
    size_t max_file_size,
    size_t max_files,
    bool rotate_on_open = false,
    const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::ansicolor_rotating_file_sink_mt>(
        logger_name, filename, max_file_size, max_files, rotate_on_open, event_handlers);
}

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> ansicolor_rotating_logger_st(
    const std::string &logger_name,
    const filename_t &filename,
    size_t max_file_size,
    size_t max_files,
    bool rotate_on_open = false,
    const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::ansicolor_rotating_file_sink_st>(
        logger_name, filename, max_file_size, max_files, rotate_on_open, event_handlers);
}
}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
    #include "ansicolor_rotating_file_sink-inl.h"
#endif
