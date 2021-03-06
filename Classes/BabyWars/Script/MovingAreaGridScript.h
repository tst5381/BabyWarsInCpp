#ifndef __MOVING_RANGE_GRID_SCRIPT__
#define __MOVING_RANGE_GRID_SCRIPT__

#include "../../BabyEngine/Actor/BaseScriptComponent.h"

struct GridIndex;

class MovingAreaGridScript : public BaseScriptComponent
{
public:
	MovingAreaGridScript();
	~MovingAreaGridScript();

	void setGridIndexAndPosition(const GridIndex & index);

	//Type name of the class. Used by the ActorFactory and can not be removed.
	static const std::string Type;
	virtual const std::string & getType() const override;

	//Disable copy/move constructor and operator=.
	MovingAreaGridScript(const MovingAreaGridScript &) = delete;
	MovingAreaGridScript(MovingAreaGridScript &&) = delete;
	MovingAreaGridScript & operator=(const MovingAreaGridScript &) = delete;
	MovingAreaGridScript & operator=(MovingAreaGridScript &&) = delete;

private:
	//Override functions.
	virtual bool vInit(const tinyxml2::XMLElement * xmlElement) override;
	virtual void vPostInit() override;

	//Implementation stuff.
	struct MovingAreaGridScriptImpl;
	std::unique_ptr<MovingAreaGridScriptImpl> pimpl;
};

#endif // !__MOVING_RANGE_GRID_SCRIPT__
