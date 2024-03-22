#include <gtest/gtest.h>
#include <Uri/Uri.hpp>
#include <stddef.h>

TEST(UriTests, StringParseUrl) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www.example.com/foo/bar"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_EQ(
        (std::vector< std::string >{
            "",
            "foo",
            "bar",
        }),
        uri.GetPath()
    );
}
TEST(UriTests, StringParseUrnDefaultPathDelimiter) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("urn:animal:dog:Golden"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ(
        (std::vector< std::string >{
        "animal:dog:Golden"
    }),
        uri.GetPath()
    );
}


TEST(UriTests, StringParseUrnSingleCharacterPathDelimiter) {
    Uri::Uri uri;
    uri.SetPathDelimiter(":");
    ASSERT_TRUE(uri.StringParse("urn:animal:dog:Golden"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ(
        (std::vector< std::string >{
        "animal",
        "dog",
        "Golden",
    }),
        uri.GetPath()
    );
}

TEST(UriTests, StringParseUrnMultiCharacterPathDelimiter) {
    Uri::Uri uri;
    uri.SetPathDelimiter("/-");
    ASSERT_TRUE(uri.StringParse("urn:an-/imal/-do/g/-Gol-den"));
    ASSERT_EQ("urn", uri.GetScheme());
    ASSERT_EQ("", uri.GetHost());
    ASSERT_EQ(
        (std::vector< std::string >{
            "an-/imal",
            "do/g",
            "Gol-den",
        }),
        uri.GetPath()
    );
}

TEST(UriTests, ParseStringCornerCases) {
    struct TestVector {
        std::string pathIn;
        std::vector< std::string > pathOut;
    };
    const std::vector< TestVector > testVectors {
        {"", {}},
        {"/", {""}},
        {"/foo", {"", "foo"}},
        {"foo/", {"foo", ""}},
    };
    size_t index = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.StringParse(testVector.pathIn)) << index;
        ASSERT_EQ(testVector.pathOut, uri.GetPath()) << index;
        ++index;
    };
}
