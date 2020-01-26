#ifndef __DL_MANAGER_UNIX_H__
#define __DL_MANAGER_UNIX_H__

#include <string>
#include <functional>
#include <dlfcn.h>

//Check for C++17 support
#if __cpp_lib_filesystem >= 201703L
#include <filesystem>
#endif

#include "Exceptions.h"

typedef void* SharedLib;

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
        SharedLib handle = nullptr;

        /**
         * This is used primarily for copying DLManager around, since the handle will not close
         * unless dlclose has been called as many times as dlopen has been called
         */
        std::string dest;
    public:
        /**
         * Default constructor, does not open any dynamic library 
         */
        DLManager() : handle(nullptr){}

        /**
         * Constructor that opens the provided dynamic library
         * 
         * @param [in] filename 
         *  The path/name of the dynamic library
         * @throw OpenLibraryException 
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        DLManager(const std::string& filename)
			: handle(nullptr)
		{
            open(filename);
        }

        /**
         * Destructor, attempts to close whatever it was holding before exiting
         * 
         * @throw CloseLibraryException 
         *  When the library cannot be closed, a CloseLibraryException is thrown
         */
        ~DLManager(){
            close();
        }

        /**
         * Copy Constructor, makes a copy of whatever was passed without destroying it
         * 
         * @param [in] in The DLManager object to be copied
         * 
         * @throw OpenLibraryException 
         *  When the library cannot be opened, an OpenLibraryException is thrown         
         */
        DLManager(const DLManager &in) : handle(nullptr){
            open(in.dest);
        }

        //Check for C++17 support
#if __cpp_lib_filesystem >= 201703L
        /**
         * Constructor that opens the provided dynamic library at the given path
         * (C++17 and later)
         *
         * @param [in] filepath
         *  The path to the dynamic library
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        DLManager(const std::filesystem::path& filepath)
            : handle(nullptr)
        {
            open(filepath);
        }
#endif

        /**
         * Move schemantics copy constructor, makes a copy of whatever was pass, and trashing it for the sake of efficiency.
         * 
         * @warning This should not be invoked directly, unless you understand what you are doing.
         * 
         * @param [in] in The DLManager object to be copied 
         */
        DLManager(DLManager &&in) {
            handle = in.handle;
            dest = in.dest;

            in.handle = nullptr;
        }

        /**
         * Normal assignment operator, you should know how this works
         * 
         * @param [in] in The DLManager object that will be assigned to the current class instance
         * 
         * @throw OpenLibraryException 
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */

        DLManager& operator=(const DLManager& in){
            dest = in.dest;
            open(dest);
            
            return *this;
        }

        /**
         * A move schemantics version of the assignment operator, does the same thing as
         * normal assignment, but it trashes the assigned object in the process.
         * 
         * @warning This should not be invoked directly, unless you understand what you are doing.
         * 
         * @param [in] in The DLManager object to be assigned to the current class instance 
         */
        DLManager& operator=(DLManager&& in){
            handle = in.handle;
            dest = in.dest;

            in.handle = nullptr;
            return *this;
        }

        //Check for C++17 support
#if __cpp_lib_filesystem >= 201703L

        /**
         * Opens the supplied dynamic library at the given path
         * (C++17 and later)
         *
         * @param [in] filepath
         *      The path to the dynamic library
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        void open(const std::filesystem::path& filepath) {
            open(filepath.string());
        }
#endif

        /**
         * Opens the supplied dynamic library
         * 
         * @param [in] filename
         *  The dynamic library to be opened
         * @throw OpenLibraryException
         *  When the library cannot be opened, an OpenLibraryException is thrown
         */
        void open(const std::string& filename){
            if(handle != nullptr){
                close();
            }
            //Clear previous errors
            dlerror();
            handle = dlopen(filename.c_str(), RTLD_LAZY);

            if(handle == nullptr){
                throw OpenLibraryException(dlerror());
            }
            dest = filename;
        }

        /**
         * Closes the previously open()'d dynamic library
         * 
         * @throw CloseLibraryException
         *  When the library cannot be closed, a CloseLibraryException is thrown 
         */
        void close(){
            if(handle == nullptr){
                return;
            }

            //Clear previous errors
            dlerror();
            int err = dlclose(handle);

            if(err != 0){
                throw CloseLibraryException(dlerror());
            }
            //Clear the dangling pointer
            handle = nullptr;
        }
        
        /**
         * Gets a function and stores it at the passed function destination
         * 
         * @tparam T The signature of the function being retrieved (the same notation as std::function)
         * @param [in] name The name of the function to be retrieved
         * @param [out] func_dest The variable to store the function
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        void getFunction(const std::string &name, std::function<T> &func_dest){
            this->getFunction<T>(name.c_str(), func_dest);
        }
        
        /**
         * Gets a function and stores it at the passed function destination
         * 
         * @tparam T The type of function being retrieved (the same notation as std::function)
         * @param [in] name The name of the function to be retrieved
         * @param [out] func_dest The variable to store the function
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        void getFunction(const char *name, std::function<T> &func_dest){
            if(this->handle == nullptr){
                throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
            }

            //Clear previous errors
            dlerror();
            func_dest = reinterpret_cast<T*>(dlsym(this->handle, name));

            if(func_dest == nullptr){
                throw NoSuchFunctionException(dlerror());
            }
        }
        
         /**
         * Gets a function and returns it
         * 
         * @tparam T The type of function being retrieved (the same notation as std::function)
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
        std::function<T> getFunction(const std::string &name){
            return this->getFunction<T>(name.c_str());
        }

        /**
         * Gets a function and returns it
         * 
         * @tparam T The type of function being retrieved (use std::function notation)
         * @param [in] name The name of the function to be retrieved
         * 
         * @return A function casted to a std::function<T>
         * 
         * @throw NoSuchFunctionException
         *  If a function cannot be found, then a NoSuchFunctionException is thrown
         * @throw NoLibraryOpenException
         *  If a dynamic library is not opened beforehand, a NoLibraryOpenException is thrown
         */
        template<typename T>
        std::function<T> getFunction(const char *name){
            if(this->handle == nullptr){
                throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
            }
            std::function<T> result;
            //Clear previous errors
            result = reinterpret_cast<T*>(dlsym(this->handle, name));

            if(result == nullptr){
                throw NoSuchFunctionException(dlerror());
            }

            return result;
        }

		/**
		 * @return The standard file suffix of the shared dynamic library for the current platform.
		 */
		static std::string getSuffix() {
#ifdef __APPLE__
			return ".dylib";
#else
			return ".so";
#endif
		}
    };
};
#endif