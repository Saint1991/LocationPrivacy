#ifdef UTILS_EXPORTS
#define IDENTIFIABLE_COLLECTION_API __declspec(dllexport)
#else
#define IDENTIFIABLE_COLLECTION_API __declspec(dllimport)
#endif

#pragma once
#include "Identifiable.h"

namespace Collection {

	///<summary>
	/// ソート済みかを監視し，適切なアルゴリズムで探索するコレクション
	///</summary>
	class IdentifiableCollection 
	{
	private:
		bool is_sorted;
		std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> find_iter_by_id(const unsigned int &id);
		std::unique_ptr<std::vector<std::shared_ptr<Identifiable>>> collection;		

	public:
		IDENTIFIABLE_COLLECTION_API IdentifiableCollection();
		IDENTIFIABLE_COLLECTION_API virtual ~IdentifiableCollection();

		IDENTIFIABLE_COLLECTION_API std::shared_ptr<Identifiable> find_by_id(const unsigned int& id);
		IDENTIFIABLE_COLLECTION_API bool remove_by_id(const unsigned int& id);

		IDENTIFIABLE_COLLECTION_API void sort();
		IDENTIFIABLE_COLLECTION_API void add(const std::shared_ptr<Identifiable> &val);
		IDENTIFIABLE_COLLECTION_API void clear();
		IDENTIFIABLE_COLLECTION_API std::vector<std::shared_ptr<Identifiable>>::size_type capacity();
		IDENTIFIABLE_COLLECTION_API std::vector<std::shared_ptr<Identifiable>>::size_type max_size();
		IDENTIFIABLE_COLLECTION_API std::size_t size();
	};
}


