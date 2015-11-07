//
//  EEForward.hpp
//  roll-eat
//
//  Created by Hoang Le Hai on 7/20/15.
//
//

#ifndef EE_LIBRARY_FORWARD_H
#define EE_LIBRARY_FORWARD_H

namespace cocos2d {
class Ref;
class Node;
class ClippingNode;
class ClippingRectangleNode;
class Scene;
class Layer;
class Sprite;
class SpriteFrame;
class Image;
class Label;
class Value;
class Vec2;
using Point = Vec2;
class __Array;
using Array = __Array;
class __Dictionary;
using Dictionary = __Dictionary;
class Event;
class EventCustom;
class EventDispatcher;
class EventListenerCustom;
class EventListenerKeyboard;
class EventListenerTouchOneByOne;
class EventListenerTouchAllAtOnce;
class Touch;
class Size;
class ProgressTimer;
class Rect;
class ParticleSystemQuad;
class Animation;
class ActionEase;
class ActionFloat;
class ActionInterval;
class Texture2D;
struct Color3B;
struct Color4B;
struct Color4F;
namespace extension {
class ScrollView;
class ControlButton;
} // namespace extension
namespace network {
class HttpClient;
class HttpResponse;
} // namespace network
namespace ui {
class Scale9Sprite;
} // namespace ui
} // namespace cocos2d

namespace CocosDenshion {
class SimpleAudioEngine;
} // namespace CocosDenshion

namespace spine {
class SkeletonAnimation;
} // namespace spine

namespace ee {
enum class ButtonState;
class NodeButton;
class Button;
class ActiveNodeButton;
class ActiveButton;
class Dialog;
class CheckBox;
class CheckAllBox;
    
template<class... Args>
class DataInfo;
    
template<class... Args>
class JniDataInfo;
    
template<class... Args>
class EventInfo;
    
template<class... Args>
class JniEventInfo;
    
template<class... Args>
using Event [[deprecated("Use EventInfo instead.")]] = EventInfo<Args...>;
} // namespace utils

namespace soomla {
class CCVirtualCurrency;
class CCVirtualCurrencyPack;
class CCSingleUseVG;
class CCSingleUsePackVG;
class CCError;
} // namespace soomla

#endif // EE_LIBRARY_FORWARD_H
