#ifdef UTILS_EXPORTS
#define IDENTIFIABLE_API __declspec(dllexport)
#else
#define IDENTIFIABLE_API __declspec(dllimport)
#endif
#pragma once
#include "stdafx.h"

///<summary>
/// IDをもつクラス用のインタフェース
///</summary>
class IDENTIFIABLE_API Identifiable
{

protected:
	long id;

public:
	
	Identifiable(long id) : id(id) {}
	virtual ~Identifiable() {}

	long get_id() const { return id; }

	bool operator==(const Identifiable& i) const { return i.id == this->id; }
	bool operator==(const long &i) const { return this->id == i; }
	bool operator!=(const Identifiable& i) const { return this->id != i.id; }
	bool operator!=(const long &i) const { return this->id != i; }
	bool operator<(const Identifiable& i) const { return this->id < i.id; }
	bool operator<(const long &i) const { return this->id < i; }
	bool operator>(const Identifiable &i) const { return this->id > i.id; }
	bool operator>(const long &i) const { return this->id > i; }
	bool operator<=(const Identifiable &i) const { return this->id <= i.id; }
	bool operator<=(const long &i) const { return this->id <= i; }
	bool operator>=(const Identifiable &i) const { return this->id >= i.id; }
	bool operator>=(const long &i) const { return this->id >= i; }

	friend bool operator==(std::shared_ptr<Identifiable const> i1, long i2) { return i1->id == i2; }
	friend bool operator!=(std::shared_ptr<Identifiable const>i1, long i2) { return i1->id != i2; }
	friend bool operator<(std::shared_ptr<Identifiable const> i1, long i2) { return i1->id < i2; }
	friend bool operator>(std::shared_ptr<Identifiable const> i1, long i2) { return i1->id > i2; }
	friend bool operator<=(std::shared_ptr<Identifiable const> i1, long i2) { return i1->id <= i2; }
	friend bool operator>=(std::shared_ptr<Identifiable const> i1, long i2) { return i1->id >= i2; }
	
};

constexpr long INVALID = -1L;

///<summary>
/// IDが重複した場合に送出する例外
///</summary>
class DuplicatedIdException : public std::domain_error
{
public:
	DuplicatedIdException(long id) : std::domain_error("Duplicated ID - " + std::to_string(id)){}
	virtual ~DuplicatedIdException() {}
};