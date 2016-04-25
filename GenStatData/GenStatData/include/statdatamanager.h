#pragma once
#ifndef __STATDATAMANAGER_H__
//////////////////////////////////
#include "info_dto.h"
/////////////////////////////
#include <memory>
#include <vector>
#include <set>
#include <map>
//////////////////////////////
#include <Poco/Types.h>
///////////////////////////
namespace Poco {
	namespace Data {
		class Session;
		class Statement;
	}// Data
}// Poco
///////////////////////////////////
namespace info {
	/////////////////////////////////////
	class StatDataManager
	{
	public:
		typedef std::map<IntType, std::string> strings_map;
		typedef std::map<IntType, strings_map> strings_mapmap;
	private:
		std::unique_ptr<Poco::Data::Session> m_session;
	private:
		static const std::string DEFAULT_SESSION_TYPE;
		static const std::string DEFAULT_CONNECTSTRING;
		void check_schema(void);
	public:
		StatDataManager(const std::string &sConnect = DEFAULT_CONNECTSTRING,
						const std::string &sType = DEFAULT_SESSION_TYPE);
		~StatDataManager();
		bool is_valid(void);
		//
		bool find_dataset(DTODataset &oSet);
		bool maintains_dataset(DTODataset &oSet);
		bool remove_dataset(const DTODataset &oSet);
		bool get_all_datasets(std::vector<DTODataset> &oVec);
		//
		bool find_indiv(DTOIndiv &oInd);
		bool find_indiv(const IntType nIndivId,DTOIndiv &oInd, std::map<IntType, std::string> &oValuesMap);
		bool find_dataset_indivs_count(DTODataset &oSet, int &nCount);
		bool find_dataset_indivs_ids(const DTODataset &oSet, std::vector<info::IntType> &oIds);
		bool find_dataset_indivs(const DTODataset &oSet, std::vector<DTOIndiv> &oInds);
		bool maintains_indivs(const std::vector<DTOIndiv> &oInds);
		bool remove_indiv(const DTOIndiv &oInd);
		//
		bool find_variable(DTOVariable &oVar);
		bool maintains_variables(const std::vector<DTOVariable> &oVars);
		bool find_variable(const IntType nVariableId, DTOVariable &oVar, std::map<IntType, std::string> &oValuesMap);
		bool find_dataset_variables_count(DTODataset &oSet, int &nCount);
		bool find_dataset_variables_ids(const DTODataset &oSet, std::vector<info::IntType> &oIds);
		bool find_dataset_variables(const DTODataset &oSet, std::vector<DTOVariable> &oVars);
		bool remove_variable(const DTOVariable &oVar);
		//
		bool find_value(DTOValue &oVal);
		bool maintains_values(const std::vector<DTOValue> &oVals);
		bool find_dataset_values_count(const DTODataset &oSet, int &nCount);
		bool find_dataset_values(const DTODataset &oSet, std::vector<DTOValue> &oVec, int skip, int taken);
		bool find_dataset_values(const DTODataset &oSet, strings_mapmap &oMap, int skip, int taken);
		bool remove_values(const std::vector<DTOValue> &oVals);
		//
		bool find_dataset_variables_types(const DTODataset &oSet, strings_map &oMap);
	}; // class 
	///////////////////////////////////////
	
}// namespace info
////////////////////////////////////
#endif // !__STATDATAMANAGER_H__
