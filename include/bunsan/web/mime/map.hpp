#pragma once

#include "bunsan/web/mime/error.hpp"

#include <boost/filesystem/path.hpp>

#include <string>
#include <unordered_map>

namespace bunsan{namespace web{namespace mime
{
    struct map_error: virtual error {};
    struct map_extension_conflict_error: virtual map_error {};

    class map
    {
    public:
        map()=default;
        map(const map &)=default;
        map &operator=(const map &)=default;
        map(map &&)=default;
        map &operator=(map &&)=default;

        void swap(map &m) noexcept;

    public:
        /// \return mime_type corresponding to extension or "application/octet-stream" as fallback
        std::string mime_type_by_extension(const std::string &extension) const;
        std::string mime_type_by_name(const boost::filesystem::path &name) const;

        /// Creates new mapping extension -> mime_type or replaces old one.
        void set(const std::string &mime_type, const std::string &extension);

        template <typename ... Args>
        void set(const std::string &mime_type, const std::string &extension, Args &&...args)
        {
            set(mime_type, extension);
            set(mime_type, std::forward<Args>(args)...);
        }

        template <typename Iterable>
        void set_all(const std::string &mime_type, const Iterable &iterable)
        {
            set_all(mime_type, begin(iterable), end(iterable));
        }

        template <typename Iter>
        void set_all(const std::string &mime_type, Iter begin, const Iter &end)
        {
            for (; begin != end; ++begin)
                set(mime_type, *begin);
        }

        /// \throws mime_file_extension_conflict_error if extension -> mime_type mapping is already present
        void put(const std::string &mime_type, const std::string &extension);

        template <typename ... Args>
        void put(const std::string &mime_type, const std::string &extension, Args &&...args)
        {
            put(mime_type, extension);
            put(mime_type, std::forward<Args>(args)...);
        }

        template <typename Iterable>
        void put_all(const std::string &mime_type, const Iterable &iterable)
        {
            put_all(mime_type, begin(iterable), end(iterable));
        }

        template <typename Iter>
        void put_all(const std::string &mime_type, Iter begin, const Iter &end)
        {
            for (; begin != end; ++begin)
                put(mime_type, *begin);
        }

    private:
        static const std::string fallback_mime_type;

        static std::string norm_extension(const std::string &extension);

    private:
        std::unordered_map<std::string, std::string> m_extension2mime_type;
    };
    inline void swap(map &a, map &b) noexcept
    {
        a.swap(b);
    }
}}}
