//
//  EEDialogTransitionProtocol.cpp
//  ee-library
//
//  Created by Zinge on 5/12/16.
//
//

#include "EEDialogTransitionProtocol.hpp"

NS_EE_BEGIN
namespace dialog {
void DialogTransitionProtocol::setShowingTransitions(const std::vector<TransitionRef>& transitions) {
    showingTransitions_ = transitions;
}

void DialogTransitionProtocol::setHidingTransitions(const std::vector<TransitionRef>& transitions) {
    hidingTransitions_ = transitions;
}
    
void DialogTransitionProtocol::addShowingTransitions(const std::vector<TransitionRef>& transitions) {
    showingTransitions_.insert(showingTransitions_.cend(),
                               transitions.cbegin(), transitions.cend());
}

void DialogTransitionProtocol::addHidingTransitions(const std::vector<TransitionRef>& transitions) {
    hidingTransitions_.insert(hidingTransitions_.cend(),
                              transitions.cbegin(), transitions.cend());
}
    
auto DialogTransitionProtocol::getShowingActions() const -> const std::vector<TransitionRef>& {
    return showingTransitions_;
}

auto DialogTransitionProtocol::getHidingActions() const -> const std::vector<TransitionRef>&  {
    return hidingTransitions_;
}
} // namespace dialog
NS_EE_END