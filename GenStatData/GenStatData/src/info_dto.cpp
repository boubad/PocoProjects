//info_dto.cpp
#include "info_dto.h"
////////////////////////////////
#include <Poco/String.h>
////////////////////////////
namespace info {
	/////////////////////////////////////////
	DTOBase::DTOBase() :m_id(0), m_version(0) {}
	DTOBase::DTOBase(const IntType nId, const IntType nVersion, const std::string &stat) :
		m_id(nId), m_version(nVersion) {
		this->status(stat);
	}
	DTOBase::DTOBase(const DTOBase &other) : m_id(other.m_id), m_version(other.m_version), m_status(other.m_status) {}
	DTOBase & DTOBase::operator=(const DTOBase &other) {
		if (this != &other) {
			this->m_id = other.m_id;
			this->m_version = other.m_version;
			this->m_status = other.m_status;
		}
		return (*this);
	}
	DTOBase::~DTOBase() {}
	IntType DTOBase::id(void) const {
		return (this->m_id);
	}// id
	IntType DTOBase::version(void) const {
		return (this->m_version);
	}// version
	std::string DTOBase::status(void) const {
		return (this->m_status);
	}
	void DTOBase::status(const std::string &s) {
		this->m_status = Poco::toUpper(Poco::trim(s));
	}
	bool DTOBase::operator==(const DTOBase &other) const {
		return (this->compareTo(other) == 0);
	}
	bool DTOBase::operator<(const DTOBase &other) const {
		return (this->compareTo(other) < 0);
	}
	IntType DTOBase::operator()(void) const {
		return (this->id());
	}
	bool DTOBase::is_removeable(void) const {
		return (this->id() != 0);
	}
	bool DTOBase::is_updateable(void) const {
		return (this->is_removeable() && this->is_writeable());
	}
	bool DTOBase::is_writeable(void) const {
		return (false);
	}
	int DTOBase::compareTo(const DTOBase &other) const {
		const IntType i1 = this->id();
		const IntType i2 = other.id();
		if (i1 < i2) {
			return (-1);
		}
		else if (i1 > i2) {
			return (1);
		}
		else {
			return (0);
		}
	}// compareTo
	////////////////////////////////////
	DTOSigleNamed::DTOSigleNamed() {}
	DTOSigleNamed::DTOSigleNamed(const IntType nId, const IntType nVersion, const std::string &stat,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc) :
		DTOBase(nId, nVersion, stat) {
		this->sigle(sSigle);
		this->name(sName);
		this->description(sDesc);
	}
	DTOSigleNamed::DTOSigleNamed(const DTOSigleNamed &other) :DTOBase(other),
		m_sigle(other.m_sigle), m_name(other.m_name), m_desc(other.m_desc) {}
	DTOSigleNamed & DTOSigleNamed::operator=(const DTOSigleNamed &other) {
		if (this != &other) {
			DTOBase::operator=(other);
			this->m_sigle = other.m_sigle;
			this->m_name = other.m_name;
			this->m_desc = other.m_desc;
		}
		return (*this);
	}
	DTOSigleNamed::~DTOSigleNamed() {}
	bool DTOSigleNamed::is_writeable(void) const {
		return (!this->sigle().empty());
	}
	std::string  DTOSigleNamed::sigle(void) const {
		return (this->m_sigle);
	}
	void DTOSigleNamed::sigle(const std::string &s) {
		this->m_sigle = Poco::toUpper(Poco::trim(s));
	}
	std::string  DTOSigleNamed::name(void) const {
		return (this->m_name);
	}
	void DTOSigleNamed::name(const std::string &s) {
		this->m_name = Poco::toUpper(Poco::trim(s));
	}
	std::string  DTOSigleNamed::description(void) const {
		return (this->m_sigle);
	}
	void DTOSigleNamed::description(const std::string &s) {
		this->m_desc = Poco::toUpper(Poco::trim(s));
	}
	//////////////////////////////////////////
	DTOSigleNamedChild::DTOSigleNamedChild() :m_parentid(0) {}
	DTOSigleNamedChild::DTOSigleNamedChild(const IntType nId, const IntType nVersion, const std::string &stat,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId) :
		DTOSigleNamed(nId, nVersion, stat, sSigle, sName, sDesc), m_parentid(nParentId) {}
	DTOSigleNamedChild::DTOSigleNamedChild(const DTOSigleNamedChild &other) : DTOSigleNamed(other),
		m_parentid(other.m_parentid) {}
	DTOSigleNamedChild & DTOSigleNamedChild::operator=(const DTOSigleNamedChild&other) {
		if (this != &other) {
			DTOSigleNamed::operator=(other);
			this->m_parentid = other.m_parentid;
		}
		return (*this);
	}
	void DTOSigleNamedChild::parent_id(IntType n) {
		this->m_parentid = n;
	}
	DTOSigleNamedChild::~DTOSigleNamedChild() {}
	IntType DTOSigleNamedChild::parent_id(void) const {
		return (this->m_parentid);
	}
	bool DTOSigleNamedChild::is_writeable(void) const {
		return ((this->parent_id() != 0) && DTOSigleNamed::is_writeable());
	}
	///////////////////////////////////////
	DTODataset::DTODataset() {}
	DTODataset::DTODataset(const std::string &sSigle, const std::string &sName, const std::string &sDesc, const std::string &stat) {
		this->sigle(sSigle);
		this->name(sName);
		this->description(sDesc);
		this->status(stat);
	}
	DTODataset::DTODataset(const IntType nId, const IntType nVersion, const std::string &stat,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc) :
		DTOSigleNamed(nId, nVersion, stat, sSigle, sName, sDesc) {}
	DTODataset::DTODataset(const DTODataset &other) : DTOSigleNamed(other) {}
	DTODataset & DTODataset::operator=(const DTODataset &other) {
		if (this != &other) {
			DTOSigleNamed::operator=(other);
		}
		return (*this);
	}
	DTODataset::~DTODataset() {}
	///////////////////////////////
	DTOIndiv::DTOIndiv() {}
	DTOIndiv::DTOIndiv(const IntType nId, const IntType nVersion, const std::string &stat,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId) :
		DTOSigleNamedChild(nId, nVersion, stat, sSigle, sName, sDesc, nParentId) {}
	DTOIndiv::DTOIndiv(const DTODataset &oSet) {
		this->parent_id(oSet.id());
	}
	DTOIndiv::DTOIndiv(const DTOIndiv &other) : DTOSigleNamedChild(other) {}
	DTOIndiv  & DTOIndiv::operator=(const DTOIndiv &other) {
		if (this != &other) {
			DTOSigleNamedChild::operator=(other);
		}
		return (*this);
	}
	DTOIndiv::~DTOIndiv() {}
	////////////////////////////////////////
	DTOVariable::DTOVariable() :m_categ(false) {}
	DTOVariable::DTOVariable(const IntType nId, const IntType nVersion, const std::string &stat,
		const std::string &sSigle, const std::string &sName, const std::string &sDesc, const IntType nParentId,
		const bool bCateg, const std::string &type, const std::string &sgenre) :
		DTOSigleNamedChild(nId, nVersion, stat, sSigle, sName, sDesc, nParentId),
		m_categ(bCateg) {
		this->vartype(type);
		this->genre(sgenre);
	}
	DTOVariable::DTOVariable(const DTODataset &oSet) :m_categ(false) {
		this->parent_id(oSet.id());
	}
	DTOVariable::DTOVariable(const DTOVariable &other) : DTOSigleNamedChild(other),
		m_categ(other.m_categ), m_vartype(other.m_vartype), m_genre(other.m_genre) {}
	DTOVariable & DTOVariable::operator=(const DTOVariable &other) {
		if (this != &other) {
			DTOSigleNamedChild::operator=(other);
			this->m_categ = other.m_categ;
			this->m_vartype = other.m_vartype;
			this->m_genre = other.m_genre;
		}
		return (*this);
	}
	DTOVariable::~DTOVariable() {}
	bool DTOVariable::is_categ(void) const {
		return (this->m_categ);
	}
	void DTOVariable::is_categ(const bool b) {
		this->m_categ = b;
	}
	IntType  DTOVariable::categ(void) const {
		return (this->m_categ) ? 1 : 0;
	}
	void  DTOVariable::categ(const IntType n) {
		this->m_categ = (n != 0) ? true : false;
	}
	std::string  DTOVariable::vartype(void) const {
		return (this->m_vartype);
	}
	void DTOVariable::vartype(const std::string &s) {
		this->m_vartype = Poco::toLower(Poco::trim(s));
	}
	std::string  DTOVariable::genre(void) const {
		return (this->m_genre);
	}// genre
	void DTOVariable::genre(const std::string &s) {
		this->m_genre = Poco::toUpper(Poco::trim(s));
	}// genre
	bool DTOVariable::is_writeable(void) const {
		return ((!this->vartype().empty()) && DTOSigleNamedChild::is_writeable());
	}
	///////////////////////////////
	DTOValue::DTOValue() :m_varid(0), m_indid(0) {}
	DTOValue::DTOValue(const DTOVariable &oVar, const DTOIndiv &oInd) :
		m_varid(oVar.id()), m_indid(oInd.id()) {}
	DTOValue::DTOValue(const IntType nId, const IntType nVersion, const std::string &stat,
		const IntType nVar, const IntType nInd, const std::string &v) : DTOBase(nId, nVersion, stat),
		m_varid(nVar), m_indid(nInd), m_val(v) {}
	DTOValue::DTOValue(const DTOValue &other) : DTOBase(other),
		m_varid(other.m_varid), m_indid(other.m_indid), m_val(other.m_val) {}
	DTOValue  & DTOValue::operator=(const DTOValue &other) {
		if (this != &other) {
			DTOBase::operator=(other);
			this->m_varid = other.m_varid;
			this->m_indid = other.m_indid;
			this->m_val = other.m_val;
		}
		return (*this);
	}
	DTOValue::~DTOValue() {}
	IntType DTOValue::variable_id(void) const {
		return (this->m_varid);
	}
	IntType DTOValue::indiv_id(void) const {
		return (this->m_indid);
	}
	std::string  DTOValue::value(void) const {
		return (this->m_val);
	}
	void DTOValue::value(const std::string &v) {
		this->m_val = v;
	}
	bool DTOValue::is_writeable(void) const {
		return ((this->variable_id() != 0) && (this->indiv_id() != 0));
	}
	///////////////////////////////////////
}// namespace info
