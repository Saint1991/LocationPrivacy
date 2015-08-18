#include "stdafx.h"
#include "CppUnitTest.h"
#include "Probability.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace UtilsTest
{		
	TEST_CLASS(ProbabilityTest)
	{
	public:
		
		TEST_METHOD(Probability_uniform_distribution)
		{

			double lower_limit = 3.0;
			double upper_limit = 4.5;

			Math::Probability prob;
			for (int i = 0; i < 100; i++) {
				double actual = prob.uniform_distribution(lower_limit, upper_limit);
				Assert::IsTrue(lower_limit <= actual && actual <= upper_limit);

				std::string message = "actual=" + std::to_string(actual) + "\n";
				Logger::WriteMessage(message.c_str());
			}
		}

		TEST_METHOD(Probability_uniform_distribution2)
		{
			int lower_limit = 3;
			int upper_limit = 10;

			Math::Probability prob;
			for (int i = 0; i < 100; i++) {
				int actual = prob.uniform_distribution(lower_limit, upper_limit);
				Assert::IsTrue(lower_limit <= actual && actual <= upper_limit);

				std::string message = "actual=" + std::to_string(actual) + "\n";
				Logger::WriteMessage(message.c_str());
			}
		}

		TEST_METHOD(Probability_gaussian_distribution)
		{
			double mean = 0.0;
			double sigma = 1.0;

			Math::Probability prob;
			for (int i = 0; i < 100; i++) {
				double value = prob.gaussian_distribution(mean, sigma);
				std::string message = "actual=" + std::to_string(value) + "\n";
				Logger::WriteMessage(message.c_str());
			}
		}

		TEST_METHOD(Probability_exponential_distribution)
		{
			double lambda = 0.5;

			Math::Probability prob;
			for (int i = 0; i < 100; i++) {
				double value = prob.exponential_distribution(lambda);
				std::string message = "actual=" + std::to_string(value) + "\n";
				Logger::WriteMessage(message.c_str());
			}
		}

	};
}