//
//  EEManagedScene.hpp
//  ee-library
//
//  Created by Zinge on 4/24/17.
//
//

#ifndef EE_LIBRARY_MANAGED_SCENE_HPP_
#define EE_LIBRARY_MANAGED_SCENE_HPP_

#include <2d/CCScene.h>

namespace ee {
class ManagedScene : public cocos2d::Scene {
private:
    using Super = cocos2d::Scene;

public:
    CREATE_FUNC(ManagedScene);

protected:
    virtual bool init() override;

    virtual void onEnter() override;
    virtual void onExit() override;
};
} // namespace ee

#endif /* EE_LIBRARY_MANAGED_SCENE_HPP_ */
