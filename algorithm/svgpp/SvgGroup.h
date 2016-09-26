#pragma once

#include "SvgAbstractObject.h"
namespace svg
{
	class SvgGroup : public SvgAbstractObject
	{
	public:
		SvgGroup();
		~SvgGroup();
		ObjectType objectType()const { return ObjectType::Group; }

		virtual void render();
		virtual void renderId();

	public:
		std::vector<std::shared_ptr<SvgAbstractObject>> m_children;
	};
}