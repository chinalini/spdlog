// Copyright(c) 2015-present, Gabi Melman & spdlog contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#pragma once

#ifndef SPDLOG_HEADER_ONLY
    #include <spdlog/details/rotating_file.h>
#endif

#include <spdlog/fmt/fmt.h>

#include <spdlog/details/file_helper.h>

namespace spdlog {
namespace details {

SPDLOG_INLINE rotating_file::rotating_file(filename_t base_filename,
                                           std::size_t max_size,
                                           std::size_t max_files,
                                           bool rotate_on_open,
                                           const file_event_handlers &event_handlers)
    : base_filename_(std::move(base_filename)),
      max_size_(max_size),
      max_files_(max_files),
      file_helper_{event_handlers} {
    if (max_size == 0) {
        throw_spdlog_ex("rotating sink constructor: max_size arg cannot be zero");
    }
    if (max_files > 200000) {
        throw_spdlog_ex("rotating sink constructor: max_files arg cannot exceed 200000");
    }
    file_helper_.open(calc_filename(base_filename_, 0));
    current_size_ = file_helper_.size();  // expensive. called only once
    if (rotate_on_open && current_size_ > 0) {
        rotate_();
        current_size_ = 0;
    }
}

SPDLOG_INLINE void rotating_file::write(const memory_buf_t &buf) {
    write(string_view_t{buf.data(), buf.size()});
}

SPDLOG_INLINE void rotating_file::write(string_view_t buf) {
    auto new_size = current_size_ + buf.size();

    // rotate if the new estimated file size exceeds max size.
    // rotate only if the real size > 0 to better deal with full disk (see issue #2261).
    // we only check the real size when new_size > max_size_ because it is relatively expensive.
    if (new_size > max_size_) {
        file_helper_.flush();
        if (file_helper_.size() > 0) {
            rotate_();
            new_size = buf.size();
        }
    }
    file_helper_.write(buf);
    current_size_ = new_size;
}

// Rotate files:
// log.txt -> log.1.txt
// log.1.txt -> log.2.txt
// log.2.txt -> log.3.txt
// log.3.txt -> delete
SPDLOG_INLINE void rotating_file::rotate_() {
    using details::os::filename_to_str;
    using details::os::path_exists;

    file_helper_.close();
    for (auto i = max_files_; i > 0; --i) {
        filename_t src = calc_filename(base_filename_, i - 1);
        if (!path_exists(src)) {
            continue;
        }
        filename_t target = calc_filename(base_filename_, i);

        if (!rename_file_(src, target)) {
            // if failed try again after a small delay.
            // this is a workaround to a windows issue, where very high rotation
            // rates can cause the rename to fail with permission denied (because of antivirus?).
            details::os::sleep_for_millis(100);
            if (!rename_file_(src, target)) {
                file_helper_.reopen(
                    true);  // truncate the log file anyway to prevent it to grow beyond its limit!
                current_size_ = 0;
                throw_spdlog_ex("rotating_file_sink: failed renaming " + filename_to_str(src) +
                                    " to " + filename_to_str(target),
                                errno);
            }
        }
    }
    file_helper_.reopen(true);
}

SPDLOG_INLINE void rotating_file::flush() { file_helper_.flush(); }

// calc filename according to index and file extension if exists.
// e.g. calc_filename("logs/mylog.txt, 3) => "logs/mylog.3.txt".
SPDLOG_INLINE filename_t rotating_file::calc_filename(const filename_t &filename,
                                                      std::size_t index) {
    if (index == 0u) {
        return filename;
    }

    filename_t basename, ext;
    std::tie(basename, ext) = details::file_helper::split_by_extension(filename);
    return fmt_lib::format(SPDLOG_FILENAME_T("{}.{}{}"), basename, index, ext);
}

SPDLOG_INLINE filename_t rotating_file::filename() const { return file_helper_.filename(); }

// delete the target if exists, and rename the src file  to target
// return true on success, false otherwise.
SPDLOG_INLINE bool rotating_file::rename_file_(const filename_t &src_filename,
                                               const filename_t &target_filename) {
    // try to delete the target file in case it already exists.
    (void)details::os::remove(target_filename);
    return details::os::rename(src_filename, target_filename) == 0;
}

}  // namespace details
}  // namespace spdlog
