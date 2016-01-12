//
//  EECheckBox.cpp
//  roll-eat
//
//  Created by enrevol on 10/3/15.
//
//

#include "EECheckBox.hpp"

#include <algorithm>

namespace_ee_begin
namespace_detail_begin
CheckBoxBase::~CheckBoxBase() {}

bool CheckBoxBase::isChecked() const {
    return _isChecked;
}

void CheckBoxBase::check() {
    setChecked(true);
}
    
void CheckBoxBase::uncheck() {
    setChecked(false);
}
    
void CheckBoxBase::setChecked(bool checked) {
    if (_isChecked != checked) {
        _isChecked = checked;
        onStateChanged(checked);
        if (_onStateChangedCallback != nullptr) {
            _onStateChangedCallback(checked);
        }
    }
}
    
void CheckBoxBase::setOnStateChangedCallback(const std::function<void(bool)>& callback) {
    _onStateChangedCallback = callback;
}
namespace_detail_end
    
void CheckAllBox::addCheckBox(CheckBox* checkBox) {
    _checkBoxList.push_back(checkBox);
    checkBox->_checkAllBox = this;
}

const std::vector<CheckBox*>& CheckAllBox::getCheckBoxList() const {
    return _checkBoxList;
}
    
void CheckAllBox::clearCheckBoxList() {
    _checkBoxList.clear();
}
    
void CheckAllBox::setChecked(bool checked) {
    CheckBoxBase::setChecked(checked);
    std::for_each(_checkBoxList.cbegin(), _checkBoxList.cend(), [checked](CheckBox* box) {
        box->setChecked(checked);
    });
}
    
bool CheckAllBox::all() const {
    return std::all_of(_checkBoxList.cbegin(), _checkBoxList.cend(), [](CheckBox* box) {
        return box->isChecked();
    });
}

bool CheckAllBox::any() const {
    return std::any_of(_checkBoxList.cbegin(), _checkBoxList.cend(), [](CheckBox* box) {
        return box->isChecked();
    });
}

bool CheckAllBox::none() const {
    return std::none_of(_checkBoxList.cbegin(), _checkBoxList.cend(), [](CheckBox* box) {
        return box->isChecked();
    });
}
    
void CheckAllBox::updateState() {
    bool newState = std::all_of(_checkBoxList.cbegin(), _checkBoxList.cend(), [](CheckBox* box) {
        return box->isChecked();
    });
    if (newState != isChecked()) {
        CheckBoxBase::setChecked(newState);
    }
}
    
void CheckBox::setChecked(bool checked) {
    CheckBoxBase::setChecked(checked);
    _checkAllBox->updateState();
}
namespace_ee_end