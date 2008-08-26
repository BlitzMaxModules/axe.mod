#define _ASSERT

#include "wiimote.h"

extern "C"{
	int OpenWiimotes();
	int PollWiimotes();
	int CloseWiimotes();
	int WiimoteButton(int id);
	float WiimoteAxis(int id);
	int SetWiimoteLEDS(int wiimote_id,int bits);
	int SetWiimoteRumble(int wiimote_id,int onoff);
};

#define MAX_PLAYERS 8
#define MAX_BUTTONS 8
#define MAX_AXIIS 32

wiimote *wii_motes[MAX_PLAYERS];
int wii_states[MAX_PLAYERS];	//0=unconnected 1=wiimote 2=addon1 etc.
int wii_buttons[MAX_PLAYERS*MAX_BUTTONS];
float wii_axiis[MAX_PLAYERS*MAX_AXIIS];
int wii_count=0;
int wii_connected=0;

int OpenWiimotes(){
	return 0;
}

int PollWiimotes(){
	int i,j,b;
	wiimote *w;
	float *axis;
// allow 1 player to join per update until fill
	if (wii_count==wii_connected&&wii_count<MAX_PLAYERS){
		wii_states[wii_count]=0;
		wii_motes[wii_count]=new wiimote();
		wii_count++;
	}
// poll all active
	for(i=0;i<wii_count;i++){
		w=wii_motes[i];
		if(wii_states[i]==0){//disconnected
			if (w->Connect(wiimote::FIRST_AVAILABLE)) {
				wii_states[i]=1;
				wii_connected++;
				w->SetReportType(wiimote::IN_BUTTONS_ACCEL_IR_EXT);
			}
		}
		if(wii_states[i]){
			if (w->RefreshState() == NO_CHANGE) continue;
						
			wii_buttons[i*MAX_BUTTONS+0]=w->Button.Bits&0x9f1f;

			axis=wii_axiis+i*MAX_AXIIS;
			axis[0]=w->Acceleration.X;
			axis[1]=w->Acceleration.Y;
			axis[2]=w->Acceleration.Z;
			axis[3]=w->Acceleration.Orientation.X;
			axis[4]=w->Acceleration.Orientation.Y;
			axis[5]=w->Acceleration.Orientation.Z;
			axis[6]=w->Acceleration.Orientation.Pitch;
			axis[7]=w->Acceleration.Orientation.Roll;
			
			for (j=0;j<4;j++){
				axis[8+j*2]=w->IR.Dot[j].X;
				axis[9+j*2]=w->IR.Dot[j].Y;
			}
						
			switch(w->ExtensionType){
				case 0:
					break;
				case 0xfefe://NUNCHUK
					b=0;
					if (w->Nunchuk.C) b=1;
					if (w->Nunchuk.Z) b|=2;
					wii_buttons[i*MAX_BUTTONS+1]=b;
					axis[16]=w->Nunchuk.Joystick.X;
					axis[17]=w->Nunchuk.Joystick.Y;
					axis[18]=w->Nunchuk.Acceleration.X;
					axis[19]=w->Nunchuk.Acceleration.Y;
					axis[20]=w->Nunchuk.Acceleration.Z;
					axis[21]=w->Nunchuk.Acceleration.Orientation.X;
					axis[22]=w->Nunchuk.Acceleration.Orientation.Y;
					axis[23]=w->Nunchuk.Acceleration.Orientation.Z;
					axis[24]=w->Nunchuk.Acceleration.Orientation.Pitch;
					axis[25]=w->Nunchuk.Acceleration.Orientation.Roll;
					break;
				case 0xfdfd://CLASSIC:
				case 0xfbfd://CLASSIC_GUITAR
					wii_buttons[i*MAX_BUTTONS+1]=w->ClassicController.Button.Bits;
					axis[16]=w->ClassicController.JoystickL.X;
					axis[17]=w->ClassicController.JoystickL.Y;
					axis[18]=w->ClassicController.JoystickR.X;
					axis[19]=w->ClassicController.JoystickR.Y;
					axis[20]=w->ClassicController.TriggerL;
					axis[21]=w->ClassicController.TriggerR;
					break;
			}
		}
	}
	return wii_connected;
}

int WiimoteButton(int index){
	return wii_buttons[index];
}

float WiimoteAxis(int index){
	return wii_axiis[index];
}

int SetWiimoteLEDS(int wiimote_id,int bits){
	wiimote *w;
	if (wiimote_id<0||wiimote_id>=MAX_PLAYERS) return -1; //illegal id
	if (wii_states[wiimote_id]==0) return -1; //not connected
	w=wii_motes[wiimote_id];
	if (!w) return -1;	//should never happen
	w->SetLEDs(bits);
	return 0;
}

int SetWiimoteRumble(int wiimote_id,int onoff){
	wiimote *w;
	if (wiimote_id<0||wiimote_id>=MAX_PLAYERS) return -1; //illegal id
	if (wii_states[wiimote_id]==0) return -1; //not connected
	w=wii_motes[wiimote_id];
	if (!w) return -1;	//should never happen
	w->SetRumble(onoff);
	return 0;
}

int CloseWiimotes(){
	int i;
	for(i=0;i<wii_count;i++){
		delete wii_motes[i];
		wii_motes[i]=0;
	}
	wii_count=0;
}
