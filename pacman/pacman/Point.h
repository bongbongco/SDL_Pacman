#pragma once
#include "Component.h"

class CPoint : public CComponent {
public:
	CPoint(CObject *_object) : CComponent(_object) {
	}

	virtual void Update() {}
	virtual void Render() {}
};