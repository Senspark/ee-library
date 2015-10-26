//
//  EECheckBox.h
//  roll-eat
//
//  Created by enrevol on 10/3/15.
//
//

#ifndef EE_LIBRARY_CHECK_BOX_H
#define EE_LIBRARY_CHECK_BOX_H

#include "EEMacro.hpp"

#include <functional>
#include <vector>

namespace_ee_begin
class CheckBox;

class CheckBoxBase {
public:
    bool isChecked() const;
    virtual void setChecked(bool checked);
    
    void uncheck();
    void check();

    void setOnStateChangedCallback(const std::function<void(bool)>& callback);
    
protected:
    virtual void onStateChanged(bool checked) = 0;
    
private:
    bool _isChecked;
    std::function<void(bool)> _onStateChangedCallback;
};

class CheckAllBox : public CheckBoxBase {
public:
    void addCheckBox(CheckBox* checkBox);
    
    const std::vector<CheckBox*>& getCheckBoxList() const;
    void clearCheckBoxList();
    
    // Test whether all sub checkboxes are checked.
    bool all() const;
    
    // Test whether any sub checkbox is checked.
    bool any() const;
    
    // Test whether no sub checkbox is checked.
    bool none() const;
    
    virtual void setChecked(bool checked) override;
    
private:
    friend class CheckBox;
    
    void updateState();
    
    std::vector<CheckBox*> _checkBoxList;
};

class CheckBox : public CheckBoxBase {
public:
    virtual void setChecked(bool checked) override;
    
private:
    friend class CheckAllBox;
    
    CheckAllBox* _checkAllBox;
};
namespace_ee_end

#endif /* EE_LIBRARY_CHECK_BOX_H */
