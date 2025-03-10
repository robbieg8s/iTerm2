//
//  iTermTextRendererTransientState.h
//  iTerm2SharedARC
//
//  Created by George Nachman on 12/22/17.
//

#import "iTermMetalCellRenderer.h"
#import "iTermMetalGlyphKey.h"
#import "iTermMetalRowData.h"
#import "iTermTextRendererCommon.h"

NS_ASSUME_NONNULL_BEGIN

extern const vector_float4 iTermIMEColor;

@class iTermCharacterBitmap;

NS_CLASS_AVAILABLE(10_11, NA)
@interface iTermTextRendererTransientState : iTermMetalCellRendererTransientState
@property (nonatomic, strong) NSMutableData *modelData;
@property (nonatomic, strong) id<MTLTexture> backgroundTexture;
@property (nonatomic) iTermMetalUnderlineDescriptor asciiUnderlineDescriptor;
@property (nonatomic) iTermMetalUnderlineDescriptor nonAsciiUnderlineDescriptor;
@property (nonatomic) iTermMetalUnderlineDescriptor strikethroughUnderlineDescriptor;
@property (nonatomic) CGFloat verticalOffset;

- (void)setGlyphKeysData:(iTermGlyphKeyData*)glyphKeysData
           glyphKeyCount:(NSUInteger)glyphKeyCount
                   count:(int)count
          attributesData:(iTermAttributesData *)attributesData
                     row:(int)row
  backgroundColorRLEData:(iTermData *)backgroundColorData  // array of iTermMetalBackgroundColorRLE background colors.
       markedRangeOnLine:(NSRange)markedRangeOnLine
                 context:(iTermMetalBufferPoolContext *)context
                creation:(NSDictionary<NSNumber *, iTermCharacterBitmap *> *(NS_NOESCAPE ^)(int x, BOOL *emoji))creation;
- (void)willDraw;
- (void)didComplete;
- (void)expireNonASCIIGlyphs;

@end

NS_ASSUME_NONNULL_END
