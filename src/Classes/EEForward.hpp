//
//  EEForward.hpp
//  ee-library.
//
//  Created by Hoang Le Hai on 7/20/15.
//
//

#ifndef EE_LIBRARY_FORWARD_HPP_
#define EE_LIBRARY_FORWARD_HPP_

/// Forward ee library classes.

namespace ee {
template <class Np, class Lp, class... Ts> class GenericLoader;

template <class T> class DynamicValue;

namespace detail {
class ButtonEx;
} // namespace detail

using Button = detail::ButtonEx;

class CheckBox;

namespace dialog {
class Dialog;
class DialogManager;
} // namespace dialog

using dialog::Dialog;
using dialog::DialogManager;

template <class T> class BackButtonListener;

class NodeV3Loader;
class UiWidgetLoader;
class UiButtonLoader;
class UiTextLoader;
class UiLayoutLoader;

class SpriteWithHSV;
class SpriteWithHSVLoader;

class BlurBackground;
class BlurBackgroundLoader;

template <int Id, class... Args> class EventInfo;
class EventDispatcher;

template <int Id, class Value, class... Keys> class DataInfo;
class DataHandler;

template <class T> struct DataTraits;
} // namespace ee

#endif // EE_LIBRARY_FORWARD_HPP_
