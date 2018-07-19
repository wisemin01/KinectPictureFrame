#include "DXUT.h"
#include "CImageFileLoader.h"


CImageFileLoader::CImageFileLoader()
{
}


CImageFileLoader::~CImageFileLoader()
{
}

vector<texture*>* CImageFileLoader::LoadImages(const string& sFolderName)
{

	vector<texture*>* textures = new vector<texture*>;

	system(("dir/b .\\" + sFolderName + "\\ > list.dat").c_str());

	FILE* fp;
	fp = fopen("list.dat", "rt");

	char curFileName[MAX_PATH]= "";

	while (!feof(fp)) 
	{
		fscanf(fp, "%s", curFileName);

		string sCurPath = "./" + sFolderName; sCurPath += "/";
		sCurPath += curFileName;

		textures->push_back(IMAGEMANAGER->GetTexture(sCurPath, sCurPath));
	}

	textures->erase(textures->end() - 1);

	fclose(fp);

	return textures;
}
