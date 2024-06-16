// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#ifndef SPDLOG_HEADER_ONLY
    #include <spdlog/sinks/ansicolor_file_sink.h>
#endif

#include <spdlog/common.h>
#include <spdlog/details/os.h>

namespace spdlog {
namespace sinks {

template <typename Mutex>
SPDLOG_INLINE ansicolor_file_sink<Mutex>::ansicolor_file_sink(
    const filename_t &filename, bool truncate, const file_event_handlers &event_handlers)
    : file_helper_{event_handlers} {
    file_helper_.open(filename, truncate);
}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_file_sink<Mutex>::set_color(level::level_enum color_level,
                                                         string_view_t color) {
    std::lock_guard<Mutex> lock(base_sink<Mutex>::mutex_);
    colors_.set_color(color_level, color);
}

template <typename Mutex>
SPDLOG_INLINE const filename_t &ansicolor_file_sink<Mutex>::filename() const {
    return file_helper_.filename();
}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_file_sink<Mutex>::sink_it_(const details::log_msg &msg) {
    msg.color_range_start = 0;
    msg.color_range_end = 0;
    memory_buf_t formatted;
    base_sink<Mutex>::formatter_->format(msg, formatted);
    for (const auto &range : colors_.ranges(msg, formatted)) {
        file_helper_.write(range);
    }
}

template <typename Mutex>
SPDLOG_INLINE void ansicolor_file_sink<Mutex>::flush_() {
    file_helper_.flush();
}

}  // namespace sinks
}  // namespace spdlog
