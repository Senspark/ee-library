#ifndef EE_LIBRARY_ANALYTICS_H
#define EE_LIBRARY_ANALYTICS_H

#include "AppConfigs.h"
#include "AppMacros.h"
#include "EEMacro.h"

// TRACKING_PREFIX should be declared in AppMacros.h.
#ifndef TRACKING_PREFIX
#   define TRACKING_PREFIX
#endif

#ifndef INTEGRATE_GOOGLE_ANALYTICS
#   define INTEGRATE_GOOGLE_ANALYTICS 0
#endif

#define TRACK_EVENT(...) GET_FUNCTION(TRACK_EVENT_, COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

// Tracking macros for Google Analytics.
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS) && (INTEGRATE_GOOGLE_ANALYTICS)
#   define TRACK_EVENT_4(category, action, label, value)        senspark::AnalyticsUtils::trackEvent(TRACKING_PREFIX category, action, label, value)
#   define TRACK_EVENT_3(category, action, label)               TRACK_EVENT_4(category, action, label, -1)
#   define TRACK_EVENT_2(category, action)                      TRACK_EVENT_4(category, action, "", -1)
#   define TRACK_SCREEN(screenName)                             senspark::AnalyticsUtils::trackScreen(TRACKING_PREFIX screenName)
#   define TRACK_USER_TIMING(category, name, label, value)      senspark::AnalyticsUtils::trackUserTiming(TRACKING_PREFIX category, name, label, value)
#else
#   define TRACK_EVENT_4(category, action, label, value)
#   define TRACK_EVENT_3(category, action, label)
#   define TRACK_EVENT_2(category, action)
#   define TRACK_SCREEN(screenName)
#   define TRACK_USER_TIMING(category, name, label, value)
#endif

#endif // EE_LIBRARY_ANALYTICS_H