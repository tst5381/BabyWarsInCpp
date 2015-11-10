#include <map>
#include <unordered_set>
#include <list>

#include "cocos2d/external/tinyxml2/tinyxml2.h"

#include "MovingAreaScript.h"
#include "MovingAreaGridScript.h"
#include "UnitMapScript.h"
#include "UnitScript.h"
#include "TileMapScript.h"
#include "../Resource/UnitData.h"
#include "../Utilities/MovingArea.h"
#include "../../BabyEngine/Actor/Actor.h"
#include "../../BabyEngine/Event/IEventDispatcher.h"
#include "../../BabyEngine/Event/EvtDataRequestDestroyActor.h"
#include "../../BabyEngine/GameLogic/BaseGameLogic.h"
#include "../../BabyEngine/Utilities/SingletonContainer.h"
#include "../../BabyEngine/Utilities/GridIndex.h"

//////////////////////////////////////////////////////////////////////////
//Definition of MovingRangeScriptImpl.
//////////////////////////////////////////////////////////////////////////
struct MovingAreaScript::MovingRangeScriptImpl
{
	MovingRangeScriptImpl() = default;
	~MovingRangeScriptImpl() = default;

	MovingArea calculateArea(const UnitScript & movingUnit, const TileMapScript & tileMap, const UnitMapScript & unitMap) const;
	void setChildrenGridActorWithIndexes(const MovingArea & movingArea, Actor & self);

	MovingArea m_MovingArea;
	std::string m_MovingRangeGridActorPath;
	std::unordered_set<ActorID> m_ChildrenGridActorIDs;
};

MovingArea MovingAreaScript::MovingRangeScriptImpl::calculateArea(const UnitScript & movingUnit, const TileMapScript & tileMap, const UnitMapScript & unitMap) const
{
	const auto originIndex = movingUnit.getGridIndex();
	const auto & movementType = movingUnit.getUnitData()->getMovementType();
	auto movingArea = MovingArea(movingUnit.getUnitData()->getMovementRange(), originIndex);
	auto visitedGridList = std::list<GridIndex>{originIndex};

	//Iterate through the visited list to see if the unit can move further. Uses BFS.
	for (const auto & currentIndex : visitedGridList){
		for (auto && nextIndex : currentIndex.getAdjacentIndexes()){
			if (!tileMap.canPassThrough(movementType, nextIndex) || !unitMap.canPassThrough(movingUnit, nextIndex))
				continue;

			auto currentlyRemainingMovementRange = movingArea.getMovingInfo(currentIndex).m_MaxRemainingMovementRange;
			auto movingCost = tileMap.getMovingCost(movementType, nextIndex);
			auto movingInfo = MovingArea::MovingInfo(movingCost, currentlyRemainingMovementRange - movingCost, unitMap.canUnitStayAtIndex(movingUnit, nextIndex), currentIndex);

			//Add the nextIndex into the visited list if we can update the area using the index.
			if (movingArea.tryUpdateIndex(nextIndex, std::move(movingInfo)))
				visitedGridList.emplace_back(std::move(nextIndex));
		}
	}

	return movingArea;
}

void MovingAreaScript::MovingRangeScriptImpl::setChildrenGridActorWithIndexes(const MovingArea & movingArea, Actor & self)
{
	auto gameLogic = SingletonContainer::getInstance()->get<BaseGameLogic>();

	for (const auto & index : movingArea.getAllIndexesInArea()){
		auto gridActor = gameLogic->createActor(m_MovingRangeGridActorPath.c_str());
		auto gridScript = gridActor->getComponent<MovingAreaGridScript>();
		gridScript->setGridIndexAndPosition(index);
		gridScript->setVisible(true);

		self.addChild(*gridActor);
		m_ChildrenGridActorIDs.emplace(gridActor->getID());
	}
}

//////////////////////////////////////////////////////////////////////////
//Implementation of WorldScript.
//////////////////////////////////////////////////////////////////////////
MovingAreaScript::MovingAreaScript() : pimpl{ std::make_unique<MovingRangeScriptImpl>() }
{
}

MovingAreaScript::~MovingAreaScript()
{
}

void MovingAreaScript::showArea(const UnitScript & movingUnit, const TileMapScript & tileMap, const UnitMapScript & unitMap)
{
	pimpl->m_MovingArea = pimpl->calculateArea(movingUnit, tileMap, unitMap);

	pimpl->setChildrenGridActorWithIndexes(pimpl->m_MovingArea, *m_OwnerActor.lock());
}

void MovingAreaScript::clearArea()
{
	m_OwnerActor.lock()->removeAllChildren();
	auto eventDispatcher = SingletonContainer::getInstance()->get<IEventDispatcher>();
	for (const auto & actorID : pimpl->m_ChildrenGridActorIDs)
		eventDispatcher->vQueueEvent(std::make_unique<EvtDataRequestDestroyActor>(actorID));

	pimpl->m_MovingArea.clear();
	pimpl->m_ChildrenGridActorIDs.clear();
}

const MovingArea & MovingAreaScript::getUnderlyingArea() const
{
	return pimpl->m_MovingArea;
}

bool MovingAreaScript::vInit(tinyxml2::XMLElement * xmlElement)
{
	auto relatedActorsPath = xmlElement->FirstChildElement("RelatedActorsPath");
	pimpl->m_MovingRangeGridActorPath = relatedActorsPath->Attribute("MovingRangeGrid");

	return true;
}

void MovingAreaScript::vPostInit()
{
}

const std::string MovingAreaScript::Type = "MovingAreaScript";

const std::string & MovingAreaScript::getType() const
{
	return Type;
}
