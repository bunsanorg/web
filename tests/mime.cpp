#define BOOST_TEST_MODULE mime_file
#include <boost/test/unit_test.hpp>

#include "bunsan/web/mime/file.hpp"
#include "bunsan/web/mime/map.hpp"

#include <sstream>
#include <vector>

BOOST_AUTO_TEST_SUITE(mime_)
namespace mime = bunsan::web::mime;

BOOST_AUTO_TEST_CASE(map_file)
{
}

BOOST_AUTO_TEST_SUITE(map)

BOOST_AUTO_TEST_CASE(basic)
{
    mime::map m;
    m.put("mime1", "ext1", "ext2");
    m.put("mime2", "ext3");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("unknown"), "application/octet-stream");
    BOOST_CHECK_EQUAL(m.mime_type_by_name("name.unknown"), "application/octet-stream");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_name("name.ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_name("name.ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_name("name.ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension(".ext1"), "mime1");
    m.set("mime3", "ext1", "ext3");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext1"), "mime3");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext3"), "mime3");
}

BOOST_AUTO_TEST_CASE(put_all)
{
    mime::map m;
    std::vector<std::string> lst1 = {"ext1", "ext2"};
    std::vector<std::string> lst2 = {"ext3", "ext4"};
    m.put_all("mime1", lst1.begin(), lst1.end());
    m.put_all("mime2", lst2);
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext4"), "mime2");
}

BOOST_AUTO_TEST_CASE(map_extension_conflict_error)
{
    mime::map m;
    m.put("mime", "ext");
    BOOST_CHECK_EXCEPTION(m.put("mime", "ext"), mime::map_extension_conflict_error,
                          [](const mime::map_extension_conflict_error &e)
                          {
                              BOOST_REQUIRE(e.get<mime::map_extension_conflict_error::mime_type>());
                              BOOST_CHECK_EQUAL(*e.get<mime::map_extension_conflict_error::mime_type>(), "mime");
                              BOOST_REQUIRE(e.get<mime::map_extension_conflict_error::extension>());
                              BOOST_CHECK_EQUAL(*e.get<mime::map_extension_conflict_error::extension>(), "ext");
                              return true;
                          });
}

BOOST_AUTO_TEST_SUITE_END() // map

BOOST_AUTO_TEST_SUITE(file)

BOOST_AUTO_TEST_CASE(load)
{
    std::istringstream sin(R"EOF(
        mime0
        mime1 ext1 ext2
        mime2 ext3 ext4 # comment
# next line is empty

        text/plain conf txt
)EOF");
    mime::file f;
    sin >> f;
    const mime::file f_ = {
        {"mime0", {}},
        {"mime1", {"ext1", "ext2"}},
        {"mime2", {"ext3", "ext4"}},
        {"text/plain", {"conf", "txt"}},
    };
    BOOST_CHECK_EQUAL(f, f_);
    const mime::map m(f);
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext3"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext4"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("conf"), "text/plain");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("txt"), "text/plain");
}

BOOST_AUTO_TEST_CASE(load_override)
{
    std::istringstream sin(R"EOF(
        mime1 ext1 ext2
        mime2 ext2 ext3
)EOF");
    mime::file f;
    sin >> f;
    const mime::map m(f);
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext1"), "mime1");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime2");
    BOOST_CHECK_EQUAL(m.mime_type_by_extension("ext2"), "mime2");
}

BOOST_AUTO_TEST_SUITE_END() // file

BOOST_AUTO_TEST_SUITE_END() // mime_
