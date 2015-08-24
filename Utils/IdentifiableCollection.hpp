
namespace Collection
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	IdentifiableCollection<ID_TYPE, DATA_TYPE>::IdentifiableCollection() : 
		std::set < std::shared_ptr<DATA_TYPE>, 
		std::function<bool(const std::shared_ptr<DATA_TYPE>, const std::shared_ptr<DATA_TYPE>)> >([](const std::shared_ptr<DATA_TYPE> a, const std::shared_ptr<DATA_TYPE> b) {
			return a->get_id() < b->get_id();
		}
	){}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	IdentifiableCollection<ID_TYPE, DATA_TYPE>::~IdentifiableCollection()
	{
	}


	///<summary>
	/// 指定したIDを持つ要素を読出し専用で取得する．
	/// 見つからない場合はnullptrを返す．
	/// ソート済みのコレクションに対しては2分探索
	/// ソートされていないコレクションに対しては前から全探索する．
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	std::shared_ptr<DATA_TYPE const> IdentifiableCollection<ID_TYPE, DATA_TYPE>::read_by_id(const ID_TYPE& id) const
	{
		auto target_iter = find(id);
		if (target_iter == end()) {
			return nullptr;
		}
		return *target_iter;
	}

	///<summary>
	/// 指定したIDを持つ要素を変更可能な状態で取得する．
	/// 見つからない場合はnullptrを返す．
	/// ソート済みのコレクションに対しては2分探索
	/// ソートされていないコレクションに対しては前から全探索する．
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	std::shared_ptr<DATA_TYPE> IdentifiableCollection<ID_TYPE, DATA_TYPE>::get_by_id(const ID_TYPE& id)
	{
		auto target_iter = find(id);
		if (target_iter == end()) {
			return nullptr;
		}
		return *target_iter;
	}

	///<summary>
	/// 指定したIDを持つ要素を探索し，削除する．
	/// 要素が存在し，削除できた場合はtrueを，それ以外の場合はfalseを返す．
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::remove_by_id(const ID_TYPE &id)
	{
		auto target_iter =  find(id);

		// 要素が見つからない場合はfalseを返して終了
		if (target_iter == end()) {
			return false;
		}

		//要素が見つかった場合は削除してtureを返す
		 erase(target_iter);
		return true;
	}

	///<summary>
	/// 含まれているID一覧を取得する
	/// 順序は昇順にソートされる
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	const std::unique_ptr<std::vector<ID_TYPE>> IdentifiableCollection<ID_TYPE, DATA_TYPE>::get_id_list() const
	{
		std::unique_ptr<std::vector<ID_TYPE>> id_list = std::make_unique<std::vector<ID_TYPE>>(size());
		for (std::set< std::shared_ptr<DATA_TYPE>>::const_iterator iter = begin(); iter != end(); iter++) {
			id_list->push_back((*iter)->get_id());
		}
		return std::move(id_list);
	}

	///<summary>
	/// 指定したIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(const ID_TYPE& id) const
	{
		bool is_contained = find(id) != end();
		return is_contained;
	}


	///<summary>
	/// 同一のIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(const DATA_TYPE& id) const
	{
		return contains(id.get_id());
	}

	///<summary>
	/// 同一のIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::contains(std::shared_ptr<DATA_TYPE const> id) const
	{
		return contains(id->get_id());
	}


	///<summary>
	/// コレクションの末尾に指定した要素を格納する
	/// IDが重複する場合はDuplicatedIdExceptionをスローする．
	/// 負のIDやmax_sizeを超える領域を指定した場合はout_of_rangeをスローする
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::add(std::shared_ptr<DATA_TYPE> val)
	{
		//既存IDの場合は追加しない
		ID_TYPE id = val->get_id();
		if (contains(val)) {
			throw DuplicatedIdException();
			return false;
		}
	
		// IDが負の場合や要素数がmax_sizeより大きくなる場合はout_of_rangeをスローする
		if (size() == max_size()) {
			throw std::out_of_range("The number of elements reached to the upper limitation");
			return false;
		}

		 insert(val);
		return true;
	}

	///<summary>
	/// コレクションの末尾に指定した要素を格納する
	/// IDが重複する場合はDuplicatedIdExceptionをスローする．
	/// 負のIDやmax_sizeを超える領域を指定した場合はout_of_rangeをスローする
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	bool IdentifiableCollection<ID_TYPE, DATA_TYPE>::add(DATA_TYPE val)
	{
		return add(std::make_shared<DATA_TYPE>(val));
	}

	///<summary>
	/// 各要素についてexecute_functionを実行する
	/// 中身の変更も影響するので注意
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	void IdentifiableCollection<ID_TYPE, DATA_TYPE>::foreach(const std::function<void(std::shared_ptr<DATA_TYPE>)>& execute_function)
	{
		for (std::set<std::shared_ptr<DATA_TYPE>>::const_iterator iter =  begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

	///<summary>
	/// 各要素についてexecute_functionを実行する
	///</summary>
	template <typename ID_TYPE, typename DATA_TYPE>
	void IdentifiableCollection<ID_TYPE, DATA_TYPE>::foreach(const std::function<void(std::shared_ptr<DATA_TYPE const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<DATA_TYPE const>>::const_iterator iter = begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

}
