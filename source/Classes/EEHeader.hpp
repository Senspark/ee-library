//
//  EEHeader.hpp
//  ee-library
//
//  Created by enrevol on 10/26/15.
//
//

#ifndef EE_LIBRARY_HEADER_HPP_
#define EE_LIBRARY_HEADER_HPP_

#include "EEForward.hpp"
#include "EEMacro.hpp"
#include "EENonCopyable.hpp"
#include "EENonMovable.hpp"
#include "EENonCopyable.hpp"
#include "EEFunctionTraits.hpp"
#include "EEEventInfo.hpp"
#include "EEData.hpp"
#include "EEDataInfo.hpp"
#include "EEImage.hpp"
#include "EECrytography.hpp"
#include "EEUtils.hpp"
#include "EECocosUtils.hpp"
#include "EEGesture.hpp"
#include "EEDynamicValue.hpp"
#include "EEDialog.hpp"
#include "EEDialogManager.hpp"
#include "EETouchManager.hpp"
#include "EETouchListener.hpp"
#include "EEGroupNode.hpp"
#include "EECheckBox.hpp"
#include "EEBackButtonListener.hpp"
#include "EEControl.hpp"
#include "EEActiveControl.hpp"
#include "EEControlLoader.hpp"
#include "EEConstexprString.hpp"
#include "EEGenericLoader.hpp"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#   include "EEJni.hpp"
#   include "EEJniBoxedType.hpp"
#   include "EEJniCaller.hpp"
#   include "EEJniConverter.hpp"
#   include "EEJniDestructor.hpp"
#   include "EEJniException.hpp"
#   include "EEJniFieldInfo.hpp"
#   include "EEJniMethodInfo.hpp"
#   include "EEJniObject.hpp"
#   include "EEJniString.hpp"
#   include "EEJniUtils.hpp"
#endif

#endif /* EE_LIBRARY_HEADER_HPP_ */
