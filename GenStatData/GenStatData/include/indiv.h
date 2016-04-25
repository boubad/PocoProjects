#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
//////////////////////////////////////
#include <map>
///////////////////////////////////////////
#include <Poco/Mutex.h>
//////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	template <typename U, typename T>
	class Indiv {
	public:
		typedef U IndexType;
		typedef T DataType;
		typedef std::map<IndexType, DataType> DataTypeMap;
		typedef Indiv<IndexType, DataType> IndivType;
		typedef typename DataTypeMap::iterator iterator;
		typedef typename DataTypeMap::const_iterator const_iterator;
	protected:
		IndexType m_index;
		DataTypeMap m_map;
	public:
		Indiv() :m_index(0) {}
		template <typename UX>
		Indiv(const UX aIndex) : m_index((U)aIndex) {}
		template <typename UX, typename TX>
		Indiv(const UX aIndex, const std::map<UX, TX> &oMap) : m_index((U)aIndex) {
			DataTypeMap &m = this->m_map;
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				U key = (U)(*it).first;
				T v = (T)(*it).second;
				m[key] = v;
			}// it
		}
		Indiv(const IndivType &other) :m_index(other.m_index), m_map(other.m_map) {}
		IndivType & operator=(const IndivType &other) {
			if (this != &other) {
				this->m_index = other.m_index;
				this->m_map = other.m_map;
			}
			return (*this);
		}
		virtual ~Indiv() {}
	public:
		virtual void clear(void) {
			this->m_map.clear();
		}
		virtual void set(const DataTypeMap &oMap) {
			DataTypeMap &m = this->m_map;
			m.clear();
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				U key = (U)(*it).first;
				T v = (T)(*it).second;
				m[key] = v;
			}// it
		}// set
		iterator begin(void) {
			return (this->m_map.begin());
		}
		const_iterator begin(void) const {
			return (this->m_map.begin());
		}
		iterator end(void) {
			return (this->m_map.end());
		}
		const_iterator end(void) const {
			return (this->m_map.end());
		}
		bool has_variable(const IndexType nVarId) const {
			return (this->m_map.find(nVarId) != this->m_map.end());
		}// has_variable
		virtual double compute_distance(const IndivType &other)  {
			size_t nc = 0;
			const DataTypeMap &oMap = this->m_map;
			double total = 0;
			for (auto it = other.begin(); it != other.end(); ++it) {
				const U key = (*it).first;
				if (oMap.find(key) != oMap.end()) {
					double v1 = double)(*it).second;
					double v2 = (double)oMap[key];
					double t = v1 - v2;
					total += t * t;
					++nc;
				}// found
			}// it
			if (nc > 1) {
				total /= nc;
			}
			return (total);
		}// compute_distance
	}; // class Indiv<U,T>
	/////////////////////////////////////////////
	template <typename U, typename T>
	class IndivCluster : public Indiv<U, T> {
	private:
		Poco::Mutex _mutex;
	protected:

	public:
		virtual void clear(void) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			Indiv<T, U>::clear();
		}
		virtual void set(const DataTypeMap &oMap) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			Indiv<T, U>::set(oMap);
		}// set
		virtual double compute_distance(const IndivType &other, W &result) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			return Indiv<U, T>::compute_distance(other);
		}// compute_distance
	};// class IndivCluster<U,T>
	///////////////////////////////////////////
}// namespace info
//////////////////////////////////////////
#endif // !__INDIV_H__
