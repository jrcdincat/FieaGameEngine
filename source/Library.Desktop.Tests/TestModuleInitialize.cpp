#include "pch.h"
#include "CppUnitTest.h"
#include "TypeManager.h"
#include "AttributedFoo.h"
#include "AttributedBar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace FieaGameEngine;
using namespace UnitTests;

namespace UnitTestsLibraryDesktop
{
	TEST_MODULE_INITIALIZE(TestModuleInitialize)
	{
		TypeManager::CreateInstance();
	}

	TEST_MODULE_CLEANUP(TestModuleCleanup)
	{
		TypeManager::DestroyInstance();
	}
}