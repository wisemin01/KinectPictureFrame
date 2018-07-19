#pragma once
class CScene abstract
{
public:
	CScene();
	virtual ~CScene();

	virtual void Init() PURE;
	virtual void Update() PURE;
	virtual void Render() PURE;
	virtual void Release() PURE;
};

