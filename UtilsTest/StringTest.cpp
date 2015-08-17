
#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(StringTest)
	{
	public:

		TEST_METHOD(erase)
		{
			std::string alphabet = "abc";
			alphabet.erase(alphabet.end() - 1);

			std::string expected = "ab";
			Assert::AreEqual(expected, alphabet);
		}
		
		TEST_METHOD(erase2) 
		{
			std::string alphabet = "abc";
			alphabet.erase(alphabet.end() - 2, alphabet.end());

			std::string expected = "a";
			Assert::AreEqual(expected, alphabet);
		}

		TEST_METHOD(replace)
		{
			std::string alphabet = "a, ";
			alphabet.replace(alphabet.end() - 2, alphabet.end(), ")");

			std::string expected = "a)";
			Assert::AreEqual(expected, alphabet);	
		}

	};
}
