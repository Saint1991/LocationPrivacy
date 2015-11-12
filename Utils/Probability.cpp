#include "stdafx.h"
#include "Probability.h"

///<summary>
/// コンストラクタ
/// メルセンヌツイスタを用いてシードを時刻で初期化
///</summary>
Math::Probability::Probability() : generator(std::make_unique<std::mt19937_64>(static_cast<unsigned long>(std::time(nullptr))))
{

}

///<summary>
/// デストラクタ
///</summary>
Math::Probability::~Probability()
{
}

///<summary>
/// 一様分布に従うrange_min以上，range_max未満の乱数を生成
///</summary>
///<param name='lower_limit'>乱数の値域の下限</param>
///<param name='upper_limit'>乱数の地域の上限</param>
double Math::Probability::uniform_distribution(double lower_limit, double upper_limit)
{
	std::uniform_real_distribution<double> dist(lower_limit, upper_limit);

	double ret = dist(*this->generator);
	return ret;
}

///<summary>
/// 一様分布に従うrange_min以上，range_max未満の乱数を生成
///</summary>
///<param name='lower_limit'>乱数の値域の下限</param>
///<param name='upper_limit'>乱数の地域の上限</param>
int Math::Probability::uniform_distribution(int lower_limit, int upper_limit)
{
	std::uniform_int_distribution<int> dist(lower_limit, upper_limit);

	int ret = dist(*this->generator);
	return ret;
}

///<summary>
/// パラメータmean, sigmaの正規分布に従う乱数を生成する．
///</summary>
///<param name='mean'>平均</param>
///<param name='sigma>標準偏差</param>
double Math::Probability::gaussian_distribution(double mean, double sigma)
{
	std::normal_distribution<double> dist(mean, sigma);

	double ret = dist(*this->generator);
	return ret;
}

///<summary>
/// パラメータlambdaの指数分布に従う乱数を生成する．
/// labmdaは非負数
///</summary>
///<param name='lambda'>パラメータλ</param>
double Math::Probability::exponential_distribution(double lambda)
{
	if (lambda < 0.0) {
		throw std::invalid_argument::exception("Param lambda must be greater than or equal to 0");
	}
	std::exponential_distribution<double> dist(lambda);
	double ret = dist(*this->generator);
	return ret;
}



