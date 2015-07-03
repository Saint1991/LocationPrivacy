#include "stdafx.h"
#include "IdentifiableCollection.h"


Collection::IdentifiableCollection::IdentifiableCollection()
{
}


Collection::IdentifiableCollection::~IdentifiableCollection()
{
}

///<summary>
/// 指定したIDを持つ要素のイテレータ参照を返す．
/// ソート済みのコレクションに対しては2分探索
/// ソートされていないコレクションに対しては前から全探索する．
///</summary>
std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> Collection::IdentifiableCollection::find_iter_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> found_iter = is_sorted ?
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
std::shared_ptr<Identifiable> Collection::IdentifiableCollection::find_by_id(const unsigned int &id)
{
	std::_Vector_iterator<std::_Vector_val<std::_Simple_types<std::shared_ptr<Identifiable>>>> target_iter = find_iter_by_id(id);
	if (target_iter->get()->get_id() != id) {
		return nullptr;
	}
	return *target_iter;
}

///<summary>
/// 指定したIDを持つ要素を探索し，削除する．
/// 要素が存在し，削除できた場合はtrueを，それ以外の場合はfalseを返す．
///</summary>
bool Collection::IdentifiableCollection::remove_by_id(const unsigned int &id)
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
void Collection::IdentifiableCollection::sort()
{
	std::sort(collection->begin(), collection->end(), [](std::shared_ptr<Identifiable> item1, std::shared_ptr<Identifiable> item2) { return item1->get_id() < item2->get_id(); });
	is_sorted = true;
}

///<summary>
/// コレクションの末尾に指定した要素を格納する
///</summary>
void Collection::IdentifiableCollection::add(const::std::shared_ptr<Identifiable> &val)
{
	collection->push_back(val);
	is_sorted = false;
}

///<summary>
/// コレクション中の要素を全て削除
///</summary>
void Collection::IdentifiableCollection::clear()
{
	collection->clear();
}


///<summary>
/// コレクションの現在確保されているメモリ容量(要素数)を取得する
///</summary>
std::vector<std::shared_ptr<Identifiable>>::size_type Collection::IdentifiableCollection::capacity()
{
	return collection->capacity();
}

///<summary>
/// コレクションに対して確保できる最大容量(要素数)を取得する
///</summary>
std::vector<std::shared_ptr<Identifiable>>::size_type Collection::IdentifiableCollection::max_size()
{
	return collection->max_size();
}

///<summary>
/// コレクション内の要素数を取得する
///</summary>
std::size_t Collection::IdentifiableCollection::size()
{
	return collection->size();
}

