#ifdef UTILS_EXPORTS
#define IDENTIFIABLE_API __declspec(dllexport)
#else
#define IDENTIFIABLE_API __declspec(dllimport)
#endif
#pragma once

///<summary>
/// IDをもつクラス用のインタフェース
///</summary>
class IDENTIFIABLE_API Identifiable
{
protected:
	unsigned int id;

public:
	
	Identifiable(unsigned int id) : id(id) {}
	virtual ~Identifiable() {}

	unsigned int get_id() { return id; }

	bool operator==(const Identifiable& i) { return i.id == this->id; }
	bool operator==(const unsigned int &i) { return this->id == i; }
	bool operator!=(const Identifiable& i) { return this->id != i.id; }
	bool operator!=(const unsigned int &i) { return this->id != i; }
	bool operator<(const Identifiable& i) { return this->id < i.id; }
	bool operator<(const unsigned int &i) { return this->id < i; }
	bool operator>(const Identifiable &i) { return this->id > i.id; }
	bool operator>(const unsigned int &i) { return this->id > i; }
	bool operator<=(const Identifiable &i) { return this->id <= i.id; }
	bool operator<=(const unsigned int &i) { return this->id <= i; }
	bool operator>=(const Identifiable &i) { return this->id >= i.id; }
	bool operator>=(const unsigned int &i) { return this->id >= i; }

	friend bool operator==(std::shared_ptr<Identifiable> i1, unsigned int i2) { return i1->id == i2; }
	friend bool operator!=(std::shared_ptr<Identifiable>i1, unsigned int i2) { return i1->id != i2; }
	friend bool operator<(std::shared_ptr<Identifiable> i1, unsigned int i2) { return i1->id < i2; }
	friend bool operator>(std::shared_ptr<Identifiable> i1, unsigned int i2) { return i1->id > i2; }
	friend bool operator<=(std::shared_ptr<Identifiable> i1, unsigned int i2) { return i1->id <= i2; }
	friend bool operator>=(std::shared_ptr<Identifiable> i1, unsigned int i2) { return i1->id >= i2; }
	
};