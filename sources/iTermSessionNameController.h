//
//  iTermSessionNameController.h
//  iTerm2SharedARC
//
//  Created by George Nachman on 5/16/18.
//

/*
 * Tab title can show:
 * - Profile name
 * - Job name
 * - Manually set name (including trigger-set name)
 * - Escape sequence set name
 * - Fallback: "Shell"
 *
 * In the new nomenclature:
 * session.name = Manually set name ?: Host-set name ?: Profile name
 *
 * Initial name comes from profile or name with substituions (e.g, $$FOO$$ -> user-entered string)
 * Restored state
 * Manual rename
 * Profile change: manual, APS, escape code, keystroke
 * Set window title
 * Set icon title
 * Profile name change
 *
 *
 *
 */
#import <Foundation/Foundation.h>

@class Profile;

@interface iTermSessionFormattingDescriptor : NSObject
@property (nonatomic) BOOL isTmuxGateway;  // self.isTmuxGateway
@property (nonatomic, copy) NSString *tmuxClientName;  // _tmuxController.clientName
@property (nonatomic) BOOL haveTmuxController;  // self.tmuxController != nil
@property (nonatomic, copy) NSString *tmuxWindowName;  // [_delegate tmuxWindowName]
@property (nonatomic) BOOL shouldShowJobName;  // [iTermPreferences boolForKey:kPreferenceKeyShowJobName]
@property (nonatomic) BOOL shouldShowProfileName;  // [iTermPreferences boolForKey:kPreferenceKeyShowProfileName]
@property (nonatomic, copy) NSString *jobName;  // self.jobName
@end

@protocol iTermSessionNameControllerDelegate<NSObject>

- (void)sessionNameControllerNameWillChangeTo:(NSString *)newName;
- (void)sessionNameControllerPresentationNameDidChangeTo:(NSString *)newName;
- (void)sessionNameControllerDidChangeWindowTitle;
- (iTermSessionFormattingDescriptor *)sessionNameControllerFormattingDescriptor;

@end

@interface iTermSessionNameController : NSObject

@property (nonatomic, weak) id<iTermSessionNameControllerDelegate> delegate;

// The first value assigned to sessionName. Used to decide if the profile name should be shown.
// It is removed when equal to the current name.
@property (nonatomic, readonly) NSString *firstSessionName;

// Session name; can be changed via escape code.
@property (nonatomic, readonly) NSString *sessionName;

// Name with extra formatting, ready to be shown in a title bar.
@property (nonatomic, readonly) NSString *presentationName;

@property (nonatomic, readonly) NSString *windowTitle;
@property (nonatomic, readonly) NSString *presentationWindowTitle;

// The original name cannot be changed by escape sequence. It is the name of the session or the
// user-assigned name.
@property (nonatomic, readonly) NSString *originalName;
@property (nonatomic, readonly) NSString *presentationOriginalName;
@property (nonatomic, readonly) NSString *terminalWindowName;
@property (nonatomic, readonly) NSString *terminalIconName;

+ (NSString *)titleFormatForProfile:(Profile *)profile;

- (instancetype)initWithProfileName:(NSString *)name
                        titleFormat:(NSString *)titleFormat NS_DESIGNATED_INITIALIZER;
- (instancetype)init NS_UNAVAILABLE;

- (void)didInitializeSessionWithName:(NSString *)newName;
- (void)profileNameDidChangeTo:(NSString *)newName;
- (void)profileDidChangeToProfileWithName:(NSString *)newName;
- (void)terminalDidSetWindowTitle:(NSString *)newName;
- (void)terminalDidSetIconTitle:(NSString *)newName;
- (void)triggerDidChangeNameTo:(NSString *)newName;
- (void)setTmuxTitle:(NSString *)tmuxTitle;
- (void)didSynthesizeFrom:(iTermSessionNameController *)real;
- (void)restoreNameFromStateDictionary:(NSDictionary *)state
                     legacyProfileName:(NSString *)legacyProfileName
                     legacySessionName:(NSString *)legacyName
                    legacyOriginalName:(NSString *)legacyOriginalName
                     legacyWindowTitle:(NSString *)legacyWindowTitle;
- (NSDictionary *)stateDictionary;

- (void)pushWindowTitle;
- (void)popWindowTitle;
- (void)pushIconTitle;
- (void)popIconTitle;

@end
