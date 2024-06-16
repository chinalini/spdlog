// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#ifndef SPDLOG_HEADER_ONLY
    #include <spdlog/sinks/ansicolor_rotating_file_sink.h>
#endif

#include <spdlog/common.h>

#include <spdlog/details/rotating_file.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/fmt/fmt.h>

#include <mutex>

namespace spdlog {
namespace sinks {

template <typename Mutex>
SPDLOG_INLINE ansicolor_rotating_file_sink<Mutex>::ansicolor_rotating_file_sink(
    filename_t base_filename,
    std::size_t max_size,
    std::size_t max_files,
    bool rotate_on_open,
    const file_event_handlers &event_handlers)
    : file_{base_filename, max_size, max_files, rotate_on_open, event_handlers} {}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_rotating_file_sink<Mutex>::set_color(level::level_enum color_level,
                                                                  string_view_t color) {
    std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
    colors_.set_color(color_level, color);
}

// calc filename according to index and file extension if exists.
// e.g. calc_filename("logs/mylog.txt, 3) => "logs/mylog.3.txt".
template <typename Mutex>
SPDLOG_INLINE filename_t
ansicolor_rotating_file_sink<Mutex>::calc_filename(const filename_t &filename, std::size_t index) {
    return details::rotating_file::calc_filename(filename, index);
}

template <typename Mutex>
SPDLOG_INLINE filename_t ansicolor_rotating_file_sink<Mutex>::filename() {
    return file_.filename();
}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_rotating_file_sink<Mutex>::sink_it_(const details::log_msg &msg) {
    msg.color_range_start = 0;
    msg.color_range_end = 0;
    memory_buf_t formatted;
    base_sink<Mutex>::formatter_->format(msg, formatted);
    for (const auto &range : colors_.ranges(msg, formatted)) {
        file_.write(range);
    }
}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_rotating_file_sink<Mutex>::flush_() {
    file_.flush();
}

}  // namespace sinks
}  // namespace spdlog
