#pragma once
#ifndef __INFO_DTO_H__
#define __INFO_DTO_H__
//////////////////////////////////
#include <string>
//////////////////////////////////////////
namespace info {
	/////////////////////////////////////////
	typedef int  IntType;
	///////////////////////////////////////
	class DTOBase
	{
	private:
		IntType m_id;
		IntType m_version;
		std::string m_status;
	protected:
		DTOBase();
		DTOBase & operator=(const DTOBase &other);
		DTOBase(const IntType nId, const IntType nVersion, const std::string &stat);
		DTOBase(const DTOBase &other);
	public:
		virtual ~DTOBase();
		IntType id(void) const;
		IntType version(void) const;
		std::string status(void) const;
		void status(const std::string &s);
		bool operator==(const DTOBase &other) const;
		bool operator<(const DTOBase &other) const;
		IntType operator()(void) const;
		bool is_removeable(void) const;
		bool is_updateable(void) const;
		virtual bool is_writeable(void) const;
	protected:
		virtual int compareTo(const DTOBase &other) const;
	};// class DTOBase<std::string>
	class DTOSigleNamed : public DTOBase {
	private:
		std::string m_sigle;
		std::string m_name;
		std::string m_desc;
	protected:
		DTOSigleNamed();
		DTOSigleNamed(const IntType nId, const IntType nVersion, const std::string &stat,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc);
		DTOSigleNamed(const DTOSigleNamed &other);
		DTOSigleNamed & operator=(const DTOSigleNamed &other);
	public:
		virtual ~DTOSigleNamed();
		virtual bool is_writeable(void) const;
		std::string  sigle(void) const;
		void sigle(const std::string &s);
		std::string  name(void) const;
		void name(const std::string &s);
		std::string  description(void) const;
		void description(const std::string &s);
	};// class DTOSigleNamed
	class DTOSigleNamedChild : public DTOSigleNamed {
	private:
		IntType m_parentid;
	protected:
		DTOSigleNamedChild();
		DTOSigleNamedChild(const DTOSigleNamedChild &other);
		DTOSigleNamedChild(const IntType nId, const IntType nVersion, const std::string &stat,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId);
		DTOSigleNamedChild & operator=(const DTOSigleNamedChild&other);
		void parent_id(IntType n);
	public:
		virtual ~DTOSigleNamedChild();
		IntType parent_id(void) const;
		virtual bool is_writeable(void) const;
	};// class DTOSigleNamedChild
	///////////////////////////////////////
	class DTODataset : public DTOSigleNamed {
	public:
		DTODataset();
		DTODataset(const std::string &sSigle, const std::string &sName, const std::string &sDesc, const std::string &stat);
		DTODataset(const IntType nId, const IntType nVersion, const std::string &stat,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc);
		DTODataset(const DTODataset &other);
		DTODataset & operator=(const DTODataset &other);
		virtual ~DTODataset();
	};// class DTODataset
	class DTOIndiv : public DTOSigleNamedChild {
	public:
		DTOIndiv();
		DTOIndiv(const IntType nId, const IntType nVersion, const std::string &stat,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId);
		DTOIndiv(const DTODataset &oSet);
		DTOIndiv(const DTOIndiv &other);
		DTOIndiv  & operator=(const DTOIndiv &other);
		virtual ~DTOIndiv();
	};// class DTOIndiv<std::string,IntType>
	class DTOVariable : public DTOSigleNamedChild {
	private:
		bool	m_categ;
		std::string m_vartype;
		std::string m_genre;
	public:
		DTOVariable();
		DTOVariable(const IntType nId, const IntType nVersion, const std::string &stat,
			const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId,
			const bool bCateg, const std::string &type, const std::string &sgenre);
		DTOVariable(const DTODataset &oSet);
		DTOVariable(const DTOVariable &other);
		DTOVariable & operator=(const DTOVariable &other);
		virtual ~DTOVariable();
		IntType categ(void) const;
		void categ(const IntType n);
		bool is_categ(void) const;
		void is_categ(const bool b);
		std::string  vartype(void) const;
		void vartype(const std::string &s);
		std::string  genre(void) const;
		void genre(const std::string &s);
		virtual bool is_writeable(void) const;
	};// class DTOVariable<std::string,IntType>
	class DTOValue : public DTOBase {
	private:
		IntType m_varid;
		IntType m_indid;
		std::string m_val;
	public:
		DTOValue();
		DTOValue(const DTOVariable &oVar, const DTOIndiv &oInd);
		DTOValue(const IntType nId, const IntType nVersion, const std::string &stat,
			const IntType nVar, const IntType nInd, const std::string &v);
		DTOValue(const DTOValue &other);
		DTOValue  & operator=(const DTOValue &other);
		virtual ~DTOValue();
		IntType variable_id(void) const;
		IntType indiv_id(void) const;
		std::string  value(void) const;
		void value(const std::string &v);
		virtual bool is_writeable(void) const;
	}; // class DTOValue
	////////////////////////////////////////
}// namespace info
/////////////////////////////////////
#endif // !__INFO_DTO_H
