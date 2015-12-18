#include <cassert>

#include "cocos2d.h"

#include "../../BabyEngine/Actor/BaseRenderComponent.h"
#include "../Script/MovingAreaScript.h"
#include "../Script/UnitMapScript.h"
#include "../Script/UnitScript.h"
#include "GameCommand.h"
#include "UnitStateTypeCode.h"
#include "WaitingUnitState.h"

//////////////////////////////////////////////////////////////////////////
//Definition of WaitingUnitStateImpl.
//////////////////////////////////////////////////////////////////////////
struct WaitingUnitState::WaitingUnitStateImpl
{
	WaitingUnitStateImpl() = default;
	~WaitingUnitStateImpl() = default;

	void showUnitAppearanceInState(UnitScript & unit) const;
	void clearUnitAppearanceInState(UnitScript & unit) const;
};

void WaitingUnitState::WaitingUnitStateImpl::showUnitAppearanceInState(UnitScript & unit) const
{
	unit.getRenderComponent()->getSceneNode()->setColor(cocos2d::Color3B(150, 150, 150));
}

void WaitingUnitState::WaitingUnitStateImpl::clearUnitAppearanceInState(UnitScript & unit) const
{
	unit.getRenderComponent()->getSceneNode()->setColor(cocos2d::Color3B(255, 255, 255));
}

//////////////////////////////////////////////////////////////////////////
//Implementation of WaitingUnitState.
//////////////////////////////////////////////////////////////////////////
WaitingUnitState::WaitingUnitState() : pimpl{ std::make_unique<WaitingUnitStateImpl>() }
{
}

WaitingUnitState::~WaitingUnitState()
{
}

UnitStateTypeCode WaitingUnitState::vGetStateTypeCode() const
{
	return UnitStateTypeCode::Waiting;
}

void WaitingUnitState::onUnitEnterState(UnitScript & unit) const
{
	pimpl->showUnitAppearanceInState(unit);
}

void WaitingUnitState::onUnitExitState(UnitScript & unit) const
{
	pimpl->clearUnitAppearanceInState(unit);
}

void WaitingUnitState::vUpdateMovingArea(MovingAreaScript & movingArea, const UnitScript & unit) const
{
	movingArea.clearAreaForUnit(unit);
}

void WaitingUnitState::vUpdateUnitMap(UnitMapScript & unitMap, const std::shared_ptr<UnitScript> & unit) const
{
	assert(unit && "UnitState::vUpdateUnitMap() the unit is expired.");
	if (unitMap.isUnitFocused(*unit)) {
		unitMap.setFocusedUnit(nullptr);
	}
}

bool WaitingUnitState::vUpdateUnitOnTouch(UnitScript & unit, const std::shared_ptr<UnitScript> & touchedUnit) const
{
	return false;
}

bool WaitingUnitState::vCanMoveAlongPath() const
{
	return false;
}

bool WaitingUnitState::vCanUndoMove() const
{
	return false;
}

std::vector<GameCommand> WaitingUnitState::vGenerateGameCommandsForUnit(const std::shared_ptr<UnitScript> & targetUnit) const
{
	return{};
}
