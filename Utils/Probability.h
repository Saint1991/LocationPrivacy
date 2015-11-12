#ifdef UTILS_EXPORTS
#define PROBABILITY_API __declspec(dllexport)
#else
#define PROBABILITY_API __declspec(dllimport)
#endif

#pragma once

namespace Math {
	class  Probability
	{
	
	private:
		std::unique_ptr<std::mt19937_64> generator;
	
	public:
		
		PROBABILITY_API Probability();
		PROBABILITY_API virtual ~Probability();

		PROBABILITY_API double uniform_distribution(double lower_limit, double upper_limit);
		PROBABILITY_API int uniform_distribution(int lower_limit, int upper_limit);
		PROBABILITY_API double gaussian_distribution(double mean, double sigma);
		PROBABILITY_API double exponential_distribution(double lambda);
	
	};
}
