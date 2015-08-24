#pragma once
#include "stdafx.h"

///<summary>
/// IDをもつクラス用のテンプレート
///</summary>
template <typename ID_TYPE>
class Identifiable
{

protected:
	ID_TYPE id;

public:
	
	Identifiable(ID_TYPE id) : id(id) {}
	virtual ~Identifiable() {}

	ID_TYPE get_id() const { return id; }

	bool operator==(const Identifiable<ID_TYPE>& i) const { return i.id == this->id; }
	bool operator==(const ID_TYPE &i) const { return this->id == i; }
	bool operator!=(const Identifiable<ID_TYPE>& i) const { return this->id != i.id; }
	bool operator!=(const ID_TYPE &i) const { return this->id != i; }
	bool operator<(const Identifiable<ID_TYPE>& i) const { return this->id < i.id; }
	bool operator<(const ID_TYPE &i) const { return this->id < i; }
	bool operator>(const Identifiable<ID_TYPE> &i) const { return this->id > i.id; }
	bool operator>(const ID_TYPE &i) const { return this->id > i; }
	bool operator<=(const Identifiable<ID_TYPE> &i) const { return this->id <= i.id; }
	bool operator<=(const ID_TYPE &i) const { return this->id <= i; }
	bool operator>=(const Identifiable<ID_TYPE> &i) const { return this->id >= i.id; }
	bool operator>=(const ID_TYPE &i) const { return this->id >= i; }

	friend bool operator==(std::shared_ptr<Identifiable<ID_TYPE> const> i1, ID_TYPE i2) { return i1->id == i2; }
	friend bool operator!=(std::shared_ptr<Identifiable<ID_TYPE> const>i1, ID_TYPE i2) { return i1->id != i2; }
	friend bool operator<(std::shared_ptr<Identifiable<ID_TYPE> const> i1, ID_TYPE i2) { return i1->id < i2; }
	friend bool operator>(std::shared_ptr<Identifiable<ID_TYPE> const> i1, ID_TYPE i2) { return i1->id > i2; }
	friend bool operator<=(std::shared_ptr<Identifiable<ID_TYPE> const> i1, ID_TYPE i2) { return i1->id <= i2; }
	friend bool operator>=(std::shared_ptr<Identifiable<ID_TYPE> const> i1, ID_TYPE i2) { return i1->id >= i2; }
	
};


constexpr long INVALID = -1L;

///<summary>
/// IDが重複した場合に送出する例外
///</summary>
template <typename ID_TYPE>
class DuplicatedIdException : public std::domain_error
{
public:
	DuplicatedIdException() : std::domain_error("Duplicated ID!"){}
	virtual ~DuplicatedIdException() {}
};