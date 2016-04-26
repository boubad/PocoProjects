#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
//////////////////////////////////////
#include <map>
#include <set>
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
	private:
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
		bool operator<(const IndivType &other) const {
			return (this->m_index < other.m_index));
		}
		bool operator==(const IndivType &other) const {
			return (this->m_index == other.m_index));
		}
		IndexType operator()(void) const {
			return (this->m_index);
		}
	public:
		IndexType index(void) const {
			return (this->m_index);
		}
		virtual void index(const IndexType n) {
			this->m_index = n;
		}
		virtual void clear(void) {
			this->m_map.clear();
		}
		virtual void add_variable(const IndexType aVarIndex, const DataType v) {
			this->m_map[aVarIndex] = v;
		}
		virtual void get_center(DataTypeMap &oMap) {
			oMap = this->m_map;
		}
		virtual void set_center(const DataTypeMap &oMap) {
			DataTypeMap &m = this->m_map;
			m.clear();
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				U key = (U)(*it).first;
				T v = (T)(*it).second;
				m[key] = v;
			}// it
		}// set_center
		double compute_distance(const IndivType &other) {
			DataTypeMap oMap1, oMap2;
			this->get_center(oMap1);
			other.get_center(oMap2);
			size_t nc = 0;
			double total = 0;
			for (auto it = oMap2.begin(); it != oMap2.end(); ++it) {
				const U key = (*it).first;
				if (oMap1.find(key) != oMap1.end()) {
					double v1 = double)(*it).second;
					double v2 = (double)oMap1[key];
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
	public:
		typedef U IndexType;
		typedef T DataType;
		typedef std::map<IndexType, DataType> DataTypeMap;
		typedef Indiv<U, T> IndivType;
		typedef std::set<IndivType> IndivSet;
		typedef IndivCluster<IndexType, DataType> IndivClusterType;
	private:
		Poco::Mutex _mutex;
		IndivSet m_set;
	public:
		IndivCluster() {}
		template <typename UX, typename TX>
		IndivCluster(const UX aIndex, const Indiv<UX, TX> &oInd) :IndivType(aIndex, oInd) {}
		IndivCluster(const IndivClusterType &other) {
			DataTypeMap oMap;
			IndivSet oSet;
			IndexType aIndex;
			IndivClusterType &oCluster = const_cast<IndivClusterType &>(other);
			oCluster.get_index_center_members(aIndex, oMap, oSet);
			IndivType::set_center(oMap);
			IndexType::index(aIndex);
			this->m_set = oSet;
		}
		IndivCkusterType & operator=(const IndivTypeCluster &other) {
			if (this != other) {
				DataTypeMap oMap;
				IndivSet oSet;
				IndivClusterType &oCluster = const_cast<IndivClusterType &>(other);
				oCluster.get_index_center_members(aIndex, oMap, oSet);
				{
					Poco::Mutex::ScopedLock oLock(this->_mutex);
					IndivType::set_center(oMap);
					IndexType::index(aIndex);
					this->m_set = oSet;
				}
			}
			return (*this);
		}
		virtual ~IndivCluster() {}
	public:
		void get_index_center_members(IndexType &aIndex,
			DataTypeMap &oMap, IndivSet &oSet) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			aIndex = this->index();
			IndivType::get_center(oMap);
			oSet = this->m_set;
		}
		void get_members(IndivSet &oSet) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			oSet = this->m_set;
		}
		void set_members(const IndivSet &oSet) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			this->m_set = oSet;
		}
		bool empty(void) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			return (this->m_set.empty());
		}
		void add(const IndivType &oInd) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			IndivType o(oInd);
			this->m_set.insert(o);
		}// add
		void update_center(void) {
			IndivSet oSet;
			this->get_members(oSet);
			if (oSet.empty()) {
				return;
			}
			std::map<IndexType, size_t> counts;
			std::map<IndexType, double> sommes;
			for (auto it = oSet.begin(); it != oSet.end(); ++it) {
				const IndivType &oInd = *it;
				for (auto jt = oInd.begin(); jt != oInd.end(); ++jt) {
					const IndexType key = (*jt).begin();
					const double v = (double)(*jt).second;
					if (counts.find(key) == counts.end()) {
						counts[key] = 1;
						sommes[key] = v;
					}
					else if (sommes.find(key) != sommes.end()) {
						double s = sommes[key];
						s += v;
						sommes[key] = s;
						size_t n = counts[key];
						++n;
						counts[key] = n;
					}
				}// jt
			}// it
			DataTypeMap oCenter;
			for (auto it = sommes.begin(); it != sommes.end(); ++it) {
				const IndexType key = (*it).first;
				if (counts.find(key) != counts.end()) {
					size_t n = counts[key];
					double s = (*it).second;
					if (n > 1) {
						s /= n;
					}
					DataType vv = (DataType)s;
					oCenter[key] = vv;
				}//
			}// it
			this->set_center(oCenter);
		}// update_center
		void clear_members(void) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			this->m_set.clear();
		}
	public:
		virtual void index(const IndexType n) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			this->m_index = n;
		}
		virtual void add_variable(const IndexType aVarIndex, const DataType v) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			IndivType::add_variable(aVarIndex, v);
		}
		virtual void clear(void) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			IndivType::clear();
		}
		virtual void get_center(DataTypeMap &oMap) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			IndivType::get_center(oMap);
		}
		virtual void set_center(const DataTypeMap &oMap) {
			Poco::Mutex::ScopedLock oLock(this->_mutex);
			IndivType::set_center(oMap);
		}// set
	};// class IndivCluster<U,T>
	///////////////////////////////////////////
}// namespace info
//////////////////////////////////////////
#endif // !__INDIV_H__
