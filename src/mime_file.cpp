#include "bunsan/web/mime_file.hpp"

#include "bunsan/enable_error_info.hpp"
#include "bunsan/filesystem/fstream.hpp"

#include <boost/assert.hpp>
#include <boost/fusion/adapted/struct/adapt_struct.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace bunsan{namespace web
{
    void mime_file::swap(mime_file &m) noexcept
    {
        using std::swap;

        swap(m_extension2mime, m.m_extension2mime);
    }

    namespace
    {
        struct mime_file_line
        {
            std::string mime_type;
            std::vector<std::string> extensions;

            void setup(mime_file &m) const
            {
                m.set_all(mime_type, extensions);
            }
        };
    }
}}

BOOST_FUSION_ADAPT_STRUCT(
    bunsan::web::mime_file_line,
    (std::string, mime_type)
    (std::vector<std::string>, extensions));

namespace bunsan{namespace web
{
    void mime_file::load(std::istream &in)
    {
        mime_file data;
        boost::optional<mime_file_line> mime_exts;

        namespace qi = boost::spirit::qi;
        namespace phoenix = boost::phoenix;
        using namespace qi::labels;
        using namespace phoenix;
        typedef std::string::const_iterator iterator;

        qi::rule<iterator, boost::optional<mime_file_line>(), qi::ascii::space_type> line_parser;
        qi::rule<iterator, mime_file_line(), qi::ascii::space_type> data_parser;
        qi::rule<iterator, void(), qi::ascii::space_type> comment_parser;
        qi::rule<iterator, std::string()> mime_type_parser, extension_parser;
        qi::rule<iterator, std::vector<std::string>(), qi::ascii::space_type> extension_list_parser;

        mime_type_parser = +(qi::graph - '#');
        extension_parser = +(qi::graph - '#');
        comment_parser = qi::char_('#') >> *qi::char_;

        extension_list_parser = *extension_parser;
        data_parser = mime_type_parser >> extension_list_parser;
        line_parser = -data_parser >> -comment_parser;

        std::string line;
        std::size_t lineno = 0;
        while (std::getline(in, line))
        {
            ++lineno;
            iterator cur = line.begin();
            const iterator end = line.end();
            if (qi::phrase_parse(cur, end, line_parser, qi::ascii::space, mime_exts))
            {
                if (cur != end)
                {
                    BOOST_THROW_EXCEPTION(mime_file_format_error() <<
                                          mime_file_format_error::row(lineno) <<
                                          mime_file_format_error::column(1 + (cur - line.begin())));
                }
                else
                {
                    if (mime_exts)
                        mime_exts->setup(data);
                    mime_exts.reset();
                }
            }
            else
            {
                BOOST_THROW_EXCEPTION(mime_file_format_error() <<
                                      mime_file_format_error::row(lineno));
            }
        }
        swap(data);
    }

    void mime_file::load(const boost::filesystem::path &path)
    {
        mime_file data;
        BUNSAN_EXCEPTIONS_WRAP_BEGIN()
        {
            bunsan::filesystem::ifstream fin(path);
            data.load(fin);
            fin.close();
        }
        BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO(bunsan::filesystem::error::path(path))
        swap(data);
    }

    std::string mime_file::mime_by_extension(const std::string &extension) const
    {
        const auto iter = m_extension2mime.find(norm_extension(extension));
        if (iter == m_extension2mime.end())
            return fallback_mime;
        else
            return iter->second;
    }

    std::string mime_file::mime_by_name(const boost::filesystem::path &name) const
    {
        return mime_by_extension(name.extension().string());
    }

    void mime_file::set(const std::string &mime, const std::string &extension)
    {
        m_extension2mime[extension] = mime;
    }

    void mime_file::put(const std::string &mime, const std::string &extension)
    {
        const std::string ext = norm_extension(extension);
        const auto iter = m_extension2mime.find(ext);
        if (iter != m_extension2mime.end())
            BOOST_THROW_EXCEPTION(mime_file_extension_conflict_error() <<
                                  mime_file_extension_conflict_error::mime(mime) <<
                                  mime_file_extension_conflict_error::extension(ext));
        m_extension2mime[extension] = mime;
    }

    const std::string mime_file::fallback_mime = "application/octet-stream";

    std::string mime_file::norm_extension(const std::string &extension)
    {
        return !extension.empty() && extension[0] == '.' ? extension.substr(1) : extension;
    }
}}
