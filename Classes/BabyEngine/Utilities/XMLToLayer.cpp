#include "cocos2d.h"

#include "XMLToLayer.h"

cocos2d::Layer * utilities::XMLToLayer(const tinyxml2::XMLElement * xmlElement)
{
	return cocos2d::Layer::create();
}
