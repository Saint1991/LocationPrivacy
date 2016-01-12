namespace Collection
{

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename T>
	Sequence<T>::Sequence() : std::vector<T>()
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(size_t n) : std::vector<T>(n)
	{

	}


	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(size_t n, const T& init_element) : std::vector<T>(n, init_element)
	{

	}

	///<summary>
	/// �R���X�g���N�^
	///</summary>
	template <typename T>
	Sequence<T>::Sequence(const std::vector<T>& collection) : std::vector<T>(collection)
	{

	}


	///<summary>
	/// �f�X�g���N�^
	///</summary>
	template <typename T>
	Sequence<T>::~Sequence()
	{

	}


	///<summary>
	/// Index��from����to�܂ł̃T�u�V�[�N�G���X���擾����
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
	/// Collection��size�ȉ��̊egram�ɑ΂���execute_function�����s����
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
	/// Collection��size�ȉ��̊egram�ɑ΂���execute_function�����s����
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
	/// �����Œ�Ő擪�C�e���[�^��ς��C�ePrefix�ɂ���execute_function�����s����
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
	/// �����Œ�Ő擪�C�e���[�^��ς��C�ePrefix�ɂ���execute_function�����s����
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
	/// seq1�̌��seq2��A������Sequence��Ԃ�
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