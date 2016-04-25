// ConsoleApplication.cpp : Defines the entry point for the console application.
//
#include "catch/catch.hpp"
/////////////////////////////////////
#include <statdatamanager.h>
//////////////////////////////////
#include <iostream>
#include <sstream>
/////////////////////////
using namespace info;
////////////////////////
TEST_CASE("StatDataManager tests", "[StatDataManager]")
{
	std::unique_ptr<StatDataManager> m_man;
	m_man.reset(new StatDataManager());
	StatDataManager *pMan = m_man.get();
	REQUIRE(pMan != nullptr);
	REQUIRE(pMan->is_valid());
	//
	std::string sigle("TESTSIGLE_SET");
	std::string name("TESTNAME_SET");
	std::string desc("TESTDESC_SET");
	std::string status("TESTSTATUS_SET");
	//
	DTODataset oSet(sigle, name, desc, status);
	bool bRet = pMan->find_dataset(oSet);
	if (!bRet) {
		bRet = pMan->maintains_dataset(oSet);
		REQUIRE(bRet);
	}
	REQUIRE(oSet.id() != 0);
	std::string namex("XXXX");
	oSet.name(namex);
	bRet = m_man->maintains_dataset(oSet);
	REQUIRE(bRet);
	std::vector<DTODataset> oVec;
	bRet = pMan->get_all_datasets(oVec);
	REQUIRE(!oVec.empty());
	//
	std::vector<DTOIndiv> oInds;
	std::string xStatus("OK");
	for (size_t i = 0; i < 512; ++i) {
		std::stringstream os;
		os << "ind" << (i + 1);
		std::string sSigle = os.str();
		DTOIndiv oInd(oSet);
		oInd.sigle(sSigle);
		oInd.name(sSigle);
		oInd.description(sSigle);
		oInd.status(xStatus);
		if (!pMan->find_indiv(oInd)) {
			oInds.push_back(oInd);
		}
	}// i
	if (!oInds.empty()) {
		bRet = pMan->maintains_indivs(oInds);
		REQUIRE(bRet);
	}
	//
	std::string vartype("float");
	std::string genre("initial");
	std::vector<DTOVariable> oVars;
	for (size_t i = 0; i < 5; ++i) {
		std::stringstream os;
		os << "var" << (i + 1);
		std::string sSigle = os.str();
		DTOVariable oVar(oSet);
		oVar.sigle(sSigle);
		oVar.name(sSigle);
		oVar.vartype(vartype);
		oVar.genre(genre);
		oVar.description(sSigle);
		oVar.status(xStatus);
		if (!pMan->find_variable(oVar)) {
			oVars.push_back(oVar);
		}
	}// i
	if (!oVars.empty()) {
		bRet = pMan->maintains_variables(oVars);
		REQUIRE(bRet);
	}
	//
	bRet = pMan->find_dataset_indivs(oSet, oInds);
	size_t nRows = oInds.size();
	REQUIRE(nRows >= 0);
	//
	int nCount = 0;
	bRet = pMan->find_dataset_indivs_count(oSet, nCount);
	REQUIRE(bRet);
	REQUIRE((size_t)nCount == nRows);
	std::vector<IntType> oIds;
	bRet = pMan->find_dataset_indivs_ids(oSet, oIds);
	REQUIRE(bRet);
	REQUIRE(oIds.size() == nRows);
	for (size_t i = 0; i < nRows; ++i) {
		IntType nIndivId = oIds[i];
		REQUIRE(nIndivId != 0);
		DTOIndiv oInd;
		std::map<IntType, std::string> oMap;
		bRet = pMan->find_indiv(nIndivId, oInd, oMap);
		REQUIRE(bRet);
		REQUIRE(oInd.id() == nIndivId);
	}// i
	//
	bRet = pMan->find_dataset_variables(oSet, oVars);
	size_t nCols = oVars.size();
	REQUIRE(nCols >= 0);
	//
	int nCountv = 0;
	bRet = pMan->find_dataset_variables_count(oSet, nCountv);
	REQUIRE(bRet);
	REQUIRE((size_t)nCountv == nCols);
	std::vector<IntType> VaroIds;
	bRet = pMan->find_dataset_variables_ids(oSet, VaroIds);
	REQUIRE(bRet);
	REQUIRE(VaroIds.size() == nCols);
	for (size_t i = 0; i < nCols; ++i) {
		IntType nVarId = VaroIds[i];
		REQUIRE(nVarId != 0);
		DTOVariable oVar;
		std::map<IntType, std::string> oMap;
		bRet = pMan->find_variable(nVarId, oVar, oMap);
		REQUIRE(bRet);
		REQUIRE(oVar.id() == nVarId);
	}// i
	//
	std::vector<DTOValue> oVals;
	for (size_t ivar = 0; ivar < nCols; ++ivar) {
		const DTOVariable &oVar = oVars[ivar];
		for (size_t irow = 0; irow < nRows; ++irow) {
			const DTOIndiv &oInd = oInds[irow];
			DTOValue val(oVar, oInd);
			if (!pMan->find_value(val)) {
				std::stringstream os;
				os << (::rand() % 100);
				std::string s = os.str();
				val.value(s);
				oVals.push_back(val);
			}
		}// irow
	}// ivar
	if (!oVals.empty()) {
		bRet = pMan->maintains_values(oVals);
		REQUIRE(bRet);
	}
	int taken = 40;
	int offset = 0;
	int nTotal = 0;
	bRet = pMan->find_dataset_values_count(oSet, nTotal);
	REQUIRE(bRet);
	REQUIRE(nTotal > 0);
	StatDataManager::strings_mapmap oMap;
	while (offset < nTotal) {
		std::vector<DTOValue> oVec;
		bRet = pMan->find_dataset_values(oSet, oVec, offset, taken);
		REQUIRE(bRet);
		int n = (int)oVec.size();
		REQUIRE(n <= taken);
		bRet = pMan->find_dataset_values(oSet, oMap, offset, taken);
		if (n < taken) {
			break;
		}
		offset += n;
	}// while
	//
	m_man.reset();
}//StatDataManager tests
