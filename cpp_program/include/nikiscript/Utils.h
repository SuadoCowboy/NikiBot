#pragma once

#include <stdint.h>
#include <string>
#include <sstream>

namespace ns {
	/// @warning this function is not meant to be used outside this header
	template<typename ...>
	static void _formatStringValue(const std::string& format, std::stringstream& buf) {
		buf << format;
	}

	/// @warning this function is not meant to be used outside this header
	template<typename T, typename ... Args>
	static void _formatStringValue(const std::string& format, std::stringstream& buf, T value, Args& ... args) {
		uint64_t idx = format.find("{}");
		if (idx == std::string::npos) {
			buf << format;
			return;
		}

		buf << format.substr(0, idx) << value;
		
		_formatStringValue(format.substr(idx+2), buf, args...);
	}

	template<typename ... Args>
	std::string formatString(const std::string& format, Args ... args) {
		std::stringstream buf;
		_formatStringValue(format, buf, args...);
		return buf.str();
	}

	/**
	  * @brief trim from start
	  * 
	  * @param s string to trim the beginning
	  * 
	  * @see ns::trim
	  * @see ns::rtrim
	  * 
	  * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	  */
	void ltrim(std::string &s);
	
	/**
	 * @brief trim from end
	 * 
	 * @param s string to trim the end
	 * 
	 * @see ns::trim
	 * @see ns::ltrim
	 * 
	 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	 */
	void rtrim(std::string &s);
	
	/**
	 * @brief trim from start and end
	 * 
	 * @param s string to trim
	 * 
	 * @see ns::ltrim
	 * @see ns::rtrim
	 * 
	 * @note https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
	 */
	void trim(std::string& s);
}