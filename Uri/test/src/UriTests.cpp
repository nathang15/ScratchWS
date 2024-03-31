#include <gtest/gtest.h>
#include <Uri/Uri.hpp>
#include <stddef.h>

TEST(UriTests, ParseStringNoScheme) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("foo/bar"));
    ASSERT_EQ("", uri.GetScheme());
    ASSERT_EQ(
        (std::vector< std::string >{
            "foo",
            "bar",
    }),
        uri.GetPath()
    );
}

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

TEST(UriTests, StringParsePortNumber) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www.example.com:8080/foo/bar"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(8080, uri.GetPort());
}

TEST(UriTests, StringParseNoPortNumber) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www.example.com/foo/bar"));
    ASSERT_EQ("http", uri.GetScheme());
    ASSERT_EQ("www.example.com", uri.GetHost());
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, StringParseTwiceFirstWithPortThenWithout) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www.example.com:8080/foo/bar"));
    ASSERT_TRUE(uri.StringParse("http://www.example.com/foo/bar"));
    ASSERT_FALSE(uri.HasPort());
}

TEST(UriTests, StringParseBadPortNumber) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.StringParse("http://www.example.com:bad/foo/bar"));
}

TEST(UriTests, StringParseBadPortNumberStartsWithNumeric) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.StringParse("http://www.example.com:8080bad/foo/bar"));
}

TEST(UriTests, StringParseLargestPortNumberValid) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www.example.com:65535/foo/bar"));
    ASSERT_TRUE(uri.HasPort());
    ASSERT_EQ(65535, uri.GetPort());
}

TEST(UriTests, StringParseBadPortNegative) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.StringParse("http://www.example.com:-8888/foo/bar"));
}

TEST(UriTests, StringParseBadPortNumberOutOfRange) {
    Uri::Uri uri;
    ASSERT_FALSE(uri.StringParse("http://www.example.com:65536/foo/bar"));
}

TEST(UriTests, StringParseEndsAfterAuthority) {
    Uri::Uri uri;
    ASSERT_TRUE(uri.StringParse("http://www/example.com"));
}

TEST(UriTests, StringParseRelativePathVsNonRelativeReference) {
    struct TestVector {
        std::string uriString;
        bool isRelativeReference;
    };
    const std::vector< TestVector > testVectors{
        {"http://www.example.com/", false},
        {"http://www.example.com", false},
        {"/", true},
        {"foo", true},
    };
    size_t index = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.StringParse(testVector.uriString)) << index;
        ASSERT_EQ(testVector.isRelativeReference, uri.IsRelativeReference()) << index;
        ++index;
    };
}

TEST(UriTests, StringParseRelativePathVsNonRelativePaths) {
    struct TestVector {
        std::string uriString;
        bool containsRelativePath;
    };
    const std::vector< TestVector > testVectors{
        {"http://www.example.com/", false},
        {"http://www.example.com", true},
        {"/", false},
        {"foo", true},
        /*
        *NOTE: only a valid test vector if empty string is a valid path
        */ 
        {"", true},
    };
    size_t index = 0;
    for (const auto& testVector : testVectors) {
        Uri::Uri uri;
        ASSERT_TRUE(uri.StringParse(testVector.uriString)) << index;
        ASSERT_EQ(testVector.containsRelativePath, uri.ContainsRelativePath()) << index;
        ++index;
    };
}
