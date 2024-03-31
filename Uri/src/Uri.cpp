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
        // Parse host
        impl_->hasPort = false;
        if (remainingUriString.substr(0, 2) == "//") {
            auto authorityEnd = remainingUriString.find('/', 2);
            if (authorityEnd == std::string::npos) {
                authorityEnd = remainingUriString.length();
            }
            const auto portDelimiter = remainingUriString.find(':');
            if (portDelimiter == std::string::npos) {
                impl_->host = remainingUriString.substr(2, authorityEnd - 2);
            }
            else {
                // TODO refactor to a different function
                impl_->host = remainingUriString.substr(2, portDelimiter - 2);
                uint32_t newPort = 0;
                for (auto c : remainingUriString.substr(portDelimiter + 1, authorityEnd - portDelimiter - 1)) {
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
            remainingUriString = remainingUriString.substr(authorityEnd);
        } else {
            impl_->host.clear();
        }

        // Parse path
        // "/" -> [""]
        // "" -> []
        // "foo/" -> ["foo", ""]
        // "/foo" -> ["", "foo"]
        impl_->path.clear();
        if (remainingUriString == "/") {
            // empty path but needs a single empty string to indicate that it is absolute.
            impl_->path.push_back("");
        }
        else if (!remainingUriString.empty()) {
            for(;;) {
                auto pathDelimiter = remainingUriString.find("/");
                if (pathDelimiter == std::string::npos) {
                    impl_->path.push_back(remainingUriString);
                    break;
                } else {
                    impl_->path.emplace_back(
                        remainingUriString.begin(),
                        remainingUriString.begin() + pathDelimiter
                    ); 
                    remainingUriString = remainingUriString.substr(pathDelimiter + 1);
                }
            }
        }
        return true;
    }

    std::string Uri::GetScheme() const {
        return impl_->scheme;
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

}
