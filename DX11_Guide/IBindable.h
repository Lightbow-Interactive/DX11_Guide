#pragma once

#include "d3d11.h"
#include <vector>

class IBindable
{
public:
	virtual void Bind(ID3D11DeviceContext* deviceContext) = 0;
	
	static void BindBindables(ID3D11DeviceContext* deviceContext, const std::vector<IBindable*>& bindables)
	{
		for (int i = 0; i < bindables.size(); ++i)
		{
			if (bindables[i])
			{
				bindables[i]->Bind(deviceContext);
			}
		}
	}

};