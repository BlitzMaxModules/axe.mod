//
//  WiiRemoteGlue.m
//

#import "WiiRemote.h"
#import "WiiRemoteDiscovery.h"

@class WiiListener;
@interface WiiListener:NSObject{
}
- (void) WiiRemoteDiscovered:(WiiRemote*)wiimote;
- (void) WiiRemoteDiscoveryError:(int)code;
@end
@implementation WiiListener
- (void) WiiRemoteDiscovered:(WiiRemote*)wiimote{
	printf("discovered wiimote!\n");fflush(stdout);
}
- (void) WiiRemoteDiscoveryError:(int)code{
	printf("discovered wiimote error=%d\n",code);fflush(stdout);
}
@end


WiiListener *listener;
WiiRemoteDiscovery *discovery;
WiiRemote *wiimote;

int OpenWiimotes(){
	
	listener=[WiiListener alloc];

	discovery=[WiiRemoteDiscovery discoveryWithDelegate:listener];
	
	[discovery start];
	
	return 25;
}

int PollWiimotes(){
	return 0;
}

int CloseWiimotes(){
//	[discovery end]
	return 0;
}

int WiimoteButton(int button_id){
	return 0;
}

float WiimoteAxis(int axiis_id){
	return 0.0;
}

void SetWiimoteLEDS(int wiimote_id,int bits){
}

void SetWiimoteRumble(wiimote_id,onoff){
}
