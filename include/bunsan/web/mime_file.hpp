#pragma once

#include "bunsan/web/error.hpp"

#include <boost/filesystem/path.hpp>

#include <string>
#include <unordered_map>
#include <utility>

namespace bunsan{namespace web
{
    struct mime_file_error: virtual error
    {
        typedef boost::error_info<struct tag_mime, std::string> mime;
        typedef boost::error_info<struct tag_extension, std::string> extension;
    };

    struct mime_file_extension_conflict_error: virtual mime_file_error {};

    class mime_file
    {
    public:
        mime_file()=default;
        mime_file(const mime_file &)=default;
        mime_file &operator=(const mime_file &)=default;
        mime_file(mime_file &&)=default;
        mime_file &operator=(mime_file &&)=default;

        void swap(mime_file &m) noexcept;

    public:
        void load(const boost::filesystem::path &path);

    public:
        /// \return mime type corresponding to extension or "application/octet-stream" as fallback
        std::string mime_by_extension(const std::string &extension) const;
        std::string mime_by_name(const boost::filesystem::path &name) const;

        void put(const std::string &mime, const std::string &extension);

        template <typename ... Args>
        void put(const std::string &mime, const std::string &extension, Args &&...args)
        {
            put(mime, extension);
            put(mime, std::forward<Args>(args)...);
        }

    private:
        static const std::string fallback_mime;

        static std::string norm_extension(const std::string &extension);

    private:
        std::unordered_map<std::string, std::string> m_extension2mime;
    };
    inline void swap(mime_file &a, mime_file &b) noexcept
    {
        a.swap(b);
    }
}}
