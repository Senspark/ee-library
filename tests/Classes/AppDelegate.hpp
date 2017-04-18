#ifndef EE_LIBRARY_APP_DELEGATE_HPP
#define EE_LIBRARY_APP_DELEGATE_HPP

#include "platform/CCApplication.h"

/**
 * @brief    The cocos2d Application.
 * The reason for implement as private inheritance is to hide some interface
 * call by Director.
 */
class AppDelegate : private cocos2d::Application {
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual void initGLContextAttrs() override;

    /**
     * @brief    Implement Director and Scene init code here.
     * @return true    Initialize success, app continue.
     * @return false   Initialize failed, app terminate.
     */
    virtual bool applicationDidFinishLaunching() override;

    /**
     * @brief  The function be called when the application enter background
     * @param  the pointer of the application
     */
    virtual void applicationDidEnterBackground() override;

    /**
     * @brief  The function be called when the application enter foreground
     * @param  the pointer of the application
     */
    virtual void applicationWillEnterForeground() override;
};

#endif // EE_LIBRARY_APP_DELEGATE_HPP
