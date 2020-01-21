#ifndef __DL_MANAGER_WINDOWS_H__
#define __DL_MANAGER_WINDOWS_H__

#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//Check for C++17 support
#if __cpp_lib_filesystem >= 201703L
#include <filesystem>
#endif

#include "Exceptions.h"

typedef HMODULE SharedLib;

namespace Polysoft {
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

		std::string getLastErrMessage() {
			DWORD dLastError = GetLastError();
			LPCTSTR strErrorMessage = NULL;

			FormatMessage(
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
				NULL,
				dLastError,
				0,
				(LPSTR)&strErrorMessage,
				0,
				NULL);
			return strErrorMessage;
		}
	public:
		/**
		 * Default constructor, does not open any dynamic library
		 */
		DLManager() : handle(nullptr) {}

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
		~DLManager() {
			close();
		}

		/**
		 * Opens the supplied dynamic library
		 *
		 * @param [in] filename
		 *  The dynamic library to be opened
		 * @throw OpenLibraryException
		 *  When the library cannot be opened, an OpenLibraryException is thrown
		 */
		void open(const std::string& filename) {
			if (handle != nullptr) {
				close();
			}
			handle = LoadLibrary(filename.c_str());

			if (handle == nullptr) {
				throw OpenLibraryException(getLastErrMessage());
			}
		}

		/**
		 * Closes the previously open()'d dynamic library
		 *
		 * @throw CloseLibraryException
		 *  When the library cannot be closed, a CloseLibraryException is thrown
		 */
		void close() {
			if (handle == nullptr) {
				return;
			}

			if(!FreeLibrary(handle)) {
				throw CloseLibraryException(getLastErrMessage());
			}
			//Clear the dangling pointer
			handle = nullptr;
		}

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
		void getFunction(const std::string& name, T& func_dest) {
			this->getFunction<T>(name.c_str(), func_dest);
		}

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
		void getFunction(const char* name, T& func_dest) {
			if (handle == nullptr) {
				throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
			}

			func_dest = reinterpret_cast<T>(GetProcAddress(handle, name););

			if (func_dest == nullptr) {
				throw NoSuchFunctionException(getLastErrMessage());
			}
		}

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
		T getFunction(const std::string& name) {
			return this->getFunction<T>(name.c_str());
		}

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
		T getFunction(const char* name) {
			if (handle == nullptr) {
				throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
			}

			T result = reinterpret_cast<T>(GetProcAddress(handle, name));

			if (result == nullptr) {
				throw NoSuchFunctionException(getLastErrMessage());
			}

			return result;
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
		DLManager(const std::filesystem::path& filepath) {
			open(filepath);
		}

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
	};
};
#endif