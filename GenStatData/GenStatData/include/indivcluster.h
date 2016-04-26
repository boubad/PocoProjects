#pragma once
#ifndef __INDIVCLUSTER_H__
#define __INDIVCLUSTER_H__
/////////////////////////////////////
#include <set>
///////////////////////////
#include <Poco/RWLock.h>
///////////////////////////////////////////
#include "indiv.h"
//////////////////////////////////////////
namespace info {
	//////////////////////////////////////////
	template <typename U = int>
	class IndivCluster : public Indiv<U> {
	public:
		typedef U IndexType;
		typedef std::map<IndexType, DbValue> DataTypeMap;
		typedef Indiv<U> IndivType;
		typedef std::set<IndivType> IndivSet;
		typedef IndivCluster<IndexType> IndivClusterType;
	private:
		Poco::RWLock _mutex;
		IndivSet m_set;
	public:
		IndivCluster() {}
		IndivCluster(const U aIndex, const Indiv<U> &oInd) :IndivType(aIndex) {
			this->m_set.insert(oInd);
		}
		IndivCluster(const IndivClusterType &other) {
			DataTypeMap oMap;
			IndivSet oSet;
			IndexType aIndex;
			IndivClusterType &oCluster = const_cast<IndivClusterType &>(other);
			oCluster.get_index_center_members(aIndex, oMap, oSet);
			IndivType::set_center(oMap);
			IndivType::index(aIndex);
			this->m_set = oSet;
		}
		IndivClusterType & operator=(const IndivClusterType &other) {
			if (this != other) {
				DataTypeMap oMap;
				IndivSet oSet;
				IndivClusterType &oCluster = const_cast<IndivClusterType &>(other);
				oCluster.get_index_center_members(aIndex, oMap, oSet);
				{
					Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
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
			Poco::RWLock::ScopedReadLock oLock(this->_mutex);
			aIndex = IndivType::index();
			IndivType::get_center(oMap);
			oSet = this->m_set;
		}
		void get_members(IndivSet &oSet) {
			Poco::RWLock::ScopedReadLock oLock(this->_mutex);
			oSet = this->m_set;
		}
		void set_members(const IndivSet &oSet) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
			this->m_set = oSet;
		}
		bool empty(void) {
			Poco::RWLock::ScopedReadLock oLock(this->_mutex);
			return (this->m_set.empty());
		}
		void add(const IndivType &oInd) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
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
				IndivType &oInd = const_cast<IndivType &>(*it);
				DataTypeMap m;
				oInd.get_center(m);
				for (auto jt = m.begin(); jt != m.end(); ++jt) {
					IndexType key = (*jt).first;
					DbValue & vv = (*jt).second;
					double v = vv.double_value();
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
					DbValue vv(s);
					oCenter[key] = vv;
				}//
			}// it
			this->set_center(oCenter);
		}// update_center
		void clear_members(void) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
			this->m_set.clear();
		}
	public:
		virtual void index(const IndexType n) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
			IndivType::index(n);
		}
		virtual void clear(void) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
			IndivType::clear();
		}
		virtual void get_center(DataTypeMap &oMap) {
			Poco::RWLock::ScopedReadLock oLock(this->_mutex);
			IndivType::get_center(oMap);
		}
		virtual void set_center(const DataTypeMap &oMap) {
			Poco::RWLock::ScopedWriteLock oLock(this->_mutex);
			IndivType::set_center(oMap);
		}// set
	};// class IndivCluster<U,T>
	  ///////////////////////////////////////////
}// namespace info
///////////////////////////////////////
#endif // !__INDIVCLUSTER_H__
