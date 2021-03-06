/**
 * Copyright (c) 2015-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#import <AppKit/AppKit.h>

#import "RCTBridge.h"
#import "RCTBridgeModule.h"
#import "RCTInvalidating.h"
#import "RCTViewManager.h"

/**
 * Posted right before re-render happens. This is a chance for views to invalidate their state so
 * next render cycle will pick up updated views and layout appropriately.
 */
RCT_EXTERN NSString *const RCTUIManagerWillUpdateViewsDueToContentSizeMultiplierChangeNotification;

/**
 * Posted whenever a new root view is registered with RCTUIManager. The userInfo property
 * will contain a RCTUIManagerRootViewKey with the registered root view.
 */
RCT_EXTERN NSString *const RCTUIManagerDidRegisterRootViewNotification;

/**
 * Posted whenever a root view is removed from the RCTUIManager. The userInfo property
 * will contain a RCTUIManagerRootViewKey with the removed root view.
 */
RCT_EXTERN NSString *const RCTUIManagerDidRemoveRootViewNotification;

/**
 * Key for the root view property in the above notifications
 */
RCT_EXTERN NSString *const RCTUIManagerRootViewKey;

@protocol RCTScrollableProtocol;

/**
 * The RCTUIManager is the module responsible for updating the view hierarchy.
 */
@interface RCTUIManager : NSObject <RCTBridgeModule, RCTInvalidating>

/**
 * Register a root view with the RCTUIManager.
 */
- (void)registerRootView:(NSView *)rootView;

/**
 * Gets the view associated with a reactTag.
 */
- (NSView *)viewForReactTag:(NSNumber *)reactTag;

/**
 * Update the frame of a view. This might be in response to a screen rotation
 * or some other layout event outside of the React-managed view hierarchy.
 */
- (void)setFrame:(CGRect)frame forView:(NSView *)view;

/**
 * Set the natural size of a view, which is used when no explicit size is set.
 * Use UIViewNoIntrinsicMetric to ignore a dimension.
 */
- (void)setIntrinsicContentSize:(CGSize)size forView:(NSView *)view;

/**
 * Update the background color of a view. The source of truth for
 * backgroundColor is the shadow view, so if to update backgroundColor from
 * native code you will need to call this method.
 */
- (void)setBackgroundColor:(NSColor *)color forView:(NSView *)rootView;

/**
 * Schedule a block to be executed on the UI thread. Useful if you need to execute
 * view logic after all currently queued view updates have completed.
 */
- (void)addUIBlock:(RCTViewManagerUIBlock)block;

/**
 * The view that is currently first responder, according to the JS context.
 */
+ (NSView *)JSResponder;

/**
 * Normally, UI changes are not applied until the complete batch of method
 * invocations from JavaScript to native has completed.
 *
 * Setting this to YES will flush UI changes sooner, which could potentially
 * result in inconsistent UI updates.
 *
 * The default is NO (recommended).
 */
@property (atomic, assign) BOOL unsafeFlushUIChangesBeforeBatchEnds;

/**
 * In some cases we might want to trigger layout from native side.
 * React won't be aware of this, so we need to make sure it happens.
 */
- (void)setNeedsLayout;

@end

/**
 * This category makes the current RCTUIManager instance available via the
 * RCTBridge, which is useful for RCTBridgeModules or RCTViewManagers that
 * need to access the RCTUIManager.
 */
@interface RCTBridge (RCTUIManager)

@property (nonatomic, readonly) RCTUIManager *uiManager;

@end
