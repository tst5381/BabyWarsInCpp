#include "EvtDataUnitIndexChangeEnd.h"

std::shared_ptr<const UnitScript> EvtDataUnitIndexChangeEnd::getUnit() const
{
	if (m_Unit.expired())
		return nullptr;

	return m_Unit.lock();
}

const GridIndex & EvtDataUnitIndexChangeEnd::getPreviousIndex() const
{
	return m_PreviousIndex;
}

const EventType EvtDataUnitIndexChangeEnd::s_EventType{ "EvtDataUnitIndexChangeEnd" };

const EventType & EvtDataUnitIndexChangeEnd::vGetType() const
{
	return s_EventType;
}
