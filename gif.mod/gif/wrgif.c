// writegif2 added by simon armstrong
// adds palette and graphic control block parameters to writegif
// now outputs gif89a header

/*  
**************************************************************************
*
*		-----------------	
*		|   WriteGIF	|
*		-----------------		
*
*_TITLE WriteGIF writes a CompuServe GIF image when passed image data and size.
*
*_DESC  WriteGIF writes a CompuServe GIF image when a GIF file pointer, 
*		 an image buffer containing raw image data, number of lines,
*		 and sample per line is passed to the function writegif.
*		 The image buffer is assumed to contain a 256 gray scaled
*		 image with one byte of data representing one sample.
*
*		 The GIF file written uses 87A GIF specs set up by
*		 CompuServe.  Under those specs its writes a GIF header,
*		 a logical screen descriptor, a global color table, an
*		 image descriptor, and the compressed image data.  It
*		 is highly recommended that anyone attempting to decipher
*		 this code should read the CompuServe specs on GIF files
*		 since many of the terms and names are taken from those
*		 specs (a familiarity with LZW compression might also be
*		 useful).
*
*		 This piece of software was origionally developed on a Sun
*		 Sparc machine.  This version was made in order to deal
*		 with PC memory limitations.  This is a MS-DOS only version.
*
*		 "The Graphics Interchange Format(c) is the Copyright property
*		 of CompuServe Incorporated.  GIF(sm) is a Service Mark
*		 property of CompuServe Incorporated."
*	
*_HIST  MAY 31 1994  David Larsen, USGS, Flagstaff Original Version
*
***************************************************************************
*/

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

typedef unsigned char CHARH;

//#include "pds.h"

/*
******************************************************************************
**
**	Defined Macros
**
******************************************************************************
*/


/*misc macros*/
#define LO_BYTE_16BIT  				(GIFdata & 255)
#define HI_BYTE_16BIT  				((GIFdata & 65280)/256)
//#define HEADER					"GIF87a"
#define HEADER					"GIF89a"


/*Error Codes*/
#define ERROR					-1	
#define NO_ERROR			 	0	

/*GIF image macros*/
#define GIF_TRAILER				59	
#define LOGICAL_SCREEN_PACKED_BIT_FIELDS	247
#define BACKGROUND_COLOR_INDEX			0
#define PIXEL_ASPECT_RATIO			0
#define IMAGE_SEPARATOR			        44	
#define IMAGE_LEFT_POSITION			0
#define IMAGE_TOP_POSITION			0
#define IMAGE_DESCRIPTOR_PACKED_BIT_FIELDS	0
#define BLOCK_TERMINATOR			0
#define TABLE_SIZE 			   	12007/*4099 12007 20011 40007 50021 */
#define FIRST_NEW_OUTPUT_CODE		        258
#define BUFFER_SIZE				251
#define MIN_CODE_SIZE				8
#define START_CODE_BIT_SIZE			9
#define CLEAR_CODE			        256	
#define MAX_STRING_CODE				4096
#define OVERFLOW				4096
#define LZW_TERMINATOR_CODE			257

/*set up macros for a BOOLEAN type*/
#define BOOLEAN					char
#define TRUE					1
#define FALSE					0

/* Macro functions used in debugging */
#define xx 					printf("xx\n")
#define yy 					printf("yy\n")
#define zz 					printf("zz\n")



/*
******************************************************************************
**
**	Type Definitions
**
******************************************************************************
*/

/*a type definition for a string table entry in the
  string table array (which is called HashTable in
  this program)*/
struct StrTabNode{	int		StringSize;  /*size of the string in
								 table*/
			int		StringCode;  /*the code given to the
							   string in table*/
		 };
typedef struct StrTabNode StringTabNode; /*gives an easy name to
					   struct StrTabNode*/



/*
**************************************************************************
**
**	Prototypes
**
**************************************************************************
*/	

 /*Function that makes the GIF file. 
	It returns an error code*/
int 		writegif(FILE *, long, long, CHARH *);

 /*Function that writes a header "GIF87A" to GIF file.
   It returns an error code*/
int 		WriteHeader(FILE *);

 /*Function that writes the Logical Screen Descriptor to GIF file.
	It returns an error code*/
int 		WriteLogicalScreenDescriptor(FILE *, long, long);

 /*Function that writes the Global Color Table to GIF file.
   It returns an error code*/
int 		WriteGlobalColorTable(FILE *);
int			WriteColorTable(FILE * GIFfile, CHARH *colors);
int			WriteControl(FILE*GIFfile, CHARH*gifcontrol,long controllength);

 /*Function that writes the image data to GIF file.
   It returns an error code*/
int 		WriteImage(FILE *, CHARH * , long , long );

 /*Function that writes the Image Descriptor to GIF file.
   It returns an error code*/
int 		WriteImageDescriptor(FILE *, long, long);

 /*Function that compresses the image and writes it to GIF file.
   It returns an error code*/
int		WriteImageDataBlock(FILE *, CHARH *,long, long);

 /*Function that compares two LZW strings.  It returns TRUE if they are
   the same and FALSE if they are not*/
BOOLEAN 	CompareString(CHARH *, int, CHARH *, int );

 /*procedure that puts a string and its string code into the hash
   table*/
void 		PutInTable(CHARH *, int, int, StringTabNode *,CHARH **);

 /*procedure that initializes the hash table to nothing*/
void 		InitTable(StringTabNode *);

 /*function that puts bytes into the GIF buffer.  If it fills up it
   writes the buffer to the GIF file. It returns an error code*/
int 		PutInGIFBuffer(FILE *,  unsigned char *, unsigned char, int *);

 /*function that finds a string in the hash table.  It returns its
   string code if it is in the table and NULL if not*/
int 		FindInTable(CHARH *, int, StringTabNode *, CHARH **);

 /*function that packs string codes and puts them into the GIF buffer.
   It returns an error code*/
int 		WriteToGIF(FILE *,unsigned char *,int,char *,int *,int,int *);

 /*function that writes 8 bit sized data to the open GIF file
   It returns an error code*/
int 		WriteGIFdata8(FILE *, unsigned char);

 /*function that writes 16 bit sized data to the open GIF file as lo
   byte and hi byte It returns an error code*/
int 		WriteGIFdata16(FILE *, unsigned int);



/*
*************************************************************************
**
**	This function calls all other functions in this file to
**	create the GIF file.  This function outputs error messages if
**	a problem occures during the creation of the GIF.  It returns
**	ERROR if there was a problem and NO_ERROR if the GIF was created
**	ok.
**
*************************************************************************
*/
int writegif(	FILE *		GIFfile,    /*pointer to GIF file*/
		long		Lines,      /*number of lines in image*/
		long		Samples,    /*number of samples per line*/
		CHARH *		ImageBuffer /*pointer to the ImageBuffer
					      containing the raw image*/
	    )
{
   printf("Writing GIF file.\n");
   /*Write the Header block. Print an error message and return the appropriate
	  error code if there was an error*/
   if(WriteHeader(GIFfile)){
      printf("**>ERROR writing the GIF file(1).\n");
      return ERROR;
   } /*end if*/

   /*Write the Logical Screen Descriptor block Print an error message and
     return the appropriate error code if there was an error*/
   if(WriteLogicalScreenDescriptor(GIFfile, Lines, Samples)){
      printf("**>ERROR writing the GIF file(2).\n");
      return ERROR;
   } /*end if*/

   /*Write the Global Color Table block.  Print an error meaasage and
     return the appropriate error code if there was an error*/
   if(WriteGlobalColorTable(GIFfile)){
      printf("**>ERROR writing the GIF file(3).\n");
      return ERROR;
   } /*end if*/

   /*Write the Image Descriptor block, compress the image,  and write the
     compressed image data.  Print an error message and return the appropriate
     error code if there was an error*/
   if(WriteImage(GIFfile, ImageBuffer, Lines, Samples)){
      printf("**>ERROR writing the GIF file(4).\n");
      return ERROR;
   } /*end if*/

   /*Write the Trailer to indicate the end of the image.  Print an error
     message return the appropriate error code if there was an error*/
   if(WriteGIFdata8(GIFfile, GIF_TRAILER)) {
      printf("**>ERROR writing the GIF file(5).\n");
      return ERROR;
   } /*end if*/


   /*if no errors occured during GIF creation then print a message and return
     the appropriate error code */
   printf("GIF file was successfully written.\n");
   return NO_ERROR;
}


int writegif2(	FILE *		GIFfile,    /*pointer to GIF file*/
		long		Lines,      /*number of lines in image*/
		long		Samples,    /*number of samples per line*/
		CHARH *		ImageBuffer, /*pointer to the ImageBuffer containing the raw image*/
		CHARH *		ImageColors, /*pointer to the 256x3 palette*/
		CHARH *		gifcontrol,long controllength
	    )
{
   printf("Writing GIF file.\n");
   /*Write the Header block. Print an error message and return the appropriate
	  error code if there was an error*/
   if(WriteHeader(GIFfile)){
      printf("**>ERROR writing the GIF file(1).\n");
      return ERROR;
   } /*end if*/

   /*Write the Logical Screen Descriptor block Print an error message and
     return the appropriate error code if there was an error*/
   if(WriteLogicalScreenDescriptor(GIFfile, Lines, Samples)){
      printf("**>ERROR writing the GIF file(2).\n");
      return ERROR;
   } /*end if*/

   /*Write the Global Color Table block.  Print an error meaasage and
     return the appropriate error code if there was an error*/

   if(WriteColorTable(GIFfile,ImageColors)){
      printf("**>ERROR writing the GIF file(3).\n");
      return ERROR;
   } /*end if*/

   if(WriteControl(GIFfile, gifcontrol,controllength)){
      printf("**>ERROR writing the GIF file(4).\n");
      return ERROR;
   } /*end if*/

   /*Write the Image Descriptor block, compress the image,  and write the
     compressed image data.  Print an error message and return the appropriate
     error code if there was an error*/
   if(WriteImage(GIFfile, ImageBuffer, Lines, Samples)){
      printf("**>ERROR writing the GIF file(4).\n");
      return ERROR;
   } /*end if*/

   /*Write the Trailer to indicate the end of the image.  Print an error
     message return the appropriate error code if there was an error*/
   if(WriteGIFdata8(GIFfile, GIF_TRAILER)) {
      printf("**>ERROR writing the GIF file(5).\n");
      return ERROR;
   } /*end if*/

   /*if no errors occured during GIF creation then print a message and return
     the appropriate error code */
   printf("GIF file was successfully written.\n");
   return NO_ERROR;
}



/*
**************************************************************************
**
** 	This function writes the header portion of the GIF file.  If an
**	error occured it returns ERROR otherwise NO_ERROR.
**
**************************************************************************
*/
int WriteHeader(FILE * GIFfile  /*pointer to the GIF file*/)
{
   char HeaderString[7] = HEADER;
   int i;

   /*write header data.

     NOTE: the sizeof operator only indicates 6 characters of the string to 
	   write because it leaves off the string terminator
   */
   /*Write the Trailer to indicate the end of the image.  Print an error
     message return the appropriate error code if there was an error*/
   for(i = 0; i < 6; i++)
      if(WriteGIFdata8(GIFfile, HeaderString[i]))
	 return ERROR;

   return NO_ERROR;

}



/*
**************************************************************************
**
** 	This function writes the logical screen descriptor portion of the
**	GIF file.  If an error occured it returns ERROR otherwise NO_ERROR.
**
**************************************************************************
*/
int WriteLogicalScreenDescriptor(	FILE * GIFfile, /*pointer to GIF file*/
					long 	Lines,  /*number of lines
							  in image*/
					long 	Samples /*number of samples
							  per line*/
				)
{
   /*write the width of the display device in pixels and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, Samples))
      return ERROR;

   /*write the height of the display device in pixels and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, Lines))
      return ERROR;


   /*write the packed bit fields for:

     Global Color Table Flag 	(bit  7  ),
     Color Resolution		(bits 6-4),
     Sort Flag 			(bit  3  ), and
	 Size of Global Color Table (bits 2-0).

     Return the appropriate error code if there is an error
   */
   if(WriteGIFdata8(GIFfile, LOGICAL_SCREEN_PACKED_BIT_FIELDS))
      return ERROR;

   /*write the background color index and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata8(GIFfile, BACKGROUND_COLOR_INDEX))
      return ERROR;

   /*write the pixel aspect ratio and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata8(GIFfile, PIXEL_ASPECT_RATIO))
      return ERROR;

   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}



/*
**************************************************************************
**
** 	This function writes the global color table portion of the
**	GIF file.  If an error occured it returns ERROR otherwise NO_ERROR.
**
**************************************************************************
*/
int WriteGlobalColorTable(FILE * GIFfile /*pointer to GIF file*/)
{
   int i;

   /*write the color table

     NOTE: since this is a 256 grey scaled image the red, green, and blue
	   colors will all have the same intensity for each table entry.
	   This type of grey scale is put the table from intesities 0 to 255.
   */
   for(i = 0; i <= 255; i++) {
      if(WriteGIFdata8(GIFfile, i))
	 return ERROR;
      if(WriteGIFdata8(GIFfile, i))
	 return ERROR;
      if(WriteGIFdata8(GIFfile, i))
	 return ERROR;
   } /*end for*/
   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}

int WriteControl(FILE*GIFfile, CHARH*gifcontrol,long controllength)
{
   int i;
   for(i = 0; i <= controllength; i++) {
      if(WriteGIFdata8(GIFfile, gifcontrol[i]))
	 return ERROR;
   } /*end for*/
   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}



int WriteColorTable(FILE * GIFfile, CHARH *colors)
{
   int i;
   for(i = 0; i <= 768; i++) {
      if(WriteGIFdata8(GIFfile, colors[i]))
	 return ERROR;
   } /*end for*/
   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}



/*
**************************************************************************
**
** 	This function writes the image data portion of the
**	GIF file.  If an error occured it returns ERROR otherwise NO_ERROR.
**	It does this by calling the functions in this file that writes
**	that data.
**
**************************************************************************
*/
int WriteImage(	FILE * GIFfile,	/*pointer to GIF file*/
		CHARH * ImageBuffer,
				/*pointer to the image buffer
				  containing th RAW image*/
		long Lines,	/*number of lines in image*/
		long Samples 	/*number of samples per line*/
	      )
{

   /*Write the Image Descriptor block and return the appropriate error code
     if there was an error*/
   if(WriteImageDescriptor(GIFfile, Lines, Samples))
      return ERROR;

   /*Write the Image Data block and return the appropriate error code
	 if there was an error*/
   if(WriteImageDataBlock(GIFfile, ImageBuffer, Lines, Samples))
      return ERROR;

   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}




/*
***************************************************************************
**
** 	This function writes the image descriptor portion of the
**	GIF image.  If an error occured it returns ERROR otherwise NO_ERROR.
**
***************************************************************************
*/
int WriteImageDescriptor(	FILE * 		GIFfile, /*pointer to GIF file*/
				long		Lines,   /*number of lines
																in image*/
				long		Samples  /*number of samples
																per line*/
			)
{
   /*write the image separator and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata8(GIFfile, IMAGE_SEPARATOR))
      return ERROR;

   /*write the image left position and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, IMAGE_LEFT_POSITION))
      return ERROR;

   /*write the image top position and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, IMAGE_TOP_POSITION))
      return ERROR;

   /*write the image width and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, Samples))
      return ERROR;

   /*write the image height and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata16(GIFfile, Lines))
      return ERROR;

   /*write the packed bit fields for:

     Local Color Table Flag 	(bit  7  ),
	  Interlaced	Flag		(bit  6  ),
     Sort Flag 			(bit  5  ),
     Reserved 			(bits 4-3), and
     Size of Local Color Table  (bits 2-0).

     Return the appropriate error code if there is an error
   */
   if(WriteGIFdata8(GIFfile, IMAGE_DESCRIPTOR_PACKED_BIT_FIELDS))
		return ERROR;

   /*return the error code for no error if everything ran ok*/
   return NO_ERROR;
}



/*
**************************************************************************
**
** 	This function writes the image data block portion of the
**	GIF image.  If an error occured it returns ERROR otherwise NO_ERROR.
**	This function does the LZW compression of the image data and
**	then writes it to the GIF file.  It should be noted that a simple
**	hashing function is used to index into the LZW string table in
**      order to speed the compression process.  It uses linear probing
**	to find an item if a collision occures.
**
**	NOTE: This file uses CompuServe terminology and standard LZW
**	      terminology when ever possible.  The term string is used
**	      here because of this.  It should be noted that this is
**	      NOT an ascii string but rather raw data from the ImageBuffer.
**	      In order to fully understand how this code works it is recommended
**	      that one reads the CompuServe GIF specs before deciphering this
**	      code.
**
**************************************************************************
*/
int WriteImageDataBlock(        FILE * 		GIFfile,/*pointer to GIF file*/
				CHARH *		ImageBuffer,
							/*pointer to the
							ImageBuffer holding
							the raw image*/
				long		Lines,  /*number of lines*/
				long		Samples /*number of samples
							per line*/
						)
{
   StringTabNode *	HashTable	= (StringTabNode *)malloc(sizeof(StringTabNode   [TABLE_SIZE+OVERFLOW]));
						      /*The Hash Table*/
   CHARH **   		HTString	= (unsigned char**)malloc(sizeof(unsigned char * [TABLE_SIZE+OVERFLOW]));
						      /*The Hash Table's string
							pointers*/
   unsigned char 	GIFBuffer[BUFFER_SIZE];       /*a buffer to hold GIF
							image data before
							writing it*/
   CHARH *		StartString 	= ImageBuffer;/*a pointer to iterate
							thru the raw data*/
   long 		StringSize 	= 1; 	      /*used to index off Start
							String*/
   int 			StringCode 	= FIRST_NEW_OUTPUT_CODE;
						      /*codes to put in string
							table*/
   int 			ByteCount	= 0;          /*holds the current
							location in the
							GIFBuffer*/
   int 			LastPos		= 0;   	      /*holds the location
							of the bit where to
							write the next byte
							in GIFBuffer*/
   char 		LeftOver        = 0;          /*holds the left over
							data that needs to be
							put in GIFBuffer*/
   int 			CodeBitSize     = START_CODE_BIT_SIZE;
						      /*Holds the current bit
							size of the StringCode
							(for GIFs this is
							variable length)*/
   int 			CurrentCode; 		      /*Holds the currents
							string code pulled out
							of the HashTable*/
   int 			PreviousCode; 		      /*Holds the previous
							string code pulled
							out of the HashTable*/
   int 			i; 			      /*an iterater to go thru
																	loops*/
   long			ImageIndex	= 0;          /*used to index through image*/


   /*check to see is dynamically allocated memory was allocated or not*/
   if(HashTable == NULL) {
      printf("Out of memory(HT)\n");
      return ERROR;
   }

   /*check to see is dynamically allocated memory was allocated or not*/
   if(HTString == NULL) {
      printf("Out of memory(HTS)\n");
      return ERROR;
   }

   /*write the minimum code size and return the
     appropriate error code if there is an error*/
   if(WriteGIFdata8(GIFfile, MIN_CODE_SIZE))
      return ERROR;

   /*write the clear code to GIF file and return the
     appropriate error code if there is an error*/
   if(WriteToGIF(GIFfile, GIFBuffer, CLEAR_CODE, &LeftOver, &LastPos,
		      CodeBitSize, &ByteCount))
      return ERROR;

   /*initialize the HashTable*/
   InitTable(HashTable);

/*This is the main loop that goes thru the raw data in the ImageBuffer
  and compresses it*/

  /*loop from the begining address of ImageBuffer to the ending address*/
  while(ImageIndex < Lines*Samples){

     /*Pull the current code out of the Hash Table according to the
       current string*/
     CurrentCode = FindInTable(StartString, StringSize, HashTable, HTString);

     /*checks to see if the current code was in the hash table*/
     if(CurrentCode == -1){

	/*write the PreviousCode to GIF file and return the appropriate
	  error code if there was an error*/
	if(WriteToGIF(GIFfile, GIFBuffer, PreviousCode, &LeftOver, &LastPos,
			CodeBitSize, &ByteCount))
	   return ERROR;

	/*increase the bit size for the string code if it outgrows the
	  old size*/
	if(StringCode >= (1 << CodeBitSize))
	   CodeBitSize++;

	/*put the newest string in the table and its string code*/
	PutInTable(StartString, StringSize, StringCode, HashTable, HTString);

	/*update string code and look at a new string*/
	StringCode++;
	StartString += (StringSize -1) * sizeof(unsigned char);
	StringSize = 0;
	ImageIndex--;
     } /*end if*/

     /*remember CurrentCode for next time thru loop*/
     PreviousCode = CurrentCode;

     /*increase string size for next time thru loop*/
     StringSize++;

     /*increase index into image*/
     ImageIndex++;

     /*this checks to see if the StringCode larger than its max size.
       If so it writes the clear code to the GIF and resets everything*/
     if(StringCode > MAX_STRING_CODE){

	/*write the clear code to GIF file and return the
	  appropriate error code if there is an error*/
	if(WriteToGIF(GIFfile, GIFBuffer, CLEAR_CODE, &LeftOver,
		      &LastPos, CodeBitSize - 1, &ByteCount))
	   return ERROR;

	/*reset everything to initial values*/
	StringSize 	= 1;
	StringCode 	= FIRST_NEW_OUTPUT_CODE;
	CodeBitSize   = START_CODE_BIT_SIZE;
	InitTable(HashTable);
     } /*end if*/

  } /*end while*/

  /*write the last string code found to GIF file and return the
    appropriate error code if there is an error*/
  if(WriteToGIF(GIFfile, GIFBuffer, CurrentCode, &LeftOver, &LastPos,
		CodeBitSize, &ByteCount))
     return ERROR;

  /*write the code to terminate image data to GIF file and return the
    appropriate error code if there is an error*/
  if(WriteToGIF(GIFfile, GIFBuffer, LZW_TERMINATOR_CODE, &LeftOver, &LastPos,
		 CodeBitSize, &ByteCount))
     return ERROR;

  /*place the left over bits from compression in the GIF buffer*/
  PutInGIFBuffer(GIFfile, GIFBuffer, LeftOver, &ByteCount);

  /*write the GIF buffer to GIF file if needed and return the
    appropriate error code if there was an error*/
  if(ByteCount) {
     if(WriteGIFdata8(GIFfile, ByteCount))
	return ERROR;
     for(i = 0; i < ByteCount; i++)
	if(WriteGIFdata8(GIFfile, GIFBuffer[i]))
	   return ERROR;
  } /*end if*/

  /*write the BLOCK_TERMINATOR and return the
    appropriate error code if there is an error*/
  if(WriteGIFdata8(GIFfile, BLOCK_TERMINATOR))
     return ERROR;

  /*normal exit*/
  return NO_ERROR;
}





/*
**************************************************************************
**
**	This is used to compare LZW strings(see WriteImageDataBlock). Instead
**	of a regular C string terminator it uses size to indicate how long
**	the strings is (similar to a pascal string) because a NULL is
**	considered valid raw data.  This is used to determine if two
**	strings or string segments are the same.  If they are then
**	it returns TRUE, otherwise it returns FALSE.  The second
**	string may contain a NULL pointer; if it does a FALSE is returned
**	regardless.
**
**************************************************************************
*/
BOOLEAN CompareString(	CHARH * 		String1,/*first string*/
			int 			Size1,  /*size of first string*/
			CHARH *		 	String2,/*second string*/
			int 			Size2   /*size of second
																	string*/
		     )
{
   int i;

   /*see if string sizes are differnt or a NULL is found. If so FALSE
     is returned*/
   if((Size1 != Size2) || (String2 == NULL))
      return FALSE;

   /*compare the strings*/
   for(i = 0; i < Size1; i++)
      if(String1[i] != String2[i])
	 return FALSE;

   /*return here if strings are the same*/
   return TRUE;
}




/*
**************************************************************************
**
**	This puts a LZW string into the string table using a hashing
**	function.
**
**************************************************************************
*/
void PutInTable(	CHARH * 		StringToPut,/*the string*/
			int 			StringSize, /*size of string*/
			int 			StringCode, /*code to represent
							      string in GIF*/
			StringTabNode * 	HashTable,  /*the hash table*/
			CHARH **		HTString    /*Hash Table string
							      pointers to put
							      string in*/
		)
{
   /*derive a location in the HashTable*/
   long Location = (((long)StringToPut[0] + (long)StringToPut[1]) * (long)StringSize * 101) % TABLE_SIZE;

   /*see if a collision occured and find next blank spot*/
   while(HashTable[Location].StringSize != 0)
      Location ++;

   /*place string and its string code in HashTable*/
   HTString[Location] = StringToPut;
   HashTable[Location].StringSize = StringSize;
   HashTable[Location].StringCode  = StringCode;

   return;
}




/*
**************************************************************************
**
**	This initializes the string table to contain nothing.
**
**************************************************************************
*/
void InitTable(StringTabNode HashTable[TABLE_SIZE+OVERFLOW] /*the hash table*/)
{
   long i;

   /*initialize the HashTable to zero length strings*/
   for(i = 0; i < TABLE_SIZE + OVERFLOW; i++)
      HashTable[i].StringSize = 0;

   return;
}



/*
**************************************************************************
**
**	This puts GIF variable length codes into a buffer.   If the
**	buffer gets full it writes it to the GIF file and clears the
**	buffer.
**
**************************************************************************
*/
int PutInGIFBuffer(	FILE * 		GIFfile,   /*pointer to GIF file*/
			unsigned char * GIFBuffer, /*pointer to GIF buffer*/
			unsigned char 	ByteToPut, /*the byte to put in buffer*/
			int * 		ByteCount  /*index into the buffer*/
		  )
{
   int i;

   /*put data into the GIF buffer*/
   GIFBuffer[(* ByteCount)++] = ByteToPut;

   /*see if the buffer is full.  if so write it to GIF file*/
   if((* ByteCount) > BUFFER_SIZE - 1) {

      /*put buffer size at the top of image data sub-block*/
      if(WriteGIFdata8(GIFfile, BUFFER_SIZE))
	 return ERROR;

      /*write data to GIF file*/
      for(i = 0; i < BUFFER_SIZE; i++)
	 if(WriteGIFdata8(GIFfile, GIFBuffer[i]))
	    return ERROR;

      /*reset the byte count*/
      (* ByteCount) = 0;
   } /*end if*/
   return NO_ERROR;
}




/*
**************************************************************************
**
**	This finds a string in the string table using a hashing function
**	and returns its variable length code as its value. If nothing is
**	found then NULL is returned
**
**	NOTE: the variable length code is returned as an int. The unused
**	      bits are set to 0.
**
**************************************************************************
*/
int FindInTable(	CHARH * 		StringToFind, /*the string*/
			int 			StringSize,   /*size of string*/
			StringTabNode * 	HashTable,    /*the table to
								look in*/
			CHARH **		HTString      /*Hash Table
								Strings to look
								through*/
		)
{
   long Location = 0;

   /*if string size is one its location is the value of the single string byte*/
   if(StringSize == 1)
      return (int)StringToFind[0];

   /*find string in HashTable*/
   for(Location = (((long)StringToFind[0] + (long)StringToFind[1]) * (long)StringSize * 101) % TABLE_SIZE;
       Location < TABLE_SIZE + OVERFLOW;
       Location++  )

      /*if a StringSize 0 is found then it is not in the table*/
      if(HashTable[Location].StringSize == 0)
	 return (-1);

      /*if the string is found return its StringCode*/
      else if(CompareString(StringToFind, StringSize,
			    HTString[Location],
			    HashTable[Location].StringSize))
	      return HashTable[Location].StringCode;

   /*went thru the entire string table and nothing was found*/
   return (-1);
}




/*
**************************************************************************
**
**	This determines the size of the variable length code and packs
**	it into bytes.  It takes those bytes and puts them in the GIF
**	buffer.  If the last part of the varible length code is
**	not long enough to pack into a byte it is put into a left over
**	variable to be packed into a byte next time through.
**
**************************************************************************
*/
int WriteToGIF(		FILE * 		GIFfile,     /*pointer to GIF file*/
			unsigned char * GIFBuffer,   /*pointer to GIF buffer*/
			int 		NewData,     /*data to write to GIF*/
			char * 		LeftOver,    /*left over bits to be
						       packed next time around*/
			int * 		LastPos,     /*the bit position of the
						       last bit used*/
			int 		CodeBitSize, /*that size of the data to
						       be written*/
			int * 		ByteCount    /*index into the GIF
						       buffer*/

	      )
{
   /*pack LeftOver with the NewData*/
   unsigned long CodeToWrite = (unsigned long)((unsigned long)NewData *(1 << (unsigned long)(*LastPos)))|(unsigned long)(*LeftOver);

   /*write the first byte of the packed string tokens for this round and
     return the appropriate error code if there is an error*/
   if(PutInGIFBuffer(GIFfile, GIFBuffer, CodeToWrite & 255, ByteCount))
      return ERROR;

   /*see if only one byte had to be written to GIFBuffer*/
   if((* LastPos) + CodeBitSize < 16)
      /*set up LeftOver for next time thru*/
      (* LeftOver) = CodeToWrite >> 8;

   else {

      /*write the second byte of the packed string tokens for this round and
	return the appropriate error code if there is an error*/
      if(PutInGIFBuffer(GIFfile, GIFBuffer,(CodeToWrite & 65280)/256,ByteCount))
	 return ERROR;

      /*set up LeftOver for next time thru*/
      (* LeftOver) = CodeToWrite >> 16;
   } /*end if*/

   /*set up a new position for next time thru*/
   (* LastPos) = ((* LastPos) + CodeBitSize) % 8;
   return NO_ERROR;
}







/*
*****************************************************************************
**
**	This writes 8 bit data to the GIF file.
**
****************************************************************************
*/
int WriteGIFdata8(	FILE *		GIFfile, /*pointer to GIF file*/
			unsigned char   GIFdata  /*8 bit data to write*/
		  )
{
   /*write 8 bit data*/
   fwrite((unsigned char *)&GIFdata, sizeof(unsigned char), 1, GIFfile);

   if(ferror(GIFfile))
      /*exit with an error*/
      return ERROR;

   /*exit normally*/
   return NO_ERROR;
}


/*
*******************************************************************************
**
**	This writes 16 bit data to the GIF file.  It is in the form
**	lo-byte hi-byte.  It call WriteGIFdata8 to write those bytes
**	to the GIF file.
**
******************************************************************************
*/
int WriteGIFdata16(	FILE *		GIFfile, /*pointer to GIF file*/
			unsigned int    GIFdata  /*16 bit data to write*/
		   )
{

   /*break up 16 bit value into lo byte and hi byte*/
   if (WriteGIFdata8(GIFfile, LO_BYTE_16BIT))
      /*exit with an error*/
      return ERROR;
   else
      if (WriteGIFdata8(GIFfile, HI_BYTE_16BIT))
	 /*exit with an error*/
	 return ERROR;
      else
	 /*exit normally*/
	 return NO_ERROR;
}



