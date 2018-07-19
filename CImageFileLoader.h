#pragma once
class CImageFileLoader
{
public:
	CImageFileLoader();
	virtual ~CImageFileLoader();

	virtual vector<texture*>* LoadImages(const string& sFolderName);
};

