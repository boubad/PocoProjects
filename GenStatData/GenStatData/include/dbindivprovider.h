#pragma once
#ifndef __DBINDIVPROVIDER_H__
#define __DBINDIVPROVIDER_H__
/////////////////////////////////////
#include <vector>
#include <algorithm>
///////////////////////////////
#include "indiv.h"
#include "statdatamanager.h"
///////////////////////////////////////////
namespace info {
	//////////////////////////////////////
	template <typename U>
	class DBIndivProvider : public IIndivProvider<U> {
	public:
		typedef Indiv<U> IndivType;
		typedef std::shared_ptr<IndivType> IndivTypePtr;
		typedef std::vector<IndivTypePtr> IndivTypePtrVector;
		typedef std::vector<IntType> ints_vector;
	public:
		DBIndivProvider(StatDataManager *pMan, const std::string sigle) :m_pman(pMan), m_current(0) {
			StatDataManager *p = this->m_pman;
			assert(p != nullptr);
			assert(p->is_valid());
			DTODataset &oSet = this->m_set;
			oSet.sigle(sigle);
			if (p->find_dataset(oSet)) {
				p->find_dataset_indivs_ids(oSet, this->m_ids);
				size_t nc = this->m_ids.size();
				if (nc > 0) {
					this->m_vec.resize(nc);
				}
			}
		}//DBIndivProvider
	private:
		StatDataManager *m_pman;
		size_t m_current;
		DTODataset m_set;
		IndivTypePtrVector m_vec;
		ints_vector m_ids;
	public:
		virtual bool is_valid(void) {
			StatDataManager *p = this->m_pman;
			return ((p != nullptr) && p->is_valid() && (this->m_set.id() != 0));
		}// is_valid
		virtual bool reset(void) {
			if (this->is_valid()) {
				this->m_current = 0;
				return (true);
			}
			return (false);
		}
		virtual bool count(int &nCount) {
			if (this->is_valid()) {
				nCount = (int)this->m_ids.size();
				return (true);
			}
			return (false);
		}
		virtual bool indiv_at(size_t pos, IndivTypePtr &oInd) {
			if (this->is_valid()) {
				if (pos >= this->m_ids.size()) {
					oInd.reset();
					return (false);
				}
				IndivTypePtr o = this->m_vec[pos];
				if (o.get() == nullptr) {
					DTOIndiv xInd;
					std::map<U, boost::any> oMap;
					StatDataManager *p = this->m_pman;
					assert(p != nullptr);
					IntType nIndivId = this->m_ids[pos];
					if (!p->find_indiv(nIndivId, xInd, oMap)) {
						oInd.reset();
						return (false);
					}
					U aIndex = (U)xInd.id();
					o = std::make_shared<IndivType>(aIndex, oMap);
					this->m_vec[pos] = o;
				}
				oInd = o;
				return true;
			}
			else {
				oInd.reset();
				return (false);
			}
		}
		virtual bool next(IndivTypePtr &oInd) {
			if (this->is_valid()) {
				size_t pos = this->m_current;
				if (pos >= this->m_ids.size()) {
					oInd.reset();
					return (false);
				}
				this->m_current = this->m_current + 1;
				return this->indiv_at(pos, oInd);
			}
			return (false);
		}
		virtual bool get_random_indivs(IndivTypePtrVector &oRes, int nCount = 1) {
			oRes.clear();
			if (!this->is_valid()) {
				return (false);
			}
			const size_t nc = this->m_ids.size();
			if ((nCount < 1) && (nCount > nc)) {
				return (false);
			}
			std::vector<size_t> temp(nc);
			for (size_t i = 0; i < nc; ++i) {
				temp[i] = i;
			}
			std::random_shuffle(temp.begin(), temp.end());
			oRes.resize(nc);
			for (size_t i = 0; i < nc; ++i) {
				size_t pos = temp[i];
				IndivTypePtr oInd;
				if (!this->indiv_at(pos, oInd)) {
					return (false);
				}
				oRes[i] = oInd;
			}// i
			return (true);
		}//get_random_points
	}; // class DBIndivProvider<U>
	///////////////////////////////////////////
}// namespace info
///////////////////////////////////////
#endif // !__DBINDIVPROVIDER_H__
