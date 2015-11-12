#include "stdafx.h"
#include "Probability.h"

///<summary>
/// �R���X�g���N�^
/// �����Z���k�c�C�X�^��p���ăV�[�h�������ŏ�����
///</summary>
Math::Probability::Probability() : generator(std::make_unique<std::mt19937_64>(static_cast<unsigned long>(std::time(nullptr))))
{

}

///<summary>
/// �f�X�g���N�^
///</summary>
Math::Probability::~Probability()
{
}

///<summary>
/// ��l���z�ɏ]��range_min�ȏ�Crange_max�����̗����𐶐�
///</summary>
///<param name='lower_limit'>�����̒l��̉���</param>
///<param name='upper_limit'>�����̒n��̏��</param>
double Math::Probability::uniform_distribution(double lower_limit, double upper_limit)
{
	std::uniform_real_distribution<double> dist(lower_limit, upper_limit);

	double ret = dist(*this->generator);
	return ret;
}

///<summary>
/// ��l���z�ɏ]��range_min�ȏ�Crange_max�����̗����𐶐�
///</summary>
///<param name='lower_limit'>�����̒l��̉���</param>
///<param name='upper_limit'>�����̒n��̏��</param>
int Math::Probability::uniform_distribution(int lower_limit, int upper_limit)
{
	std::uniform_int_distribution<int> dist(lower_limit, upper_limit);

	int ret = dist(*this->generator);
	return ret;
}

///<summary>
/// �p�����[�^mean, sigma�̐��K���z�ɏ]�������𐶐�����D
///</summary>
///<param name='mean'>����</param>
///<param name='sigma>�W���΍�</param>
double Math::Probability::gaussian_distribution(double mean, double sigma)
{
	std::normal_distribution<double> dist(mean, sigma);

	double ret = dist(*this->generator);
	return ret;
}

///<summary>
/// �p�����[�^lambda�̎w�����z�ɏ]�������𐶐�����D
/// labmda�͔񕉐�
///</summary>
///<param name='lambda'>�p�����[�^��</param>
double Math::Probability::exponential_distribution(double lambda)
{
	if (lambda < 0.0) {
		throw std::invalid_argument::exception("Param lambda must be greater than or equal to 0");
	}
	std::exponential_distribution<double> dist(lambda);
	double ret = dist(*this->generator);
	return ret;
}



