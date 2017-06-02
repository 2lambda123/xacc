/***********************************************************************************
 * Copyright (c) 2016, UT-Battelle
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Contributors:
 *   Initial API and implementation - Alex McCaskey
 *
 **********************************************************************************/
#ifndef XACC_UTILS_REGISTRY_HPP_
#define XACC_UTILS_REGISTRY_HPP_

#include "Singleton.hpp"
#include "Utils.hpp"
#include <map>
#include <iostream>
namespace xacc {

/**
 * Registry is a Singleton that provides a
 * mapping of string ids to creation functions that
 * create and return the provided Registry template
 * parameter T.
 *
 * Clients can add new creation functions to be placed
 * in the map with a unique name key, and can request
 * that the Registry return a new created instance of
 * the template parameter T.
 */
template<typename T, typename... TArgs>
class Registry : public Singleton<Registry<T, TArgs...>> {
protected:

	/**
	 * Reference to the database of creation functions
	 * for classes of superclass type T.
	 */
	std::map<std::string, std::function<std::shared_ptr<T>(TArgs...)>> registry;

public:

	/**
	 * Add a new creation function to the Registry, keyed
	 * on the provided string id.
	 *
	 * @param id
	 * @param f
	 * @return
	 */
	bool add(const std::string& id,
			std::function<std::shared_ptr<T>(TArgs...)> f) {
		if (registry.find(id) != registry.end()) {
			XACCInfo(id + " already exists in Registry. Ignoring and retaining previous Registry entry");
			return true;
		}
		bool s = registry.insert(std::make_pair(id, f)).second;
		if (!s) {
			XACCError("Could not add " + id + " to the Registry.");
		} else {
			return s;
		}
	}

	/**
	 * Create an instance of T by using the creation
	 * function found at the given key string id.
	 * @param id
	 * @return
	 */
	std::shared_ptr<T> create(const std::string& id, TArgs... args) {
		auto search = registry.find(id);
		if (search != registry.end()) {
			return registry[id](args...);
		} else {
			XACCError("Invalid Registry map id string - " + id);
		}
	}

	/**
	 * Return the number of creation functions
	 * this registry contains.
	 * @return
	 */
	std::size_t size() {
		return registry.size();
	}
};
}

#endif