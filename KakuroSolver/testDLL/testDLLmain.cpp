#include <Windows.h>
#include <iostream>
#include <utility>
#include <list>
#include "..\KakuroSolver\kkrsolv.h"
#include "testDLL.h"
#include "testutil.h"

int main()
{
	int maj, min, rel;
	std::list<std::pair<const wchar_t *, bool>> results;

	logMessage(L"****************************\n** Kakuro Solver DLL test **\n****************************\n");
	KkrGetVersion(&maj, &min, &rel);
	logMessage(L"Solver version: ");
	logValue(maj); logMessage(L"."); logValue(min); logMessage(L"."); logValue(rel);
	logMessage(L"\n\n");

	results.push_back(std::make_pair(L"test1", test1()));
	std::cout << std::endl;
	results.push_back(std::make_pair(L"test2", test2()));
	std::cout << std::endl;
	results.push_back(std::make_pair(L"test3", test3()));

	logMessage(L"\n*******************\n***** SUMMARY *****\n*******************\n");
	for(auto result : results) {
		logResult(result.first, result.second);
	}

	return 0;
}
