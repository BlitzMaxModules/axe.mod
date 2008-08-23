//oggencoder.c

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <time.h>
#include <math.h>

#include <vorbis/vorbisenc.h>
//#include <vorbisenc.h>

#define READ 1024

typedef struct oggwriter oggwriter;

struct oggwriter{
	ogg_stream_state os; //take physical pages, weld into a logical stream of packets 
	ogg_page         og; //one Ogg bitstream page.  Vorbis packets are inside 
	ogg_packet       op; //one raw packet of data for decode 
	
	vorbis_info      vi; //struct that stores all the static vorbis bitstream settings 
	vorbis_comment   vc; //struct that stores all the user comments 
	
	vorbis_dsp_state vd; //central working state for the packet->PCM decoder 
	vorbis_block     vb; //local working space for packet->PCM decode 
};

int Encode_Ogg(void *stream,void(*writefunc)(void *bytes,int count,void *stream),int freq,int channels,float *samples,int length,float compression){

	oggwriter *ogg;	
	int eos;
	int result;

	ogg=(oggwriter*)malloc(sizeof(oggwriter));

	vorbis_info_init(&ogg->vi);
	
	result=vorbis_encode_init_vbr(&ogg->vi,channels,freq,compression);	

	if(result) return -1;	//error format not supported...

// add a comment 

	vorbis_comment_init(&ogg->vc);
	vorbis_comment_add_tag(&ogg->vc,"ENCODER","encoder_example.c");
	
// set up the analysis state and auxiliary encoding storage 

	vorbis_analysis_init(&ogg->vd,&ogg->vi);

	vorbis_block_init(&ogg->vd,&ogg->vb);
	
	srand(time(NULL));
	ogg_stream_init(&ogg->os,rand());
		
	ogg_packet header;
	ogg_packet header_comm;
	ogg_packet header_code;
	
	vorbis_analysis_headerout(&ogg->vd,&ogg->vc,&header,&header_comm,&header_code);
	ogg_stream_packetin(&ogg->os,&header); //automatically placed in its own page 
	ogg_stream_packetin(&ogg->os,&header_comm);
	ogg_stream_packetin(&ogg->os,&header_code);

//This ensures the actual audio data will start on a new page, as per spec
	while(1){
		result=ogg_stream_flush(&ogg->os,&ogg->og);
		if(result==0)break;
		writefunc(ogg->og.header,ogg->og.header_len,stream);
		writefunc(ogg->og.body,ogg->og.body_len,stream);
	}

	int i,c,n;
	eos=0;
	
	while(!eos){
		if (length>0){
			float **buffer=vorbis_analysis_buffer(&ogg->vd,READ);
			n=length;if (n>READ) n=READ;			
//uninterleave samples
			for (i=0;i<n;i++){
				for (c=0;c<channels;c++){
					buffer[c][i]=*samples++;
				}
			}	
			length=length-n;
//tell the library how much we actually submitted
			vorbis_analysis_wrote(&ogg->vd,i);
		}else{
			vorbis_analysis_wrote(&ogg->vd,0);
		}
		while(vorbis_analysis_blockout(&ogg->vd,&ogg->vb)==1){
//analysis, assume we want to use bitrate management
			vorbis_analysis(&ogg->vb,NULL);
			vorbis_bitrate_addblock(&ogg->vb);		
			while(vorbis_bitrate_flushpacket(&ogg->vd,&ogg->op)){
//weld the packet into the bitstream
				ogg_stream_packetin(&ogg->os,&ogg->op);
//write out pages (if any)
				while(!eos){
					result=ogg_stream_pageout(&ogg->os,&ogg->og);
					if(result==0)break;					
					writefunc(ogg->og.header,ogg->og.header_len,stream);
					writefunc(ogg->og.body,ogg->og.body_len,stream);
					if (ogg_page_eos(&ogg->og)) eos=1;
				}
			}
		}
	}		
//clean up and exit.  vorbis_info_clear() must be called last 
	ogg_stream_clear(&ogg->os);
	vorbis_block_clear(&ogg->vb);
	vorbis_dsp_clear(&ogg->vd);
	vorbis_comment_clear(&ogg->vc);
	vorbis_info_clear(&ogg->vi);
	
	free(ogg);

	return 0;	
}
