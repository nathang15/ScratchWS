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
#include <stdint.h>

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
         * Returns the user info element of the URI.
         *
         * @return the user info
         *
        */
        std::string GetUserInfo() const;

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


        /**
        * Returns True if URI includes a port number else False.
        *
        * @return boolean value
        */

        bool HasPort() const;


        /**
        * Returns the port number if URI includes one.
        *
        * @return the unsinged port number
        *
        * @note The returned port number is only valid if HasPort == True
        */

        uint16_t GetPort() const;

        /**
        * Indicates whether or not URI is relative reference
        *
        * @return true if is relative. false otherwise
        */
        bool IsRelativeReference() const;

        /**
        * Indicates whether or not URI contains relative path
        *
        * @return true if contains relative path. false otherwise
        */
        bool ContainsRelativePath() const;

        /**
        * Return the fragment of Uri if has one
        * @return the fragment
        * 
        */
        std::string GetFragment() const;

        /**
        * Return the query of Uri if has one
        * @return the query
        *
        */
        std::string GetQuery() const;


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
