#ifndef __COMPONENTTRANSFORM_H__
#define __COMPONENTTRANSFORM_H__

#include "MathGeoLib.h"
#include "ComponentManager.h"

class GameObject;

class ComponentTransfrom : public ComponentManager
{
public:
	ComponentTransfrom(GameObject* owner);
	virtual ~ComponentTransfrom();

	float3 GetPosition();
	float3 GetRotation();
	float3 GetScale();

private:
	void OnCreate();
	void OnDestroy();
	void OnEnable();
	void OnDisable();

public:
	float4x4 transform;

};

#endif //!_COMPONENTTRANSFORM_H__