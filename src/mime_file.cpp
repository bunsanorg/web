#include "bunsan/web/mime_file.hpp"

#include "bunsan/enable_error_info.hpp"
#include "bunsan/filesystem/fstream.hpp"

namespace bunsan{namespace web
{
    void mime_file::swap(mime_file &m) noexcept
    {
        using std::swap;

        swap(m_extension2mime, m.m_extension2mime);
    }

    void mime_file::load(const boost::filesystem::path &path)
    {
        mime_file data;
        BUNSAN_EXCEPTIONS_WRAP_BEGIN()
        {
            bunsan::filesystem::ifstream fin(path);
            // TODO
            fin.close();
        }
        BUNSAN_EXCEPTIONS_WRAP_END()
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
