#include "bunsan/web/mime/file.hpp"

#include "bunsan/enable_error_info.hpp"
#include "bunsan/filesystem/fstream.hpp"

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/spirit/include/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

#include <algorithm>

namespace bunsan{namespace web{namespace mime
{
    file::file(const map &m)
    {
        // TODO
        BOOST_ASSERT_MSG(false, "Not implemented");
    }

    file::operator map() const
    {
        map m;
        for (const entry &e: *this)
            m.set_all(e.mime_type, e.extensions);
        return m;
    }

    void file::sort()
    {
        for (entry &e: *this)
            std::sort(e.extensions.begin(), e.extensions.end());
        std::sort(begin(), end(),
                  [](const entry &a, const entry &b)
                  { return a.mime_type < b.mime_type; });
    }

    std::istream &operator>>(std::istream &in, file &f)
    {
        // TODO
        BOOST_ASSERT_MSG(false, "Not implemented");
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const file &f)
    {
        // TODO
        BOOST_ASSERT_MSG(false, "Not implemented");
        return out;
    }

    file load_file(const boost::filesystem::path &path)
    {
        file f;
        BUNSAN_EXCEPTIONS_WRAP_BEGIN()
        {
            bunsan::filesystem::ifstream fin(path);
            fin >> f;
            fin.close();
        }
        BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO(bunsan::filesystem::error::path(path))
        return f;
    }

    void save_file(const boost::filesystem::path &path, const file &f)
    {
        BUNSAN_EXCEPTIONS_WRAP_BEGIN()
        {
            bunsan::filesystem::ofstream fout(path);
            fout << f;
            fout.close();
        }
        BUNSAN_EXCEPTIONS_WRAP_END_ERROR_INFO(bunsan::filesystem::error::path(path))
    }
}}}
