#pragma once
#ifndef __STATDTO_TYPEHANDLES_H__
#define __STATDTO_TYPEHANDLES_H__
//////////////////////////////////////
#include "info_dto.h"
///////////////////////////////////////
#include <Poco/String.h>
#include <Poco/Any.h>
#include <Poco/Data/TypeHandler.h>
//////////////////////////////////////////
namespace info {
	///////////////////////////////////////
	class VariableIdValue {
	public:
		IntType first;
		std::string second;
		VariableIdValue() :first(0) {}
		VariableIdValue(const IntType iv, const std::string &s) :first(iv), second(s) {}
		VariableIdValue(const VariableIdValue &other) :first(other.first), second(other.second) {}
		VariableIdValue & operator=(const VariableIdValue &other) {
			if (this != &other) {
				this->first = other.first;
				this->second = other.second;
			}
			return (*this);
		}
		~VariableIdValue() {}
		IntType operator()(void) const {
			return (this->first);
		}
		bool operator<(const VariableIdValue &other) const {
			return (this->first < other.first);
		}
	};
	/////////////////////////////////////////
	////////////////////////////////////////
}// namspace info}
//////////////////////////////////////
namespace Poco {
	namespace Data {
		///////////////////////////////////////////
		typedef Poco::UInt64 DatabaseIntType;
		typedef info::IntType IntType;
		////////////////////////////////////////////
		template <>
		class TypeHandler<class info::VariableIdValue >
		{
			typedef info::VariableIdValue  MyType;
		public:
			static void bind(std::size_t pos, const MyType & obj,
				AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg(!pBinder.isNull());
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				//
				TypeHandler<DatabaseIntType>::bind(pos++, obj.first, pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.second, pBinder, dir);
			}

			static std::size_t size()
			{
				return 2; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const MyType & obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg(!pPrepare.isNull());
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.first, pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.second, pPrepare);
			}

			static void extract(std::size_t pos, MyType &obj, const MyType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg(!pExt.isNull());
				DatabaseIntType nId;
				std::string sigle;

				TypeHandler<DatabaseIntType>::extract(pos++, nId, defVal.first, pExt);
				TypeHandler<std::string>::extract(pos++, sigle, defVal.second, pExt);
				obj = MyType((IntType)nId, sigle);
			}

		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		}; // class TypeHandler<info::DTODataset >
		////////////////////////////////////////
		template <>
		class TypeHandler<class info::DTODataset >
		{
			/*
			static const char *SQL_CREATE_DATASET =
			"CREATE TABLE IF NOT EXISTS dbdataset("
			" datasetid INTEGER PRIMARY KEY AUTOINCREMENT,"
			" optlock INTEGER NOT NULL DEFAULT 1,"
			" sigle TEXT NOT NULL UNIQUE,"
			" nom TEXT DEFAULT NULL,"
			" description TEXT DEFAULT NULL,"
			" status TEXT NULL"
			" )";
			*/
			typedef info::DTODataset  MyType;
		public:
			static void bind(std::size_t pos, const MyType & obj,
				AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg(!pBinder.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				//
				TypeHandler<DatabaseIntType>::bind(pos++, obj.id(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.version(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.sigle(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.name(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.description(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.status(), pBinder, dir);
			}

			static std::size_t size()
			{
				return 6; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const MyType & obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg(!pPrepare.isNull());
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.id(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.version(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.sigle(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.name(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.description(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.status(), pPrepare);
			}

			static void extract(std::size_t pos, MyType &obj, const MyType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg(!pExt.isNull());
				DatabaseIntType nId;
				DatabaseIntType nVersion;
				std::string sigle, name, desc, status;

				TypeHandler<DatabaseIntType>::extract(pos++, nId, defVal.id(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nVersion, defVal.version(), pExt);
				TypeHandler<std::string>::extract(pos++, sigle, defVal.sigle(), pExt);
				TypeHandler<std::string>::extract(pos++, name, defVal.name(), pExt);
				TypeHandler<std::string>::extract(pos++, desc, defVal.description(), pExt);
				TypeHandler<std::string>::extract(pos++, status, defVal.status(), pExt);
				obj = MyType((IntType)nId, (IntType)nVersion, status, sigle, name, desc);
			}

		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		}; // class TypeHandler<info::DTODataset >
		//////////////////////////////////////////////
		template <>
		class TypeHandler<class info::DTOIndiv >
		{
			/*
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
			*/
			typedef info::DTOIndiv  MyType;
		public:
			static void bind(std::size_t pos, const MyType & obj,
				AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg(!pBinder.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				//
				TypeHandler<DatabaseIntType>::bind(pos++, obj.id(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.version(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.parent_id(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.sigle(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.name(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.description(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.status(), pBinder, dir);
			}

			static std::size_t size()
			{
				return 7; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const MyType & obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg(!pPrepare.isNull());
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.id(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.version(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.parent_id(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.sigle(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.name(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.description(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.status(), pPrepare);
			}

			static void extract(std::size_t pos, MyType &obj, const MyType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg(!pExt.isNull());
				DatabaseIntType nId;
				DatabaseIntType nVersion, nParentId;
				std::string sigle, name, desc, status;

				TypeHandler<DatabaseIntType>::extract(pos++, nId, defVal.id(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nVersion, defVal.version(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nParentId, defVal.parent_id(), pExt);
				TypeHandler<std::string>::extract(pos++, sigle, defVal.sigle(), pExt);
				TypeHandler<std::string>::extract(pos++, name, defVal.name(), pExt);
				TypeHandler<std::string>::extract(pos++, desc, defVal.description(), pExt);
				TypeHandler<std::string>::extract(pos++, status, defVal.status(), pExt);
				obj = MyType((IntType)nId, (IntType)nVersion, status, sigle, name, desc, (IntType)nParentId);
			}
		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		}; // class TypeHandler<info::DTOIndiv >
		///////////////////////////////////////////
		template <>
		class TypeHandler<class info::DTOVariable >
		{
			/*
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
			" status TEXT NULL"
			*/
			typedef info::DTOVariable  MyType;
		public:
			static void bind(std::size_t pos, const MyType & obj,
				AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg(!pBinder.isNull());
				// the table is defined as Person (FirstName VARCHAR(30), lastName VARCHAR, SocialSecNr INTEGER(3))
				// Note that we advance pos by the number of columns the datatype uses! For string/int this is one.
				//
				TypeHandler<DatabaseIntType>::bind(pos++, obj.id(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.version(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.parent_id(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.sigle(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.vartype(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.genre(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.categ(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.name(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.description(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.status(), pBinder, dir);
			}

			static std::size_t size()
			{
				return 10; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const MyType & obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg(!pPrepare.isNull());
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.id(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.version(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.parent_id(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.sigle(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.vartype(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.genre(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.categ(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.name(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.description(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.status(), pPrepare);
			}

			static void extract(std::size_t pos, MyType &obj, const MyType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg(!pExt.isNull());
				DatabaseIntType nId;
				DatabaseIntType nVersion, nParentId, nCateg;
				std::string sigle, name, desc, status, vartype, genre;

				TypeHandler<DatabaseIntType>::extract(pos++, nId, defVal.id(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nVersion, defVal.version(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nParentId, defVal.parent_id(), pExt);
				TypeHandler<std::string>::extract(pos++, sigle, defVal.sigle(), pExt);
				TypeHandler<std::string>::extract(pos++, vartype, defVal.vartype(), pExt);
				TypeHandler<std::string>::extract(pos++, genre, defVal.genre(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nCateg, defVal.categ(), pExt);
				TypeHandler<std::string>::extract(pos++, name, defVal.name(), pExt);
				TypeHandler<std::string>::extract(pos++, desc, defVal.description(), pExt);
				TypeHandler<std::string>::extract(pos++, status, defVal.status(), pExt);
				//
				bool bCateg = (nCateg != 0) ? true : false;
				obj = MyType((IntType)nId, (IntType)nVersion, status, sigle, name, desc, (IntType)nParentId, bCateg, vartype, genre);
			}
		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		}; // class TypeHandler<info::DTOIndiv >
		///////////////////////////////////////////////
		template <>
		class TypeHandler<class info::DTOValue >
		{
			/*
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
				*/
			typedef info::DTOValue  MyType;
		public:
			static void bind(std::size_t pos, const MyType & obj,
				AbstractBinder::Ptr pBinder, AbstractBinder::Direction dir)
			{
				poco_assert_dbg(!pBinder.isNull());
				//
				TypeHandler<DatabaseIntType>::bind(pos++, obj.id(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.version(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.variable_id(), pBinder, dir);
				TypeHandler<DatabaseIntType>::bind(pos++, obj.indiv_id(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.value(), pBinder, dir);
				TypeHandler<std::string>::bind(pos++, obj.status(), pBinder, dir);
			}

			static std::size_t size()
			{
				return 6; // we handle three columns of the Table!
			}

			static void prepare(std::size_t pos, const MyType & obj, AbstractPreparator::Ptr pPrepare)
			{
				poco_assert_dbg(!pPrepare.isNull());
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.id(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.version(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.variable_id(), pPrepare);
				TypeHandler<DatabaseIntType>::prepare(pos++, obj.indiv_id(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.value(), pPrepare);
				TypeHandler<std::string>::prepare(pos++, obj.status(), pPrepare);
			}

			static void extract(std::size_t pos, MyType &obj, const MyType& defVal, AbstractExtractor::Ptr pExt)
				/// obj will contain the result, defVal contains values we should use when one column is NULL
			{
				poco_assert_dbg(!pExt.isNull());
				DatabaseIntType nId;
				DatabaseIntType nVersion, nVarId, nIndId;
				std::string status, value;

				TypeHandler<DatabaseIntType>::extract(pos++, nId, defVal.id(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nVersion, defVal.version(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nVarId, defVal.variable_id(), pExt);
				TypeHandler<DatabaseIntType>::extract(pos++, nIndId, defVal.indiv_id(), pExt);
				TypeHandler<std::string>::extract(pos++, value, defVal.value(), pExt);
				TypeHandler<std::string>::extract(pos++, status, defVal.status(), pExt);
				//
				obj = MyType((IntType)nId, (IntType)nVersion, status, (IntType)nVarId, (IntType)nIndId, value);
			}
		private:
			TypeHandler();
			~TypeHandler();
			TypeHandler(const TypeHandler&);
			TypeHandler& operator=(const TypeHandler&);
		}; // class TypeHandler<info::DTOIndiv >
		////////////////////////////////////////////
	}// namespace Data
}// namespace Poco
//////////////////////////////////////
#endif // __STATDTO_TYPEHANDLES_H__
