// ConsoleApplication.cpp : Defines the entry point for the console application.
//
#include "catch/catch.hpp"
/////////////////////////////////////
#include <statdatamanager.h>
#include <dbindivprovider.h>
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
	typedef IIndivProvider<IndexType> IIndivProviderType;
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
	}//DBIndivProvider tests
}//StatDataManager tests
