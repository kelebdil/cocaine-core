//
// Copyright (C) 2011-2012 Rim Zaidullin <creator@bash.org.ru>
//
// Licensed under the BSD 2-Clause License (the "License");
// you may not use this file except in compliance with the License.
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef COCAINE_HELPERS_BLOB_HPP
#define COCAINE_HELPERS_BLOB_HPP

#include <boost/shared_ptr.hpp>
#include <boost/detail/atomic_count.hpp>
#include <boost/thread/mutex.hpp>

namespace cocaine { namespace helpers {

class none { };
class hash;

template<class IntegrityPolicy>
class blob_t {
    public:

    public:
        blob_t():
            m_data(NULL),
            m_size(0)
        {
            initialize();
        }

        blob_t(const void * data, size_t size):
            m_data(NULL),
            m_size(0)
        {
            initialize(data, size);
        }
        
        blob_t(const blob_t& other):
            m_data(NULL),
            m_size(0)
        {
            initialize();
            copy(other);
        }

        ~blob_t() {
            if(*m_ref_counter == 0) {
                return;
            }

            if(m_data && --*m_ref_counter == 0) {
                delete[] m_data;
            }
        }
 
        blob_t& operator=(const blob_t& other) {
            copy(other);
            return *this;
        }
        
        bool operator==(const blob_t& other) const;
        bool operator!=(const blob_t& other) const;

        const void* data() const {
            return static_cast<const void*>(m_data);
        }

        size_t size() const {
            return m_size;
        }

        bool empty() const {
            return m_size == 0;
        }
        
        void clear();

    private:
        typedef boost::detail::atomic_count reference_counter;
        
        void initialize() {
            m_ref_counter.reset(new reference_counter(0));
        }

        void initialize(const void * data, size_t size) {
            initialize();

            if(data == NULL || size == 0) {
                return;
            }

            m_data = new unsigned char[size];
            m_size = size;

            memcpy(m_data, data, size);
        
            ++*m_ref_counter;
        }

        void copy(const blob_t& other) {
            if(other.empty()) {
                return;
            }

            m_data = other.m_data;
            m_size = other.m_size;
            m_ref_counter = other.m_ref_counter;

            ++*m_ref_counter;
        }
        
    private:
        // Data
        unsigned char * m_data;
        size_t m_size;

        // Atomic reference counter
        boost::shared_ptr<reference_counter> m_ref_counter;

        // Synchronization
        boost::mutex m_mutex;
};

}

typedef helpers::blob_t<helpers::none> blob_t;

}

#endif // COCAINE_HELPERS_BLOB_HPP