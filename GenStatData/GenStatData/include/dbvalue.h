/*
* dbvalue.h
*
*  Created on: 17 sept. 2013
*      Author: Boubacar
*/

#ifndef DBVALUE_H_
#define DBVALUE_H_
//////////////////////////////
#include <cassert>
#include <memory>
#include <string>
//////////////////////
#include <boost/any.hpp>
////////////////////////////
namespace info {
	//////////////////////////////////
	typedef unsigned char byte;
	///////////////////////////////////
	class Blob {
	private:
		size_t m_size;
		std::unique_ptr<byte> m_data;
	public:
		Blob();
		Blob(size_t nSize);
		Blob(const byte *pData, size_t nSize);
		Blob(const Blob &other);
		Blob & operator=(const Blob &other);
		~Blob();
	public:
		inline size_t size(void) const {
			return (this->m_size);
		}
		inline const byte *data(void) const {
			return (this->m_data.get());
		}
		inline byte *data(void) {
			return (this->m_data.get());
		}
		inline bool is_valid(void) const {
			return ((this->m_size > 0) && (this->m_data.get() != nullptr));
		}
		void data(const byte *pData, size_t nSize);
	};
	///////////////////////////////////////
	typedef Blob *pBlob;
	///////////////////////////////////////////
	// class Blob
	//////////////////////////////////
	class DbValue {
	private:
		boost::any m_val;
	public:
		DbValue() {
		}
		explicit DbValue(const boost::any &v) :m_val(v) {

		}
		explicit DbValue(bool bval) :
			m_val(bval) {

		}
		explicit DbValue(short ival) :
			m_val(ival) {

		}
		explicit DbValue(int ival) :
			m_val(ival) {

		}
		explicit DbValue(float fval) :
			m_val(fval) {

		}
		explicit DbValue(double dval) :
			m_val(dval) {

		}
		explicit DbValue(const std::string &sval) :
			m_val(sval) {

		}
		explicit DbValue(const std::wstring &wsval) :
			m_val(wsval) {

		}
		explicit DbValue(const Blob &oBlob) {
			Blob b(oBlob);
			this->m_val = boost::any(b);
		}
		DbValue(const DbValue &other) :m_val(other.m_val) {

		}
		DbValue & operator=(const DbValue &other) {
			if (this != &other) {
				this->m_val = other.m_val;
			}
			return (*this);
		}
		~DbValue() {

		}
	public:
		DbValue & operator=(bool b) {
			this->m_val = boost::any(b);
			return *this;
		}
		DbValue & operator=(short ival) {
			this->m_val = boost::any(ival);
			return *this;
		}
		DbValue & operator=(int ival) {
			this->m_val = boost::any(ival);
			return *this;
		}
		DbValue & operator=(float fval) {
			this->m_val = boost::any(fval);
			return *this;
		}
		DbValue & operator=(double dval) {
			this->m_val = boost::any(dval);
			return *this;
		}
		DbValue & operator=(const std::string &s) {
			std::string ss(s);
			this->m_val = boost::any(ss);
			return *this;
		}
		DbValue & operator=(const std::wstring &s) {
			std::wstring ss(s);
			this->m_val = boost::any(ss);
			return *this;
		}
		DbValue & operator=(const Blob &oBlob) {
			Blob b(oBlob);
			this->m_val = boost::any(b);
			return *this;
		}
	public:
		inline const boost::any & value(void) const {
			return (this->m_val);
		}
		inline bool is_valid(void) const {
			return (!this->m_val.empty());
		}
		inline bool empty(void) const {
			return (!this->m_val.empty());
		}
		bool bool_value(void) const;
		short short_value(void) const;
		int int_value(void) const;
		float float_value(void) const;
		double double_value(void) const;
		bool string_value(std::string &s) const;
		bool string_value(std::wstring &s) const;
		bool blob_value(Blob &oBlob) const;
	};
	//////////////////////////////
} /* namespace sqlite */
#endif /* DBVALUE_H_ */
