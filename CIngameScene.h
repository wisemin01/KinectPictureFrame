#pragma once
#include "CScene.h"

class CHandCircle;
class CPictures;

class CIngameScene :
	public CScene
{
private:

	CHandCircle * m_RHCircle;
	CHandCircle * m_LHCircle;

	CPictures   * m_pPictures;

public:
	CIngameScene();
	virtual ~CIngameScene();

	virtual void Init()	override;
	virtual void Update()	override;
	virtual void Render()	override;
	virtual void Release()	override;
};
