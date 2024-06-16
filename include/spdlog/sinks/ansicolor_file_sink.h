// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/details/ansicolors.h>
#include <spdlog/details/file_helper.h>
#include <spdlog/details/null_mutex.h>
#include <spdlog/details/synchronous_factory.h>
#include <spdlog/sinks/base_sink.h>

#include <mutex>
#include <string>

namespace spdlog {
namespace sinks {
/*
 * ANSI-colored file sink with single file as target
 */
template <typename Mutex>
class ansicolor_file_sink final : public base_sink<Mutex> {
public:
    explicit ansicolor_file_sink(const filename_t &filename,
                                 bool truncate = false,
                                 const file_event_handlers &event_handlers = {});
    void set_color(level::level_enum color_level, string_view_t color);
    const filename_t &filename() const;

protected:
    void sink_it_(const details::log_msg &msg) override;
    void flush_() override;

private:
    details::ansicolors colors_;
    details::file_helper file_helper_;
};

using ansicolor_file_sink_mt = ansicolor_file_sink<std::mutex>;
using ansicolor_file_sink_st = ansicolor_file_sink<details::null_mutex>;

}  // namespace sinks

//
// factory functions
//
template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> ansicolor_logger_mt(const std::string &logger_name,
                                                   const filename_t &filename,
                                                   bool truncate = false,
                                                   const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::ansicolor_file_sink_mt>(logger_name, filename, truncate,
                                                                   event_handlers);
}

template <typename Factory = spdlog::synchronous_factory>
inline std::shared_ptr<logger> ansicolor_logger_st(const std::string &logger_name,
                                                   const filename_t &filename,
                                                   bool truncate = false,
                                                   const file_event_handlers &event_handlers = {}) {
    return Factory::template create<sinks::ansicolor_file_sink_st>(logger_name, filename, truncate,
                                                                   event_handlers);
}

}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
    #include "ansicolor_file_sink-inl.h"
#endif
