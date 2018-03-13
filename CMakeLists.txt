cmake_minimum_required(VERSION 3.1)

project(ee_library)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wchar-subscripts -Wcomment -Wformat=2 -Wnonnull -Winit-self")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wmain -Wmissing-braces -Wmissing-include-dirs -Wparentheses -Wsequence-point")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wreturn-type -Waddress -Wmissing-declarations -Wmissing-field-initializers")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wunused-function -Wunused-label -Wuninitialized")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z")

include_directories(
    src/Classes
    src/Classes/experimental
)

set(LIB_SRC
    src/Classes/EESceneSwitcher.cpp
    src/Classes/EEDialogComponent.cpp
    src/Classes/EESha1.cpp
    src/Classes/EEDialogGuard.cpp
    src/Classes/EEUtils.cpp
    src/Classes/EEClippingRectangleNodeLoader.cpp
    src/Classes/EEImageBuilder.cpp
    src/Classes/EEDataUtils.cpp
    src/Classes/EEImage.cpp
    src/Classes/EEBlurBackground.cpp
    src/Classes/EEButtonLoader.cpp
    src/Classes/EESpriteWithHsvLoader.cpp
    src/Classes/EEUiTextLoader.cpp
    src/Classes/EELanguageDelegate.cpp
    src/Classes/EEHsvProtocol.cpp
    src/Classes/EEContinuousAction.cpp
    src/Classes/EESkeletonAnimationLoader.cpp
    src/Classes/EEShaderUtils.cpp
    src/Classes/EEAction.cpp
    src/Classes/EEScale9SpriteWithHsv.cpp
    src/Classes/EELanguageFormatter.cpp
    src/Classes/EEUiWidgetCallback.cpp
    src/Classes/EESkeletonBone.cpp
    src/Classes/EEBlurBackgroundLoader.cpp
    src/Classes/EEDialogCommand.cpp
    src/Classes/EEBoxBlur1D.cpp
    src/Classes/EEDialogManager.cpp
    src/Classes/EEDialogTree.cpp
    src/Classes/EEDynamicValue.cpp
    src/Classes/EEClippingRectangleNode.cpp
    src/Classes/EEBackButtonComponent.cpp
    src/Classes/EEUiWidgetLoader.cpp
    src/Classes/EEConsole.cpp
    src/Classes/EEUiCheckBoxLoader.cpp
    src/Classes/EEDataHandler.cpp
    src/Classes/EETransitionFadeOut.cpp
    src/Classes/EEButton.cpp
    src/Classes/EEMd5.cpp
    src/Classes/EEEventInfoV1.cpp
    src/Classes/EEBackButtonListener.cpp
    src/Classes/EEPreferences.cpp
    src/Classes/EEUiLayoutLoader.cpp
    src/Classes/EESpineFactory.cpp
    src/Classes/EEShaderManager.cpp
    src/Classes/EELanguage.cpp
    src/Classes/EECrc.cpp
    src/Classes/EEBoxBlur2D.cpp
    src/Classes/EEPoolUtils.cpp
    src/Classes/EENodeV3Loader.cpp
    src/Classes/EEDialogUtils.cpp
    src/Classes/EELanguageSwitcher.cpp
    src/Classes/EEEventDispatcher.cpp
    src/Classes/EEMultilingualDelegate.cpp
    src/Classes/EEUiButtonLoader.cpp
    src/Classes/EEConsoleDelegate.cpp
    src/Classes/EEDataTraits.cpp
    src/Classes/EETentBlur1D.cpp
    src/Classes/EEShader.cpp
    src/Classes/EESpriteWithHsv.cpp
    src/Classes/EEManagedScene.cpp
    src/Classes/EESkeletonBoneLoader.cpp
    src/Classes/EEDialog.cpp
    src/Classes/EECheckBox.cpp
    src/Classes/EECommandPool.cpp
)

set(LIB_HEADERS
    src/Classes/EEDataInfo.hpp
    src/Classes/EEMeta.hpp
    src/Classes/EEPool.hpp
    src/Classes/EEEvent.hpp
    src/Classes/EEDataTraits.hpp
    src/Classes/EEShader.hpp
    src/Classes/EESpriteWithHsv.hpp
    src/Classes/EEManagedScene.hpp
    src/Classes/EESkeletonBoneLoader.hpp
    src/Classes/EEHeader.hpp
    src/Classes/EEDialog.hpp
    src/Classes/EECheckBox.hpp
    src/Classes/EECommandPool.hpp
    src/Classes/EEButton.hpp
    src/Classes/EECocos2dxFwd.hpp
    src/Classes/EEEventInfoV1.hpp
    src/Classes/EEEventListener.hpp
    src/Classes/EEBackButtonListener.hpp
    src/Classes/EEUiLayoutLoader.hpp
    src/Classes/EEPreferences.hpp
    src/Classes/EESpineFactory.hpp
    src/Classes/EEShaderManager.hpp
    src/Classes/EEEventInfoV2.hpp
    src/Classes/EELanguage.hpp
    src/Classes/EEPoolUtils.hpp
    src/Classes/EEDialogUtils.hpp
    src/Classes/EENodeV3Loader.hpp
    src/Classes/EELanguageSwitcher.hpp
    src/Classes/EEDataMeta.hpp
    src/Classes/EEEventDispatcher.hpp
    src/Classes/EEMultilingualDelegate.hpp
    src/Classes/EEUiButtonLoader.hpp
    src/Classes/EEConsoleDelegate.hpp
    src/Classes/EELanguageFormatter.hpp
    src/Classes/EEAction.hpp
    src/Classes/EEScale9SpriteWithHsv.hpp
    src/Classes/EEBlurBackgroundLoader.hpp
    src/Classes/EEDialogCommand.hpp
    src/Classes/EESkeletonBone.hpp
    src/Classes/EEUiWidgetCallback.hpp
    src/Classes/EEDialogTree.hpp
    src/Classes/EEDialogManager.hpp
    src/Classes/EEDynamicValue.hpp
    src/Classes/EELazyPtr.hpp
    src/Classes/EEBackButtonComponent.hpp
    src/Classes/EEClippingRectangleNode.hpp
    src/Classes/EEGenericLoader.hpp
    src/Classes/EEUiCheckBoxLoader.hpp
    src/Classes/EEUiWidgetLoader.hpp
    src/Classes/EEConsole.hpp
    src/Classes/EEForward.hpp
    src/Classes/EEScopeGuard.hpp
    src/Classes/EETransitionFadeOut.hpp
    src/Classes/EEDataHandler.hpp
    src/Classes/EEFlags.hpp
    src/Classes/EESceneSwitcher.hpp
    src/Classes/EEDialogComponent.hpp
    src/Classes/EEDialogGuard.hpp
    src/Classes/EEUtils.hpp
    src/Classes/EEClippingRectangleNodeLoader.hpp
    src/Classes/EEImageBuilder.hpp
    src/Classes/EEDataUtils.hpp
    src/Classes/EEMacro.hpp
    src/Classes/EEImage.hpp
    src/Classes/EEButtonLoader.hpp
    src/Classes/EEBlurBackground.hpp
    src/Classes/EESpriteWithHsvLoader.hpp
    src/Classes/EEUiTextLoader.hpp
    src/Classes/EECrytography.hpp
    src/Classes/EEHsvProtocol.hpp
    src/Classes/EELanguageDelegate.hpp
    src/Classes/EESpineMacro.hpp
    src/Classes/EESkeletonAnimationLoader.hpp
    src/Classes/EEContinuousAction.hpp
    src/Classes/EEShaderUtils.hpp
)

add_library(${PROJECT_NAME} STATIC ${LIB_SRC} ${LIB_HEADERS})

target_link_libraries(${PROJECT_NAME}
    cocos2d
)

target_include_directories(${PROJECT_NAME} PUBLIC
    src/Classes
    src/Classes/experimental
)