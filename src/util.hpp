/*
 * util.hpp
 *
 *  Created on: Mar 20, 2018
 *      Author: pkomon
 */

#ifndef UTIL_HPP_
#define UTIL_HPP_

#include <vector>

namespace util{


	template<typename T>
	bool vector_contains(std::vector<T> vec, T t){
		return std::find(vec.begin(), vec.end(), t) != vec.end();
	}

}

#endif /* UTIL_HPP_ */
