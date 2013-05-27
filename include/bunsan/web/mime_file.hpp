#pragma once

#include "bunsan/web/error.hpp"
#include "bunsan/filesystem/error.hpp"

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

    struct mime_file_format_error: virtual mime_file_error
    {
        typedef boost::error_info<struct tag_row, std::size_t> row;
        typedef boost::error_info<struct tag_column, std::size_t> column;
        typedef filesystem::error::path path;
    };

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
        /// \note Uses set(), i.e. for multiple
        void load(std::istream &in);
        void load(const boost::filesystem::path &path);

    public:
        /// \return mime type corresponding to extension or "application/octet-stream" as fallback
        std::string mime_by_extension(const std::string &extension) const;
        std::string mime_by_name(const boost::filesystem::path &name) const;

        /// Creates new mapping extension -> mime or replaces old one.
        void set(const std::string &mime, const std::string &extension);

        template <typename ... Args>
        void set(const std::string &mime, const std::string &extension, Args &&...args)
        {
            set(mime, extension);
            set(mime, std::forward<Args>(args)...);
        }

        template <typename Iterable>
        void set_all(const std::string &mime, const Iterable &iterable)
        {
            set_all(mime, begin(iterable), end(iterable));
        }

        template <typename Iter>
        void set_all(const std::string &mime, Iter begin, const Iter &end)
        {
            for (; begin != end; ++begin)
                set(mime, *begin);
        }

        /// \throws mime_file_extension_conflict_error if extension -> mime mapping is already present
        void put(const std::string &mime, const std::string &extension);

        template <typename ... Args>
        void put(const std::string &mime, const std::string &extension, Args &&...args)
        {
            put(mime, extension);
            put(mime, std::forward<Args>(args)...);
        }

        template <typename Iterable>
        void put_all(const std::string &mime, const Iterable &iterable)
        {
            put_all(mime, begin(iterable), end(iterable));
        }

        template <typename Iter>
        void put_all(const std::string &mime, Iter begin, const Iter &end)
        {
            for (; begin != end; ++begin)
                put(mime, *begin);
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
