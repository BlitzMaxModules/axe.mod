Strict

Import axe.PixmapGraphics

Global DisplayGraphics:TGraphics

Function testpixmapdrawing(pix:TPixmap)
	Local image2:TImage

	Local gfx:TGraphics
	
	gfx=PixmapGraphics(pix)	
	SetGraphics gfx

	image2=LoadImage("b-max.png")
	If image2=Null End
	
	SetClsColor 255,0,255
	Cls
	SetColor 0,255,255
	Plot 50,50
	Plot 100,100
	SetColor 0,25,25
	DrawRect 200,20,200,200
	SetColor 255,255,255
	DrawLine 0,0,100,100
	
	Local tri#[]=New Float[100]
	For Local i=0 Until 100
		tri[i]=Rnd(300)
	Next
	DrawPoly tri	
	
	DrawText "Hello World",100,10

	DrawImage image2,100,100
	
	DrawOval 20,20,300,100
	DrawOval 20,20,100,300
	
	Flip

	CloseGraphics gfx
	SetGraphics DisplayGraphics

End Function


Local pix:TPixmap
Local image:TImage


DisplayGraphics=Graphics(640,480)

pix=CreatePixmap(640,400,PF_RGBA8888)

testpixmapdrawing(pix)

image=LoadImage(pix)

DrawImage image,0,0

Flip

While True
	If WaitEvent()=EVENT_APPTERMINATE End		
	If KeyHit(KEY_ESCAPE) End
	
Wend



