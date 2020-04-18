#include "lightclass.h"


LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColour(float red, float green, float blue, float alpha)
{
	m_ambientColour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDiffuseColour(float red, float green, float blue, float alpha)
{
	m_diffuseColour = XMFLOAT4(red, green, blue, alpha);
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = XMFLOAT3(x, y, z);
	return;
}

XMFLOAT4 LightClass::GetAmbientColour()
{
	return m_ambientColour;
}

XMFLOAT4 LightClass::GetDiffuseColour()
{
	return m_diffuseColour;
}


XMFLOAT3 LightClass::GetDirection()
{
	return m_direction;
}