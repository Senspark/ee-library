//
//  EEManagedScene.cpp
//  ee-library
//
//  Created by Zinge on 4/24/17.
//
//

#include "EEManagedScene.hpp"

namespace ee {
bool ManagedScene::init() {
    if (not Super::init()) {
        return false;
    }
    return true;
}

void ManagedScene::onEnter() {
    Super::onEnter();
}

void ManagedScene::onExit() {
    Super::onExit();
}
} // namespace ee
