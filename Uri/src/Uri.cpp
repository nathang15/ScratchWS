#include <string>
#include <vector>
#include <Uri/Uri.hpp>

namespace Uri {
    /**
     * This contains the private properties of a Uri instance.
     */
    struct Uri::Impl {
        /**
        * Scheme of the URI
        */
        std::string pathDelimiter = "/";

        /**
        * Scheme of the URI
        */
        std::string scheme;

        /**
        * Host of the URI
        */
        std::string host;

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

    void Uri::SetPathDelimiter(const std::string& newPathDelimiter) {
        impl_->pathDelimiter = newPathDelimiter;
    }

    bool Uri::StringParse(const std::string& uriString) {
        // TODO Refactor later!!
        // Parse scheme
        const auto schemeEnd = uriString.find(":");
        impl_->scheme = uriString.substr(0, schemeEnd);
        auto remainingUriString = uriString.substr(schemeEnd + 1);

        // Parse host
        if (remainingUriString.substr(0, 2) == "//") {
            const auto authorityEnd = remainingUriString.find(impl_->pathDelimiter, 2);
            impl_->host = remainingUriString.substr(2, authorityEnd - 2);
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
        if (remainingUriString == impl_->pathDelimiter) {
            // empty path but needs a single empty string to indicate that it is absolute.
            impl_->path.push_back("");
        }
        else if (!remainingUriString.empty()) {
            for(;;) {
                auto pathDelimiter = remainingUriString.find(impl_->pathDelimiter);
                if (pathDelimiter == std::string::npos) {
                    impl_->path.push_back(remainingUriString);
                    break;
                } else {
                    impl_->path.emplace_back(
                        remainingUriString.begin(),
                        remainingUriString.begin() + pathDelimiter
                    ); 
                    remainingUriString = remainingUriString.substr(pathDelimiter + impl_->pathDelimiter.length());
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

}
