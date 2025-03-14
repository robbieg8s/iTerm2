// -*- mode:objc -*-
// $Id: PTYScrollView.h,v 1.6 2004-03-14 06:05:38 ujwal Exp $
/*
 **  PTYScrollView.h
 **
 **  Copyright (c) 2002, 2003
 **
 **  Author: Fabian, Ujwal S. Setlur
 **      Initial code by Kiichi Kusama
 **
 **  Project: iTerm
 **
 **  Description: NSScrollView subclass. Handles user scroll detection.
 **
 **  This program is free software; you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation; either version 2 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program; if not, write to the Free Software
 **  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#import <Cocoa/Cocoa.h>

typedef NS_ENUM(NSUInteger, PTYScrollerState) {
    PTYScrollerStateOverlayHidden,
    PTYScrollerStateOverlayVisibleNarrow,
    PTYScrollerStateOverlayVisibleWide,
    PTYScrollerStateLegacy
};

@protocol PTYScrollerDelegate<NSObject>
- (void)userScrollDidChange:(BOOL)userScroll;
- (NSScrollView *)ptyScrollerScrollView NS_AVAILABLE_MAC(10_14);
- (void)ptyScrollerDidTransitionToState:(PTYScrollerState)state;
- (void)ptyScrollerFrameDidChange;
@end

@interface PTYScroller : NSScroller

@property(nonatomic, assign) id<PTYScrollerDelegate> ptyScrollerDelegate;
@property(nonatomic, assign) BOOL userScroll;
@property(nonatomic, readonly) PTYScrollerState ptyScrollerState;

@end

@interface PTYScrollView : NSScrollView

+ (BOOL)shouldDismember NS_AVAILABLE_MAC(10_14);

// More specific type for the base class's method.
- (PTYScroller *)ptyVerticalScroller;

- (instancetype)initWithFrame:(NSRect)frame hasVerticalScroller:(BOOL)hasVerticalScroller;
- (void)detectUserScroll;
- (BOOL)isLegacyScroller;

// Accumulate vertical scroll from the event. If it's enough to scroll one or more lines, deduct
// that from the total and return the number of rows to scroll by. The result will always be an
// integer.
- (CGFloat)accumulateVerticalScrollFromEvent:(NSEvent *)theEvent;

// These overrides a gross hack for putting extra content inside the scrollview next to terminal
// contents. The "rightExtra" value should arguably be folded into cSize/fSize, but then the type
// system wouldn't be able to help me find all the places where we add extra size to the right-hand
// side of a scrolleriew (such as we already do for legacy scrollers), which happens to be exactly
// what is needed for timestamps adjacent to content.
+ (NSSize)frameSizeForContentSize:(NSSize)cSize
          horizontalScrollerClass:(Class)horizontalScrollerClass
            verticalScrollerClass:(Class)verticalScrollerClass
                       borderType:(NSBorderType)type
                      controlSize:(NSControlSize)controlSize
                    scrollerStyle:(NSScrollerStyle)scrollerStyle NS_UNAVAILABLE;

+ (NSSize)frameSizeForContentSize:(NSSize)cSize
            hasHorizontalScroller:(BOOL)hFlag
              hasVerticalScroller:(BOOL)vFlag
                       borderType:(NSBorderType)type NS_UNAVAILABLE;

+ (NSSize)frameSizeForContentSize:(NSSize)cSize
          horizontalScrollerClass:(Class)horizontalScrollerClass
            verticalScrollerClass:(Class)verticalScrollerClass
                       borderType:(NSBorderType)type
                      controlSize:(NSControlSize)controlSize
                    scrollerStyle:(NSScrollerStyle)scrollerStyle
                       rightExtra:(CGFloat)rightExtra;

+ (NSSize)frameSizeForContentSize:(NSSize)cSize
            hasHorizontalScroller:(BOOL)hFlag
              hasVerticalScroller:(BOOL)vFlag
                       borderType:(NSBorderType)type
                       rightExtra:(CGFloat)rightExtra;

+ (NSSize)contentSizeForFrameSize:(NSSize)fSize
          horizontalScrollerClass:(Class)horizontalScrollerClass
            verticalScrollerClass:(Class)verticalScrollerClass
                       borderType:(NSBorderType)type
                      controlSize:(NSControlSize)controlSize
                    scrollerStyle:(NSScrollerStyle)scrollerStyle NS_UNAVAILABLE;

+ (NSSize)contentSizeForFrameSize:(NSSize)fSize
            hasHorizontalScroller:(BOOL)hFlag
              hasVerticalScroller:(BOOL)vFlag
                       borderType:(NSBorderType)type NS_UNAVAILABLE;

+ (NSSize)contentSizeForFrameSize:(NSSize)fSize
          horizontalScrollerClass:(Class)horizontalScrollerClass
            verticalScrollerClass:(Class)verticalScrollerClass
                       borderType:(NSBorderType)type
                      controlSize:(NSControlSize)controlSize
                    scrollerStyle:(NSScrollerStyle)scrollerStyle
                       rightExtra:(CGFloat)rightExtra;

+ (NSSize)contentSizeForFrameSize:(NSSize)fSize
            hasHorizontalScroller:(BOOL)hFlag
              hasVerticalScroller:(BOOL)vFlag
                       borderType:(NSBorderType)type
                       rightExtra:(CGFloat)rightExtra;

@end
