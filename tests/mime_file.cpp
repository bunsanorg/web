#define BOOST_TEST_MODULE mime_file
#include <boost/test/unit_test.hpp>

#include "bunsan/web/mime_file.hpp"

namespace bw = bunsan::web;

BOOST_AUTO_TEST_SUITE(mime_file)

BOOST_AUTO_TEST_CASE(basic)
{
    bw::mime_file m;
    m.put("mime1", "ext1", "ext2");
    m.put("mime2", "ext3");
    BOOST_CHECK_EQUAL(m.mime_by_extension("unknown"), "application/octet-stream");
    BOOST_CHECK_EQUAL(m.mime_by_name("name.unknown"), "application/octet-stream");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_by_name("name.ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_name("name.ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_name("name.ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_by_extension(".ext1"), "mime1");
}

BOOST_AUTO_TEST_SUITE_END() // mime_file
