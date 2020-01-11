#ifndef __DL_MANAGER_H__
#define __DL_MANAGER_H__

//Check for C++17 support
#if __cplusplus >= 201703L
#include <filesystem>
#endif

#include <string>

#ifdef __linux__
typedef void* SharedLib;
#endif

namespace Polysoft{
    /**
     * 
     */
    class DLManager {
    private:
        /**
         *  This is used for storing the handle for whichever library is being used to actually load
         *  the dynamic library.
         */
        SharedLib handle;
    public:
        /**
         * Default constructor, does not open any dynamic library 
         */
        DLManager();

        /**
         * Constructor that opens the provided dynamic library
         * 
         * @param [in] filename 
         *  The path/name of the dynamic library
         * @throw OpenLibraryException 
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        DLManager(const std::string& filename);

        /**
         * Destructor, attempts to close whatever it was holding before exiting
         * 
         * @throw CloseLibraryException 
         *  When the library cannot be closed, a CloseLibraryException is thrown
         */
        ~DLManager();

        /**
         * Opens the supplied dynamic library
         * 
         * @param [in] filename
         *  The dynamic library to be opened
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        void open(const std::string& filename);

        /**
         * Closes the previously open()'d dynamic library
         * 
         * @throw CloseLibraryException
         *  When the library cannot be closed, a CloseLibraryException is thrown 
         */
        void close();
        
        /**
         * Gets a function and stores it at the passed function destination
         * 
         * @tparam T The type of function being retrieved (currently tested against C function pointers)
         * @param [in] name The name of the function to be retrieved
         * @param [out] func_dest The variable to store the function
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        void getFunction(const std::string &name, T &func_dest);
        
        /**
         * Gets a function and stores it at the passed function destination
         * 
         * @tparam T The type of function being retrieved (currently tested against C function pointers)
         * @param [in] name The name of the function to be retrieved
         * @param [out] func_dest The variable to store the function
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        void getFunction(const char *name, T &func_dest);
        
         /**
         * Gets a function and returns it
         * 
         * @tparam T The type of function being retrieved (currently tested against C function pointers)
         * @param [in] name The name of the function to be retrieved
         * 
         * @return A function casted to type T
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        T getFunction(const std::string &name);

        /**
         * Gets a function and returns it
         * 
         * @tparam T The type of function being retrieved (currently tested against C function pointers)
         * @param [in] name The name of the function to be retrieved
         * 
         * @return A function casted to type T
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        T getFunction(const char *name);

//Check for C++17 support
#if __cplusplus >= 201703L 
        /**
         * Constructor that opens the provided dynamic library at the given path
         * (C++17 and later)
         * 
         * @param [in] filepath 
         *  The path to the dynamic library
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        DLManager(const std::filesystem::path& filepath);

        /**
         * Opens the supplied dynamic library at the given path
         * (C++17 and later)
         * 
         * @param [in] filepath
         *      The path to the dynamic library
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        void open(const std::filesystem::path& filepath);
#endif
    };
};

#endif