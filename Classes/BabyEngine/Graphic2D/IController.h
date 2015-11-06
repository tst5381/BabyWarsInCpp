#ifndef __I_CONTROLLER__
#define __I_CONTROLLER__

#include <memory>

class Actor;

class IController
{
public:
	virtual ~IController() = default;

	virtual void setTarget(const std::shared_ptr<Actor> & actor) = 0;
};

#endif // !__I_CONTROLLER__
