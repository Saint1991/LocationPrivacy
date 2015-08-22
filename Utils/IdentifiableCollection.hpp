
namespace Collection
{
	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename T>
	IdentifiableCollection<T>::IdentifiableCollection() : 
		std::set < std::shared_ptr<T>, std::function<bool(const std::shared_ptr<T>, const std::shared_ptr<T>)> >([](const std::shared_ptr<T> a, const std::shared_ptr<T> b) {
			return a->get_id() < b->get_id();
		}
	){}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename T>
	IdentifiableCollection<T>::~IdentifiableCollection()
	{
	}


	///<summary>
	/// 指定したIDを持つ要素を読出し専用で取得する．
	/// 見つからない場合はnullptrを返す．
	/// ソート済みのコレクションに対しては2分探索
	/// ソートされていないコレクションに対しては前から全探索する．
	///</summary>
	template <typename T>
	std::shared_ptr<T const> IdentifiableCollection<T>::read_by_id(const long &id) const
	{
		auto target_iter = find(std::make_shared<Identifiable>(id));
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
	template <typename T>
	std::shared_ptr<T> IdentifiableCollection<T>::get_by_id(const long &id)
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
	template <typename T>
	bool IdentifiableCollection<T>::remove_by_id(const long &id)
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
	template <typename T>
	const std::unique_ptr<std::vector<long>> IdentifiableCollection<T>::get_id_list() const
	{
		std::unique_ptr<std::vector<long>> id_list = std::make_unique<std::vector<long>>(size(), INVALID);
		for (std::set< std::shared_ptr<T>>::const_iterator iter = begin(), long index = 0; iter != end(); iter++, index++) {
			id_list->at(index) = (*iter)->get_id();
		}
		return std::move(id_list);
	}

	///<summary>
	/// 指定したIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(long id) const
	{
		bool is_contained = find(id) != end();
		return is_contained;
	}


	///<summary>
	/// 同一のIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(T id) const
	{
		return contains(id.get_id());
	}

	///<summary>
	/// 同一のIDを持つ要素を含んでいるかを判定する
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::contains(std::shared_ptr<T const> id) const
	{
		return contains(id->get_id());
	}


	///<summary>
	/// コレクションの末尾に指定した要素を格納する
	/// IDが重複する場合はDuplicatedIdExceptionをスローする．
	/// 負のIDやmax_sizeを超える領域を指定した場合はout_of_rangeをスローする
	///</summary>
	template <typename T>
	bool IdentifiableCollection<T>::add(std::shared_ptr<T> val)
	{
		//既存IDの場合は追加しない
		long id = val->get_id();
		if (contains(val)) {
			throw DuplicatedIdException(id);
			return false;
		}
	
		// IDが負の場合や要素数がmax_sizeより大きくなる場合はout_of_rangeをスローする
		if (id < 0 || size() == max_size()) {
			throw std::out_of_range("ID must be a natural number");
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
	template <typename T>
	bool IdentifiableCollection<T>::add(T val)
	{
		return add(std::make_shared<T>(val));
	}

	///<summary>
	/// 各要素についてexecute_functionを実行する
	/// 中身の変更も影響するので注意
	///</summary>
	template <typename T>
	void IdentifiableCollection<T>::foreach(const std::function<void(std::shared_ptr<T>)>& execute_function)
	{
		for (std::set<std::shared_ptr<T>>::const_iterator iter =  begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

	///<summary>
	/// 各要素についてexecute_functionを実行する
	///</summary>
	template <typename T>
	void IdentifiableCollection<T>::foreach(const std::function<void(std::shared_ptr<T const>)>& execute_function) const
	{
		for (std::set<std::shared_ptr<T const>>::const_iterator iter = begin(); iter != end(); iter++) {
			execute_function(*iter);
		}
	}

}
