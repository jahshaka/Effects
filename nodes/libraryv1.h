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
		this->initTest();
		initMath();
		initObject();
	}

	void initMath();

	void initTest();

	void initObject();
};