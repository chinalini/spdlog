// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#include <spdlog/common.h>

#include <spdlog/details/file_helper.h>

#include <tuple>

namespace spdlog {
namespace details {

class SPDLOG_API rotating_file {
public:
    explicit rotating_file(filename_t base_filename,
                           std::size_t max_size,
                           std::size_t max_files,
                           bool rotate_on_open,
                           const file_event_handlers &event_handlers);
    rotating_file(const rotating_file &) = delete;
    rotating_file &operator=(const rotating_file &) = delete;

    static filename_t calc_filename(const filename_t &filename, std::size_t index);

    void write(const memory_buf_t &buf);
    void write(string_view_t buf);

    void flush();

    filename_t filename() const;

private:
    // Rotate files:
    // log.txt -> log.1.txt
    // log.1.txt -> log.2.txt
    // log.2.txt -> log.3.txt
    // log.3.txt -> delete
    void rotate_();

    // delete the target if exists, and rename the src file  to target
    // return true on success, false otherwise.
    bool rename_file_(const filename_t &src_filename, const filename_t &target_filename);

    filename_t base_filename_;
    std::size_t max_size_;
    std::size_t max_files_;
    std::size_t current_size_;
    file_helper file_helper_;
};

}  // namespace details
}  // namespace spdlog

#ifdef SPDLOG_HEADER_ONLY
    #include "rotating_file-inl.h"
#endif
