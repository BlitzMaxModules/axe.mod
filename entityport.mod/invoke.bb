Function Invoke(cmd,bank,textbank,pc)
	Select cmd
		Case 1 ;EntityClass
			entity=PeekInt(bank,pc+0)
			n=PokeString(bank,0,EntityClass$ ( entity ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 2 ;EntityName
			entity=PeekInt(bank,pc+0)
			n=PokeString(bank,0,EntityName$ ( entity ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 3 ;NameEntity
			entity=PeekInt(bank,pc+0)
			name$=PeekString(bank,pc+4)
			NameEntity entity,name$
			Return pc+8
		Case 4 ;FreeEntity
			entity=PeekInt(bank,pc+0)
			FreeEntity entity
			Return pc+4
		Case 5 ;ShowEntity
			entity=PeekInt(bank,pc+0)
			ShowEntity entity
			Return pc+4
		Case 6 ;HideEntity
			entity=PeekInt(bank,pc+0)
			HideEntity entity
			Return pc+4
		Case 7 ;EntityOrder
			entity=PeekInt(bank,pc+0)
			order=PeekInt(bank,pc+4)
			EntityOrder entity,order
			Return pc+8
		Case 8 ;EntityAutoFade
			entity=PeekInt(bank,pc+0)
			near#=PeekFloat(bank,pc+4)
			far#=PeekFloat(bank,pc+8)
			EntityAutoFade entity,near#,far#
			Return pc+12
		Case 9 ;EntityFX
			entity=PeekInt(bank,pc+0)
			fx=PeekInt(bank,pc+4)
			EntityFX entity,fx
			Return pc+8
		Case 10 ;EntityBlend
			entity=PeekInt(bank,pc+0)
			blend=PeekInt(bank,pc+4)
			EntityBlend entity,blend
			Return pc+8
		Case 11 ;EntityTexture
			entity=PeekInt(bank,pc+0)
			texture=PeekInt(bank,pc+4)
			frame=PeekInt(bank,pc+8)
			index=PeekInt(bank,pc+12)
			EntityTexture entity,texture,frame,index
			Return pc+16
		Case 12 ;EntityShininess
			entity=PeekInt(bank,pc+0)
			shininess#=PeekFloat(bank,pc+4)
			EntityShininess entity,shininess#
			Return pc+8
		Case 13 ;EntityAlpha
			entity=PeekInt(bank,pc+0)
			alpha#=PeekFloat(bank,pc+4)
			EntityAlpha entity,alpha#
			Return pc+8
		Case 14 ;EntityColor
			entity=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			EntityColor entity,red#,green#,blue#
			Return pc+16
		Case 15 ;PaintEntity
			entity=PeekInt(bank,pc+0)
			brush=PeekInt(bank,pc+4)
			PaintEntity entity,brush
			Return pc+8
		Case 16 ;FindChild
			entity=PeekInt(bank,pc+0)
			name$=PeekString(bank,pc+4)
			PokeInt bank,0,FindChild ( entity,name$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 17 ;GetChild
			entity=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeInt bank,0,GetChild ( entity,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 18 ;CountChildren
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,CountChildren ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 19 ;EntityParent
			entity=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			world=PeekInt(bank,pc+8)
			EntityParent entity,parent,world
			Return pc+12
		Case 20 ;Animating
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,Animating ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 21 ;AnimLength
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,AnimLength ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 22 ;AnimTime
			entity=PeekInt(bank,pc+0)
			PokeFloat bank,0,AnimTime# ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 23 ;AnimSeq
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,AnimSeq ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 24 ;ExtractAnimSeq
			entity=PeekInt(bank,pc+0)
			first__frame=PeekInt(bank,pc+4)
			last_frame=PeekInt(bank,pc+8)
			anim_seq=PeekInt(bank,pc+12)
			PokeInt bank,0,ExtractAnimSeq ( entity,first__frame,last_frame,anim_seq )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 25 ;AddAnimSeq
			entity=PeekInt(bank,pc+0)
			length=PeekInt(bank,pc+4)
			PokeInt bank,0,AddAnimSeq ( entity,length )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 26 ;SetAnimKey
			entity=PeekInt(bank,pc+0)
			frame=PeekInt(bank,pc+4)
			pos_key=PeekInt(bank,pc+8)
			rot_key=PeekInt(bank,pc+12)
			scale_key=PeekInt(bank,pc+16)
			SetAnimKey entity,frame,pos_key,rot_key,scale_key
			Return pc+20
		Case 27 ;Animate
			entity=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			speed#=PeekFloat(bank,pc+8)
			sequence=PeekInt(bank,pc+12)
			transition#=PeekFloat(bank,pc+16)
			Animate entity,mode,speed#,sequence,transition#
			Return pc+20
		Case 28 ;SetAnimTime
			entity=PeekInt(bank,pc+0)
			time#=PeekFloat(bank,pc+4)
			anim_seq=PeekInt(bank,pc+8)
			SetAnimTime entity,time#,anim_seq
			Return pc+12
		Case 29 ;AlignToVector
			entity=PeekInt(bank,pc+0)
			vector_x#=PeekFloat(bank,pc+4)
			vector_y#=PeekFloat(bank,pc+8)
			vector_z#=PeekFloat(bank,pc+12)
			axis=PeekInt(bank,pc+16)
			rate#=PeekFloat(bank,pc+20)
			AlignToVector entity,vector_x#,vector_y#,vector_z#,axis,rate#
			Return pc+24
		Case 30 ;PointEntity
			entity=PeekInt(bank,pc+0)
			target=PeekInt(bank,pc+4)
			roll#=PeekFloat(bank,pc+8)
			PointEntity entity,target,roll#
			Return pc+12
		Case 31 ;RotateEntity
			entity=PeekInt(bank,pc+0)
			pitch#=PeekFloat(bank,pc+4)
			yaw#=PeekFloat(bank,pc+8)
			roll#=PeekFloat(bank,pc+12)
			world=PeekInt(bank,pc+16)
			RotateEntity entity,pitch#,yaw#,roll#,world
			Return pc+20
		Case 32 ;ScaleEntity
			entity=PeekInt(bank,pc+0)
			x_scale#=PeekFloat(bank,pc+4)
			y_scale#=PeekFloat(bank,pc+8)
			z_scale#=PeekFloat(bank,pc+12)
			world=PeekInt(bank,pc+16)
			ScaleEntity entity,x_scale#,y_scale#,z_scale#,world
			Return pc+20
		Case 33 ;PositionEntity
			entity=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			world=PeekInt(bank,pc+16)
			PositionEntity entity,x#,y#,z#,world
			Return pc+20
		Case 34 ;TranslateEntity
			entity=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			world=PeekInt(bank,pc+16)
			TranslateEntity entity,x#,y#,z#,world
			Return pc+20
		Case 35 ;TurnEntity
			entity=PeekInt(bank,pc+0)
			pitch#=PeekFloat(bank,pc+4)
			yaw#=PeekFloat(bank,pc+8)
			roll#=PeekFloat(bank,pc+12)
			world=PeekInt(bank,pc+16)
			TurnEntity entity,pitch#,yaw#,roll#,world
			Return pc+20
		Case 36 ;MoveEntity
			entity=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			MoveEntity entity,x#,y#,z#
			Return pc+16
		Case 37 ;CollisionTriangle
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeInt bank,0,CollisionTriangle ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 38 ;CollisionSurface
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeInt bank,0,CollisionSurface ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 39 ;CollisionEntity
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeInt bank,0,CollisionEntity ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 40 ;CollisionTime
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionTime# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 41 ;CollisionNZ
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionNZ# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 42 ;CollisionNY
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionNY# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 43 ;CollisionNX
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionNX# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 44 ;CollisionZ
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionZ# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 45 ;CollisionY
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionY# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 46 ;CollisionX
			entity=PeekInt(bank,pc+0)
			collision_index=PeekInt(bank,pc+4)
			PokeFloat bank,0,CollisionX# ( entity,collision_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 47 ;CountCollisions
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,CountCollisions ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 48 ;EntityCollided
			entity=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			PokeInt bank,0,EntityCollided ( entity,mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 49 ;EntityDistance
			source_entity=PeekInt(bank,pc+0)
			destination_entity=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityDistance# ( source_entity,destination_entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 50 ;EntityBox
			entity=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			width#=PeekFloat(bank,pc+16)
			height#=PeekFloat(bank,pc+20)
			depth#=PeekFloat(bank,pc+24)
			EntityBox entity,x#,y#,z#,width#,height#,depth#
			Return pc+28
		Case 51 ;EntityRadius
			entity=PeekInt(bank,pc+0)
			x_radius#=PeekFloat(bank,pc+4)
			y_radius#=PeekFloat(bank,pc+8)
			EntityRadius entity,x_radius#,y_radius#
			Return pc+12
		Case 52 ;GetEntityType
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,GetEntityType ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 53 ;GetParent
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,GetParent ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 54 ;EntityPickMode
			entity=PeekInt(bank,pc+0)
			pick_geometry=PeekInt(bank,pc+4)
			obscurer=PeekInt(bank,pc+8)
			EntityPickMode entity,pick_geometry,obscurer
			Return pc+12
		Case 55 ;EntityType
			entity=PeekInt(bank,pc+0)
			collision_mode=PeekInt(bank,pc+4)
			recursive=PeekInt(bank,pc+8)
			EntityType entity,collision_mode,recursive
			Return pc+12
		Case 56 ;ResetEntity
			entity=PeekInt(bank,pc+0)
			ResetEntity entity
			Return pc+4
		Case 57 ;DeltaYaw
			src_entity=PeekInt(bank,pc+0)
			dest_entity=PeekInt(bank,pc+4)
			PokeFloat bank,0,DeltaYaw# ( src_entity,dest_entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 58 ;DeltaPitch
			src_entity=PeekInt(bank,pc+0)
			dest_entity=PeekInt(bank,pc+4)
			PokeFloat bank,0,DeltaPitch# ( src_entity,dest_entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 59 ;VectorPitch
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			PokeFloat bank,0,VectorPitch# ( x#,y#,z# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 60 ;VectorYaw
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			PokeFloat bank,0,VectorYaw# ( x#,y#,z# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 61 ;TFormedZ
			PokeFloat bank,0,TFormedZ# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 62 ;TFormedY
			PokeFloat bank,0,TFormedY# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 63 ;TFormedX
			PokeFloat bank,0,TFormedX# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 64 ;TFormNormal
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			source_entity=PeekInt(bank,pc+12)
			dest_entity=PeekInt(bank,pc+16)
			TFormNormal x#,y#,z#,source_entity,dest_entity
			Return pc+20
		Case 65 ;TFormVector
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			source_entity=PeekInt(bank,pc+12)
			dest_entity=PeekInt(bank,pc+16)
			TFormVector x#,y#,z#,source_entity,dest_entity
			Return pc+20
		Case 66 ;TFormPoint
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			source_entity=PeekInt(bank,pc+12)
			dest_entity=PeekInt(bank,pc+16)
			TFormPoint x#,y#,z#,source_entity,dest_entity
			Return pc+20
		Case 67 ;GetMatElement
			entity=PeekInt(bank,pc+0)
			row=PeekInt(bank,pc+4)
			column=PeekInt(bank,pc+8)
			PokeFloat bank,0,GetMatElement# ( entity,row,column )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 68 ;EntityRoll
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityRoll# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 69 ;EntityYaw
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityYaw# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 70 ;EntityPitch
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityPitch# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 71 ;EntityZ
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityZ# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 72 ;EntityY
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityY# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 73 ;EntityX
			entity=PeekInt(bank,pc+0)
			world=PeekInt(bank,pc+4)
			PokeFloat bank,0,EntityX# ( entity,world )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 74 ;CopyEntity
			entity=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CopyEntity ( entity,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 75 ;EmitSound
			sound=PeekInt(bank,pc+0)
			entity=PeekInt(bank,pc+4)
			PokeInt bank,0,EmitSound ( sound,entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 76 ;CreateListener
			parent=PeekInt(bank,pc+0)
			rolloff_factor#=PeekFloat(bank,pc+4)
			doppler_scale#=PeekFloat(bank,pc+8)
			distance_scale#=PeekFloat(bank,pc+12)
			PokeInt bank,0,CreateListener ( parent,rolloff_factor#,doppler_scale#,distance_scale# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 77 ;ModifyTerrain
			terrain=PeekInt(bank,pc+0)
			terrain_x=PeekInt(bank,pc+4)
			terrain_z=PeekInt(bank,pc+8)
			height#=PeekFloat(bank,pc+12)
			realtime=PeekInt(bank,pc+16)
			ModifyTerrain terrain,terrain_x,terrain_z,height#,realtime
			Return pc+20
		Case 78 ;TerrainHeight
			terrain=PeekInt(bank,pc+0)
			terrain_x=PeekInt(bank,pc+4)
			terrain_z=PeekInt(bank,pc+8)
			PokeFloat bank,0,TerrainHeight# ( terrain,terrain_x,terrain_z )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 79 ;TerrainSize
			terrain=PeekInt(bank,pc+0)
			PokeInt bank,0,TerrainSize ( terrain )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 80 ;TerrainZ
			terrain=PeekInt(bank,pc+0)
			world_x#=PeekFloat(bank,pc+4)
			world_y#=PeekFloat(bank,pc+8)
			world_z#=PeekFloat(bank,pc+12)
			PokeFloat bank,0,TerrainZ# ( terrain,world_x#,world_y#,world_z# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 81 ;TerrainY
			terrain=PeekInt(bank,pc+0)
			world_x#=PeekFloat(bank,pc+4)
			world_y#=PeekFloat(bank,pc+8)
			world_z#=PeekFloat(bank,pc+12)
			PokeFloat bank,0,TerrainY# ( terrain,world_x#,world_y#,world_z# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 82 ;TerrainX
			terrain=PeekInt(bank,pc+0)
			world_x#=PeekFloat(bank,pc+4)
			world_y#=PeekFloat(bank,pc+8)
			world_z#=PeekFloat(bank,pc+12)
			PokeFloat bank,0,TerrainX# ( terrain,world_x#,world_y#,world_z# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 83 ;TerrainShading
			terrain=PeekInt(bank,pc+0)
			enable=PeekInt(bank,pc+4)
			TerrainShading terrain,enable
			Return pc+8
		Case 84 ;TerrainDetail
			terrain=PeekInt(bank,pc+0)
			detail_level=PeekInt(bank,pc+4)
			morph=PeekInt(bank,pc+8)
			TerrainDetail terrain,detail_level,morph
			Return pc+12
		Case 85 ;LoadTerrain
			heightmap_file$=PeekString(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,LoadTerrain ( heightmap_file$,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 86 ;CreateTerrain
			grid_size=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CreateTerrain ( grid_size,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 87 ;CreatePlane
			segments=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CreatePlane ( segments,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 88 ;CreateMirror
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreateMirror ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 89 ;BSPAmbientLight
			bsp=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			BSPAmbientLight bsp,red#,green#,blue#
			Return pc+16
		Case 90 ;BSPLighting
			bsp=PeekInt(bank,pc+0)
			use_lightmaps=PeekInt(bank,pc+4)
			BSPLighting bsp,use_lightmaps
			Return pc+8
		Case 91 ;LoadBSP
			file$=PeekString(bank,pc+0)
			gamma_adj#=PeekFloat(bank,pc+4)
			parent=PeekInt(bank,pc+8)
			PokeInt bank,0,LoadBSP ( file$,gamma_adj#,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 92 ;MD2Animating
			md2=PeekInt(bank,pc+0)
			PokeInt bank,0,MD2Animating ( md2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 93 ;MD2AnimLength
			md2=PeekInt(bank,pc+0)
			PokeInt bank,0,MD2AnimLength ( md2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 94 ;MD2AnimTime
			md2=PeekInt(bank,pc+0)
			PokeFloat bank,0,MD2AnimTime# ( md2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 95 ;AnimateMD2
			md2=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			speed#=PeekFloat(bank,pc+8)
			first__frame=PeekInt(bank,pc+12)
			last_frame=PeekInt(bank,pc+16)
			transition#=PeekFloat(bank,pc+20)
			AnimateMD2 md2,mode,speed#,first__frame,last_frame,transition#
			Return pc+24
		Case 96 ;LoadMD2
			file$=PeekString(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,LoadMD2 ( file$,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 97 ;SpriteViewMode
			sprite=PeekInt(bank,pc+0)
			view_mode=PeekInt(bank,pc+4)
			SpriteViewMode sprite,view_mode
			Return pc+8
		Case 98 ;HandleSprite
			sprite=PeekInt(bank,pc+0)
			x_handle#=PeekFloat(bank,pc+4)
			y_handle#=PeekFloat(bank,pc+8)
			HandleSprite sprite,x_handle#,y_handle#
			Return pc+12
		Case 99 ;ScaleSprite
			sprite=PeekInt(bank,pc+0)
			x_scale#=PeekFloat(bank,pc+4)
			y_scale#=PeekFloat(bank,pc+8)
			ScaleSprite sprite,x_scale#,y_scale#
			Return pc+12
		Case 100 ;RotateSprite
			sprite=PeekInt(bank,pc+0)
			angle#=PeekFloat(bank,pc+4)
			RotateSprite sprite,angle#
			Return pc+8
		Case 101 ;LoadSprite
			file$=PeekString(bank,pc+0)
			texture_flags=PeekInt(bank,pc+4)
			parent=PeekInt(bank,pc+8)
			PokeInt bank,0,LoadSprite ( file$,texture_flags,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 102 ;CreateSprite
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreateSprite ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 103 ;CreatePivot
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreatePivot ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 104 ;LightConeAngles
			light=PeekInt(bank,pc+0)
			inner_angle#=PeekFloat(bank,pc+4)
			outer_angle#=PeekFloat(bank,pc+8)
			LightConeAngles light,inner_angle#,outer_angle#
			Return pc+12
		Case 105 ;LightRange
			light=PeekInt(bank,pc+0)
			range#=PeekFloat(bank,pc+4)
			LightRange light,range#
			Return pc+8
		Case 106 ;LightColor
			light=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			LightColor light,red#,green#,blue#
			Return pc+16
		Case 107 ;CreateLight
			mode=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CreateLight ( mode,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 108 ;PickedTriangle
			PokeInt bank,0,PickedTriangle ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 109 ;PickedSurface
			PokeInt bank,0,PickedSurface ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 110 ;PickedEntity
			PokeInt bank,0,PickedEntity ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 111 ;PickedTime
			PokeFloat bank,0,PickedTime# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 112 ;PickedNZ
			PokeFloat bank,0,PickedNZ# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 113 ;PickedNY
			PokeFloat bank,0,PickedNY# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 114 ;PickedNX
			PokeFloat bank,0,PickedNX# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 115 ;PickedZ
			PokeFloat bank,0,PickedZ# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 116 ;PickedY
			PokeFloat bank,0,PickedY# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 117 ;PickedX
			PokeFloat bank,0,PickedX# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 118 ;CameraPick
			camera=PeekInt(bank,pc+0)
			viewport_x#=PeekFloat(bank,pc+4)
			viewport_y#=PeekFloat(bank,pc+8)
			PokeInt bank,0,CameraPick ( camera,viewport_x#,viewport_y# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 119 ;LinePick
			x#=PeekFloat(bank,pc+0)
			y#=PeekFloat(bank,pc+4)
			z#=PeekFloat(bank,pc+8)
			dx#=PeekFloat(bank,pc+12)
			dy#=PeekFloat(bank,pc+16)
			dz#=PeekFloat(bank,pc+20)
			radius#=PeekFloat(bank,pc+24)
			PokeInt bank,0,LinePick ( x#,y#,z#,dx#,dy#,dz#,radius# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+28
		Case 120 ;EntityPick
			entity=PeekInt(bank,pc+0)
			range#=PeekFloat(bank,pc+4)
			PokeInt bank,0,EntityPick ( entity,range# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 121 ;EntityVisible
			src_entity=PeekInt(bank,pc+0)
			dest_entity=PeekInt(bank,pc+4)
			PokeInt bank,0,EntityVisible ( src_entity,dest_entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 122 ;EntityInView
			entity=PeekInt(bank,pc+0)
			camera=PeekInt(bank,pc+4)
			PokeInt bank,0,EntityInView ( entity,camera )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 123 ;ProjectedZ
			PokeFloat bank,0,ProjectedZ# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 124 ;ProjectedY
			PokeFloat bank,0,ProjectedY# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 125 ;ProjectedX
			PokeFloat bank,0,ProjectedX# ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 126 ;CameraProject
			camera=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			CameraProject camera,x#,y#,z#
			Return pc+16
		Case 127 ;CameraFogMode
			camera=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			CameraFogMode camera,mode
			Return pc+8
		Case 128 ;CameraFogRange
			camera=PeekInt(bank,pc+0)
			near#=PeekFloat(bank,pc+4)
			far#=PeekFloat(bank,pc+8)
			CameraFogRange camera,near#,far#
			Return pc+12
		Case 129 ;CameraFogColor
			camera=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			CameraFogColor camera,red#,green#,blue#
			Return pc+16
		Case 130 ;CameraViewport
			camera=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			width=PeekInt(bank,pc+12)
			height=PeekInt(bank,pc+16)
			CameraViewport camera,x,y,width,height
			Return pc+20
		Case 131 ;CameraProjMode
			camera=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			CameraProjMode camera,mode
			Return pc+8
		Case 132 ;CameraClsMode
			camera=PeekInt(bank,pc+0)
			cls_color=PeekInt(bank,pc+4)
			cls_zbuffer=PeekInt(bank,pc+8)
			CameraClsMode camera,cls_color,cls_zbuffer
			Return pc+12
		Case 133 ;CameraClsColor
			camera=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			CameraClsColor camera,red#,green#,blue#
			Return pc+16
		Case 134 ;CameraRange
			camera=PeekInt(bank,pc+0)
			near#=PeekFloat(bank,pc+4)
			far#=PeekFloat(bank,pc+8)
			CameraRange camera,near#,far#
			Return pc+12
		Case 135 ;CameraZoom
			camera=PeekInt(bank,pc+0)
			zoom#=PeekFloat(bank,pc+4)
			CameraZoom camera,zoom#
			Return pc+8
		Case 136 ;CreateCamera
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreateCamera ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 137 ;TriangleVertex
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			vertex=PeekInt(bank,pc+8)
			PokeInt bank,0,TriangleVertex ( surface,index,vertex )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 138 ;VertexW
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			coord_set=PeekInt(bank,pc+8)
			PokeFloat bank,0,VertexW# ( surface,index,coord_set )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 139 ;VertexV
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			coord_set=PeekInt(bank,pc+8)
			PokeFloat bank,0,VertexV# ( surface,index,coord_set )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 140 ;VertexU
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			coord_set=PeekInt(bank,pc+8)
			PokeFloat bank,0,VertexU# ( surface,index,coord_set )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 141 ;VertexAlpha
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexAlpha# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 142 ;VertexBlue
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexBlue# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 143 ;VertexGreen
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexGreen# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 144 ;VertexRed
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexRed# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 145 ;VertexNZ
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexNZ# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 146 ;VertexNY
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexNY# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 147 ;VertexNX
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexNX# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 148 ;VertexZ
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexZ# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 149 ;VertexY
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexY# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 150 ;VertexX
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeFloat bank,0,VertexX# ( surface,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 151 ;CountTriangles
			surface=PeekInt(bank,pc+0)
			PokeInt bank,0,CountTriangles ( surface )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 152 ;CountVertices
			surface=PeekInt(bank,pc+0)
			PokeInt bank,0,CountVertices ( surface )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 153 ;VertexTexCoords
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			u#=PeekFloat(bank,pc+8)
			v#=PeekFloat(bank,pc+12)
			w#=PeekFloat(bank,pc+16)
			coord_set=PeekInt(bank,pc+20)
			VertexTexCoords surface,index,u#,v#,w#,coord_set
			Return pc+24
		Case 154 ;VertexColor
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			red#=PeekFloat(bank,pc+8)
			green#=PeekFloat(bank,pc+12)
			blue#=PeekFloat(bank,pc+16)
			alpha#=PeekFloat(bank,pc+20)
			VertexColor surface,index,red#,green#,blue#,alpha#
			Return pc+24
		Case 155 ;VertexNormal
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			nx#=PeekFloat(bank,pc+8)
			ny#=PeekFloat(bank,pc+12)
			nz#=PeekFloat(bank,pc+16)
			VertexNormal surface,index,nx#,ny#,nz#
			Return pc+20
		Case 156 ;VertexCoords
			surface=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			x#=PeekFloat(bank,pc+8)
			y#=PeekFloat(bank,pc+12)
			z#=PeekFloat(bank,pc+16)
			VertexCoords surface,index,x#,y#,z#
			Return pc+20
		Case 157 ;AddTriangle
			surface=PeekInt(bank,pc+0)
			v0=PeekInt(bank,pc+4)
			v1=PeekInt(bank,pc+8)
			v2=PeekInt(bank,pc+12)
			PokeInt bank,0,AddTriangle ( surface,v0,v1,v2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 158 ;AddVertex
			surface=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			u#=PeekFloat(bank,pc+16)
			v#=PeekFloat(bank,pc+20)
			w#=PeekFloat(bank,pc+24)
			PokeInt bank,0,AddVertex ( surface,x#,y#,z#,u#,v#,w# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+28
		Case 159 ;PaintSurface
			surface=PeekInt(bank,pc+0)
			brush=PeekInt(bank,pc+4)
			PaintSurface surface,brush
			Return pc+8
		Case 160 ;ClearSurface
			surface=PeekInt(bank,pc+0)
			clear_vertices=PeekInt(bank,pc+4)
			clear_triangles=PeekInt(bank,pc+8)
			ClearSurface surface,clear_vertices,clear_triangles
			Return pc+12
		Case 161 ;FindSurface
			mesh=PeekInt(bank,pc+0)
			brush=PeekInt(bank,pc+4)
			PokeInt bank,0,FindSurface ( mesh,brush )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 162 ;GetEntityBrush
			entity=PeekInt(bank,pc+0)
			PokeInt bank,0,GetEntityBrush ( entity )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 163 ;GetSurfaceBrush
			surface=PeekInt(bank,pc+0)
			PokeInt bank,0,GetSurfaceBrush ( surface )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 164 ;CreateSurface
			mesh=PeekInt(bank,pc+0)
			brush=PeekInt(bank,pc+4)
			PokeInt bank,0,CreateSurface ( mesh,brush )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 165 ;MeshCullBox
			mesh=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			width#=PeekFloat(bank,pc+16)
			height#=PeekFloat(bank,pc+20)
			depth#=PeekFloat(bank,pc+24)
			MeshCullBox mesh,x#,y#,z#,width#,height#,depth#
			Return pc+28
		Case 166 ;GetSurface
			mesh=PeekInt(bank,pc+0)
			surface_index=PeekInt(bank,pc+4)
			PokeInt bank,0,GetSurface ( mesh,surface_index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 167 ;CountSurfaces
			mesh=PeekInt(bank,pc+0)
			PokeInt bank,0,CountSurfaces ( mesh )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 168 ;MeshesIntersect
			mesh_a=PeekInt(bank,pc+0)
			mesh_b=PeekInt(bank,pc+4)
			PokeInt bank,0,MeshesIntersect ( mesh_a,mesh_b )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 169 ;MeshDepth
			mesh=PeekInt(bank,pc+0)
			PokeFloat bank,0,MeshDepth# ( mesh )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 170 ;MeshHeight
			mesh=PeekInt(bank,pc+0)
			PokeFloat bank,0,MeshHeight# ( mesh )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 171 ;MeshWidth
			mesh=PeekInt(bank,pc+0)
			PokeFloat bank,0,MeshWidth# ( mesh )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 172 ;LightMesh
			mesh=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			range#=PeekFloat(bank,pc+16)
			x#=PeekFloat(bank,pc+20)
			y#=PeekFloat(bank,pc+24)
			z#=PeekFloat(bank,pc+28)
			LightMesh mesh,red#,green#,blue#,range#,x#,y#,z#
			Return pc+32
		Case 173 ;UpdateNormals
			mesh=PeekInt(bank,pc+0)
			UpdateNormals mesh
			Return pc+4
		Case 174 ;AddMesh
			source_mesh=PeekInt(bank,pc+0)
			dest_mesh=PeekInt(bank,pc+4)
			AddMesh source_mesh,dest_mesh
			Return pc+8
		Case 175 ;PaintMesh
			mesh=PeekInt(bank,pc+0)
			brush=PeekInt(bank,pc+4)
			PaintMesh mesh,brush
			Return pc+8
		Case 176 ;FlipMesh
			mesh=PeekInt(bank,pc+0)
			FlipMesh mesh
			Return pc+4
		Case 177 ;FitMesh
			mesh=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			width#=PeekFloat(bank,pc+16)
			height#=PeekFloat(bank,pc+20)
			depth#=PeekFloat(bank,pc+24)
			uniform=PeekInt(bank,pc+28)
			FitMesh mesh,x#,y#,z#,width#,height#,depth#,uniform
			Return pc+32
		Case 178 ;PositionMesh
			mesh=PeekInt(bank,pc+0)
			x#=PeekFloat(bank,pc+4)
			y#=PeekFloat(bank,pc+8)
			z#=PeekFloat(bank,pc+12)
			PositionMesh mesh,x#,y#,z#
			Return pc+16
		Case 179 ;RotateMesh
			mesh=PeekInt(bank,pc+0)
			pitch#=PeekFloat(bank,pc+4)
			yaw#=PeekFloat(bank,pc+8)
			roll#=PeekFloat(bank,pc+12)
			RotateMesh mesh,pitch#,yaw#,roll#
			Return pc+16
		Case 180 ;ScaleMesh
			mesh=PeekInt(bank,pc+0)
			x_scale#=PeekFloat(bank,pc+4)
			y_scale#=PeekFloat(bank,pc+8)
			z_scale#=PeekFloat(bank,pc+12)
			ScaleMesh mesh,x_scale#,y_scale#,z_scale#
			Return pc+16
		Case 181 ;CopyMesh
			mesh=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CopyMesh ( mesh,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 182 ;CreateCone
			segments=PeekInt(bank,pc+0)
			solid=PeekInt(bank,pc+4)
			parent=PeekInt(bank,pc+8)
			PokeInt bank,0,CreateCone ( segments,solid,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 183 ;CreateCylinder
			segments=PeekInt(bank,pc+0)
			solid=PeekInt(bank,pc+4)
			parent=PeekInt(bank,pc+8)
			PokeInt bank,0,CreateCylinder ( segments,solid,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 184 ;CreateSphere
			segments=PeekInt(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,CreateSphere ( segments,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 185 ;CreateCube
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreateCube ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 186 ;CreateMesh
			parent=PeekInt(bank,pc+0)
			PokeInt bank,0,CreateMesh ( parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 187 ;LoadAnimSeq
			entity=PeekInt(bank,pc+0)
			file$=PeekString(bank,pc+4)
			PokeInt bank,0,LoadAnimSeq ( entity,file$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 188 ;LoadAnimMesh
			file$=PeekString(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,LoadAnimMesh ( file$,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 189 ;LoadMesh
			file$=PeekString(bank,pc+0)
			parent=PeekInt(bank,pc+4)
			PokeInt bank,0,LoadMesh ( file$,parent )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 190 ;BrushFX
			brush=PeekInt(bank,pc+0)
			fx=PeekInt(bank,pc+4)
			BrushFX brush,fx
			Return pc+8
		Case 191 ;BrushBlend
			brush=PeekInt(bank,pc+0)
			blend=PeekInt(bank,pc+4)
			BrushBlend brush,blend
			Return pc+8
		Case 192 ;GetBrushTexture
			brush=PeekInt(bank,pc+0)
			index=PeekInt(bank,pc+4)
			PokeInt bank,0,GetBrushTexture ( brush,index )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 193 ;BrushTexture
			brush=PeekInt(bank,pc+0)
			texture=PeekInt(bank,pc+4)
			frame=PeekInt(bank,pc+8)
			index=PeekInt(bank,pc+12)
			BrushTexture brush,texture,frame,index
			Return pc+16
		Case 194 ;BrushShininess
			brush=PeekInt(bank,pc+0)
			shininess#=PeekFloat(bank,pc+4)
			BrushShininess brush,shininess#
			Return pc+8
		Case 195 ;BrushAlpha
			brush=PeekInt(bank,pc+0)
			alpha#=PeekFloat(bank,pc+4)
			BrushAlpha brush,alpha#
			Return pc+8
		Case 196 ;BrushColor
			brush=PeekInt(bank,pc+0)
			red#=PeekFloat(bank,pc+4)
			green#=PeekFloat(bank,pc+8)
			blue#=PeekFloat(bank,pc+12)
			BrushColor brush,red#,green#,blue#
			Return pc+16
		Case 197 ;FreeBrush
			brush=PeekInt(bank,pc+0)
			FreeBrush brush
			Return pc+4
		Case 198 ;LoadBrush
			file$=PeekString(bank,pc+0)
			texture_flags=PeekInt(bank,pc+4)
			u_scale#=PeekFloat(bank,pc+8)
			v_scale#=PeekFloat(bank,pc+12)
			PokeInt bank,0,LoadBrush ( file$,texture_flags,u_scale#,v_scale# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 199 ;CreateBrush
			red#=PeekFloat(bank,pc+0)
			green#=PeekFloat(bank,pc+4)
			blue#=PeekFloat(bank,pc+8)
			PokeInt bank,0,CreateBrush ( red#,green#,blue# )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 200 ;TextureFilter
			match_text$=PeekString(bank,pc+0)
			texture_flags=PeekInt(bank,pc+4)
			TextureFilter match_text$,texture_flags
			Return pc+8
		Case 201 ;ClearTextureFilters
			ClearTextureFilters 
			Return pc+0
		Case 202 ;TextureBuffer
			texture=PeekInt(bank,pc+0)
			frame=PeekInt(bank,pc+4)
			PokeInt bank,0,TextureBuffer ( texture,frame )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 203 ;SetCubeMode
			texture=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			SetCubeMode texture,mode
			Return pc+8
		Case 204 ;SetCubeFace
			texture=PeekInt(bank,pc+0)
			face=PeekInt(bank,pc+4)
			SetCubeFace texture,face
			Return pc+8
		Case 205 ;TextureName
			texture=PeekInt(bank,pc+0)
			n=PokeString(bank,0,TextureName$ ( texture ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 206 ;TextureHeight
			texture=PeekInt(bank,pc+0)
			PokeInt bank,0,TextureHeight ( texture )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 207 ;TextureWidth
			texture=PeekInt(bank,pc+0)
			PokeInt bank,0,TextureWidth ( texture )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 208 ;PositionTexture
			texture=PeekInt(bank,pc+0)
			u_offset#=PeekFloat(bank,pc+4)
			v_offset#=PeekFloat(bank,pc+8)
			PositionTexture texture,u_offset#,v_offset#
			Return pc+12
		Case 209 ;RotateTexture
			texture=PeekInt(bank,pc+0)
			angle#=PeekFloat(bank,pc+4)
			RotateTexture texture,angle#
			Return pc+8
		Case 210 ;ScaleTexture
			texture=PeekInt(bank,pc+0)
			u_scale#=PeekFloat(bank,pc+4)
			v_scale#=PeekFloat(bank,pc+8)
			ScaleTexture texture,u_scale#,v_scale#
			Return pc+12
		Case 211 ;TextureCoords
			texture=PeekInt(bank,pc+0)
			coords=PeekInt(bank,pc+4)
			TextureCoords texture,coords
			Return pc+8
		Case 212 ;TextureBlend
			texture=PeekInt(bank,pc+0)
			blend=PeekInt(bank,pc+4)
			TextureBlend texture,blend
			Return pc+8
		Case 213 ;FreeTexture
			texture=PeekInt(bank,pc+0)
			FreeTexture texture
			Return pc+4
		Case 214 ;LoadAnimTexture
			file$=PeekString(bank,pc+0)
			flags=PeekInt(bank,pc+4)
			width=PeekInt(bank,pc+8)
			height=PeekInt(bank,pc+12)
			first_=PeekInt(bank,pc+16)
			count=PeekInt(bank,pc+20)
			PokeInt bank,0,LoadAnimTexture ( file$,flags,width,height,first_,count )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+24
		Case 215 ;LoadTexture
			file$=PeekString(bank,pc+0)
			flags=PeekInt(bank,pc+4)
			PokeInt bank,0,LoadTexture ( file$,flags )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 216 ;CreateTexture
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			flags=PeekInt(bank,pc+8)
			frames=PeekInt(bank,pc+12)
			PokeInt bank,0,CreateTexture ( width,height,flags,frames )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
		Case 217 ;Stats3D
			mode=PeekInt(bank,pc+0)
			PokeFloat bank,0,Stats3D# ( mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 218 ;TrisRendered
			PokeInt bank,0,TrisRendered ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 219 ;ActiveTextures
			PokeInt bank,0,ActiveTextures ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 220 ;ClearWorld
			entities=PeekInt(bank,pc+0)
			brushes=PeekInt(bank,pc+4)
			textures=PeekInt(bank,pc+8)
			ClearWorld entities,brushes,textures
			Return pc+12
		Case 221 ;RenderWorld
			tween#=PeekFloat(bank,pc+0)
			RenderWorld tween#
			Return pc+4
		Case 222 ;CaptureWorld
			CaptureWorld 
			Return pc+0
		Case 223 ;UpdateWorld
			elapsed_time#=PeekFloat(bank,pc+0)
			UpdateWorld elapsed_time#
			Return pc+4
		Case 224 ;Collisions
			source_mode=PeekInt(bank,pc+0)
			destination_mode=PeekInt(bank,pc+4)
			method=PeekInt(bank,pc+8)
			response=PeekInt(bank,pc+12)
			Collisions source_mode,destination_mode,method,response
			Return pc+16
		Case 225 ;ClearCollisions
			ClearCollisions 
			Return pc+0
		Case 226 ;AmbientLight
			red#=PeekFloat(bank,pc+0)
			green#=PeekFloat(bank,pc+4)
			blue#=PeekFloat(bank,pc+8)
			AmbientLight red#,green#,blue#
			Return pc+12
		Case 227 ;WireFrame
			enable=PeekInt(bank,pc+0)
			WireFrame enable
			Return pc+4
		Case 228 ;AntiAlias
			enable=PeekInt(bank,pc+0)
			AntiAlias enable
			Return pc+4
		Case 229 ;Dither
			enable=PeekInt(bank,pc+0)
			Dither enable
			Return pc+4
		Case 230 ;WBuffer
			enable=PeekInt(bank,pc+0)
			WBuffer enable
			Return pc+4
		Case 231 ;GfxDriverCaps3D
			PokeInt bank,0,GfxDriverCaps3D ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 232 ;HWTexUnits
			PokeInt bank,0,HWTexUnits ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 233 ;HWMultiTex
			enable=PeekInt(bank,pc+0)
			HWMultiTex enable
			Return pc+4
		Case 234 ;LoaderMatrix
			file_ext$=PeekString(bank,pc+0)
			xx#=PeekFloat(bank,pc+4)
			xy#=PeekFloat(bank,pc+8)
			xz#=PeekFloat(bank,pc+12)
			yx#=PeekFloat(bank,pc+16)
			yy#=PeekFloat(bank,pc+20)
			yz#=PeekFloat(bank,pc+24)
			zx#=PeekFloat(bank,pc+28)
			zy#=PeekFloat(bank,pc+32)
			zz#=PeekFloat(bank,pc+36)
			LoaderMatrix file_ext$,xx#,xy#,xz#,yx#,yy#,yz#,zx#,zy#,zz#
			Return pc+40
		Case 235 ;NetMsgData
			n=PokeString(bank,0,NetMsgData$ ( ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+0
		Case 236 ;NetMsgTo
			PokeInt bank,0,NetMsgTo ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 237 ;NetMsgFrom
			PokeInt bank,0,NetMsgFrom ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 238 ;NetMsgType
			PokeInt bank,0,NetMsgType ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 239 ;RecvNetMsg
			PokeInt bank,0,RecvNetMsg ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 240 ;SendNetMsg
			mode=PeekInt(bank,pc+0)
			msg$=PeekString(bank,pc+4)
			from_player=PeekInt(bank,pc+8)
			to_player=PeekInt(bank,pc+12)
			reliable=PeekInt(bank,pc+16)
			PokeInt bank,0,SendNetMsg ( mode,msg$,from_player,to_player,reliable )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+20
		Case 241 ;NetPlayerLocal
			player=PeekInt(bank,pc+0)
			PokeInt bank,0,NetPlayerLocal ( player )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 242 ;NetPlayerName
			player=PeekInt(bank,pc+0)
			n=PokeString(bank,0,NetPlayerName$ ( player ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 243 ;DeleteNetPlayer
			player=PeekInt(bank,pc+0)
			DeleteNetPlayer player
			Return pc+4
		Case 244 ;CreateNetPlayer
			name$=PeekString(bank,pc+0)
			PokeInt bank,0,CreateNetPlayer ( name$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 245 ;StopNetGame
			StopNetGame 
			Return pc+0
		Case 246 ;JoinNetGame
			game_name$=PeekString(bank,pc+0)
			ip_address$=PeekString(bank,pc+4)
			PokeInt bank,0,JoinNetGame ( game_name$,ip_address$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 247 ;HostNetGame
			game_name$=PeekString(bank,pc+0)
			PokeInt bank,0,HostNetGame ( game_name$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 248 ;StartNetGame
			PokeInt bank,0,StartNetGame ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 249 ;Load3DSound
			filename$=PeekString(bank,pc+0)
			PokeInt bank,0,Load3DSound ( filename$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 250 ;ChannelPlaying
			channel=PeekInt(bank,pc+0)
			PokeInt bank,0,ChannelPlaying ( channel )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 251 ;ChannelPan
			channel=PeekInt(bank,pc+0)
			pan#=PeekFloat(bank,pc+4)
			ChannelPan channel,pan#
			Return pc+8
		Case 252 ;ChannelVolume
			channel=PeekInt(bank,pc+0)
			volume#=PeekFloat(bank,pc+4)
			ChannelVolume channel,volume#
			Return pc+8
		Case 253 ;ChannelPitch
			channel=PeekInt(bank,pc+0)
			pitch=PeekInt(bank,pc+4)
			ChannelPitch channel,pitch
			Return pc+8
		Case 254 ;ResumeChannel
			channel=PeekInt(bank,pc+0)
			ResumeChannel channel
			Return pc+4
		Case 255 ;PauseChannel
			channel=PeekInt(bank,pc+0)
			PauseChannel channel
			Return pc+4
		Case 256 ;StopChannel
			channel=PeekInt(bank,pc+0)
			StopChannel channel
			Return pc+4
		Case 257 ;PlayCDTrack
			track=PeekInt(bank,pc+0)
			mode=PeekInt(bank,pc+4)
			PokeInt bank,0,PlayCDTrack ( track,mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 258 ;PlayMusic
			midifile$=PeekString(bank,pc+0)
			PokeInt bank,0,PlayMusic ( midifile$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 259 ;PlaySound
			sound=PeekInt(bank,pc+0)
			PokeInt bank,0,PlaySound ( sound )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 260 ;SoundPan
			sound=PeekInt(bank,pc+0)
			pan#=PeekFloat(bank,pc+4)
			SoundPan sound,pan#
			Return pc+8
		Case 261 ;SoundVolume
			sound=PeekInt(bank,pc+0)
			volume#=PeekFloat(bank,pc+4)
			SoundVolume sound,volume#
			Return pc+8
		Case 262 ;SoundPitch
			sound=PeekInt(bank,pc+0)
			pitch=PeekInt(bank,pc+4)
			SoundPitch sound,pitch
			Return pc+8
		Case 263 ;LoopSound
			sound=PeekInt(bank,pc+0)
			LoopSound sound
			Return pc+4
		Case 264 ;FreeSound
			sound=PeekInt(bank,pc+0)
			FreeSound sound
			Return pc+4
		Case 265 ;LoadSound
			filename$=PeekString(bank,pc+0)
			PokeInt bank,0,LoadSound ( filename$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 266 ;DirectInputEnabled
			PokeInt bank,0,DirectInputEnabled ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 267 ;EnableDirectInput
			enable=PeekInt(bank,pc+0)
			EnableDirectInput enable
			Return pc+4
		Case 268 ;FlushJoy
			FlushJoy 
			Return pc+0
		Case 269 ;JoyVDir
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyVDir ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 270 ;JoyUDir
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyUDir ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 271 ;JoyZDir
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyZDir ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 272 ;JoyYDir
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyYDir ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 273 ;JoyXDir
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyXDir ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 274 ;JoyHat
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyHat ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 275 ;JoyRoll
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyRoll# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 276 ;JoyYaw
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyYaw# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 277 ;JoyPitch
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyPitch# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 278 ;JoyV
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyV# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 279 ;JoyU
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyU# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 280 ;JoyZ
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyZ# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 281 ;JoyY
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyY# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 282 ;JoyX
			port=PeekInt(bank,pc+0)
			PokeFloat bank,0,JoyX# ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 283 ;JoyWait
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyWait ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 284 ;WaitJoy
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,WaitJoy ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 285 ;GetJoy
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,GetJoy ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 286 ;JoyHit
			button=PeekInt(bank,pc+0)
			port=PeekInt(bank,pc+4)
			PokeInt bank,0,JoyHit ( button,port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 287 ;JoyDown
			button=PeekInt(bank,pc+0)
			port=PeekInt(bank,pc+4)
			PokeInt bank,0,JoyDown ( button,port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 288 ;JoyType
			port=PeekInt(bank,pc+0)
			PokeInt bank,0,JoyType ( port )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 289 ;MoveMouse
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			MoveMouse x,y
			Return pc+8
		Case 290 ;FlushMouse
			FlushMouse 
			Return pc+0
		Case 291 ;MouseZSpeed
			PokeInt bank,0,MouseZSpeed ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 292 ;MouseYSpeed
			PokeInt bank,0,MouseYSpeed ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 293 ;MouseXSpeed
			PokeInt bank,0,MouseXSpeed ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 294 ;MouseZ
			PokeInt bank,0,MouseZ ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 295 ;MouseY
			PokeInt bank,0,MouseY ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 296 ;MouseX
			PokeInt bank,0,MouseX ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 297 ;MouseWait
			PokeInt bank,0,MouseWait ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 298 ;WaitMouse
			PokeInt bank,0,WaitMouse ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 299 ;GetMouse
			PokeInt bank,0,GetMouse ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 300 ;MouseHit
			button=PeekInt(bank,pc+0)
			PokeInt bank,0,MouseHit ( button )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 301 ;MouseDown
			button=PeekInt(bank,pc+0)
			PokeInt bank,0,MouseDown ( button )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 302 ;FlushKeys
			FlushKeys 
			Return pc+0
		Case 303 ;WaitKey
			PokeInt bank,0,WaitKey ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 304 ;GetKey
			PokeInt bank,0,GetKey ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 305 ;KeyHit
			key=PeekInt(bank,pc+0)
			PokeInt bank,0,KeyHit ( key )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 306 ;KeyDown
			key=PeekInt(bank,pc+0)
			PokeInt bank,0,KeyDown ( key )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 307 ;HidePointer
			HidePointer 
			Return pc+0
		Case 308 ;ShowPointer
			ShowPointer 
			Return pc+0
		Case 309 ;Locate
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			Locate x,y
			Return pc+8
		Case 310 ;Input
			prompt$=PeekString(bank,pc+0)
			n=PokeString(bank,0,Input$ ( prompt$ ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 311 ;Print
			string$=PeekString(bank,pc+0)
			Print string$
			Return pc+4
		Case 312 ;Write
			string$=PeekString(bank,pc+0)
			Write string$
			Return pc+4
		Case 313 ;ImageRectCollide
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			rect_x=PeekInt(bank,pc+16)
			rect_y=PeekInt(bank,pc+20)
			rect_width=PeekInt(bank,pc+24)
			rect_height=PeekInt(bank,pc+28)
			PokeInt bank,0,ImageRectCollide ( image,x,y,frame,rect_x,rect_y,rect_width,rect_height )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+32
		Case 314 ;ImageRectOverlap
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			rect_x=PeekInt(bank,pc+12)
			rect_y=PeekInt(bank,pc+16)
			rect_width=PeekInt(bank,pc+20)
			rect_height=PeekInt(bank,pc+24)
			PokeInt bank,0,ImageRectOverlap ( image,x,y,rect_x,rect_y,rect_width,rect_height )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+28
		Case 315 ;RectsOverlap
			x1=PeekInt(bank,pc+0)
			y1=PeekInt(bank,pc+4)
			width1=PeekInt(bank,pc+8)
			height1=PeekInt(bank,pc+12)
			x2=PeekInt(bank,pc+16)
			y2=PeekInt(bank,pc+20)
			width2=PeekInt(bank,pc+24)
			height2=PeekInt(bank,pc+28)
			PokeInt bank,0,RectsOverlap ( x1,y1,width1,height1,x2,y2,width2,height2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+32
		Case 316 ;ImagesCollide
			image1=PeekInt(bank,pc+0)
			x1=PeekInt(bank,pc+4)
			y1=PeekInt(bank,pc+8)
			frame1=PeekInt(bank,pc+12)
			image2=PeekInt(bank,pc+16)
			x2=PeekInt(bank,pc+20)
			y2=PeekInt(bank,pc+24)
			frame2=PeekInt(bank,pc+28)
			PokeInt bank,0,ImagesCollide ( image1,x1,y1,frame1,image2,x2,y2,frame2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+32
		Case 317 ;ImagesOverlap
			image1=PeekInt(bank,pc+0)
			x1=PeekInt(bank,pc+4)
			y1=PeekInt(bank,pc+8)
			image2=PeekInt(bank,pc+12)
			x2=PeekInt(bank,pc+16)
			y2=PeekInt(bank,pc+20)
			PokeInt bank,0,ImagesOverlap ( image1,x1,y1,image2,x2,y2 )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+24
		Case 318 ;TFormFilter
			enable=PeekInt(bank,pc+0)
			TFormFilter enable
			Return pc+4
		Case 319 ;TFormImage
			image=PeekInt(bank,pc+0)
			a#=PeekFloat(bank,pc+4)
			b#=PeekFloat(bank,pc+8)
			c#=PeekFloat(bank,pc+12)
			d#=PeekFloat(bank,pc+16)
			TFormImage image,a#,b#,c#,d#
			Return pc+20
		Case 320 ;RotateImage
			image=PeekInt(bank,pc+0)
			angle#=PeekFloat(bank,pc+4)
			RotateImage image,angle#
			Return pc+8
		Case 321 ;ResizeImage
			image=PeekInt(bank,pc+0)
			width#=PeekFloat(bank,pc+4)
			height#=PeekFloat(bank,pc+8)
			ResizeImage image,width#,height#
			Return pc+12
		Case 322 ;ScaleImage
			image=PeekInt(bank,pc+0)
			xscale#=PeekFloat(bank,pc+4)
			yscale#=PeekFloat(bank,pc+8)
			ScaleImage image,xscale#,yscale#
			Return pc+12
		Case 323 ;ImageYHandle
			image=PeekInt(bank,pc+0)
			PokeInt bank,0,ImageYHandle ( image )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 324 ;ImageXHandle
			image=PeekInt(bank,pc+0)
			PokeInt bank,0,ImageXHandle ( image )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 325 ;ImageHeight
			image=PeekInt(bank,pc+0)
			PokeInt bank,0,ImageHeight ( image )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 326 ;ImageWidth
			image=PeekInt(bank,pc+0)
			PokeInt bank,0,ImageWidth ( image )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 327 ;AutoMidHandle
			enable=PeekInt(bank,pc+0)
			AutoMidHandle enable
			Return pc+4
		Case 328 ;MidHandle
			image=PeekInt(bank,pc+0)
			MidHandle image
			Return pc+4
		Case 329 ;HandleImage
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			HandleImage image,x,y
			Return pc+12
		Case 330 ;MaskImage
			image=PeekInt(bank,pc+0)
			red=PeekInt(bank,pc+4)
			green=PeekInt(bank,pc+8)
			blue=PeekInt(bank,pc+12)
			MaskImage image,red,green,blue
			Return pc+16
		Case 331 ;DrawBlockRect
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			rect_x=PeekInt(bank,pc+12)
			rect_y=PeekInt(bank,pc+16)
			rect_width=PeekInt(bank,pc+20)
			rect_height=PeekInt(bank,pc+24)
			frame=PeekInt(bank,pc+28)
			DrawBlockRect image,x,y,rect_x,rect_y,rect_width,rect_height,frame
			Return pc+32
		Case 332 ;DrawImageRect
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			rect_x=PeekInt(bank,pc+12)
			rect_y=PeekInt(bank,pc+16)
			rect_width=PeekInt(bank,pc+20)
			rect_height=PeekInt(bank,pc+24)
			frame=PeekInt(bank,pc+28)
			DrawImageRect image,x,y,rect_x,rect_y,rect_width,rect_height,frame
			Return pc+32
		Case 333 ;TileBlock
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			TileBlock image,x,y,frame
			Return pc+16
		Case 334 ;TileImage
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			TileImage image,x,y,frame
			Return pc+16
		Case 335 ;DrawBlock
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			DrawBlock image,x,y,frame
			Return pc+16
		Case 336 ;DrawImage
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			DrawImage image,x,y,frame
			Return pc+16
		Case 337 ;ImageBuffer
			image=PeekInt(bank,pc+0)
			frame=PeekInt(bank,pc+4)
			PokeInt bank,0,ImageBuffer ( image,frame )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 338 ;GrabImage
			image=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			frame=PeekInt(bank,pc+12)
			GrabImage image,x,y,frame
			Return pc+16
		Case 339 ;SaveImage
			image=PeekInt(bank,pc+0)
			bmpfile$=PeekString(bank,pc+4)
			frame=PeekInt(bank,pc+8)
			PokeInt bank,0,SaveImage ( image,bmpfile$,frame )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 340 ;FreeImage
			image=PeekInt(bank,pc+0)
			FreeImage image
			Return pc+4
		Case 341 ;LoadAnimImage
			bmpfile$=PeekString(bank,pc+0)
			cellwidth=PeekInt(bank,pc+4)
			cellheight=PeekInt(bank,pc+8)
			first_=PeekInt(bank,pc+12)
			count=PeekInt(bank,pc+16)
			PokeInt bank,0,LoadAnimImage ( bmpfile$,cellwidth,cellheight,first_,count )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+20
		Case 342 ;CreateImage
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			frames=PeekInt(bank,pc+8)
			PokeInt bank,0,CreateImage ( width,height,frames )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 343 ;CopyImage
			image=PeekInt(bank,pc+0)
			PokeInt bank,0,CopyImage ( image )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 344 ;LoadImage
			bmpfile$=PeekString(bank,pc+0)
			PokeInt bank,0,LoadImage ( bmpfile$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 345 ;CloseMovie
			movie=PeekInt(bank,pc+0)
			CloseMovie movie
			Return pc+4
		Case 346 ;MoviePlaying
			movie=PeekInt(bank,pc+0)
			PokeInt bank,0,MoviePlaying ( movie )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 347 ;MovieHeight
			movie=PeekInt(bank,pc+0)
			PokeInt bank,0,MovieHeight ( movie )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 348 ;MovieWidth
			movie=PeekInt(bank,pc+0)
			PokeInt bank,0,MovieWidth ( movie )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 349 ;DrawMovie
			movie=PeekInt(bank,pc+0)
			x=PeekInt(bank,pc+4)
			y=PeekInt(bank,pc+8)
			w=PeekInt(bank,pc+12)
			h=PeekInt(bank,pc+16)
			PokeInt bank,0,DrawMovie ( movie,x,y,w,h )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+20
		Case 350 ;OpenMovie
			file$=PeekString(bank,pc+0)
			PokeInt bank,0,OpenMovie ( file$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 351 ;StringHeight
			string$=PeekString(bank,pc+0)
			PokeInt bank,0,StringHeight ( string$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 352 ;StringWidth
			string$=PeekString(bank,pc+0)
			PokeInt bank,0,StringWidth ( string$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 353 ;FontHeight
			PokeInt bank,0,FontHeight ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 354 ;FontWidth
			PokeInt bank,0,FontWidth ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 355 ;FreeFont
			font=PeekInt(bank,pc+0)
			FreeFont font
			Return pc+4
		Case 356 ;LoadFont
			fontname$=PeekString(bank,pc+0)
			height=PeekInt(bank,pc+4)
			bold=PeekInt(bank,pc+8)
			italic=PeekInt(bank,pc+12)
			underline=PeekInt(bank,pc+16)
			PokeInt bank,0,LoadFont ( fontname$,height,bold,italic,underline )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+20
		Case 357 ;CopyRect
			source_x=PeekInt(bank,pc+0)
			source_y=PeekInt(bank,pc+4)
			width=PeekInt(bank,pc+8)
			height=PeekInt(bank,pc+12)
			dest_x=PeekInt(bank,pc+16)
			dest_y=PeekInt(bank,pc+20)
			src_buffer=PeekInt(bank,pc+24)
			dest_buffer=PeekInt(bank,pc+28)
			CopyRect source_x,source_y,width,height,dest_x,dest_y,src_buffer,dest_buffer
			Return pc+32
		Case 358 ;Text
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			text$=PeekString(bank,pc+8)
			centre_x=PeekInt(bank,pc+12)
			centre_y=PeekInt(bank,pc+16)
			Text x,y,text$,centre_x,centre_y
			Return pc+20
		Case 359 ;Line
			x1=PeekInt(bank,pc+0)
			y1=PeekInt(bank,pc+4)
			x2=PeekInt(bank,pc+8)
			y2=PeekInt(bank,pc+12)
			Line x1,y1,x2,y2
			Return pc+16
		Case 360 ;Oval
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			width=PeekInt(bank,pc+8)
			height=PeekInt(bank,pc+12)
			solid=PeekInt(bank,pc+16)
			Oval x,y,width,height,solid
			Return pc+20
		Case 361 ;Rect
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			width=PeekInt(bank,pc+8)
			height=PeekInt(bank,pc+12)
			solid=PeekInt(bank,pc+16)
			Rect x,y,width,height,solid
			Return pc+20
		Case 362 ;Plot
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			Plot x,y
			Return pc+8
		Case 363 ;Cls
			Cls 
			Return pc+0
		Case 364 ;SetFont
			font=PeekInt(bank,pc+0)
			SetFont font
			Return pc+4
		Case 365 ;ClsColor
			red=PeekInt(bank,pc+0)
			green=PeekInt(bank,pc+4)
			blue=PeekInt(bank,pc+8)
			ClsColor red,green,blue
			Return pc+12
		Case 366 ;ColorBlue
			PokeInt bank,0,ColorBlue ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 367 ;ColorGreen
			PokeInt bank,0,ColorGreen ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 368 ;ColorRed
			PokeInt bank,0,ColorRed ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 369 ;GetColor
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			GetColor x,y
			Return pc+8
		Case 370 ;Color
			red=PeekInt(bank,pc+0)
			green=PeekInt(bank,pc+4)
			blue=PeekInt(bank,pc+8)
			Color red,green,blue
			Return pc+12
		Case 371 ;Viewport
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			width=PeekInt(bank,pc+8)
			height=PeekInt(bank,pc+12)
			Viewport x,y,width,height
			Return pc+16
		Case 372 ;Origin
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			Origin x,y
			Return pc+8
		Case 373 ;CopyPixelFast
			src_x=PeekInt(bank,pc+0)
			src_y=PeekInt(bank,pc+4)
			src_buffer=PeekInt(bank,pc+8)
			dest_x=PeekInt(bank,pc+12)
			dest_y=PeekInt(bank,pc+16)
			dest_buffer=PeekInt(bank,pc+20)
			CopyPixelFast src_x,src_y,src_buffer,dest_x,dest_y,dest_buffer
			Return pc+24
		Case 374 ;CopyPixel
			src_x=PeekInt(bank,pc+0)
			src_y=PeekInt(bank,pc+4)
			src_buffer=PeekInt(bank,pc+8)
			dest_x=PeekInt(bank,pc+12)
			dest_y=PeekInt(bank,pc+16)
			dest_buffer=PeekInt(bank,pc+20)
			CopyPixel src_x,src_y,src_buffer,dest_x,dest_y,dest_buffer
			Return pc+24
		Case 375 ;WritePixelFast
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			argb=PeekInt(bank,pc+8)
			buffer=PeekInt(bank,pc+12)
			WritePixelFast x,y,argb,buffer
			Return pc+16
		Case 376 ;ReadPixelFast
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			buffer=PeekInt(bank,pc+8)
			PokeInt bank,0,ReadPixelFast ( x,y,buffer )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 377 ;WritePixel
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			argb=PeekInt(bank,pc+8)
			buffer=PeekInt(bank,pc+12)
			WritePixel x,y,argb,buffer
			Return pc+16
		Case 378 ;ReadPixel
			x=PeekInt(bank,pc+0)
			y=PeekInt(bank,pc+4)
			buffer=PeekInt(bank,pc+8)
			PokeInt bank,0,ReadPixel ( x,y,buffer )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 379 ;UnlockBuffer
			buffer=PeekInt(bank,pc+0)
			UnlockBuffer buffer
			Return pc+4
		Case 380 ;LockBuffer
			buffer=PeekInt(bank,pc+0)
			LockBuffer buffer
			Return pc+4
		Case 381 ;SaveBuffer
			buffer=PeekInt(bank,pc+0)
			bmpfile$=PeekString(bank,pc+4)
			PokeInt bank,0,SaveBuffer ( buffer,bmpfile$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 382 ;LoadBuffer
			buffer=PeekInt(bank,pc+0)
			bmpfile$=PeekString(bank,pc+4)
			PokeInt bank,0,LoadBuffer ( buffer,bmpfile$ )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+8
		Case 383 ;GraphicsBuffer
			PokeInt bank,0,GraphicsBuffer ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 384 ;SetBuffer
			buffer=PeekInt(bank,pc+0)
			SetBuffer buffer
			Return pc+4
		Case 385 ;GraphicsDepth
			PokeInt bank,0,GraphicsDepth ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 386 ;GraphicsHeight
			PokeInt bank,0,GraphicsHeight ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 387 ;GraphicsWidth
			PokeInt bank,0,GraphicsWidth ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 388 ;Flip
			vwait=PeekInt(bank,pc+0)
			Flip vwait
			Return pc+4
		Case 389 ;VWait
			frames=PeekInt(bank,pc+0)
			VWait frames
			Return pc+4
		Case 390 ;ScanLine
			PokeInt bank,0,ScanLine ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 391 ;BackBuffer
			PokeInt bank,0,BackBuffer ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 392 ;FrontBuffer
			PokeInt bank,0,FrontBuffer ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 393 ;GammaBlue
			blue=PeekInt(bank,pc+0)
			PokeFloat bank,0,GammaBlue# ( blue )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 394 ;GammaGreen
			green=PeekInt(bank,pc+0)
			PokeFloat bank,0,GammaGreen# ( green )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 395 ;GammaRed
			red=PeekInt(bank,pc+0)
			PokeFloat bank,0,GammaRed# ( red )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 396 ;UpdateGamma
			calibrate=PeekInt(bank,pc+0)
			UpdateGamma calibrate
			Return pc+4
		Case 397 ;SetGamma
			src_red=PeekInt(bank,pc+0)
			src_green=PeekInt(bank,pc+4)
			src_blue=PeekInt(bank,pc+8)
			dest_red#=PeekFloat(bank,pc+12)
			dest_green#=PeekFloat(bank,pc+16)
			dest_blue#=PeekFloat(bank,pc+20)
			SetGamma src_red,src_green,src_blue,dest_red#,dest_green#,dest_blue#
			Return pc+24
		Case 398 ;EndGraphics
			EndGraphics 
			Return pc+0
		Case 399 ;Graphics3D
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			depth=PeekInt(bank,pc+8)
			mode=PeekInt(bank,pc+12)
			Graphics3D width,height,depth,mode
			Return pc+16
		Case 400 ;Graphics
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			depth=PeekInt(bank,pc+8)
			mode=PeekInt(bank,pc+12)
			Graphics width,height,depth,mode
			Return pc+16
		Case 401 ;Windowed3D
			PokeInt bank,0,Windowed3D ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 402 ;GfxMode3D
			mode=PeekInt(bank,pc+0)
			PokeInt bank,0,GfxMode3D ( mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 403 ;GfxMode3DExists
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			depth=PeekInt(bank,pc+8)
			PokeInt bank,0,GfxMode3DExists ( width,height,depth )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 404 ;CountGfxModes3D
			PokeInt bank,0,CountGfxModes3D ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 405 ;GfxDriver3D
			driver=PeekInt(bank,pc+0)
			PokeInt bank,0,GfxDriver3D ( driver )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 406 ;TotalVidMem
			PokeInt bank,0,TotalVidMem ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 407 ;AvailVidMem
			PokeInt bank,0,AvailVidMem ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 408 ;GfxModeDepth
			mode=PeekInt(bank,pc+0)
			PokeInt bank,0,GfxModeDepth ( mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 409 ;GfxModeHeight
			mode=PeekInt(bank,pc+0)
			PokeInt bank,0,GfxModeHeight ( mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 410 ;GfxModeWidth
			mode=PeekInt(bank,pc+0)
			PokeInt bank,0,GfxModeWidth ( mode )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+4
		Case 411 ;GfxModeExists
			width=PeekInt(bank,pc+0)
			height=PeekInt(bank,pc+4)
			depth=PeekInt(bank,pc+8)
			PokeInt bank,0,GfxModeExists ( width,height,depth )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+12
		Case 412 ;CountGfxModes
			PokeInt bank,0,CountGfxModes ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 413 ;SetGfxDriver
			driver=PeekInt(bank,pc+0)
			SetGfxDriver driver
			Return pc+4
		Case 414 ;GfxDriverName
			driver=PeekInt(bank,pc+0)
			n=PokeString(bank,0,GfxDriverName$ ( driver ))
			win32WriteFile stdout,bank,n,resultbuffer,0
			Return pc+4
		Case 415 ;CountGfxDrivers
			PokeInt bank,0,CountGfxDrivers ( )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+0
		Case 416 ;CallDLL
			dll_name$=PeekString(bank,pc+0)
			func_name$=PeekString(bank,pc+4)
			in_bank=PeekInt(bank,pc+8)
			out_bank=PeekInt(bank,pc+12)
			PokeInt bank,0,CallDLL ( dll_name$,func_name$,in_bank,out_bank )
			win32WriteFile stdout,bank,4,resultbuffer,0
			Return pc+16
	End Select
End Function


