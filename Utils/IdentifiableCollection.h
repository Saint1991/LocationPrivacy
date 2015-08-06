#pragma once
#include "Identifiable.h"

namespace Collection 
{

	///<summary>
	/// ソート済みかを監視し，適切なアルゴリズムで探索するコレクションテンプレート
	/// 特殊化にはIdentifiableを継承しているクラスのみ使用可能
	///</summary>
	template <typename T>
	class IdentifiableCollection
	{
		
	//TがIdentifiableを継承しているか判定
	static_assert(std::is_base_of<Identifiable, T>::value, "Template Type T is not Derived from Identifiable at IdentifiableCollection.h");

	private:
		std::unique_ptr < std::set < std::shared_ptr<T>, std::function<bool (const std::shared_ptr<T>, const std::shared_ptr<T>)> > > collection;

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T const> read_by_id(const unsigned long& id) const;
		std::shared_ptr<T> get_by_id(const unsigned long& id);
		bool remove_by_id(const unsigned long& id);
		
		bool contains(Identifiable id) const;
		bool contains(unsigned long id) const;
		bool add(std::shared_ptr<T> val);
		bool add(T val);
		std::size_t size() const;
		void clear();
	};
}

#include "IdentifiableCollection.hpp"
