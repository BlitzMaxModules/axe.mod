// freeport.linux.c

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/ioctl.h>
//#include <linux/joystick.h>

int openserial(const char *deviceFilePath,int baudrate){
	int         fileDescriptor = -1;
	int         handshake;
	
	struct termios  options;
	
	fileDescriptor = open(deviceFilePath, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fileDescriptor == -1){
		printf("Error opening serial port %s - %s(%d).\n", deviceFilePath, strerror(errno), errno);
		return -1;
	}	
	return 0;
 }

/*
    // Note that open() follows POSIX semantics: multiple open() calls to 
    // the same file will succeed unless the TIOCEXCL ioctl is issued.
    // This will prevent additional opens except by root-owned processes.
    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
 
    if (ioctl(fileDescriptor, TIOCEXCL) == 1) {
        printf("Error setting TIOCEXCL on %s - %s(%d).\n", deviceFilePath, strerror(errno), errno);
        return -1;
    }
 
    // Now that the device is open, clear the O_NONBLOCK flag so 
    // subsequent I/O will block.
    // See fcntl(2) ("man 2 fcntl") for details.
 
    if (fcntl(fileDescriptor, F_SETFL, 0) == kMyErrReturn)
    {
        printf("Error clearing O_NONBLOCK %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
        goto error;
    }
 
    // Get the current options and save them so we can restore the 
    // default settings later.
    if (tcgetattr(fileDescriptor, &gOriginalTTYAttrs) == kMyErrReturn)
    {
        printf("Error getting tty attributes %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
        goto error;
    }
 
    // The serial port attributes such as timeouts and baud rate are set by 
    // modifying the termios structure and then calling tcsetattr to
    // cause the changes to take effect. Note that the
    // changes will not take effect without the tcsetattr() call.
    // See tcsetattr(4) ("man 4 tcsetattr") for details.
 
    options = gOriginalTTYAttrs;
 
    // Print the current input and output baud rates.
    // See tcsetattr(4) ("man 4 tcsetattr") for details.
 
    printf("Current input baud rate is %d\n", (int) cfgetispeed(&options));
    printf("Current output baud rate is %d\n", (int) cfgetospeed(&options));
 
    // Set raw input (non-canonical) mode, with reads blocking until either 
    // a single character has been received or a one second timeout expires.
    // See tcsetattr(4) ("man 4 tcsetattr") and termios(4) ("man 4 termios") 
    // for details.
 
    cfmakeraw(&options);
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 10;
 
    // The baud rate, word length, and handshake options can be set as follows:

    cfsetspeed(&options, B115200);
	options.c_cflag |= (CS8|CCTS_OFLOW|CRTS_IFLOW);

/* 
    cfsetspeed(&options, B19200);   // Set 19200 baud    
     options.c_cflag |= (CS7        |// Use 7 bit words
            PARENB     |        // Enable parity (even parity if PARODD 
                                // not also set)
            CCTS_OFLOW |        // CTS flow control of output
            CRTS_IFLOW);        // RTS flow control of input
 
   // Print the new input and output baud rates.
 
    printf("Input baud rate changed to %d\n", (int) cfgetispeed(&options));
    printf("Output baud rate changed to %d\n", (int) cfgetospeed(&options));
 
    // Cause the new options to take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &options) == kMyErrReturn)
    {
        printf("Error setting tty attributes %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
        goto error;
    }
 
    // To set the modem handshake lines, use the following ioctls.
    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
 
    if (ioctl(fileDescriptor, TIOCSDTR) == kMyErrReturn) 
    // Assert Data Terminal Ready (DTR)
    {
        printf("Error asserting DTR %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
    }
 
    if (ioctl(fileDescriptor, TIOCCDTR) == kMyErrReturn) 
    // Clear Data Terminal Ready (DTR)
    {
        printf("Error clearing DTR %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
    }
 
    handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
    // Set the modem lines depending on the bits set in handshake.
    if (ioctl(fileDescriptor, TIOCMSET, &handshake) == kMyErrReturn)
    {
        printf("Error setting handshake lines %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
    }
 
    // To read the state of the modem lines, use the following ioctl.
    // See tty(4) ("man 4 tty") and ioctl(2) ("man 2 ioctl") for details.
 
    if (ioctl(fileDescriptor, TIOCMGET, &handshake) == kMyErrReturn)
    // Store the state of the modem lines in handshake.
    {
        printf("Error getting handshake lines %s - %s(%d).\n",
            deviceFilePath, strerror(errno), errno);
    }
 
    printf("Handshake lines currently set to %d\n", handshake);    
 
    // Success:
    return fileDescriptor;
 
    // Failure:
error:
    if (fileDescriptor != kMyErrReturn)
    {
        close(fileDescriptor);
    }
 
    return -1;
}

void closeserial(int fileDescriptor){
    // Block until all written output has been sent from the device.
    // Note that this call is simply passed on to the serial device driver.
    // See tcsendbreak(3) ("man 3 tcsendbreak") for details.
    if (tcdrain(fileDescriptor) == kMyErrReturn)
    {
        printf("Error waiting for drain - %s(%d).\n",
            strerror(errno), errno);
    }
 
    // It is good practice to reset a serial port back to the state in
    // which you found it. This is why we saved the original termios struct
    // The constant TCSANOW (defined in termios.h) indicates that
    // the change should take effect immediately.
    if (tcsetattr(fileDescriptor, TCSANOW, &gOriginalTTYAttrs) == 
                    kMyErrReturn)
    {
        printf("Error resetting tty attributes - %s(%d).\n",
                    strerror(errno), errno);
    }
 
    close(fileDescriptor);
}

int copycftoascii(CFTypeRef cf,char *buffer,int len){
	if (cf){
		Boolean result=CFStringGetCString(cf,buffer,len,kCFStringEncodingASCII);
		if (result) return strlen(buffer);
	}
	return 0;
}

int enumports(char *buffer,int len)
{
	kern_return_t	kernResult;
	io_iterator_t	serialPortIterator;
	io_object_t	modemService;
	mach_port_t	masterPort;
	CFMutableDictionaryRef classesToMatch;
	Boolean		result;
	CFTypeRef   	cf,cf2;
	int 			count;
	
	*buffer='\0';
	count=0;	
	kernResult=IOMasterPort(MACH_PORT_NULL,&masterPort);
	if(KERN_SUCCESS!=kernResult) return 0;
	classesToMatch=IOServiceMatching(kIOSerialBSDServiceValue);// Serial devices are instances of class IOSerialBSDClient.
	if(classesToMatch==NULL){
		printf("IOServiceMatching returned a NULL dictionary.\n");fflush(stdout);
	}else {
		CFDictionarySetValue(classesToMatch,CFSTR(kIOSerialBSDTypeKey),CFSTR(kIOSerialBSDAllTypes));
	}	
	kernResult=IOServiceGetMatchingServices(masterPort,classesToMatch,&serialPortIterator);//matchingServices);
	if(kernResult!=KERN_SUCCESS) return 0;
//	printf("iterating....\n");fflush(stdout);
	while((modemService=IOIteratorNext(serialPortIterator))){
		cf=IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOCalloutDeviceKey),kCFAllocatorDefault,0);
		int n=copycftoascii(cf,buffer,len);
		CFRelease(cf);
		if (n){
			count++;
			buffer[n++]='-';
			buffer=buffer+n;
			len=len-n;
			cf=IORegistryEntryCreateCFProperty(modemService,CFSTR(kIOTTYBaseNameKey),kCFAllocatorDefault,0);
			n=copycftoascii(cf,buffer,len);
			CFRelease(cf);
			buffer[n++]='|';
			buffer=buffer+n;
			len=len-n;
		}
		(void)IOObjectRelease(modemService);
	}
	IOObjectRelease(serialPortIterator);    // Release the iterator.
	*buffer='\0';
	return count;
}

int ComOpen(portname$z,baud){
	return -1;
}




// #include "freeport.h"

struct linuxcontroller{
	pthread_t	thread;
	int		threadid;
	int		open,fd,fp;
	char		name[256];
	int		buttoncount,axiscount;
	int		button;
	float		axis[16];
};

typedef struct linuxjoy linuxjoy;
typedef struct js_event js_event;
typedef struct sched_param sched_param;

void *joythread(void *v)
{
	linuxjoy	*j;
	js_event 	js;
	int		n,b;
	
	int		policy;
	sched_param	sched;	
	
	pthread_getschedparam(pthread_self(),&policy,&sched);
	sched.sched_priority++;
	policy=SCHED_RR;
	pthread_setschedparam(pthread_self(),policy,&sched);
	
	j=(linuxjoy*)v;
	
	while (j->open)
	{
		b=sizeof(struct js_event)-j->fp;
		n=read(j->fd,&js,b);
		if (n<=0) break;
		if (n<b) {j->fp+=b;continue;}
		j->fp=0;
		switch (js.type & ~JS_EVENT_INIT)
		{
		case JS_EVENT_AXIS:
			n=js.number;
			if (n>=0 && n<16) j->axis[n]=js.value/32767.0;
			break;
		case JS_EVENT_BUTTON:
			n=1<<js.number;
			if (js.value) j->button|=n;else j->button&=~n;
			break;
		}
	}
	close(j->fd);
	j->fd=0;
	return 0;
}
						
linuxjoy *getjoy(int n)
{
	linuxjoy	*j;
	char		fname[16];
	int		fd;
	
	sprintf(fname,"/dev/js%d",n);
	fd=open(fname,O_RDONLY);
	if (fd==-1) return 0;
	j=(linuxjoy*)calloc(1,sizeof(struct linuxjoy));
	j->fd=fd;
	ioctl(fd,JSIOCGNAME(256),&j->name);
	ioctl(fd,JSIOCGAXES,&j->axiscount);
	ioctl(fd,JSIOCGBUTTONS,&j->buttoncount);
//	fcntl(fd,F_SETFL,O_NONBLOCK);	
	j->open=1;
	pthread_attr_t	attr;
	pthread_attr_init(&attr);
//		pthread_attr_setschedpolicy(&attr,SCHED_RR);
//		pthread_attr_getschedparam(&attr);
//		printf("mypid=%x\n",getpid());
//		pthread_attr_setschedparam(&attr,1);
	j->threadid=pthread_create(&j->thread,&attr,joythread,(void*)j);
		
	return j;
}

void updatejoy(linuxjoy *j,int *buttons,float *axis)
{
	*buttons=j->button;
	memcpy(axis,j->axis,16*4);
}

void freejoy(struct linuxjoy *j)
{
	int timeout=5;
	j->open=0;
	while (timeout-- && j->fd) sleep(1);
//	close(j->fd);
}

// standard freejoy interface

int		ljoyopen;
int		ljoycount;
linuxjoy	*ljoys[8];

int JoyCount()
{
	linuxjoy	*j;
	int		i,n;
	
	if (!ljoyopen)
	{
		n=0;
		for (i=0;i<8;i++)
		{
			j=getjoy(i);
			if (j) ljoys[n++]=j;
		}
		ljoycount=n;
		ljoyopen=1;
	}
	return ljoycount;
}

char *JoyCName(int port)
{
	if (port>=0 && port<ljoycount) return ljoys[port]->name;
	return 0;
}

int JoyButtonCaps(int port)
{
	if (port>=0 && port<ljoycount) return (1<<ljoys[port]->buttoncount)-1;
	return 0;
}

int JoyAxisCaps(int port)
{
	if (port>=0 && port<ljoycount) return (1<<ljoys[port]->axiscount)-1;
	return 0;
}

int ReadJoy(int port,int *buttons,float *axis)
{
	if (port<0 || port>=ljoycount) return 0;
	updatejoy (ljoys[port],buttons,axis);
	return 1;
}

void WriteJoy(int port,int channel,float value)
{
}
*/
