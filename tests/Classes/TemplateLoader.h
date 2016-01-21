//
//  TemplateLoader.h
//  roll-eat
//
//  Created by Hoang Le Hai on 5/27/15.
//
//

#ifndef RAE_TEMPLATE_LOADER_H
#define RAE_TEMPLATE_LOADER_H

#include "cocosbuilder/CCNodeLoader.h"

template<class NodeType, class ParentLoader>
class TemplateLoader : public ParentLoader {
public:
    static TemplateLoader<NodeType, ParentLoader>* loader() {
        auto ret = new TemplateLoader<NodeType, ParentLoader>();
        if (ret != nullptr) {
            ret->autorelease();
        } else {
            delete ret;
            ret = nullptr;
        }
        return ret;
    }

private:
    virtual NodeType* createNode(cocos2d::Node* pParent, cocosbuilder::CCBReader* ccbReader) {
        return NodeType::create();
    }
};

#endif /* RAE_TEMPLATE_LOADER_H */