#include "cocos2d/external/tinyxml2/tinyxml2.h"

#include "../../BabyEngine/Actor/Actor.h"
#include "../../BabyEngine/GameLogic/BaseGameLogic.h"
#include "../../BabyEngine/Utilities/SingletonContainer.h"
#include "../../BabyEngine/Utilities/XMLToSize.h"
#include "TerrainInfoPanelScript.h"

//////////////////////////////////////////////////////////////////////////
//Definition of TerrainInfoPanelScriptImpl.
//////////////////////////////////////////////////////////////////////////
struct TerrainInfoPanelScript::TerrainInfoPanelScriptImpl
{
	TerrainInfoPanelScriptImpl() = default;
	~TerrainInfoPanelScriptImpl() = default;

	static std::string s_BackgroundActorPath;
	static cocos2d::Size s_PanelSize;
};

std::string TerrainInfoPanelScript::TerrainInfoPanelScriptImpl::s_BackgroundActorPath;

//////////////////////////////////////////////////////////////////////////
//Implementation of TerrainInfoPanelScript.
//////////////////////////////////////////////////////////////////////////
TerrainInfoPanelScript::TerrainInfoPanelScript() : pimpl{ std::make_unique<TerrainInfoPanelScriptImpl>() }
{
}

TerrainInfoPanelScript::~TerrainInfoPanelScript()
{
}

bool TerrainInfoPanelScript::vInit(const tinyxml2::XMLElement * xmlElement)
{
	static auto isStaticMemberInitialized = false;
	if (isStaticMemberInitialized)
		return true;

	auto relatedActorsElement = xmlElement->FirstChildElement("RelatedActorsPath");
	TerrainInfoPanelScriptImpl::s_BackgroundActorPath = relatedActorsElement->Attribute("Background");

	isStaticMemberInitialized = true;
	return true;
}

void TerrainInfoPanelScript::vPostInit()
{
	auto ownerActor = m_OwnerActor.lock();
	auto gameLogic = SingletonContainer::getInstance()->get<BaseGameLogic>();

	auto backgroundActor = gameLogic->createActor(TerrainInfoPanelScriptImpl::s_BackgroundActorPath.c_str());
	ownerActor->addChild(*backgroundActor);
}

const std::string TerrainInfoPanelScript::Type{ "TerrainInfoPanelScript" };

const std::string & TerrainInfoPanelScript::getType() const
{
	return Type;
}