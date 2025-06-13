#include <tcl.h>
#include <AppKit/NSPasteboard.h>

static void ClearTransientClip(void *clientData) {
    (void) clientData;
    [[NSPasteboard generalPasteboard] clearContents];
}

@interface pasteboardOwner: NSObject <NSPasteboardTypeOwner>

@property(retain) NSString *clip;

@end

@implementation pasteboardOwner

- (void) pasteboard: (NSPasteboard *) sender
 provideDataForType: (NSString *) type
{
    [sender setString:self.clip forType:type];
    [self setClip: nil];
    Tcl_CreateTimerHandler(100, ClearTransientClip, NULL);
}

@end

static pasteboardOwner *owner = nil;

void initPasteboard() {
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    if (owner == nil) {
	owner = [[pasteboardOwner alloc] init];
	[owner retain];
	[pb declareTypes:[NSArray arrayWithObject:NSStringPboardType]
		   owner:nil];
	[pb setString:[[NSString alloc] initWithUTF8String:""]
	      forType:NSStringPboardType];
    }
}

void addTransientClip(const char *clip) {    
    NSPasteboard *pb = [NSPasteboard generalPasteboard];
    [owner setClip: [[NSString alloc] initWithUTF8String:clip]];
    [pb addTypes:[NSArray arrayWithObject:NSStringPboardType]
	   owner:owner];
}

/*
 * Local Variables:
 * mode: objc
 * c-basic-offset: 4
 * fill-column: 78
 * End:
 */
