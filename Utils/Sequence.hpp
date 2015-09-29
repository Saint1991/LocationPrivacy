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
	Sequence<T>::Sequence(std::vector<T> collection) : std::vector<T>(collection)
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
}