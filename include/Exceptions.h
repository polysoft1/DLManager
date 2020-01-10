#ifndef __EXCEPTIONS_H__
#define __EXCEPTIONS_H__

#include <stdexcept>

namespace Polysoft {

    /**
     * A generic runtime exception that is used as a parent class for all other exceptions
     */
    class DLException : public std::runtime_error{
    public:
        /**
         * Creates an exception with an error message
         * The error message can be accessed using the what() member function
         * 
         * @param [in] what_arg The error message
         */
        DLException(const std::string &what_arg) : std::runtime_error(what_arg){}

        /**
         * Creates an exception with an error message
         * The error message can be accessed using the what() member function
         * 
         * @param [in] what_arg The error message
         */
        DLException(const char *what_arg) : std::runtime_error(what_arg){}
    };

    /**
     * An exception for when there is a problem opening the dynamic library
     */ 
    class OpenLibraryException : public DLException {
    public:
        OpenLibraryException(const std::string& what_arg ) : DLException(what_arg){}
        OpenLibraryException(const char *what_arg) : DLException(what_arg){}
    };

    /**
     * An exception for when a function could not be found within the dynamic library 
     */
    class NoSuchFunctionException : public DLException {
    public:
        NoSuchFunctionException(const std::string& what_arg) : DLException(what_arg){}
        NoSuchFunctionException(const char *what_arg) : DLException(what_arg){}
    };

    /**
     * An exception for when there is a problem closing a library 
     */
    class CloseLibraryException : public DLException {
    public:
        CloseLibraryException(const std::string& what_arg) : DLException(what_arg){}
        CloseLibraryException(const char *what_arg) : DLException(what_arg){}
    };

    /**
     * An exception for when the user did not open a library before trying to access a function
     */
    class NoLibraryOpenException : public DLException {
    public:
        NoLibraryOpenException(const std::string& what_arg) : DLException(what_arg){}
        NoLibraryOpenException(const char *what_arg) : DLException(what_arg){}
    };
};

#endif