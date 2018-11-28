#pragma once

#include "../graph/library.h"

class LibraryV1 : public NodeLibrary
{
public:
	LibraryV1()
	{
		init();
	}

	void init()
	{
		initTest();
		initMath();
		initObject();
		initVector();
	}

	void initMath();

	void initTest();

	void initObject();

	void initVector();
};