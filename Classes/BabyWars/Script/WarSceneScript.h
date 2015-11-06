#ifndef __WAR_SCENE_SCRIPT__
#define __WAR_SCENE_SCRIPT__

#include "../../BabyEngine/Actor/BaseScriptComponent.h"

namespace cocos2d{
	class Vec2;
}

class WarSceneScript : public BaseScriptComponent
{
public:
	WarSceneScript();
	~WarSceneScript();

	//Type name of the class. Used by the ActorFactory and can not be removed.
	static const std::string Type;
	virtual const std::string & getType() const override;

	void loadWarScene(const char * xmlPath);

	bool canActivateUnitAtPosition(const cocos2d::Vec2 & pos) const;
	bool isUnitActiveAtPosition(const cocos2d::Vec2 & pos) const;

	//Disable copy/move constructor and operator=.
	WarSceneScript(const WarSceneScript &) = delete;
	WarSceneScript(WarSceneScript &&) = delete;
	WarSceneScript & operator=(const WarSceneScript &) = delete;
	WarSceneScript & operator=(WarSceneScript &&) = delete;

private:
	//Override functions.
	virtual bool vInit(tinyxml2::XMLElement *xmlElement) override;
	virtual void vPostInit() override;

	//Implementation stuff.
	struct WarSceneScriptImpl;
	std::shared_ptr<WarSceneScriptImpl> pimpl;
};

#endif // !__WAR_SCENE_SCRIPT__
