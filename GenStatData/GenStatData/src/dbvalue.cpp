/*
* dbvalue.cpp
*
*  Created on: 17 sept. 2013
*      Author: Boubacar
*/

#include "../include/dbvalue.h"
///////////////////////////
#include <sstream>
#include <locale>
#include <codecvt>
/////////////////////////////////////////
#include <boost/algorithm/string.hpp>
/////////////////////////////////////////
namespace info {
	///////////////////////////////////////////////
	inline std::wstring info_s2ws(const std::string& str)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.from_bytes(str);
	}

	inline std::string info_ws2s(const std::wstring& wstr)
	{
		typedef std::codecvt_utf8<wchar_t> convert_typeX;
		std::wstring_convert<convert_typeX, wchar_t> converterX;

		return converterX.to_bytes(wstr);
	}
	///////////////////////////////////////////////
	Blob::Blob() :
		m_size(0) {
	} // Blob
	Blob::Blob(size_t nSize) :
		m_size(0) {
		if (nSize > 0) {
			this->m_data.reset(new byte[nSize]);
			assert(this->m_data.get() != nullptr);
			this->m_size = nSize;
		} // nSize
	} // Blob
	Blob::Blob(const byte *pData, size_t nSize) :
		m_size(0) {
		if (nSize > 0) {
			this->m_data.reset(new byte[nSize]);
			byte *pDest = this->m_data.get();
			assert(pDest != nullptr);
			this->m_size = nSize;
			if (pData != nullptr) {
				const byte *pSrc = pData;
				while (nSize > 0) {
					*pDest++ = *pSrc++;
					--nSize;
				}
			} // pData
		} // nSize
	} // Blob
	Blob::Blob(const Blob &other) :
		m_size(0) {
		size_t nSize = other.m_size;
		const byte *pData = other.m_data.get();
		if (nSize > 0) {
			this->m_data.reset(new byte[nSize]);
			byte *pDest = this->m_data.get();
			assert(pDest != nullptr);
			this->m_size = nSize;
			if (pData != nullptr) {
				const byte *pSrc = pData;
				while (nSize > 0) {
					*pDest++ = *pSrc++;
					--nSize;
				}
			} // pData
		} // nSi
	} // Blob
	Blob & Blob::operator=(const Blob &other) {
		if (this != &other) {
			this->m_data.reset();
			this->m_size = 0;
			size_t nSize = other.m_size;
			const byte *pData = other.m_data.get();
			if (nSize > 0) {
				this->m_data.reset(new byte[nSize]);
				byte *pDest = this->m_data.get();
				assert(pDest != nullptr);
				this->m_size = nSize;
				if (pData != nullptr) {
					const byte *pSrc = pData;
					while (nSize > 0) {
						*pDest++ = *pSrc++;
						--nSize;
					}
				} // pData
			} // nSi
		} // copy
		return (*this);
	} // operator=
	Blob::~Blob() {
	} // ~Blob
	void Blob::data(const byte *pData, size_t nSize) {
		byte *pOld = this->m_data.get();
		if (pOld != pData) {
			this->m_data.reset();
			this->m_size = 0;
			if (nSize > 0) {
				this->m_data.reset(new byte[nSize]);
				byte *pDest = this->m_data.get();
				assert(pDest != nullptr);
				this->m_size = nSize;
				if (pData != nullptr) {
					const byte *pSrc = pData;
					while (nSize > 0) {
						*pDest++ = *pSrc++;
						--nSize;
					}
				} // pData
			} // nSize
		} // copy
	} // data
	  ////////////////////////////////////////////
	bool DbValue::bool_value(void) const {
		bool oRet = false;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				oRet = boost::any_cast<bool>(v);
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (x != 0) ? true : false;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (x != 0.0f) ? true : false;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (x != 0.0) ? true : false;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					auto c = *(xx.begin());
					oRet = (c == 't') || (c == '1') || (c == 'v') || (c == 'o');
				} // not empty
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					auto c = *(xx.begin());
					oRet = (c == L't') || (c == L'1') || (c == L'v') || (c == L'o');
				} // not empty
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				oRet = x.is_valid();
			}
		} // not empty
		return (oRet);
	} // bool_value
	short DbValue::short_value(void) const {
		short oRet = -1;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = x;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (short)x;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (short)x;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (short)x;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				oRet = (short)x.size();
			}
		} // not empty
		return (oRet);
	} // short_value
	int DbValue::int_value(void) const {
		int oRet = -1;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1 : 0;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (int)x;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = x;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (int)x;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (int)x;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				oRet = (int)x.size();
			}
		} // not empty
		return (oRet);
	} // int_value
	float DbValue::float_value(void) const {
		float oRet = -1.0f;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0f : 0.0f;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (float)x;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (float)x;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = x;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = (float)x;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				oRet = (float)x.size();
			}
		} // not empty
		return (oRet);
	} // float_value
	double DbValue::double_value(void) const {
		double oRet = -1.0;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				oRet = (b) ? 1.0 : 0.0;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				oRet = (double)x;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				oRet = (double)x;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				oRet = (double)x;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				oRet = x;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				std::string xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::stringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				std::wstring xx = boost::to_lower_copy(boost::trim_copy(x));
				if (!xx.empty()) {
					std::wstringstream in(xx);
					in >> oRet;
				} // not empty
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				oRet = (double)x.size();
			}
		} // not empty
		return (oRet);
	} // double_value
	bool DbValue::string_value(std::string &s) const {
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				s = (b) ? "true" : "false";
				bRet = true;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				std::stringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				s = x;
				bRet = true;
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				s = info_ws2s(x);
				bRet = true;
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				const byte *p0 = x.data();
				size_t n = x.size();
				if ((n > 0) && (p0 != nullptr)) {
					std::unique_ptr<char> oTemp(new char[n + 1]);
					char *pDest = oTemp.get();
					if (pDest != nullptr) {
						char *pd = pDest;
						size_t nx = n;
						while (nx > 0) {
							char c = (char)(*p0);
							*pd = c;
							p0++;
							pd++;
							--nx;
						}
						pDest[n] = (char)0;
						s = pDest;
						bRet = true;
					} // pDest
				} // valid
			}
		} // not empty
		return (bRet);
	} // string_value
	bool DbValue::string_value(std::wstring &s) const {
		bool bRet = false;
		s.clear();
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			const std::type_info & t = v.type();
			if (t == typeid(bool)) {
				bool b = boost::any_cast<bool>(v);
				s = (b) ? L"true" : L"false";
				bRet = true;
			}
			else if (t == typeid(short)) {
				short x = boost::any_cast<short>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(int)) {
				int x = boost::any_cast<int>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(float)) {
				float x = boost::any_cast<float>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(double)) {
				double x = boost::any_cast<double>(v);
				std::wstringstream os;
				os << x;
				s = os.str();
				bRet = true;
			}
			else if (t == typeid(std::wstring)) {
				std::wstring x = boost::any_cast<std::wstring>(v);
				s = x;
				bRet = true;
			}
			else if (t == typeid(std::string)) {
				std::string x = boost::any_cast<std::string>(v);
				s = info_s2ws(x);
				bRet = true;
			}
			else if (t == typeid(Blob)) {
				Blob x = boost::any_cast<Blob>(v);
				const byte *p0 = x.data();
				size_t n = x.size();
				if ((n > 0) && (p0 != nullptr)) {
					std::unique_ptr<wchar_t> oTemp(new wchar_t[n + 1]);
					wchar_t *pDest = oTemp.get();
					if (pDest != nullptr) {
						wchar_t *pd = pDest;
						size_t nx = n;
						while (nx > 0) {
							wchar_t c = (wchar_t)(*p0);
							*pd = c;
							p0++;
							pd++;
							--nx;
						}
						pDest[n] = (wchar_t)0;
						s = pDest;
						bRet = true;
					} // pDest
				} // valid
			}

		} // not empty
		return (bRet);
	} // string_value
	bool DbValue::blob_value(Blob &oBlob) const {
		bool bRet = false;
		const boost::any &v = this->m_val;
		if (!v.empty()) {
			if (v.type() == typeid(Blob)) {
				oBlob = boost::any_cast<Blob>(v);
				bRet = true;
			}
		} // not empty
		return (bRet);
	} // blob_value
	  /////////////////////////////////////////////
} /* namespace sqlite */
