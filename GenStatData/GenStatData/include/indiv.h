#pragma once
#ifndef __INDIV_H__
#define __INDIV_H__
//////////////////////////////////////
#include <cassert>
#include <memory>
#include <map>
#include <vector>
///////////////////////////////////////////
#include "dbvalue.h"
//////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	template <typename U = int>
	class Indiv {
	public:
		typedef U IndexType;
		typedef std::map<IndexType, DbValue> DataTypeMap;
		typedef Indiv<IndexType> IndivType;
	private:
		IndexType m_index;
		DataTypeMap m_map;
	public:
		Indiv() :m_index(0) {}
		template <typename UX>
		Indiv(const UX aIndex) : m_index((U)aIndex) {}
		template <typename UX>
		Indiv(const UX aIndex, const std::map<UX, boost::any> &oMap) : m_index((U)aIndex) {
			DataTypeMap &m = this->m_map;
			for (auto it = oMap.begin(); it != oMap.end(); ++it) {
				U key = (U)(*it).first;
				boost::any v = (*it).second;
				m[key] = DbValue(v);
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
			return (this->m_index < other.m_index);
		}
		bool operator==(const IndivType &other) const {
			return (this->m_index == other.m_index);
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
		virtual void get_center(DataTypeMap &oMap) {
			oMap = this->m_map;
		}
		virtual void set_center(const DataTypeMap &oMap) {
			this->m_map = oMap;
		}// set_center
		double compute_distance(IndivType &other) {
			DataTypeMap oMap1, oMap2;
			this->get_center(oMap1);
			other.get_center(oMap2);
			size_t nc = 0;
			double total = 0;
			for (auto it = oMap2.begin(); it  != oMap2.end(); ++it) {
				const U key = (*it).first;
				if (oMap1.find(key) != oMap1.end()) {
					const DbValue & v1 = (*it).second;
					const DbValue & v2 = oMap1[key];
					double t = v1.double_value() - v2.double_value();
					total += t * t;
					++nc;
				}// found
			}// it
			if (nc > 1) {
				total /= nc;
			}
			return (total);
		}// compute_distance
	}; // class Indiv<U>
	////////////////////////////////////////////
	template <typename U>
	class IIndivProvider {
	public:
		typedef Indiv<U> IndivType;
		typedef std::shared_ptr<IndivType> IndivTypePtr;
		typedef std::vector<IndivTypePtr> IndivTypePtrVector;
	public:
		virtual bool is_valid(void) = 0;
		virtual bool reset(void) = 0;
		virtual bool count(int &nCount) = 0;
		virtual bool indiv_at(size_t pos, IndivTypePtr &oInd) = 0;
		virtual bool next(IndivTypePtr &oInd) = 0;
		virtual bool get_random_indivs(IndivTypePtrVector &oRes, int nCount = 1) = 0;
	}; // interface IIndivProvider<U>
	//////////////////////////////////////////
}// namespace info
//////////////////////////////////////////
#endif // !__INDIV_H__
