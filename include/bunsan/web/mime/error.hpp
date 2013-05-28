#pragma once

#include "bunsan/web/error.hpp"

namespace bunsan{namespace web{namespace mime
{
    struct error: virtual web::error
    {
        typedef boost::error_info<struct tag_mime_type, std::string> mime_type;
        typedef boost::error_info<struct tag_extension, std::string> extension;
    };
}}}
