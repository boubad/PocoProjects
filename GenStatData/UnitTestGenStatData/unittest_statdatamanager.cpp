#include "stdafx.h"
#include "CppUnitTest.h"
//////////////////////////////////////
#include <statdatamanager.h>
////////////////////////////////////////
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace info;
//////////////////////////////////////////
namespace UnitTestGenStatData
{		
	TEST_CLASS(UnitTestStatDataManager)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			std::unique_ptr<StatDataManager> m_man;
			m_man.reset(new StatDataManager());
			StatDataManager *pMan = m_man.get();
			Assert::IsNotNull(pMan);
			Assert::IsTrue(pMan->is_valid());
			m_man.reset();
		}

	};
	
}