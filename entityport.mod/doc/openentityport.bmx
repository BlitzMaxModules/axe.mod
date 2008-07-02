' openentityport.bmx

' test correct function of the entityconsole daemon

Framework axe.entityport

Import brl.system

Strict

OpenEntityPort

Graphics3D(640,480,0,2)

Local light=CreateLight(1,0)

TurnEntity light,100,0,100,0

Local cube=CreateCube(0)

MoveEntity cube,0,0,10

RotateEntity cube,10,100,1,0

Local cam=CreateCamera(0)

While Not KeyHit(1)
	RenderWorld 1
	Flip 1
	TurnEntity cube,1,2,1,0
Wend

CloseEntityPort

End
