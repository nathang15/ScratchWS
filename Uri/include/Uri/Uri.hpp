#ifndef URI_HPP
#define URI_HPP

/**
 * @file Uri.hpp
 *
 * This module declares the Uri::Uri class.
 *
 */

#include <memory>
#include <string>
#include <vector>

namespace Uri {

    /**
     * This class represents a Uniform Resource Identifier (URI),
     * as defined in RFC 3986 (https://tools.ietf.org/html/rfc3986).
     */
    class Uri {
        // Lifecycle management
    public:
        ~Uri();
        Uri(const Uri&) = delete;
        Uri(Uri&&) = delete;
        Uri& operator=(const Uri&) = delete;
        Uri& operator=(Uri&&) = delete;

        // Public methods
    public:
        /**
         * This is the default constructor.
         */
        Uri();

        /**
         * Set path delimiter
         */

        void SetPathDelimiter(const std::string& newPathDelimiter);

        /**
         * Builds the URI from parsed elements from rendered string of URI.
         * 
         * @param[in] uriString This is the string rendered from URI to be passed in
         *      
         * @return True if URI is parsed correctly else False
         */
        bool StringParse(const std::string& uriString);

        /**
         * Returns the scheme of the URI
         * 
         * @return the scheme of URI
         * 
         * @retval "" if there is no scheme element in URI
        */
        std::string GetScheme() const;

        /**
         * Returns the host of the URI
         * 
         * @return the host of URI
         * 
         * @retval "" if there is no host element in URI
        */
        std::string GetHost() const;

        /**
         * Returns the path of the URI as a sequence
         * 
         * @note if the first step is empty, then URI has absolute path
         * 
         * @return the path of URI
         * 
         * @retval "" if there is no path element in URI
        */
        std::vector< std::string > GetPath() const;


        // Private properties
    private:
        /**
         * This is the type of structure that contains the private
         * properties of the instance.  It is defined in the implementation
         * and declared here to ensure that it is scoped inside the class.
         */
        struct Impl;

        /**
         * This contains the private properties of the instance.
         */
        std::unique_ptr< struct Impl > impl_;
    };

}

#endif; /* URI_HPP */
