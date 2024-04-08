#include <string>
#include <vector>
#include <Uri/Uri.hpp>
#include <inttypes.h>

namespace Uri {
    /**
     * This contains the private properties of a Uri instance.
     */
    struct Uri::Impl {
        /**
        * Scheme of the URI
        */

        /**
        * Scheme of the URI
        */
        std::string scheme;

        /**
        * UserInfo of the URI
        */
        std::string userInfo;

        /**
        * Host of the URI
        */
        std::string host;

        /** A Flag indicating URI has port number
        */
        bool hasPort = false;

        /** Port number
        */
        uint16_t port = 0;

        /**
        * Path of the URI
        */
std::vector< std::string > path;

/**
* This is the fragment of the Uri
*/
std::string fragment;

/**
* This is the query of the Uri
*/
std::string query;
    };

    Uri::~Uri() = default;

    Uri::Uri()
        : impl_(new Impl)
    {
    }

    bool Uri::StringParse(const std::string& uriString) {
        // TODO Refactor later!!
        // Parse scheme
        const auto schemeEnd = uriString.find(":");
        std::string remainingUriString;
        if (schemeEnd == std::string::npos) {
            impl_->scheme.clear();
            remainingUriString = uriString;
        }
        else {
            impl_->scheme = uriString.substr(0, schemeEnd);
            remainingUriString = uriString.substr(schemeEnd + 1);
        }
        // Parse authority
        impl_->hasPort = false;
        const auto pathEnd = remainingUriString.find_first_of("?#");
        auto authorityAndPathString = remainingUriString.substr(0, pathEnd);
        const auto queryAndOrFragment = remainingUriString.substr(authorityAndPathString.length());
        std::string hostPortAndPathString;
        if (authorityAndPathString.substr(0, 2) == "//") {
            // Strip off authority marker
            authorityAndPathString = authorityAndPathString.substr(2);

            // Separate the authority from path
            auto authorityEnd = authorityAndPathString.find('/', 2);
            if (authorityEnd == std::string::npos) {
                authorityEnd = authorityAndPathString.length();
            }

            // Check if there is a userInfo, if yes, extract it.
            const auto userInfoDelimiter = authorityAndPathString.find('@');
            if (userInfoDelimiter == std::string::npos) {
                impl_->userInfo.clear();
                hostPortAndPathString = authorityAndPathString;
            }
            else {
                impl_->userInfo = authorityAndPathString.substr(0, userInfoDelimiter);
                hostPortAndPathString = authorityAndPathString.substr(userInfoDelimiter + 1);
            }
            auto authority = authorityAndPathString;

            // parsing host and port from authority and path.
            const auto portDelimiter = hostPortAndPathString.find(':');
            if (portDelimiter == std::string::npos) {
                impl_->host = hostPortAndPathString.substr(0, authorityEnd);
            }
            else {
                // TODO refactor to a different function
                impl_->host = hostPortAndPathString.substr(0, portDelimiter);
                uint32_t newPort = 0;
                for (auto c : hostPortAndPathString.substr(portDelimiter + 1, authorityEnd - portDelimiter - 1)) {
                    if ((c < '0') || (c > '9')) {
                        return false;
                    }
                    newPort *= 10;
                    newPort += (uint16_t)(c - '0');
                    if (
                        (newPort & ~((1 << 16) - 1)) != 0
                        ) {
                        return false;
                    }
                }
                impl_->port = (uint16_t)newPort;
                impl_->hasPort = true;
            }
            hostPortAndPathString = authorityAndPathString.substr(authorityEnd);
        }
        else {
            impl_->host.clear();
            hostPortAndPathString = authorityAndPathString;
        }

        auto pathString = hostPortAndPathString;
        // Parse path
        impl_->path.clear();
        if (pathString == "/") {
            // empty path but needs a single empty string to indicate that it is absolute.
            impl_->path.push_back("");
            pathString.clear();
        }
        else if (!remainingUriString.empty()) {
            for (;;) {
                auto pathDelimiter = pathString.find("/");
                if (pathDelimiter == std::string::npos) {
                    impl_->path.push_back(pathString);
                    pathString.clear();
                    break;
                }
                else {
                    impl_->path.emplace_back(
                        pathString.begin(),
                        pathString.begin() + pathDelimiter
                    );
                    pathString = pathString.substr(pathDelimiter + 1);
                }
            }
        }

        // Parse the fragment if there is one
        const auto fragmentDelimiter = queryAndOrFragment.find('#');
        if (fragmentDelimiter == std::string::npos) {
            impl_->fragment.clear();
            remainingUriString = queryAndOrFragment;
        }
        else {
            impl_->fragment = queryAndOrFragment.substr(fragmentDelimiter + 1);
            remainingUriString = queryAndOrFragment.substr(0, fragmentDelimiter);
        }

        // Next parse the query if there is one
        if (remainingUriString.empty()) {
            impl_->query.clear();
        }
        else {
            impl_->query = remainingUriString.substr(1);
        }

        return true;
    }

    std::string Uri::GetScheme() const {
        return impl_->scheme;
    }

    std::string Uri::GetUserInfo() const {
        return impl_->userInfo;
    }

    std::string Uri::GetHost() const {
        return impl_->host;
    }

    std::vector< std::string > Uri::GetPath() const {
        return impl_->path;
    }

    bool Uri::HasPort() const {
        return impl_->hasPort;
    }

    uint16_t Uri::GetPort() const {
        return impl_->port;
    }

    bool Uri::IsRelativeReference() const {
        return impl_->scheme.empty();
    }

    bool Uri::ContainsRelativePath() const {
        if (impl_->path.empty()) {
            return true;
        }
        else {
            return !impl_->path[0].empty();
        }
    }

    std::string Uri::GetQuery() const {
        return impl_->query;
    }

    std::string Uri::GetFragment() const {
        return impl_->fragment;
    }

}
