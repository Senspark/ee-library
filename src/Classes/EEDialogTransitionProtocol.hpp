//
//  EEDialogTransitionProtocol.hpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#ifndef EEDialogTransitionProtocol_hpp
#define EEDialogTransitionProtocol_hpp

#include "EEMacro.hpp"
#include "EECocos2dxFwd.hpp"

#include <base/CCRefPtr.h>

NS_EE_BEGIN
namespace dialog {
class DialogManager;
class DialogTransitionProtocol;
} // namespace dialog

using dialog::DialogTransitionProtocol;

namespace dialog {
class DialogTransitionProtocol {
public:
    using TransitionType = cocos2d::FiniteTimeAction;
    using TransitionRef = cocos2d::RefPtr<TransitionType>;
    
    template<class... Args>
    using enable_if_args_are_transition_types =
        std::enable_if_t<
            (std::is_base_of<
                TransitionType,
                std::remove_pointer_t<std::decay_t<Args>>
             >::value && ...)
        >;
    
    virtual ~DialogTransitionProtocol() = default;
    
    void setShowingTransitions(const std::vector<TransitionRef>& transitions);
    void setHidingTransitions(const std::vector<TransitionRef>& transitions);
    
    void addShowingTransitions(const std::vector<TransitionRef>& transitions);
    void addHidingTransitions(const std::vector<TransitionRef>& transitions);
    
    const std::vector<TransitionRef>& getShowingActions() const;
    const std::vector<TransitionRef>& getHidingActions() const;
    
    template<class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    setShowingTransitions(Transitions&&... transitions) {
        setShowingTransitions({ std::forward<Transitions>(transitions)... });
    }
    
    template<class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    setHidingTransitions(Transitions&&... transitions) {
        setHidingTransitions({ std::forward<Transitions>(transitions)... });
    }
    
    template<class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    addShowingTransitions(Transitions&&... transitions) {
        addShowingTransitions({ std::forward<Transitions>(transitions)... });
    }
    
    template<class... Transitions>
    enable_if_args_are_transition_types<Transitions...>
    addHidingTransitions(Transitions&&... transitions) {
        addHidingTransitions({ std::forward<Transitions>(transitions)... });
    }
    
protected:
    friend class DialogManager;
    
    virtual void onShowBegan() {}
    virtual void onShowEnded() {}
    virtual void onHideBegan() {}
    virtual void onHideEnded() {}
    
private:
    std::vector<TransitionRef> showingTransitions_;
    std::vector<TransitionRef> hidingTransitions_;
};
} // namespace dialog
NS_EE_END

#endif /* EEDialogTransitionProtocol_hpp */
