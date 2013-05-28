#pragma once

#include "bunsan/web/mime/error.hpp"
#include "bunsan/web/mime/map.hpp"

#include "bunsan/forward_constructor.hpp"
#include "bunsan/filesystem/error.hpp"

#include <boost/filesystem/path.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

#include <vector>

namespace bunsan{namespace web{namespace mime
{
    namespace detail
    {
        struct file_entry
        {
            template <typename Archive>
            void serialize(Archive &ar, const unsigned int)
            {
                ar & BOOST_SERIALIZATION_NVP(mime_type);
                ar & BOOST_SERIALIZATION_NVP(extensions);
            }

            inline bool operator==(const file_entry &e) const
            {
                return mime_type == e.mime_type && extensions == e.extensions;
            }

            std::string mime_type;
            std::vector<std::string> extensions;
        };
    }

    struct file_error: virtual error {};

    struct file_format_error: virtual file_error
    {
        typedef boost::error_info<struct tag_row, std::size_t> row;
        typedef boost::error_info<struct tag_column, std::size_t> column;
        typedef filesystem::error::path path;
    };

    struct file: std::vector<detail::file_entry>
    {
        typedef detail::file_entry entry;

        BUNSAN_INHERIT_EXPLICIT_CONSTRUCTOR(file, std::vector<entry>)
        file(const file &)=default;
        file &operator=(const file &)=default;
        file(file &&)=default;
        file &operator=(file &&)=default;

        explicit file(const map &m);
        explicit operator map() const;

        void sort();
    };

    std::istream &operator>>(std::istream &in, file &f);
    std::ostream &operator<<(std::ostream &out, const file &f);

    file load_file(const boost::filesystem::path &path);
    void save_file(const boost::filesystem::path &path, const file &f);
}}}

BOOST_FUSION_ADAPT_STRUCT(
    bunsan::web::mime::file::entry,
    (std::string, mime_type)
    (std::vector<std::string>, extensions));
