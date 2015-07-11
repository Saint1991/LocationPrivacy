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
		std::unique_ptr<std::vector<std::shared_ptr<T>>> collection;		

	public:
		IdentifiableCollection();
		virtual ~IdentifiableCollection();

		std::shared_ptr<T> find_by_id(const unsigned long& id);
		bool remove_by_id(const unsigned long& id);
		
		bool contains(Identifiable id);
		bool contains(unsigned long id);
		bool add(std::shared_ptr<T> val);
		bool add(T val);
		void sort();
		std::size_t size() const;
		void clear();
	};
}






///<summary>
/// コンストラクタ
///</summary>
template <typename T>
Collection::IdentifiableCollection<T>::IdentifiableCollection() : collection(std::make_unique < std::vector<std::shared_ptr<T>>>())
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
/// 指定したIDを持つ要素を返す．
/// 見つからない場合はnullptrを返す．
/// ソート済みのコレクションに対しては2分探索
/// ソートされていないコレクションに対しては前から全探索する．
///</summary>
template <typename T>
std::shared_ptr<T> Collection::IdentifiableCollection<T>::find_by_id(const unsigned long &id)
{
	auto target_iter = is_sorted ? std::lower_bound(collection->begin(), collection->end(), id) : 	std::find(collection->begin(), collection->end(), id);
	if (target_iter == collection->end() || target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// 指定したIDを持つ要素を探索し，削除する．
/// 要素が存在し，削除できた場合はtrueを，それ以外の場合はfalseを返す．
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::remove_by_id(const unsigned long &id)
{
	auto target_iter = is_sorted ? std::lower_bound(collection->begin(), collection->end(), id) : std::find(collection->begin(), collection->end(), id);

	// 要素が見つからない場合はfalseを返して終了
	if (target_iter->get()->get_id() != id) {
		return false;
	}

	//要素が見つかった場合は削除してtureを返す
	collection->erase(target_iter);
	return true;
}

///<summary>
/// 指定したIDを持つ要素を含んでいるかを判定する
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::contains(unsigned long id)
{
	bool is_contained = find_by_id(id) != nullptr;
	return is_contained;
}

///<summary>
/// 指定したIDを持つ要素を含んでいるかを判定する
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::contains(Identifiable id)
{
	return contains(id.get_id());
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
/// IDが重複する場合はDuplicatedIdExceptionをスローする．
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::add(std::shared_ptr<T> val)
{
	
	//既存IDの場合は追加しない
	unsigned long id = val->get_id();
	if (contains(id)) {
		throw DuplicatedIdException(id);
		return false;
	}
	
	collection->push_back(val);
	is_sorted = false;
	return true;
}

///<summary>
/// コレクションの末尾に指定した要素を格納する
/// IDが重複する場合はDuplicatedIdExceptionをスローする．
///</summary>
template <typename T>
bool Collection::IdentifiableCollection<T>::add(T val)
{
	return add(std::make_shared<T>(val));
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
std::size_t Collection::IdentifiableCollection<T>::size() const
{
	return collection->size();
}
