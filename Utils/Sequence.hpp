namespace Collection
{

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename T>
	Sequence<T>::Sequence() : std::vector<T>()
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(size_t n) : std::vector<T>(n)
	{

	}


	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(size_t n, const T& init_element) : std::vector<T>(n, init_element)
	{

	}

	///<summary>
	/// コンストラクタ
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(const std::vector<T>& collection) : std::vector<T>(collection)
	{

	}


	///<summary>
	/// デストラクタ
	///</summary>
	template <typename T>
	Sequence<T>::~Sequence()
	{

	}


	///<summary>
	/// Indexがfromからtoまでのサブシークエンスを取得する
	///</summary>
	template <typename T>
	Sequence<T> Sequence<T>::subsequence(int from, int to) const
	{
		if (from == 0 && to == size() - 1) return *this;
		
		Sequence<T> sub;
		for (int i = from; i <= to; i++) {
			sub.push_back(at(i));
		}
		return sub;
	}

	///<summary>
	/// Collectionのsize以下の各gramに対してexecute_functionを実行する
	///</summary>
	template <typename T>
	void Sequence<T>::for_each_gram(const std::function<void(iterator, iterator)>& execute_function)
	{
		for (int n = size(); n > 0; n++) {
			Sequence<T>::iterator head = begin();
			for (Sequence<T>::iterator tail = head + n - 1; tail != end(); tail++) {
				execute_function(head++, tail);
			}
		}
	}


	///<summary>
	/// Collectionのsize以下の各gramに対してexecute_functionを実行する
	///</summary>
	template <typename T>
	void Sequence<T>::for_each_gram(const std::function<void(const_iterator, const_iterator)>& execute_function) const
	{
		for (int n = size(); n > 0; n++) {
			Sequence<T>::const_iterator head = begin();
			for (Sequence<T>::const_iterator tail = head + n - 1; tail != end(); tail++) {
				execute_function(head++, tail);
			}
		}
	}


	///<summary>
	/// 末尾固定で先頭イテレータを変えつつ，各Prefixについてexecute_functionを実行する
	///</summary>
	template <typename T>
	void Sequence<T>::for_each_prefix(const std::function<void(iterator, iterator)>& execute_function)
	{
		if (size() == 0) return;
		for (Sequence<T>::iterator iter = begin(); iter != end() - 1; iter++) {
			execute_function(iter, end() - 1);
		}
	}

	///<summary>
	/// 末尾固定で先頭イテレータを変えつつ，各Prefixについてexecute_functionを実行する
	///</summary>
	template <typename T>
	void Sequence<T>::for_each_prefix(const std::function<void(const_iterator, const_iterator)>& execute_function) const
	{
		if (size() == 0) return;
		for (Sequence<T>::const_iterator iter = begin(); iter != end() - 1; iter++) {
			execute_function(iter, end() - 1);
		}
	}

	///<summary>
	/// seq1の後にseq2を連結したSequenceを返す
	///</summary>
	template <typename T>
	Sequence<T> concat(const Collection::Sequence<T>& seq1, const Collection::Sequence<T>& seq2)
	{
		Sequence<T> ret;
		std::copy(seq1.begin(), seq1.end(), std::back_inserter(ret));
		std::copy(seq2.begin(), seq2.end(), std::back_inserter(ret));
		return ret;
	}
}