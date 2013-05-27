#define BOOST_TEST_MODULE mime_file
#include <boost/test/unit_test.hpp>

#include "bunsan/web/mime_file.hpp"

#include <sstream>
#include <vector>

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

BOOST_AUTO_TEST_CASE(put_all)
{
    bw::mime_file m;
    std::vector<std::string> lst1 = {"ext1", "ext2"};
    std::vector<std::string> lst2 = {"ext3", "ext4"};
    m.put_all("mime1", lst1.begin(), lst1.end());
    m.put_all("mime2", lst2);
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext4"), "mime2");
}

BOOST_AUTO_TEST_CASE(load)
{
    std::istringstream sin(R"EOF(
        mime0
        mime1 ext1 ext2
        mime2 ext3 ext4 # comment
# next line is empty

        text/plain conf txt
)EOF");
    bw::mime_file m;
    m.load(sin);
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_by_extension("ext4"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_by_extension("conf"), "text/plain");
    BOOST_CHECK_EQUAL(m.mime_by_extension("txt"), "text/plain");
}

BOOST_AUTO_TEST_SUITE_END() // mime_file
