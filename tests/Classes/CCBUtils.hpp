//
//  CCBUtils.h
//  roll-eat
//
//  Created by Hoang Le Hai on 5/26/15.
//
//

#ifndef EE_LIBRARY_CCB_UTILS_HPP
#define EE_LIBRARY_CCB_UTILS_HPP

#include <string>

class CCBUtils {
public:
    static void initLoaders();

    static cocos2d::Scene* openScene(const std::string& filename);
    static cocos2d::Node* openNode(const std::string& filename);
};

#endif /* EE_LIBRARY_CCB_UTILS_HPP */
