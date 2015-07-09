#ifdef UTILS_EXPORTS
#define IDENTIFIABLE_COLLECTION_API __declspec(dllexport)
#else
#define IDENTIFIABLE_COLLECTION_API __declspec(dllimport)
#endif

#pragma once
#include "Identifiable.h"

namespace Collection {

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
		bool is_sorted;
		std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> find_iter_by_id(const unsigned int &id);
		std::unique_ptr<std::vector<std::shared_ptr<T>>> collection;		

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T> find_by_id(const unsigned int& id);
		bool remove_by_id(const unsigned int& id);

		std::size_t size();
		void sort();
		void add(const std::shared_ptr<T> &val);
		void clear();
	};
}






///<summary>
/// コンストラクタ
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::IdentifiableCollection()
{
}


///<summary>
/// デストラクタ
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::~IdentifiableCollection()
{
}

///<summary>
/// 指定したIDを持つ要素のイテレータ参照を返す．
/// ソート済みのコレクションに対しては2分探索
/// ソートされていないコレクションに対しては前から全探索する．
///</summary>
template <typename T>
std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> Collection::IdentifiableCollection<T>::find_iter_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> found_iter = is_sorted ?
		std::lower_bound(collection->begin(), collection->end(), id) :
		std::find(collection->begin(), collection->end(), id);
	return found_iter;
}

///<summary>
/// 指定したIDを持つ要素を返す．
/// 見つからない場合はnullptrを返す．
/// ソート済みのコレクションに対しては2分探索
/// ソートされていないコレクションに対しては前から全探索する．
///</summary>
template <typename T>
std::shared_ptr<T> Collection::IdentifiableCollection<T>::find_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<T>>>> target_iter = find_iter_by_id(id);
	if (target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// 指定したIDを持つ要素を探索し，削除する．
/// 要素が存在し，削除できた場合はtrueを，それ以外の場合はfalseを返す．
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::remove_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> target_iter = find_iter_by_id(id);

	// 要素が見つからない場合はfalseを返して終了
	if (target_iter->get()->get_id() != id) {
		return false;
	}

	//要素が見つかった場合は削除してtureを返す
	collection->erase(target_iter);
	return true;
}

///<summary>
/// 要素をIDで昇順にソートする．
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::sort()
{
	std::sort(collection->begin(), collection->end(), [](std::shared_ptr<T> item1, std::shared_ptr<Identifiable> item2) { return item1->get_id() < item2->get_id(); });
	is_sorted = true;
}

///<summary>
/// コレクションの末尾に指定した要素を格納する
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::add(const::std::shared_ptr<T> &val)
{
	collection->push_back(val);
	is_sorted = false;
}

///<summary>
/// コレクション中の要素を全て削除
///</summary>
template <typename T>
void Collection::IdentifiableCollection<T>::clear()
{
	collection->clear();
}


///<summary>
/// コレクション内の要素数を取得する
///</summary>
template <typename T>
std::size_t Collection::IdentifiableCollection<T>::size()
{
	return collection->size();
}


