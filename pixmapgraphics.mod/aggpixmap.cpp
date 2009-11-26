// agg pixmap glue code

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "agg.h"

struct pixras{
	agg::rendering_buffer *rbuf;
	agg::renderer<agg::span_rgba32> *ren;
	agg::rasterizer ras;
	
	pixras(unsigned char *buf,int width,int height){
		rbuf=new agg::rendering_buffer(buf, width, height, width * 4);
		ren=new agg::renderer<agg::span_rgba32>(*rbuf);		
		ras.gamma(1.3);
		ras.filling_rule(agg::fill_even_odd);
	}	
};

extern "C"{
	int ras_open(unsigned char *rgba,int width,int height);
	void ras_close(int rashandle);
	void ras_clear(int rashandle);
	void ras_moveto(int rashandle,int x,int y);
	void ras_lineto(int rashandle,int x,int y);
	void ras_render(int rashandle,int r,int g,int b,int a);
};

int ras_open(unsigned char *rgba,int width,int height){
	return (int)new pixras(rgba,width,height);
}

void ras_close(int rashandle){
	delete (pixras*)rashandle;
}
void ras_clear(int rashandle){
	pixras *r=(pixras *)rashandle;
	r->ren->clear(agg::rgba8(255, 255, 255));
}
void ras_moveto(int rashandle,int x,int y){
	pixras *r=(pixras *)rashandle;
	r->ras.move_to(x,y);
}
void ras_lineto(int rashandle,int x,int y){
	pixras *r=(pixras *)rashandle;
	r->ras.line_to(x,y);
}
void ras_render(int rashandle,int r,int g,int b,int a){
	pixras *p=(pixras *)rashandle;
	p->ras.render(*p->ren,agg::rgba8(r,g,b));
}
