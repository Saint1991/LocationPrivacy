#pragma once
#include "stdafx.h"

///<summary>
/// IDをもつクラス用のインタフェース
///</summary>
class Identifiable
{
protected:
	unsigned long id;

public:
	
	Identifiable(unsigned long id) : id(id) {}
	virtual ~Identifiable() {}

	unsigned long get_id() const { return id; }

	bool operator==(const Identifiable& i) const { return i.id == this->id; }
	bool operator==(const unsigned long &i) const { return this->id == i; }
	bool operator!=(const Identifiable& i) const { return this->id != i.id; }
	bool operator!=(const unsigned long &i) const { return this->id != i; }
	bool operator<(const Identifiable& i) const { return this->id < i.id; }
	bool operator<(const unsigned long &i) const { return this->id < i; }
	bool operator>(const Identifiable &i) const { return this->id > i.id; }
	bool operator>(const unsigned long &i) const { return this->id > i; }
	bool operator<=(const Identifiable &i) const { return this->id <= i.id; }
	bool operator<=(const unsigned long &i) const { return this->id <= i; }
	bool operator>=(const Identifiable &i) const { return this->id >= i.id; }
	bool operator>=(const unsigned long &i) const { return this->id >= i; }

	friend bool operator==(std::shared_ptr<Identifiable const> i1, unsigned long i2) { return i1->id == i2; }
	friend bool operator!=(std::shared_ptr<Identifiable const>i1, unsigned long i2) { return i1->id != i2; }
	friend bool operator<(std::shared_ptr<Identifiable const> i1, unsigned long i2) { return i1->id < i2; }
	friend bool operator>(std::shared_ptr<Identifiable const> i1, unsigned long i2) { return i1->id > i2; }
	friend bool operator<=(std::shared_ptr<Identifiable const> i1, unsigned long i2) { return i1->id <= i2; }
	friend bool operator>=(std::shared_ptr<Identifiable const> i1, unsigned long i2) { return i1->id >= i2; }
	
};


///<summary>
/// IDが重複した場合に送出する例外
///</summary>
class DuplicatedIdException : public std::domain_error
{
public:
	DuplicatedIdException(unsigned long id) : std::domain_error("Duplicated ID - " + std::to_string(id)){}
	virtual ~DuplicatedIdException() {}
};