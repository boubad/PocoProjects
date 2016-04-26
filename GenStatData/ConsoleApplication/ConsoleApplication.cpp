// ConsoleApplication.cpp : Defines the entry point for the console application.
//
#include "catch/catch.hpp"
/////////////////////////////////////
#include <statdatamanager.h>
#include <dbindivprovider.h>
#include <indivcluster.h>
////////////////////////////////////
#include <infotestdata.h>
//////////////////////////////////
#include <iostream>
#include <sstream>
/////////////////////////
using namespace info;
////////////////////////
TEST_CASE("StatDataManager tests", "[StatDataManager]")
{
	typedef int IndexType;
	typedef Indiv<IndexType> IndivType;
	typedef std::shared_ptr<IndivType> IndivTypePtr;
	typedef std::vector<IndivTypePtr> IndivTypePtrVector;
	typedef IIndivProvider<IndexType> IIndivProviderType;
	typedef IndivCluster<IndexType> IndivClusterType;
	//
	StatDataManager *pMan = InfoTestData::get_statmanager();
	REQUIRE(pMan != nullptr);
	REQUIRE(pMan->is_valid());
	std::string setName;
	InfoTestData::get_test_name(setName);
	REQUIRE(!setName.empty());
	//
	std::unique_ptr<DBIndivProvider<IndexType> > m_provider(new DBIndivProvider<IndexType>(pMan, setName));
	IIndivProviderType *pProvider = m_provider.get();
	REQUIRE(pProvider != nullptr);
	//
	SECTION("DBIndivProvider tests") {
		pProvider->reset();
		do {
			IndivTypePtr oInd1, oInd2;
			if (!pProvider->next(oInd1)) {
				break;
			}
			IndivType *p1 = oInd1.get();
			REQUIRE(p1 != nullptr);
			if (!pProvider->next(oInd2)) {
				break;
			}
			IndivType *p2 = oInd2.get();
			REQUIRE(p2 != nullptr);
			double dist = p1->compute_distance(*p2);
			REQUIRE(dist >= 0);
		} while (true);
		pProvider->reset();
		int nc = 5;
		IndivTypePtrVector ovec;
		bool bRet = pProvider->get_random_indivs(ovec, nc);
		REQUIRE(bRet);
		REQUIRE((int)ovec.size() == nc);
		std::vector<IndivClusterType> oClusters;
		IndexType aIndex = 1;
		for (auto it = ovec.begin(); it != ovec.end(); ++it) {
			IndivTypePtr oInd = *it;
			const IndivType *pInd = oInd.get();
			REQUIRE(pInd != nullptr);
			IndivClusterType c(aIndex++, *pInd);
			c.update_center();
			c.clear_members();
			oClusters.push_back(c);
		}// it
		size_t nbClusters = oClusters.size();
		pProvider->reset();
		do {
			IndivTypePtr oInd;
			if (!pProvider->next(oInd)) {
				break;
			}
			IndivType *p = oInd.get();
			REQUIRE(p != nullptr);
			double dMin = 0;
			size_t imin = 0;
			for (size_t i = 0; i < nbClusters; ++i) {
				IndivClusterType &c = oClusters[i];
				double d = c.compute_distance(*p);
				if (i == 0) {
					dMin = d;
					imin = i;
				}
				else if (d < dMin) {
					dMin = d;
					imin = i;
				}
			}// i
			IndivClusterType &cx = oClusters[imin];
			cx.add(*p);
		} while (true);
		pProvider->reset();
	}//DBIndivProvider tests
}//StatDataManager tests
