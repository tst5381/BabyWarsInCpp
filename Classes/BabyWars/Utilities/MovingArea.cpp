#include "MovingArea.h"
#include "../../BabyEngine/Utilities/GridIndex.h"

MovingArea::MovingInfo::MovingInfo(int movingCost, int remainingMovementRange, bool canStay, const GridIndex & previousGrid) : m_MovingCost{ movingCost }, m_MaxRemainingMovementRange{ remainingMovementRange }, m_CanStay{ canStay }, m_PreviousIndex{ previousGrid }
{
}

MovingArea::MovingArea(int remainingMovementRange, const GridIndex & startingIndex)
{
	m_Map.emplace(startingIndex, MovingInfo(0, remainingMovementRange, true, startingIndex));
}

std::pair<MovingArea::MovingInfo, bool> MovingArea::getMovingInfo(const GridIndex & index) const
{
	const auto infoIter = m_Map.find(index);
	if (infoIter == m_Map.end())
		return std::make_pair(MovingInfo(), false);

	return std::make_pair(infoIter->second, true);
}

std::vector<GridIndex> MovingArea::getAllIndexesInArea() const
{
	auto indexes = std::vector<GridIndex>();
	for (const auto & indexInfo : m_Map)
		indexes.emplace_back(indexInfo.first);

	return indexes;
}

bool MovingArea::hasIndex(const GridIndex & index) const
{
	return m_Map.find(index) != m_Map.end();
}

void MovingArea::clear()
{
	m_Map.clear();
}

bool MovingArea::tryUpdateIndex(const GridIndex & index, MovingInfo && movingInfo)
{
	if (movingInfo.m_MaxRemainingMovementRange < 0)
		return false;

	auto indexIter = m_Map.find(index);
	if (indexIter == m_Map.end()){
		m_Map.emplace(std::make_pair(index, std::move(movingInfo)));
		return true;
	}

	if (indexIter->second.m_MaxRemainingMovementRange >= movingInfo.m_MaxRemainingMovementRange)
		return false;

	indexIter->second = std::move(movingInfo);
	return true;
}
