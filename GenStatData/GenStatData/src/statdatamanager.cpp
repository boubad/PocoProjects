
#include "Poco/SharedPtr.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/Session.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/Recordset.h"
#include "Poco/Data/TypeHandler.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/ODBC/Connector.h"
#include "Poco/Mutex.h"
#include "Poco/Exception.h"
//////////////////////////////////////////
#include <vector>
#include <iostream>
//////////////////////////////
#include "..\include\statdatamanager.h"
#include "stat_dto_typehandlers.h"
///////////////////////////////////
namespace info {
	//
	static const char *SQL_CREATE_DATASET =
		"CREATE TABLE IF NOT EXISTS dbdataset("
		" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" sigle TEXT NOT NULL UNIQUE,"
		" nom TEXT DEFAULT NULL,"
		" description TEXT DEFAULT NULL,"
		" status TEXT NULL"
		" )";
	static const char *SQL_FIND_DATASET_BY_ID =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset WHERE datasetid = ?";
	static const char *SQL_FIND_DATASET_BY_SIGLE =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset WHERE UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_DATASET =
		"INSERT INTO dbdataset (sigle,nom,description,status)"
		" VALUES (?,?,?,?)";
	static const char *SQL_UPDATE_DATASET =
		"UPDATE dbdataset SET optlock = optlock + 1,"
		" sigle = ?, nom = ?, description = ?, status = ? WHERE datasetid = ?";
	static const char *SQL_REMOVE_DATASET =
		"DELETE FROM dbdataset WHERE datasetid = ?";
	static const char *SQL_FIND_ALL_DATASETS =
		"SELECT datasetid,optlock,sigle,nom,description,status"
		" FROM dbdataset ORDER BY sigle ASC";
	//////////////////////////////////////////////
	static const char *SQL_CREATE_INDIV =
		"CREATE TABLE IF NOT EXISTS dbindiv("
		" individ INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" datasetid INTEGER NOT NULL,"
		" sigle TEXT NOT NULL,"
		" nom TEXT  NULL,"
		" description TEXT  NULL,"
		" status TEXT NULL,"
		" CONSTRAINT uc_indiv UNIQUE (datasetid, sigle),"
		" CONSTRAINT fk_indiv_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
		" )";
	static const char *SQL_DATASET_INDIVS_IDS =
		"SELECT individ"
		" FROM dbindiv WHERE datasetid = ?";
	static const char *SQL_FIND_DATASET_INDIVS =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
		" FROM dbindiv WHERE datasetid = ?";
	static const char *SQL_INDIV_BY_ID =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
		" FROM dbindiv WHERE individ = ?";
	static const char *SQL_INDIV_BY_DATASET_AND_SIGLE =
		"SELECT individ,optlock,datasetid,sigle,nom,description,status"
		" FROM dbindiv WHERE datasetid = ? AND UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_INDIV =
		"INSERT INTO dbindiv (datasetid,sigle,nom,description,status)"
		" VALUES(?,?,?,?, ?)";
	static const char *SQL_UPDATE_INDIV =
		"UPDATE dbindiv SET optlock = OPTLOCK + 1,"
		" sigle = ?, nom = ?, description = ?, status = ? WHERE individ = ?";
	static const char *SQL_REMOVE_INDIV =
		"REMOVE FROM dbindiv WHERE individ = ?";
	static const char *SQL_FIND_DATASET_INDIVS_COUNT =
		"SELECT COUNT(*) FROM dbindiv"
		" WHERE datasetid = ?";
	static const char *SQL_FIND_INDIV_VALUES =
		"SELECT variableid,stringval FROM dbvalue WHERE individ = ?";
	////////////////////////////////////////////////////////////
	static const char *SQL_CREATE_VARIABLE =
		"CREATE TABLE IF NOT EXISTS dbvariable("
		" variableid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" datasetid INTEGER NOT NULL,"
		" sigle TEXT NOT NULL,"
		" vartype TEXT NOT NULL,"
		" genre TEXT NULL,"
		" categvar INTEGER NOT NULL DEFAULT 0,"
		" nom TEXT NULL,"
		" description TEXT NULL,"
		" status TEXT NULL,"
		" CONSTRAINT uc_variable UNIQUE (datasetid, sigle),"
		" CONSTRAINT fk_variable_dataset FOREIGN KEY (datasetid) REFERENCES dbdataset (datasetid) ON DELETE CASCADE"
		" )";
	static const char *SQL_DATASET_VARIABLES_IDS =
		"SELECT variableid"
		" FROM dbvariable WHERE datasetid = ?";
	static const char *SQL_FIND_DATASET_VARIABLES =
		"SELECT variableid,optlock,datasetid,sigle,vartype,genre,categvar,nom,description,status"
		" FROM dbvariable WHERE datasetid = ?";
	static const char *SQL_VARIABLE_BY_ID =
		"SELECT variableid,optlock,datasetid,sigle,vartype,genre,categvar,nom,description,status"
		" FROM dbvariable WHERE variableid = ?";
	static const char *SQL_VARIABLE_BY_DATASET_AND_SIGLE =
		"SELECT variableid,optlock,datasetid,sigle,vartype,genre,categvar,nom,description,status"
		" FROM dbvariable WHERE datasetid = ? AND UPPER(LTRIM(RTRIM(sigle))) = ?";
	static const char *SQL_INSERT_VARIABLE =
		"INSERT INTO dbvariable (datasetid,sigle,vartype,genre,categvar,nom,description,status)"
		" VALUES (?, ?,?,?,?,?,?,?)";
	static const char *SQL_UPDATE_VARIABLE =
		"UPDATE dbvariable SET optlock = optlock + 1,"
		" sigle = ?, vartype = ?, genre = ?, catgegvar = ?, nom = ?, description = ?, status = ? WHERE variableid = ?";
	static const char *SQL_REMOVE_VARIABLE =
		"DELETE FROM dbvariable WHERE variableid = ?";
	static const char *SQL_FIND_DATASET_VARIABLES_COUNT =
		"SELECT COUNT(*) FROM dbvariable"
		" WHERE datasetid = ?";
	static const char *SQL_FIND_VARIABLE_VALUES =
		"SELECT individ,stringval FROM dbvalue WHERE variableid = ?";
	///////////////////////////////////////////////////////////////////////
	static const char *SQL_CREATE_VALUE =
		"CREATE TABLE IF NOT EXISTS dbvalue("
		" valueid INTEGER PRIMARY KEY AUTOINCREMENT,"
		" optlock INTEGER NOT NULL DEFAULT 1,"
		" variableid INTEGER NOT NULL,"
		" individ INTEGER NOT NULL,"
		" stringval TEXT NULL,"
		" status TEXT NULL,"
		" CONSTRAINT uc_value UNIQUE (variableid, individ),"
		" CONSTRAINT fk_value_variable FOREIGN KEY (variableid) REFERENCES dbvariable (variableid) ON DELETE CASCADE,"
		" CONSTRAINT fk_value_indiv FOREIGN KEY (individ) REFERENCES dbindiv (individ) ON DELETE CASCADE"
		" )";
	static const char *SQL_DELETE_VARIABLE_VALUES =
		"DELETE FROM dbvalue WHERE variableid = ?";
	static const char *SQL_DELETE_INDIV_VALUES =
		"DELETE FROM dbvalue WHERE individ = ?";
	static const char *SQL_FIND_VARIABLE_TYPE =
		"SELECT vartype FROM dbvariable WHERE variableid = ?";
	static const char *SQL_FIND_DATASET_VALUES_COUNT = "SELECT COUNT(*)"
		" FROM dbvalue a, dbvariable b"
		" WHERE a.variableid = b.variableid AND b.datasetid = ?";
	static const char *SQL_FIND_DATASET_VALUES =
		"SELECT a.valueid,a.optlock,a.variableid,a.individ,a.stringval,a.status"
		" FROM dbvalue a, dbvariable b"
		" WHERE a.variableid = b.variableid AND b.datasetid = ?"
		" ORDER BY a.variableid ASC, a.individ ASC"
		" LIMIT ? OFFSET ?";
	static const char *SQL_VALUES_BY_VARIABLE_INDIV =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE variableid = ? AND individ = ?";
	static const char *SQL_VALUE_BY_ID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE valueid = ?";
	static const char *SQL_INSERT_VALUE =
		"INSERT INTO dbvalue (variableid,individ,stringval,status)"
		" VALUES(?,?,?,?)";
	static const char *SQL_UPDATE_VALUE =
		"UPDATE dbvalue SET optlock = optlock + 1,"
		" stringval = ?, status = ? WHERE valueid = ? ";
	static const char *SQL_REMOVE_VALUE =
		"DELETE from dbvalue WHERE valueid = ?";
	static const char *SQL_VALUES_BY_VARIABLEID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE variableid = ?";
	static const char *SQL_VALUES_BY_INDIVID =
		"SELECT valueid,optlock,variableid,individ,stringval,status"
		" FROM dbvalue WHERE individ = ?";
	////////////////////////////////////////
	static const char *CREATE_SQL[] = {
		SQL_CREATE_DATASET,
		SQL_CREATE_VARIABLE,
		SQL_CREATE_INDIV,
		SQL_CREATE_VALUE,
		nullptr };
	//////////////////////////////////////////
	//
	using namespace Poco;
	using namespace Poco::Data;
	using namespace Poco::Data::Keywords;
	//
	static const char *SZ_SQLITE = "SQLite";
	static const char *SZ_ODBC = "ODBC";
	//
	const std::string StatDataManager::DEFAULT_SESSION_TYPE(SZ_SQLITE);
	const std::string StatDataManager::DEFAULT_CONNECTSTRING("statdatasets.sqlite");
	static std::set<std::string> st_types;
	static std::string TYPE_SQLITE(SZ_SQLITE);
	static std::string TYPE_ODBC(SZ_ODBC);
	Poco::Mutex st_mutex;
	///////////////////////////////////////////////
	bool StatDataManager::remove_dataset(const DTODataset &oSet) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		bool bInTrans = false;
		Session & sess = *(this->m_session);
		try {
			Poco::UInt64 nId = oSet.id();
			if (nId == 0) {
				return (false);
			}
			std::vector<Poco::UInt64> oVarIds, oIndIds;
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			//
			std::string sqlVarIds(SQL_DATASET_VARIABLES_IDS);
			sess << sqlVarIds, use(nId), into(oVarIds), now;
			std::string sqlIndIds(SQL_DATASET_INDIVS_IDS);
			sess << sqlIndIds, use(nId), into(oIndIds), now;
			if (!oVarIds.empty()) {
				std::string sql(SQL_DELETE_VARIABLE_VALUES);
				sess << sql, use(oVarIds), now;
				std::string sqlP(SQL_REMOVE_VARIABLE);
				sess << sqlP, use(oVarIds), now;
			}// vars
			if (!oIndIds.empty()) {
				std::string sql(SQL_DELETE_INDIV_VALUES);
				sess << sql, use(oIndIds), now;
				std::string sqlP(SQL_REMOVE_INDIV);
				sess << sqlP, use(oIndIds), now;
			}// inds
			{
				std::string sql(SQL_REMOVE_DATASET);
				sess << sql, use(nId), now;
			}
			//
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}// remove_dataset
	bool StatDataManager::remove_indiv(const DTOIndiv &oInd) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			DTOIndiv xVar(oInd);
			if (!this->find_indiv(xVar)) {
				return(false);
			}
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nId = xVar.id();
			std::string sqlvalues(SQL_DELETE_INDIV_VALUES);
			sess << sqlvalues, use(nId), now;
			std::string sql(SQL_REMOVE_INDIV);
			sess << sql, use(nId), now;
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}//remove_indiv
	bool StatDataManager::remove_variable(const DTOVariable &oVar) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			DTOVariable xVar(oVar);
			if (!this->find_variable(xVar)) {
				return(false);
			}
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nId = xVar.id();
			std::string sqlvalues(SQL_DELETE_VARIABLE_VALUES);
			sess << sqlvalues, use(nId),now;
			std::string sql(SQL_REMOVE_VARIABLE);
			sess << sql, use(nId), now;
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}//remove_variable
	bool StatDataManager::remove_values(const std::vector<DTOValue> &oVals) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nId;
			std::string sql(SQL_REMOVE_VALUE);
			Statement stmtRemove = (sess << sql, use(nId));
			for (auto it = oVals.begin(); it != oVals.end(); ++it) {
				const DTOValue &oVal = *it;
				DTOValue  xVar(oVal);
				if (this->find_value(xVar)) {
					nId = xVar.id();
					if (nId != 0) {
						stmtRemove.execute();
					}
				}
			}// it
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}// remove_values
	bool StatDataManager::find_dataset_values(const DTODataset &oSet, strings_mapmap &oMap, int skip, int taken) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			std::vector<DTOValue> oVec;
			Poco::UInt64 offset = skip;
			Poco::UInt64 limit = taken;
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_VALUES);
			sess << sql, use(nDatasetId), use(limit), use(offset), into(oVec), now;
			for (auto it = oVec.begin(); it != oVec.end(); ++it) {
				const DTOValue &v = *it;
				const IntType nVarId = v.variable_id();
				const IntType nIndId = v.indiv_id();
				std::string sval = Poco::trim(v.value());
				if (sval.empty()) {
					continue;
				}
				auto jt = oMap.find(nIndId);
				if (jt == oMap.end()) {
					strings_map m;
					m[nVarId] = sval;
					oMap[nIndId] = m;
				}
				else {
					strings_map & m = (*jt).second;
					m[nVarId] = sval;
				}
			}// it
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_values
	bool  StatDataManager::find_dataset_values(const DTODataset &oSet, std::vector<DTOValue> &oVec, int skip, int taken) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oVec.clear();
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Poco::UInt64 offset = skip;
			Poco::UInt64 limit = taken;
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_VALUES);
			sess << sql, use(nDatasetId), use(limit), use(offset), into(oVec), now;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_values
	bool StatDataManager::find_dataset_values_count(const DTODataset &oSet, int &nCount) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			nCount = 0;
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_VALUES_COUNT);
			Poco::UInt64 nn = 0;
			sess << sql, use(nDatasetId), into(nn), now;
			nCount = (int)nn;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_values_count
	bool StatDataManager::maintains_values(const std::vector<DTOValue> &oVals) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nVarId, nIndId, nId;
			std::string status, sval;
			std::string sqlInsert(SQL_INSERT_VALUE);
			std::string sqlUpdate(SQL_UPDATE_VALUE);
			Statement stmtInsert = (sess << sqlInsert, use(nVarId), use(nIndId), use(sval), use(status));
			Statement stmtUpdate = (sess << sqlUpdate, use(sval), use(status), use(nId));
			for (auto it = oVals.begin(); it != oVals.end(); ++it) {
				const DTOValue &oVal = *it;
				if (oVal.is_writeable()) {
					DTOValue  xVar(oVal);
					this->find_value(xVar);
					nId = xVar.id();
					nVarId = xVar.variable_id();
					nIndId = xVar.indiv_id();
					sval = oVal.value();
					status = oVal.status();
					if (nId != 0) {
						stmtUpdate.execute();
					}
					else {
						stmtInsert.execute();
					}
				}// writeable
			}// it
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}//maintains_values
	bool StatDataManager::find_value(DTOValue &oVal) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = oVal.id();
			if (nId != 0) {
				std::string sql(SQL_VALUE_BY_ID);
				DTOValue xVar;
				poco_assert_dbg(xVar.id() == 0);
				sess << sql, use(nId), into(xVar), now;
				if (xVar.id() != 0) {
					oVal = xVar;
					return (true);
				}
			}
			else {
				Poco::UInt64 nVarId = oVal.variable_id();
				Poco::UInt64 nInd = oVal.indiv_id();
				if ((nVarId == 0) || (nInd == 0)) {
					return (false);
				}
				std::string sql(SQL_VALUES_BY_VARIABLE_INDIV);
				DTOValue xVar;
				poco_assert_dbg(xVar.id() == 0);
				sess << sql, use(nVarId), use(nInd), into(xVar), now;
				if (xVar.id() != 0) {
					oVal = xVar;
					return (true);
				}
			}
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}// find_value
	//////////////////////////////////////////////
	bool  StatDataManager::find_variable(const IntType nVariableId, DTOVariable &oVar, std::map<IntType, std::string> &oValuesMap) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oValuesMap.clear();
			if (nVariableId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = nVariableId;
			{
				std::string sql(SQL_VARIABLE_BY_ID);
				DTOVariable xVar;
				poco_assert_dbg(xVar.id() == 0);
				sess << sql, use(nId), into(xVar), now;
				if (xVar.id() == 0) {
					return (false);
				}
				oVar = xVar;
			}
			{
				std::vector<VariableIdValue > oVec;
				std::string sql(SQL_FIND_VARIABLE_VALUES);
				sess << sql, use(nId), into(oVec), now;
				for (auto it = oVec.begin(); it != oVec.end(); ++it) {
					IntType key = (IntType)(*it).first;
					std::string val = (*it).second;
					oValuesMap[key] = val;
				}// it
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
		}
		return (bRet);
	}//find_variable
	bool StatDataManager::find_dataset_variables_count(DTODataset &oSet, int &nCount) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			nCount = 0;
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_VARIABLES_COUNT);
			Poco::UInt64 nn = 0;
			sess << sql, use(nDatasetId), into(nn), now;
			nCount = (int)nn;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_variables_count
	bool StatDataManager::find_dataset_variables(const DTODataset &oSet, std::vector<DTOVariable> &oVars) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oVars.clear();
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_VARIABLES);
			sess << sql, use(nDatasetId), into(oVars), now;
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
		}
		return (bRet);
	}//find_dataset_variables
	bool StatDataManager::find_dataset_variables_ids(const DTODataset &oSet, std::vector<info::IntType> &oIds) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oIds.clear();
			Poco::UInt64  nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_DATASET_VARIABLES_IDS);
			sess << sql, use(nDatasetId), into(oIds), now;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_variables_ids
	bool StatDataManager::maintains_variables(const std::vector<DTOVariable> &oVars) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nDatasetId, nId, nCateg;
			std::string sigle, name, description, status, genre, vartype;
			std::string sqlInsert(SQL_INSERT_VARIABLE);
			std::string sqlUpdate(SQL_UPDATE_VARIABLE);
			Statement stmtInsert = (sess << sqlInsert, use(nDatasetId), use(sigle), use(vartype), use(genre), use(nCateg), use(name), use(description), use(status));
			Statement stmtUpdate = (sess << sqlUpdate, use(sigle), use(vartype), use(genre), use(nCateg), use(name), use(description), use(status), use(nId));
			for (auto it = oVars.begin(); it != oVars.end(); ++it) {
				const DTOVariable &oVar = *it;
				if (oVar.is_writeable()) {
					DTOVariable  xVar(oVar);
					this->find_variable(xVar);
					nId = xVar.id();
					nDatasetId = xVar.parent_id();
					sigle = oVar.sigle();
					vartype = oVar.vartype();
					genre = oVar.genre();
					nCateg = oVar.categ();
					name = oVar.name();
					description = oVar.description();
					status = oVar.status();
					if (nId != 0) {
						stmtUpdate.execute();
					}
					else {
						stmtInsert.execute();
					}
				}// writeable
			}// it
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}//maintains_variables
	bool StatDataManager::find_variable(DTOVariable &oVar) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = oVar.id();
			if (nId != 0) {
				std::string sql(SQL_VARIABLE_BY_ID);
				DTOVariable xVar;
				poco_assert_dbg(xVar.id() == 0);
				sess << sql, use(nId), into(xVar), now;
				if (xVar.id() != 0) {
					oVar = xVar;
					return (true);
				}
			}
			else {
				Poco::UInt64 nDatasetId = oVar.parent_id();
				std::string sigle = oVar.sigle();
				if ((nDatasetId == 0) || sigle.empty()) {
					return (false);
				}
				std::string sql(SQL_VARIABLE_BY_DATASET_AND_SIGLE);
				DTOVariable xVar;
				poco_assert_dbg(xVar.id() == 0);
				sess << sql, use(nDatasetId), use(sigle), into(xVar), now;
				if (xVar.id() != 0) {
					oVar = xVar;
					return (true);
				}
			}
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}// find_variable
	////////////////////////////////////////////////
	bool  StatDataManager::find_indiv(const IntType nIndivId, DTOIndiv &oInd, std::map<IntType, std::string> &oValuesMap) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oValuesMap.clear();
			if (nIndivId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = nIndivId;
			{
				std::string sql(SQL_INDIV_BY_ID);
				DTOIndiv xInd;
				poco_assert_dbg(xInd.id() == 0);
				sess << sql, use(nId), into(xInd), now;
				if (xInd.id() == 0) {
					return (false);
				}
				oInd = xInd;
			}
			{
				std::vector<VariableIdValue > oVec;
				std::string sql(SQL_FIND_INDIV_VALUES);
				sess << sql, use(nId), into(oVec), now;
				for (auto it = oVec.begin(); it != oVec.end(); ++it) {
					IntType key = (IntType)(*it).first;
					std::string val = (*it).second;
					oValuesMap[key] = val;
				}// it
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
		}
		return (bRet);
	}//find_indiv
	bool StatDataManager::find_dataset_indivs_count(DTODataset &oSet, int &nCount) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			nCount = 0;
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_INDIVS_COUNT);
			Poco::UInt64 nn = 0;
			sess << sql, use(nDatasetId), into(nn), now;
			nCount = (int)nn;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_indivs_count
	bool StatDataManager::find_dataset_indivs(const DTODataset &oSet, std::vector<DTOIndiv> &oInds) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oInds.clear();
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_DATASET_INDIVS);
			sess << sql, use(nDatasetId), into(oInds), now;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_indivs
	bool StatDataManager::find_dataset_indivs_ids(const DTODataset &oSet, std::vector<info::IntType> &oIds) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oIds.clear();
			Poco::UInt64 nDatasetId = oSet.id();
			if (nDatasetId == 0) {
				return (false);
			}
			Session & sess = *(this->m_session);
			std::string sql(SQL_DATASET_INDIVS_IDS);
			sess << sql, use(nDatasetId), into(oIds), now;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset_indivs_ids
	bool StatDataManager::maintains_indivs(const std::vector<DTOIndiv> &oInds) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		Session & sess = *(this->m_session);
		bool bInTrans = false;
		try {
			if (sess.canTransact()) {
				sess.begin();
				bInTrans = true;
			}
			Poco::UInt64 nDatasetId, nId;
			std::string sigle, name, description, status;
			std::string sqlInsert(SQL_INSERT_INDIV);
			std::string sqlUpdate(SQL_UPDATE_INDIV);
			Statement stmtInsert = (sess << sqlInsert, use(nDatasetId), use(sigle), use(name), use(description), use(status));
			Statement stmtUpdate = (sess << sqlUpdate, use(sigle), use(name), use(description), use(status), use(nId));
			for (auto it = oInds.begin(); it != oInds.end(); ++it) {
				const DTOIndiv &oInd = *it;
				if (oInd.is_writeable()) {
					DTOIndiv  xInd(oInd);
					this->find_indiv(xInd);
					nId = xInd.id();
					nDatasetId = xInd.parent_id();
					sigle = oInd.sigle();
					name = oInd.name();
					description = oInd.description();
					status = oInd.status();
					if (nId != 0) {
						stmtUpdate.execute();
					}
					else {
						stmtInsert.execute();
					}
				}// writeable
			}// it
			if (bInTrans) {
				sess.commit();
				bInTrans = false;
			}
			bRet = true;
		}
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
			if (bInTrans) {
				sess.rollback();
				bInTrans = false;
			}
		}
		return (bRet);
	}//maintains_indivs
	bool StatDataManager::find_indiv(DTOIndiv &oInd) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = oInd.id();
			if (nId != 0) {
				std::string sql(SQL_INDIV_BY_ID);
				DTOIndiv xInd;
				poco_assert_dbg(xInd.id() == 0);
				sess << sql, use(nId), into(xInd), now;
				if (xInd.id() != 0) {
					oInd = xInd;
					return (true);
				}
			}
			else {
				Poco::UInt64 nDatasetId = oInd.parent_id();
				std::string sigle = oInd.sigle();
				if ((nDatasetId == 0) || sigle.empty()) {
					return (false);
				}
				std::string sql(SQL_INDIV_BY_DATASET_AND_SIGLE);
				DTOIndiv xInd;
				poco_assert_dbg(xInd.id() == 0);
				sess << sql, use(nDatasetId), use(sigle), into(xInd), now;
				if (xInd.id() != 0) {
					oInd = xInd;
					return (true);
				}
			}
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}// find_indiv
	/////////////////////////////////////
	bool StatDataManager::get_all_datasets(std::vector<DTODataset> &oVec) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			oVec.clear();
			Session & sess = *(this->m_session);
			std::string sql(SQL_FIND_ALL_DATASETS);
			sess << sql, into(oVec), now;
			bRet = true;
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}// get_all_datasets 
	bool StatDataManager::maintains_dataset(DTODataset &oSet) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			if (!oSet.is_writeable()) {
				return (false);
			}
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = oSet.id();
			std::string sigle = oSet.sigle();
			std::string name = oSet.name();
			std::string desc = oSet.description();
			std::string status = oSet.status();
			if (nId != 0) {
				std::string sql(SQL_FIND_DATASET_BY_ID);
				DTODataset xSet;
				poco_assert_dbg(xSet.id() == 0);
				sess << sql, use(nId), into(xSet), now;
				if (xSet.id() != 0) {
					nId = oSet.id();
				}
				else {
					nId = 0;
				}
			}// nId
			{
				std::string sql(SQL_FIND_DATASET_BY_SIGLE);
				DTODataset xSet;
				poco_assert_dbg(xSet.id() == 0);
				sess << sql, use(sigle), into(xSet), now;
				Poco::UInt64 ns = xSet.id();
				if ((nId != 0) && (ns != 0) && (ns != nId)) {
					return (false);
				}
				if ((ns != 0) && (nId == 0)) {
					nId = ns;
				}
			}
			if (nId == 0) {
				std::string sql(SQL_INSERT_DATASET);
				sess << sql, use(sigle), use(name), use(desc), use(status), now;
			}
			else {
				std::string sql(SQL_UPDATE_DATASET);
				sess << sql, use(sigle), use(name), use(desc), use(status), use(nId), now;
			}
			bRet = this->find_dataset(oSet);
		}// try
		catch (Poco::Exception & ex) {
			std::string message = ex.message();
		}
		return (bRet);
	}//maintains_dataset
	bool StatDataManager::find_dataset(DTODataset &oSet) {
		poco_assert_dbg(this->is_valid());
		bool bRet = false;
		try {
			Session & sess = *(this->m_session);
			Poco::UInt64 nId = oSet.id();
			if (nId != 0) {
				std::string sql(SQL_FIND_DATASET_BY_ID);
				DTODataset xSet;
				poco_assert_dbg(xSet.id() == 0);
				sess << sql, use(nId), into(xSet), now;
				if (xSet.id() == nId) {
					oSet = xSet;
					bRet = true;
				}
			}
			if (!bRet) {
				std::string sigle = oSet.sigle();
				if (sigle.empty()) {
					return (false);
				}
				std::string sql(SQL_FIND_DATASET_BY_SIGLE);
				DTODataset xSet;
				poco_assert_dbg(xSet.id() == 0);
				sess << sql, use(sigle), into(xSet), now;
				if (xSet.id() != 0) {
					oSet = xSet;
					bRet = true;
				}
			}// bRet
		}
		catch (Poco::Exception & /*ex*/) {}
		return (bRet);
	}//find_dataset
	//////////////////////////////////
	StatDataManager::StatDataManager(const std::string &sConnect /*= DEFAULT_CONNECTSTRING*/,
		const std::string &sType /*= DEFAULT_SESSION_TYPE*/)
	{
		try {
			{
				Poco::Mutex::ScopedLock oLock(st_mutex);
				if (sType == TYPE_SQLITE) {
					if (st_types.find(TYPE_SQLITE) == st_types.end()) {
						Poco::Data::SQLite::Connector::registerConnector();
						st_types.insert(TYPE_SQLITE);
					}
				}
				else if (sType == TYPE_ODBC) {
					if (st_types.find(TYPE_ODBC) == st_types.end()) {
						Poco::Data::ODBC::Connector::registerConnector();
						st_types.insert(TYPE_ODBC);
					}
				}
			}// Loc
			this->m_session.reset(new Session(sType, sConnect));
			if (this->is_valid()) {
				if (sType == TYPE_SQLITE) {
					this->check_schema();
				}
			}
		}
		catch (Poco::Exception &ex) {
			std::string message = ex.message();
		}
	}
	StatDataManager::~StatDataManager()
	{
	}
	bool StatDataManager::is_valid(void) {
		Session *p = this->m_session.get();
		return  ((p != nullptr) && p->isConnected());
	}//is_valid
	void StatDataManager::check_schema(void) {
		poco_assert_dbg(this->is_valid());
		try {
			int i = 0;
			Session & sess = *(this->m_session);
			while (CREATE_SQL[i] != nullptr) {
				const char *pszSQL = CREATE_SQL[i];
				std::string sql(pszSQL);
				sess << sql, now;
				++i;
			} // ok
		}
		catch (Poco::Exception &ex) {
			std::string messeage = ex.message();
		}
	}//check_schema
}// namespace info
