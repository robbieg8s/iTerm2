//
//  iTermSetupCfgParser.m
//  iTerm2SharedARC
//
//  Created by George Nachman on 6/24/18.
//

#import "iTermSetupCfgParser.h"

#import "NSArray+iTerm.h"
#import "NSStringITerm.h"
#import "RegexKitLite.h"
#import "ini.h"

@implementation iTermSetupCfgParser {
    NSMutableArray<NSString *> *_installRequiresParts;
    NSArray<NSString *> *_dependencies;
}

+ (void)writeSetupCfgToFile:(NSString *)file
                       name:(NSString *)name
               dependencies:(NSArray<NSString *> *)dependencies
        ensureiTerm2Present:(BOOL)ensureiTerm2Present
              pythonVersion:(NSString *)pythonVersion
         environmentVersion:(NSInteger)environmentVersion {
    assert(pythonVersion);

    NSString *sanitizedPythonVersion = pythonVersion;
    NSArray<NSString *> *parts = [pythonVersion componentsSeparatedByString:@"."];
    if (parts.count > 2) {
        sanitizedPythonVersion = [[parts subarrayToIndex:2] componentsJoinedByString:@"."];
    }
    if (ensureiTerm2Present && ![dependencies containsObject:@"iterm2"]) {
        dependencies = [@[ @"iterm2" ] arrayByAddingObjectsFromArray:dependencies];
    }
    NSString *contents = [NSString stringWithFormat:
                          @"# WARNING: This file was generated automatically by iTerm2. Do not edit it manually.\n"
                          @"[metadata]\n"
                          @"name=%@\n"
                          @"version=1.0\n"
                          @"\n"
                          @"[options]\n"
                          @"scripts=%@/%@.py\n"
                          @"install_requires=%@\n"
                          @"python_requires = =%@\n"
                          @"\n"
                          @"[iterm2]\n"
                          @"environment = >=%@\n",
                          name,
                          name,
                          name,
                          [[NSSet setWithArray:dependencies].allObjects componentsJoinedByString:@"; "],
                          sanitizedPythonVersion,
                          @(environmentVersion)];
    [contents writeToFile:file atomically:NO encoding:NSUTF8StringEncoding error:nil];
}

static int iTermSetupCfgParserIniHandler(void *user,
                                          const char *section,
                                          const char *name,
                                          const char *value) {
    iTermSetupCfgParser *self = (__bridge iTermSetupCfgParser *)user;
    [self handleSection:[NSString stringWithUTF8String:section]
                   name:[NSString stringWithUTF8String:name]
                  value:[NSString stringWithUTF8String:value]];
    return 1;
}

- (instancetype)initWithPath:(NSString *)path {
    self = [super init];
    if (self) {
        NSError *error = nil;
        _content = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:&error];
        if (!_content || error) {
            return nil;
        }
        _installRequiresParts = [NSMutableArray array];
        if (ini_parse_string(_content.UTF8String, iTermSetupCfgParserIniHandler, (__bridge void *)self)) {
            return nil;
        }
        [self computeDependencies:[_installRequiresParts componentsJoinedByString:@";"]];
        _installRequiresParts = nil;
    }
    return self;
}

- (void)handleSection:(NSString *)section
                 name:(NSString *)name
                value:(NSString *)value {
    if ([section isEqualToString:@"metadata"] && [name isEqualToString:@"name"]) {
        _name = [value copy];
        return;
    }
    if ([section isEqualToString:@"options"] && [name isEqualToString:@"python_requires"]) {
        [self computePythonVersion:value];
        return;
    }
    if ([section isEqualToString:@"options"] && [name isEqualToString:@"install_requires"]) {
        NSString *trimmed = [value stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
        if (trimmed.length > 0) {
            [_installRequiresParts addObject:trimmed];
        }
        return;
    }
    if ([section isEqualToString:@"iterm2"] && [name isEqualToString:@"environment"]) {
        [self computeMininumEnvironmentVersion:value];
        return;
    }
}

- (void)computePythonVersion:(NSString *)expression {
    if (![expression hasPrefix:@"="]) {
        return;
    }
    NSString *version = [expression substringFromIndex:1];
    NSArray<NSString *> *parts = [version componentsSeparatedByString:@"."];
    if ([parts anyWithBlock:^BOOL(NSString *anObject) {
        return ![anObject isNumeric];
    }]) {
        // Contains a non-numeric value
        return;
    }

    _pythonVersion = version;
}

// Sets one of _dependencies or _dependenciesError
- (void)computeDependencies:(NSString *)value {
    NSArray<NSString *> *names = [[[value componentsSeparatedByString:@";"] mapWithBlock:^id(NSString *anObject) {
        return [anObject stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    }] filteredArrayUsingBlock:^BOOL(NSString *value) {
        return value.length > 0;
    }];
    _dependencies = [[NSSet setWithArray:names] allObjects];
}

- (void)computeMininumEnvironmentVersion:(NSString *)value {
    if (![value hasPrefix:@">="]) {
        return;
    }
    _minimumEnvironmentVersion = [[value substringFromIndex:2] integerValue];
}

@end
