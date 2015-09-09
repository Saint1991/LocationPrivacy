#include "stdafx.h"
#include "CppUnitTest.h"
#include "Bindable.h"
#include "cppconn\prepared_statement.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UtilsTest
{
	TEST_CLASS(BindableTest)
	{
	public:

		class BindableSample : public Bindable {
			
			void bind(sql::PreparedStatement* statement, const Db::TableStructure& table) const {

			}
			
			void bind(sql::PreparedStatement* statement, const std::list<std::string>& columns) const {

			}
		};

		TEST_METHOD(CompileCheck)
		{
			BindableSample sample;
		}
	};
}
