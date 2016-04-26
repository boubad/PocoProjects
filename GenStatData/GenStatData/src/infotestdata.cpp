#include "../include/infotestdata.h"
////////////////////////////////
#include "../include/statdatamanager.h"
#include "../include/dbvalue.h"
//////////////////////////////////////
#include <cassert>
#include <map>
#include <iostream>
#include <sstream>
////////////////////////////////////
namespace info {
	///////////////////////////////////
	static const size_t st_conso_cols = 7;
	static const size_t st_conso_rows = 12;
	static const int st_conso_data[] = {
		332,428,354,1437,526,247,437,
		293,559,388,1527,567,239,258,
		372,767,562,1948,927,235,433,
		406,563,341,1507,544,324,407,
		386,608,396,1501,558,319,363,
		438,843,689,2345,1148,243,341,
		534,660,367,1620,638,414,407,
		460,699,484,1856,762,400,416,
		385,789,621,2366,1149,304,282,
		655,776,423,1848,759,495,486,
		584,995,548,2056,893,518,319,
		515,1097,887,2630,1187,561,284
	};
	static const size_t st_mortal_cols = 6;
	static const size_t st_mortal_rows = 19;
	static const int st_mortal_data[] = { 241,16,330,43,363,325,
	156, 9, 225,10,535,328,
	85,19,349,7,281,345,
	210,12,230,21,298,269,
	156,10,260,13,367,144,
	251, 26,180,29,387,55,
	194,11,151,13,384,122,
	225, 9, 195,26,276,128,
	54,11,219,19,224,319,
	40,136,215,18,320,43,
	241,6,168,11,230,107,
	101,5,179,23,380,9,
	82,15,155,18,342,59,
	40,4,136,17,237,225,
	104,6,138,22,346,41,
	38,7,182,32,314,37,
	89,7,169,10,218,47,
	79,10,130,14,203,36,
	121,102,220,26,273,158
	};
	////////////////////////////////////
	static const char *st_type = "float";
	static const char *st_genre = "I";
	static const char *st_status = "OK";
	static const char *st_database_filename = "testdb.sqlite";
	static const char *st_conso_name = "conso_data";
	static const char *st_conso_vars[] = { "PAIN","LEGUMES","FRUITS","VIANDES","VOLAILLES","LAIT","VIN" };
	static const char *st_conso_inds[] = { "MA2","EM2","CA2","MA3","EM3",
	"CA3","MA4","EM4","CA4","MA5","EM5","CA5" };
	///////////////////////////////////
	static const char *st_mortal_name = "mortal_data";
	static const char *st_mortal_vars[] = { "SUICI","HOMIC","AROUT","AINDU","AAUTR","CIRFO" };
	static const char *st_mortal_inds[] = { "AUTRICHE","FRANCE","PORTUGAL","ALLFEDER","BELGIQUE",
	"FINLANDE","SUEDE","SUISSE","ITALIE","IRLNORD",
	"DANNEMARK","ISLANDE","ECOSSE","ESPAGNE","NORVEGE",
	"IRLSUD","PABAS","ANGGALLE","USA" };
	///////////////////////////////////////
	std::unique_ptr<StatDataManager> InfoTestData::m_manager;
	static const char *st_test_set_name = "test_data";
	///////////////////////////////////////////
	static bool fill_random_data(StatDataManager *pMan) {
		std::string sigle(st_test_set_name);
		std::string name("TESTNAME_SET");
		std::string desc("TESTDESC_SET");
		std::string status("TESTSTATUS_SET");
		//
		DTODataset oSet(sigle, name, desc, status);
		bool bRet = pMan->find_dataset(oSet);
		if (!bRet) {
			if (!pMan->maintains_dataset(oSet)) {
				return (false);
			}
		}
		assert(oSet.id() != 0);
		//
		std::vector<DTOIndiv> oInds;
		std::string xStatus("OK");
		for (size_t i = 0; i < 1024; ++i) {
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
			if (!pMan->maintains_indivs(oInds)) {
				return (false);
			}
		}
		//
		std::string vartype("float");
		std::string genre("initial");
		std::vector<DTOVariable> oVars;
		for (size_t i = 0; i < 10; ++i) {
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
			if (!pMan->maintains_variables(oVars)) {
				return (false);
			}
		}
		//
		bRet = pMan->find_dataset_indivs(oSet, oInds);
		bRet = bRet && pMan->find_dataset_variables(oSet, oVars);
		if (!bRet) {
			return (false);
		}
		size_t nCols = oVars.size();
		size_t nRows = oInds.size();
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
			if (!pMan->maintains_values(oVals)) {
				return (false);
			}
		}
		return (true);
	}//fill_random_data
	//////////////////////////////////////////
	static bool import_data(StatDataManager *pMan, const std::string &setName,
		const std::vector<std::string> &rowNames, const std::vector<std::string> &colNames,
		std::vector<int> &gdata) {
		DTODataset oSet;
		oSet.sigle(setName);
		bool bRet = pMan->find_dataset(oSet);
		if (!bRet) {
			oSet.name(setName);
			oSet.description(setName);
			oSet.status(st_status);
			if (!pMan->maintains_dataset(oSet)) {
				return (false);
			}
			if (!pMan->find_dataset(oSet)) {
				return (false);
			}
		}
		std::vector<DTOIndiv> oInds;
		const size_t nRows = rowNames.size();
		for (size_t i = 0; i < nRows; ++i) {
			std::string sigle = rowNames[i];
			DTOIndiv oInd(oSet);
			oInd.sigle(sigle);
			if (!pMan->find_indiv(oInd)) {
				oInd.name(sigle);
				oInd.description(sigle);
				oInds.push_back(oInd);
			}
		}// i
		if (!oInds.empty()) {
			if (!pMan->maintains_indivs(oInds)) {
				return (false);
			}
		}
		std::vector<DTOVariable> oVars;
		const size_t nCols = colNames.size();
		for (size_t i = 0; i < nCols; ++i) {
			std::string sigle = colNames[i];
			DTOVariable oVar(oSet);
			oVar.sigle(sigle);
			if (!pMan->find_variable(oVar)) {
				oVar.vartype(st_type);
				oVar.genre(st_genre);
				oVar.name(sigle);
				oVar.description(sigle);
				oVars.push_back(oVar);
			}
		}// i
		if (!oVars.empty()) {
			if (!pMan->maintains_variables(oVars)) {
				return (false);
			}
		}
		bRet = pMan->find_dataset_indivs(oSet, oInds) && pMan->find_dataset_variables(oSet, oVars);
		if (!bRet) {
			return (false);
		}
		std::map<std::string, DTOVariable *> oVarsMap;
		for (size_t i = 0; i < oVars.size(); ++i) {
			DTOVariable *pVar = &(oVars[i]);
			std::string key = pVar->sigle();
			oVarsMap[key] = pVar;
		}// i
		std::map<std::string, DTOIndiv *> oIndsMap;
		for (size_t i = 0; i < oInds.size(); ++i) {
			DTOIndiv *pInd = &(oInds[i]);
			std::string key = pInd->sigle();
			oIndsMap[key] = pInd;
		}// i
		std::vector<DTOValue> oVals;
		for (size_t i = 0; i < oVars.size(); ++i) {
			std::string s1 = colNames[i];
			DTOVariable *pVar = oVarsMap[s1];
			for (size_t j = 0; j < oInds.size(); ++j) {
				std::string s2 = rowNames[i];
				DTOIndiv *pInd = oIndsMap[s2];
				DTOValue vv(*pVar, *pInd);
				if (!pMan->find_value(vv)) {
					float fval = (float)gdata[j * nCols + i];
					DbValue vx(fval);
					std::string sval;
					vx.string_value(sval);
					vv.value(sval);
					oVals.push_back(vv);
				}
			}// j
		}// i
		if (!oVals.empty()) {
			if (!pMan->maintains_values(oVals)) {
				return (false);
			}
		}
		return (true);
	}// import_data
	///////////////////////////////////////
	StatDataManager *InfoTestData::get_statmanager(void) {
		StatDataManager *pMan = m_manager.get();
		if (pMan != nullptr) {
			return (pMan);
		}
		std::string name(st_database_filename);
		m_manager.reset(new StatDataManager(name));
		pMan = m_manager.get();
		assert(pMan != nullptr);
		assert(pMan->is_valid());
		std::vector<std::string> indNames, varNames;
		std::vector<int> data;
		size_t nCols, nRows;
		std::string setName;
		get_conso_data(setName, nRows, nCols, data, indNames, varNames);
		bool bRet = import_data(pMan, setName, indNames, varNames, data);
		assert(bRet);
		get_mortal_data(setName, nRows, nCols, data, indNames, varNames);
		bRet = import_data(pMan, setName, indNames, varNames, data);
		assert(bRet);
		bRet = fill_random_data(pMan);
		assert(bRet);
		return pMan;
	}//get_statmanager
	///////////////////////////////////////
	void InfoTestData::get_test_name(std::string &name) {
		name = st_test_set_name;
	}
	void InfoTestData::get_mortal_name(std::string &name) {
		name = st_mortal_name;
	}
	void InfoTestData::get_conso_name(std::string &name) {
		name = st_conso_name;
	}
	void InfoTestData::get_default_type_genre_status(std::string &type, std::string &genre, std::string &status) {
		type = st_type;
		genre = st_genre;
		status = st_status;
	}//get_default_type_genre_status
	void InfoTestData::get_database_filename(std::string &filename) {
		filename = st_database_filename;
	}// get_database_filename
	 ///////////////////////////////////////
	void InfoTestData::get_mortal_data(std::string &name, size_t &nRows, size_t &nCols,
		std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames) {
		name = st_mortal_name;
		nRows = st_mortal_rows;
		assert(nRows > 1);
		nCols = st_mortal_cols;
		assert(nCols > 1);
		const size_t n = (size_t)(nCols * nRows);
		data.resize(n);
		assert(data.size() == n);
		indNames.resize(nRows);
		assert(indNames.size() == nRows);
		varNames.resize(nCols);
		assert(varNames.size() == nCols);
		const int *pData = &(st_mortal_data[0]);
		for (size_t i = 0; i < nCols; ++i) {
			varNames[i] = st_mortal_vars[i];
		}// i
		for (size_t i = 0; i < nRows; ++i) {
			indNames[i] = st_mortal_inds[i];
			for (size_t j = 0; j < nCols; ++j) {
				const size_t pos = (size_t)(i * nCols + j);
				data[pos] = pData[pos];
			}// j
		}// i
	}//get_conso_data
	void InfoTestData::get_conso_data(std::string &name, size_t &nRows, size_t &nCols,
		std::vector<int> &data, std::vector<std::string> &indNames, std::vector<std::string> &varNames) {
		name = st_conso_name;
		nRows = st_conso_rows;
		assert(nRows > 1);
		nCols = st_conso_cols;
		assert(nCols > 1);
		const size_t n = (size_t)(nCols * nRows);
		data.resize(n);
		assert(data.size() == n);
		indNames.resize(nRows);
		assert(indNames.size() == nRows);
		varNames.resize(nCols);
		assert(varNames.size() == nCols);
		const int *pData = &(st_conso_data[0]);
		for (size_t i = 0; i < nCols; ++i) {
			varNames[i] = st_conso_vars[i];
		}// i
		for (size_t i = 0; i < nRows; ++i) {
			indNames[i] = st_conso_inds[i];
			for (size_t j = 0; j < nCols; ++j) {
				const size_t pos = (size_t)(i * nCols + j);
				data[pos] = pData[pos];
			}// j
		}// i
	}//get_conso_data
	 ////////////////////////////////////
}// namespace info
