// terris.cpp ////////////////////////////////////////////////
//************************************************//
//       作品名：俄罗斯方块                       //
//       作者: 王宁                               //
//       联系方式:qq 30298344                     //
//       代码完成时间:2006年6月6日                //
//       编译环境:win XP & VC++ 6.0 & DX SDK8.1   //
//       版权说明:本demo为开源代码,你可任意       //
//                使用,但请保留此段               //
//                                                //
//************************************************//

// INCLUDES ///////////////////////////////////////////////

#define WIN32_LEAN_AND_MEAN  // just say no to MFC

#define INITGUID

#include <windows.h>   // include important windows stuff
#include <windowsx.h> 
#include <mmsystem.h>
#include <iostream.h> // include important C/C++ stuff
#include <conio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h> 
#include <math.h>
#include <io.h>
#include <fcntl.h>

#include <ddraw.h> // include directdraw

// DEFINES ////////////////////////////////////////////////

// defines for windows 
#define WINDOW_CLASS_NAME "WINCLASS1"

// default screen size
#define SCREEN_WIDTH    640  // size of screen
#define SCREEN_HEIGHT   480
#define SCREEN_BPP      8    // bits per pixel

#define BITMAP_ID            0x4D42 // universal id for a bitmap
#define MAX_COLORS_PALETTE   256

// TYPES //////////////////////////////////////////////////////

// basic unsigned types
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;

// container structure for bitmaps .BMP file
typedef struct BITMAP_FILE_TAG
	{
		BITMAPFILEHEADER bitmapfileheader;  // this contains the bitmapfile header
        BITMAPINFOHEADER bitmapinfoheader;  // this is all the info including the palette
        PALETTEENTRY     palette[256];      // we will store the palette here
        UCHAR            *buffer;           // this is a pointer to the data

	} BITMAP_FILE, *BITMAP_FILE_PTR;

typedef struct BRICK_TAG
	{
		int downheight[4];					//向下时用到的方块中各列高度的值
		int rightwidth[4];					//  右					宽度	
		int leftwidth[4];					//	左					宽度
		int upheight[4];					//为了重设TOP[]值而设的变量
	}BRICK,*BRICK_PTR;

typedef struct BLOCK_TAG
	{
		int x,y;
		int width[7][4];
		int height[7][4];
		int rows,cols;
		int currentframe;
		int type;
		int fixcounter;
		LPDIRECTDRAWSURFACE7 frame[4];
		BRICK brick[7][4];

	}BLOCK, *BLOCK_PTR;

	int width[7][4] =  {60,40,60,40,
						60,40,60,40,
						60,40,60,40,
						80,20,80,20,
						40,40,40,40,
						60,40,60,40,
						60,40,60,40};

	int height[7][4]=  {40,60,40,60,
						40,60,40,60,
						40,60,40,60,
						20,80,20,80,
						40,40,40,40,
						40,60,40,60,
						40,60,40,60,};

	int downheight[7][4][4]={20,40,40,0,  60,40,0,0, 20,40,40,0,  60,40,0,0,
							 40,40,20,0,  40,60,0,0, 40,40,20,0,  40,60,0,0,
							 40,40,40,0,  60,40,0,0, 20,40,20,0,  40,60,0,0,
						 	 20,20,20,20, 80,0,0,0,  20,20,20,20, 80,0,0,0,
							 40,40,0,0,   40,40,0,0, 40,40,0,0,   40,40,0,0,
							 40,40,40,0,  60,20,0,0, 20,20,40,0,  60,60,0,0,
							 40,40,40,0,  60,60,0,0, 40,20,20,0,  20,60,0,0};
	
	int upheight[7][4][4] = {40,40,20,0,  40,60,0,0, 40,40,20,0,  40,60,0,0,
						     20,40,40,0,  60,40,0,0, 20,40,40,0,  60,40,0,0,
							 20,40,20,0,  60,40,0,0, 40,40,40,0,  40,60,0,0,
						 	 20,20,20,20, 80,0,0,0,  20,20,20,20, 80,0,0,0,
							 40,40,0,0,   40,40,0,0, 40,40,0,0,   40,40,0,0,
							 40,20,20,0,  60,60,0,0, 40,40,40,0,  20,60,0,0,
							 20,20,40,0,  60,20,0,0, 40,40,40,0,  60,60,0,0};

	int blockgrid[7][4][4][4] ={1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
								0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
								1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,
								0,1,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
								0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,
								0,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,
								0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,1,0,0,1,0,0,0,0,0,0,0,
								1,1,1,0,0,1,0,0,0,0,0,0,0,0,0,0,
								0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0,
								1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
								1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,
								1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,
								1,1,0,0,1,0,0,0,1,0,0,0,0,0,0,0,
								1,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
								0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0,
								0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,
								1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,
								1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,
								1,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0};

// MACROS /////////////////////////////////////////////////

// tests if a key is up or down
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

// initializes a direct draw struct
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct));ddstruct.dwSize=sizeof(ddstruct); }

// 全局变量 ////////////////////////////////////////////////

HWND      main_window_handle = NULL; // globally track main window
HINSTANCE hinstance_app      = NULL; // globally track hinstance
int       window_closed      = 0;    // tracks if window is closed

static int col;

int top[10];   //记录当前最高值，
int grid[20][10];	//记录当前各单元方块有无的状态
bool above_grid_noblockflag;

int grid_row,grid_col;
int i,j,k;
int maxtop = 0;
long score = 0;
int killlayer_counter;
int speed_counter = 0;
bool flag_down = true;		//是否可以下移
bool flag_left = true;		//是否可以左移
bool flag_right = true;		//是否可以右移
bool downed = false;	//标识已本次循环已按过下落，即不需要自然下落了

// directdraw stuff
LPDIRECTDRAW7         lpdd         = NULL;   // dd4 object
LPDIRECTDRAWSURFACE7  lpddsprimary = NULL;   // dd primary surface
LPDIRECTDRAWSURFACE7  lpddsback    = NULL;   // dd back surface
LPDIRECTDRAWSURFACE7  lpddsbackground = NULL;// this will hold the background image
LPDIRECTDRAWSURFACE7  lpddsbackground2 = NULL;// this will hold the background image
LPDIRECTDRAWPALETTE   lpddpal      = NULL;   // a pointer to the created dd palette
LPDIRECTDRAWCLIPPER   lpddclipper  = NULL;   // dd clipper
PALETTEENTRY          palette[256];          // color palette
PALETTEENTRY          save_palette[256];     // used to save palettes
DDSURFACEDESC2        ddsd;                  // a direct draw surface description struct
DDBLTFX               ddbltfx;               // used to fill
DDSCAPS2              ddscaps;               // a direct draw surface capabilities struct
HRESULT               ddrval;                // result back from dd calls
DWORD                 start_clock_count = 0; // used for timing

BITMAP_FILE           bitmap;                // holds the bitmap
BLOCK				  block;


// 函数声明  //////////////////////////////////////////////

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height);
int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename);
int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap);
int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds,int color);
int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap, LPDIRECTDRAWSURFACE7 lpdds, int cx,int cy);
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height,
										  int mem_flags, int color_key);
int DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, int x, int y, 
                      int width, int height, LPDIRECTDRAWSURFACE7 dest, 
                      int transparent);    
LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list);
int Draw_Text_GDI(char *text, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds);



// 函数定义 //函数定义//函数定义/////函数定义///////函数定义///////////////////////////////

int Load_Bitmap_File(BITMAP_FILE_PTR bitmap, char *filename)
{
// this function opens a bitmap file and loads the data into bitmap

	int file_handle,  // the file handle
		index;        // looping index

	UCHAR   *temp_buffer = NULL; // used to convert 24 bit images to 16 bit
	OFSTRUCT file_data;          // the file data information

	//  打开文件如果文件存在的话open the file if it exists
	if ((file_handle = OpenFile(filename,&file_data,OF_READ))==-1)
	   return(0);

	// 载入文件头
	_lread(file_handle, &bitmap->bitmapfileheader,sizeof(BITMAPFILEHEADER));

	//是否为位图
	if (bitmap->bitmapfileheader.bfType!=BITMAP_ID)
	   {
	   _lclose(file_handle);	   // close the file
	   return(0);	   // return error
	   } // end if

	// now we know this is a bitmap, so read in all the sections

	// 载入信息头 now load the bitmap info header
	_lread(file_handle, &bitmap->bitmapinfoheader,sizeof(BITMAPINFOHEADER));

	// 载入调色板
	if (bitmap->bitmapinfoheader.biBitCount == 8)	//如果有调色板的话
	   {
	   _lread(file_handle, &bitmap->palette,MAX_COLORS_PALETTE*sizeof(PALETTEENTRY));
	   // now set all the flags in the palette correctly and fix the reversed 
	   // BGR RGBQUAD data format
	   for (index=0; index < MAX_COLORS_PALETTE; index++)
		   {
		   // 反转红和绿色，
		   int temp_color                = bitmap->palette[index].peRed;
		   bitmap->palette[index].peRed  = bitmap->palette[index].peBlue;
		   bitmap->palette[index].peBlue = temp_color;
       
		   // always set the flags word to this
		   bitmap->palette[index].peFlags = PC_NOCOLLAPSE;
		   } // end for index
		} // end if

	// finally the image data itself
	_lseek(file_handle,-(int)(bitmap->bitmapinfoheader.biSizeImage),SEEK_END);

	// now read in the image, if the image is 8 or 16 bit then simply read it
	// but if its 24 bit then read it into a temporary area and then convert
	// it to a 16 bit image

	if (bitmap->bitmapinfoheader.biBitCount==8 || bitmap->bitmapinfoheader.biBitCount==16 || 
		bitmap->bitmapinfoheader.biBitCount==24)
	   {
	   // delete the last image if there was one
	   if (bitmap->buffer)
		   free(bitmap->buffer);

	   // allocate the memory for the image
	   if (!(bitmap->buffer = (UCHAR *)malloc(bitmap->bitmapinfoheader.biSizeImage)))
		  {
		  // close the file
		  _lclose(file_handle);

		  // return error
		  return(0);
		  } // end if

	   // now read it in
	   _lread(file_handle,bitmap->buffer,bitmap->bitmapinfoheader.biSizeImage);

	   } // end if
	else
	   {//不是8位，16位或是24位
	   // serious problem
	   return(0);

	   } // end else

	#if 0
	// write the file info out 
	printf("\nfilename:%s \nsize=%d \nwidth=%d \nheight=%d \nbitsperpixel=%d \ncolors=%d \nimpcolors=%d",
			filename,
			bitmap->bitmapinfoheader.biSizeImage,
			bitmap->bitmapinfoheader.biWidth,
			bitmap->bitmapinfoheader.biHeight,
			bitmap->bitmapinfoheader.biBitCount,
			bitmap->bitmapinfoheader.biClrUsed,
			bitmap->bitmapinfoheader.biClrImportant);
	#endif

	// close the file
	_lclose(file_handle);

	// flip the bitmap
	Flip_Bitmap(bitmap->buffer, 
				bitmap->bitmapinfoheader.biWidth*(bitmap->bitmapinfoheader.biBitCount/8), 
				bitmap->bitmapinfoheader.biHeight);
	//int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)

	// return success
	return(1);

} // end Load_Bitmap_File

///***********************************************************

int Unload_Bitmap_File(BITMAP_FILE_PTR bitmap)
{
// this function releases all memory associated with "bitmap"
	if (bitmap->buffer)
	   {
	   // release memory
	   free(bitmap->buffer);

	   // reset pointer
	   bitmap->buffer = NULL;

	   } // end if

	// return success
	return(1);

} // end Unload_Bitmap_File

//**********************************************************

int Flip_Bitmap(UCHAR *image, int bytes_per_line, int height)
{
// this function is used to flip bottom-up .BMP images

	UCHAR *buffer; // used to perform the image processing
	int index;     // looping index

	// allocate the temporary buffer
	if (!(buffer = (UCHAR *)malloc(bytes_per_line*height)))
	   return(0);

	// copy image to work area
	memcpy(buffer,image,bytes_per_line*height);

	// flip vertically
	for (index=0; index < height; index++)
		memcpy(&image[((height-1) - index)*bytes_per_line],
			   &buffer[index*bytes_per_line], bytes_per_line);

	// release the memory
	free(buffer);

	// return success
	return(1);

} // end Flip_Bitmap

///////////////////////////////////////////////////////////////

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
                                         int num_rects,
                                         LPRECT clip_list)
{
// this function creates a clipper from the sent clip list and attaches
// it to the sent surface

	int index;                         // looping var
	LPDIRECTDRAWCLIPPER lpddclipper;   // pointer to the newly created dd clipper
	LPRGNDATA region_data;             // pointer to the region data that contains
									   // the header and clip list

	// first create the direct draw clipper
	if (FAILED(lpdd->CreateClipper(0,&lpddclipper,NULL)))
	   return(NULL);

	// now create the clip list from the sent data

	// first allocate memory for region data
	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER)+num_rects*sizeof(RECT));

	// now copy the rects into region data
	memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

	// set up fields of header
	region_data->rdh.dwSize          = sizeof(RGNDATAHEADER);
	region_data->rdh.iType           = RDH_RECTANGLES;
	region_data->rdh.nCount          = num_rects;
	region_data->rdh.nRgnSize        = num_rects*sizeof(RECT);

	region_data->rdh.rcBound.left    =  64000;
	region_data->rdh.rcBound.top     =  64000;
	region_data->rdh.rcBound.right   = -64000;
	region_data->rdh.rcBound.bottom  = -64000;

	// find bounds of all clipping regions
	for (index=0; index<num_rects; index++)
		{
		// test if the next rectangle unioned with the current bound is larger
		if (clip_list[index].left < region_data->rdh.rcBound.left)
		   region_data->rdh.rcBound.left = clip_list[index].left;

		if (clip_list[index].right > region_data->rdh.rcBound.right)
		   region_data->rdh.rcBound.right = clip_list[index].right;

		if (clip_list[index].top < region_data->rdh.rcBound.top)
		   region_data->rdh.rcBound.top = clip_list[index].top;

		if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
		   region_data->rdh.rcBound.bottom = clip_list[index].bottom;

		} // end for index

	// now we have computed the bounding rectangle region and set up the data
	// now let's set the clipping list

	if (FAILED(lpddclipper->SetClipList(region_data, 0)))
	   {
	   // release memory and return error
	   free(region_data);
	   return(NULL);
	   } // end if

	// now attach the clipper to the surface
	if (FAILED(lpdds->SetClipper(lpddclipper)))
	   {
	   // release memory and return error
	   free(region_data);
	   return(NULL);
	   } // end if

	// all is well, so release memory and send back the pointer to the new clipper
	free(region_data);
	return(lpddclipper);

} // end DDraw_Attach_Clipper


////用指定color填充入指定表面//////////////////////////////////////////////////   
int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds,int color)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(ddbltfx);

	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = color; 

	// ready to blt to surface
	lpdds->Blt(NULL,       // ptr to dest rectangle NULL为整个表面
			   NULL,       // ptr to source surface, NA            
			   NULL,       // ptr to source rectangle, NA
			   DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
			   &ddbltfx);  // ptr to DDBLTFX structure

	// return success
	return(1);
} // end DDraw_Fill_Surface

//在目标表面任意矩形内填充颜色
int Colore_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds,
						int x, int y,                 // 目标表面的X,Y
						int width,  int height,        // 目标和宽，和高
						int color)
{
	DDBLTFX ddbltfx; // this contains the DDBLTFX structure

	// clear out the structure and set the size field 
	DDRAW_INIT_STRUCT(ddbltfx);

	// set the dwfillcolor field to the desired color
	ddbltfx.dwFillColor = color; 

	RECT dest_rect;  // the destination rectangle
	// fill in the destination rect
	dest_rect.left   = x;
	dest_rect.top    = y;
	dest_rect.right  = x+width;
	dest_rect.bottom = y+height;

	// ready to blt to surface
	lpdds->Blt(&dest_rect, // ptr to dest rectangle NULL为整个表面
			   NULL,       // ptr to source surface, NA            
			   NULL,       // ptr to source rectangle, NA
			   DDBLT_COLORFILL | DDBLT_WAIT,   // fill and wait                   
			   &ddbltfx);  // ptr to DDBLTFX structure

	// return success
	return(1);
} // end Colore_Fill_Surface

//////将原表面传到目标表面的X,Y位置/////////////////////////////////////////////////////////

int DDraw_Draw_Surface(LPDIRECTDRAWSURFACE7 source, // 源表面
                       int x, int y,                 // 目标表面的X,Y
                       int width, int height,        // 源图和宽，和高
                       LPDIRECTDRAWSURFACE7 dest,    // 目标表面
                       int transparent = 1)          // transparency flag
{
// draw a bob at the x,y defined in the BOB
// on the destination surface defined in dest

	RECT dest_rect,   // the destination rectangle
		 source_rect; // the source rectangle                             

	// fill in the destination rect
	dest_rect.left   = x;
	dest_rect.top    = y;
	dest_rect.right  = x+width;
	dest_rect.bottom = y+height;

	// fill in the source rect
	source_rect.left    = 0;
	source_rect.top     = 0;
	source_rect.right   = width;
	source_rect.bottom  = height;

	// test transparency flag

	if (transparent)        //带透明
	   {
	   // enable color key blit
	   // blt to destination surface
	   if (FAILED(dest->Blt(&dest_rect, // ptr to dest rectangle NULL为整个表面
							source,		// ptr to source surface, NA
						 &source_rect,	// ptr to source rectangle, NA
						 (DDBLT_WAIT | DDBLT_KEYSRC),// fill and wait 
						 NULL)))		// ptr to DDBLTFX structure
			   return(0);
	   } // end if
	else					//不透明
	   {
	   // perform blit without color key
	   // blt to destination surface
	   if (FAILED(dest->Blt(&dest_rect,
							source,
							&source_rect,
							(DDBLT_WAIT),
							NULL)))
			   return(0);

	   } // end if

	// return success
	return(1);

} // end DDraw_Draw_Surface

//从一个表面  blt到 另一个表面////////////////////////////////////////////////
int SurfacetoSurface(LPDIRECTDRAWSURFACE7 source, // 源表面
					   int sx, int sy,				// 源表面的X,Y
					   int swidth, int sheight,		// 源和宽，和高
                       int dx, int dy,                 // 目标表面的X,Y
                       int dwidth,  int dheight,        // 目标和宽，和高
                       LPDIRECTDRAWSURFACE7 dest,    // 目标表面
                       int transparent = 1)          // transparency flag
{
// draw a bob at the x,y defined in the BOB
// on the destination surface defined in dest

	RECT dest_rect,   // the destination rectangle
		 source_rect; // the source rectangle                             

	// fill in the source rect
	source_rect.left    = sx;
	source_rect.top     = sy;
	source_rect.right   = sx+swidth;
	source_rect.bottom  = sy+sheight;

	// fill in the destination rect
	dest_rect.left   = dx;
	dest_rect.top    = dy;
	dest_rect.right  = dx+dwidth;
	dest_rect.bottom = dy+dheight;

	// test transparency flag

	if (transparent)        //带透明
	   {
	   // enable color key blit
	   // blt to destination surface
	   if (FAILED(dest->Blt(&dest_rect, // ptr to dest rectangle NULL为整个表面
							source,		// ptr to source surface, NA
						 &source_rect,	// ptr to source rectangle, NA
						 (DDBLT_WAIT | DDBLT_KEYSRC),// fill and wait 
						 NULL)))		// ptr to DDBLTFX structure
			   return(0);
	   } // end if
	else					//不透明
	   {
	   // perform blit without color key
	   // blt to destination surface
	   if (FAILED(dest->Blt(&dest_rect,
							source,
							&source_rect,
							(DDBLT_WAIT),
							NULL)))
			   return(0);

	   } // end if

	// return success
	return(1);

} // end SurfacetoSurface


//*******从bitmap结构中装载  各小位  图数据   到表面************************************/
int Scan_Image_Bitmap(BITMAP_FILE_PTR bitmap,     // bitmap file to scan image data from
                      LPDIRECTDRAWSURFACE7 lpdds, // surface to hold data
                      int cx, int cy)             // cell to scan image from
{
	UCHAR *source_ptr,   // working pointers
		  *dest_ptr;

	DDSURFACEDESC2 ddsd;  //  direct draw surface description 

	DDRAW_INIT_STRUCT(ddsd)

	// get the addr to destination surface memory

	// 对目标表面加锁
	lpdds->Lock(NULL,&ddsd,DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR,NULL);

	// compute position to start scanning bits from
	cx = cx*(ddsd.dwWidth+1) + 1;	// 1代表边框的1个像素
	cy = cy*(ddsd.dwHeight+1) + 1;

	// extract bitmap data
	source_ptr = bitmap->buffer + (cy*(bitmap->bitmapinfoheader.biWidth) )+cx;

	// assign a pointer to the memory surface for manipulation
	dest_ptr = (UCHAR *)ddsd.lpSurface;

	// iterate thru each scanline and copy bitmap
	for (int index_y=0; index_y < (int)ddsd.dwHeight; index_y++)
    {
		// copy next line of data to destination
		memcpy(dest_ptr, source_ptr, ddsd.dwWidth);

		// advance pointers
		source_ptr += bitmap->bitmapinfoheader.biWidth;
		dest_ptr   += (ddsd.lPitch);
    } // end for index_y

	// 解锁 
	lpdds->Unlock(NULL);

	// return success
	return(1);

} // end Scan_Image_Bitmap

//*************建立任意表面（离屏表面）***************************
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int mem_flags, int color_key = 0)
{
// this function creates an offscreen plain surface

	DDSURFACEDESC2 ddsd;         // working description
	LPDIRECTDRAWSURFACE7 lpdds;  // temporary surface
    
	// set to access caps, width, and height
	memset(&ddsd,0,sizeof(ddsd));
	ddsd.dwSize  = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	// set dimensions of the new bitmap surface
	ddsd.dwWidth  =  width;		//表面宽(像素)
	ddsd.dwHeight =  height;	//表面高(像素)

	// set surface to offscreen plain
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | mem_flags;	//mem-flags表明在显存还是内存

	// create the surface
	if (FAILED(lpdd->CreateSurface(&ddsd,&lpdds,NULL)))
	   return(NULL);

	// test if user wants a color key
	if (color_key >= 0)
	   {
	   // set color key to color 0
	   DDCOLORKEY color_key; // used to set color key
	   color_key.dwColorSpaceLowValue  = 0;
	   color_key.dwColorSpaceHighValue = 0;

	   // now set the color key for source blitting
	   lpdds->SetColorKey(DDCKEY_SRCBLT, &color_key);
	   } // end if

	// return surface
	return(lpdds);
} // end DDraw_Create_Surface

//WindowProc//////////////WindowProc////////////////WindowProc

LRESULT CALLBACK WindowProc(HWND hwnd, 
						    UINT msg, 
                            WPARAM wparam, 
                            LPARAM lparam)
{
	// this is the main message handler of the system
	PAINTSTRUCT		ps;		// used in WM_PAINT
	HDC				hdc;	// handle to a device context
	//char buffer[80];        // used to print strings

	// what is the message 
	switch(msg)
		{	
		case WM_CREATE: 
			{
			// do initialization stuff here
			// return success
			return(0);
			} break;
   
		case WM_PAINT: 
			{
			// simply validate the window 
   			hdc = BeginPaint(hwnd,&ps);	 
        
			// end painting
			EndPaint(hwnd,&ps);

			// return success
			return(0);
   			} break;

		case WM_DESTROY: 
			{

			// kill the application, this sends a WM_QUIT message 
			PostQuitMessage(0);

			// return success
			return(0);
			} break;

		default:break;

		} // end switch

	// process any messages that we didn't take care of 
	return (DefWindowProc(hwnd, msg, wparam, lparam));

} // end WinProc
///////////////////////////////////////////////////////////

int Draw_Text_GDI(char *text,int value, int x,int y,COLORREF color, LPDIRECTDRAWSURFACE7 lpdds)
{
	// this function draws the sent text on the sent surface 
	// using color index as the color in the palette

	HDC xdc; // the working dc

	// get the dc from surface
	if (FAILED(lpdds->GetDC(&xdc)))
	   return(0);

	// set the colors for the text up
	SetTextColor(xdc,color);

	// set background mode to transparent so black isn't copied
	SetBkMode(xdc, TRANSPARENT);

	// draw the text a
	TextOut(xdc,x,y,text,strlen(text));

	// release the dc
	lpdds->ReleaseDC(xdc);

	// return success
	return(1);
} // end Draw_Text_GDI

////////////////////////////////////////////////////////////

int Game_Init(void *parms = NULL, int num_parms = 0)
{
	// 建立DirectDraw 7.0 对像
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
       return(0);
	// 设置协作级别为：全屏
	if (FAILED(lpdd->SetCooperativeLevel(main_window_handle, 
                                      DDSCL_FULLSCREEN | DDSCL_ALLOWMODEX | 
                                      DDSCL_EXCLUSIVE | DDSCL_ALLOWREBOOT)))
		return(0);
	//  设置显示模式为： 640x480x8
	if (FAILED(lpdd->SetDisplayMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,0,0)))
		return(0);

	// we need a complex surface system with a primary and backbuffer

	// clear ddsd and set size
	DDRAW_INIT_STRUCT(ddsd); 
	// enable valid fields
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
	// request a complex, flippable
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_COMPLEX | DDSCAPS_FLIP;
	// set the backbuffer count field to 1, use 2 for triple buffering
	ddsd.dwBackBufferCount = 1;
	// 建立主表面
	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL)))
	   return(0);

	// 建立后备表面
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
	if (FAILED(lpddsprimary->GetAttachedSurface(&ddsd.ddsCaps, &lpddsback)))
	  return(0);

	// 用随机色建立调色表（1到254）
	for (int color=1; color < 255; color++)
		{
		// fill with random RGB values
		palette[color].peRed   = rand()%256;
		palette[color].peGreen = rand()%256;
		palette[color].peBlue  = rand()%256;

		// set flags field to PC_NOCOLLAPSE
		palette[color].peFlags = PC_NOCOLLAPSE;
		} // end for color

	// 0为黑，255为白
	palette[0].peRed     = 0;
	palette[0].peGreen   = 0;
	palette[0].peBlue    = 0;
	palette[0].peFlags   = PC_NOCOLLAPSE;

	palette[255].peRed   = 255;
	palette[255].peGreen = 255;
	palette[255].peBlue  = 255;
	palette[255].peFlags = PC_NOCOLLAPSE;

	if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_ALLOW256 | 
									DDPCAPS_INITIALIZE, 
									palette,&lpddpal, NULL)))
	return(0);

	// 关联调色板  到  主表面
	if (FAILED(lpddsprimary->SetPalette(lpddpal)))
	   return(0);

	// 在后备表面上设置剪切器
	RECT screen_rect= {0,0,SCREEN_WIDTH-1,SCREEN_HEIGHT-1};
	lpddclipper = DDraw_Attach_Clipper(lpddsback,1,&screen_rect);
	//LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,int num_rects,LPRECT clip_list)

	// 清理主表面和后备表面
	DDraw_Fill_Surface(lpddsprimary,0);
	DDraw_Fill_Surface(lpddsback,0);

		//-----------------------------------------------------------------------------
		// 读入背景位图到BITMAP
		if (!Load_Bitmap_File(&bitmap,"bk.bmp"))
		   return(0);
		// 建立用来放背景的离屏表面
		lpddsbackground = DDraw_Create_Surface(SCREEN_WIDTH,SCREEN_HEIGHT,0,-1);
		// 将背景位图复制到背景表面 
		// lock the surface
		lpddsbackground->Lock(NULL,&ddsd, DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,NULL);
		// get video pointer to BackGroud surfce
		UCHAR *image_buffer = (UCHAR *)ddsd.lpSurface;       
		// test if memory is linear
		if (ddsd.lPitch == SCREEN_WIDTH)
		   {
		   // copy memory from double buffer to primary buffer
		   memcpy((void *)image_buffer, (void *)bitmap.buffer, SCREEN_WIDTH*SCREEN_HEIGHT);
		   } // end if
		else	//表面宽度和屏幕宽度不同时
		   { // non-linear
		   // make copy of source and destination addresses
		   UCHAR *dest_ptr = image_buffer;
		   UCHAR *src_ptr  = bitmap.buffer;
		   // memory is non-linear, copy line by line
		   for (int y=0; y < SCREEN_HEIGHT; y++)	//逐行复制
			   {
			   // copy line
			   memcpy((void *)dest_ptr, (void *)src_ptr, SCREEN_WIDTH);
			   // advance pointers to next line
			   dest_ptr+=ddsd.lPitch;
			   src_ptr +=SCREEN_WIDTH;
			   } // end for
		   } // end else
		// now unlock the background surface
		if (FAILED(lpddsbackground->Unlock(NULL)))
		   return(0);
		// 卸载背景位图
		Unload_Bitmap_File(&bitmap);
	//-------------------------------------------------------------------
	// 建立用来放背景的离屏表面,用于消层时下移背景的像素
	lpddsbackground2 = DDraw_Create_Surface(SCREEN_WIDTH,SCREEN_HEIGHT,0,-1);

	// seed random number generator 初始随机数产生器，这样每次才真正随机
	srand(GetTickCount());
	
	for(i=0;i<7;i++)
	{
	  for(j=0;j<4;j++) 
		block.width[i][j]=width[i][j];
	}
	for(i=0; i<7; i++)
	{
	  for(j=0; j<4; j++)
		block.height[i][j] = height[i][j];
	}

	for(i=0; i<7; i++)						//用来向下移动判断碰撞用的高度
	{
		for(j=0; j<4; j++)
			for(k=0; k<4; k++)
				block.brick[i][j].downheight[k] = downheight[i][j][k];
	}

	for(i=0; i<7; i++)						//用来向左移动时判断碰撞用的宽度
	{
		for(j=0; j<4; j++)
			for(k=0; k<4; k++)
				block.brick[i][j].upheight[k] = upheight[i][j][k];
	}

	//初始化方块结构***************************************
	block.type = rand()%7;			//方块初始类型是随机的
	block.currentframe = rand()%3;	//方块初始帧是随机的
	if(block.width[block.type][block.currentframe]==40)
	  block.x = 300;				//初始方块左上角的位置
	else block.x =280;	
	block.y = 0;
	block.fixcounter = 0;

	block.rows = block.height[block.type][block.currentframe]/20;	//行数
	block.cols = block.width[block.type][block.currentframe]/20;	//列数


	//*******************************************************************


	// 读入方块位图到BITMAP结构********************************************
	if (!Load_Bitmap_File(&bitmap,"blocks.bmp"))
	   return(0);
	// 用方块位图的调色板重设主表面的调色板  lpddpal已和主表面关联
	if (FAILED(lpddpal->SetEntries(0,0,MAX_COLORS_PALETTE,bitmap.palette)))
	   return(0);
	// 建立各小表面并载入相应位图
	for (int index = 0; index < 4; index++)
	  {
		// 建立 4 个表面存放各帧图型
		  block.frame[index] = DDraw_Create_Surface(80,80,0,0);  //int width, int height, int mem_flags, int color_key=0
		  // 把位图装进相应表面	//这里没有用 memcpy
		  Scan_Image_Bitmap(&bitmap,                 // bitmap file to scan image data "from"
						  block.frame[index],	   // surface "to" hold data
						  index, block.type);               // cell(X,Y) to scan image from 
	  } // end for index
	Unload_Bitmap_File(&bitmap);	//卸载方块位图

	//***********************************************************************************

	for (i = 0; i < 10; i++)		//初始化各列当前最高位置的变量
		top[i]=0;
	
	for (i = 0; i < 20; i++)		//初始化用于消层的变量（表明每格子当前有无方块）
	  for ( j = 0; j < 10; j++)
		grid[j][j]=0;	

	maxtop = 0;

	// return success or failure or your own return code here
	return(1);
} // end Game_Init**************************************************************************************

//键盘处理过程	//移动方块****************************************************************************
inline void Keypro()
{
	if(KEYDOWN(VK_TAB))Sleep(1000);
	//有按键时，处理方块X,Y坐标
	if (KEYDOWN(VK_LEFT))
	{		//如果不能左移
	  for(i=0; i<block.rows; i++)	//逐行
		for(j=0; j<block.cols; j++)	//逐列
		  if(blockgrid[block.type][block.currentframe][i][j] == 1)
			if(grid[grid_row-i][grid_col+j-1] == 1  || block.x == 220)
			{
			  flag_left = false;	//不能左移
			  break;
			}
		if(flag_left)	//如果 能左移
		{
		  block.x-=20;	//左移
		  grid_col = (block.x-220)/20;	//当前方块左上角所在的列号
		}
		
		//如果不能再下移
		for(i=0; i<block.cols; i++)
			for(j=0; j<block.rows; j++)	
			  if(blockgrid[block.type][block.currentframe][block.rows - j - 1][i] == 1)
				if(grid[grid_row - block.rows + j][grid_col+i] == 1 
					|| block.y+block.height[block.type][block.currentframe] == 400)
				{
					flag_down = 0;	//不能下移
					if(block.fixcounter == 0)
						block.fixcounter=1;
					break;
				}
		if(flag_down)		//如果经左移后可以继续下落
		{
			block.fixcounter = 0;
		}
	}

	else if (KEYDOWN(VK_RIGHT))
	{//如果不能右移
	  for(i=0; i<block.rows; i++)		//逐行
		for(j=0; j<block.cols; j++)		//逐列
		  if(blockgrid[block.type][block.currentframe][i][block.cols-1-j] == 1)
			  if(grid[grid_row-i][grid_col+block.cols-j] == 1  
				|| block.x+block.width[block.type][block.currentframe] == 420)
			{
			  flag_right = false;	//不能右移
			  break;
			}
		if(flag_right)	//如果能右移
		{
		  block.x+=20;	//右移
		  grid_col = (block.x-220)/20;	//当前方块左上角所在的列号
		}
		//如果不能再下移
		for(i=0; i<block.cols; i++)
			for(j=0; j<block.rows; j++)	
			  if(blockgrid[block.type][block.currentframe][block.rows - j - 1][i] == 1)
				if(grid[grid_row - block.rows + j][grid_col+i] == 1 
					|| block.y+block.height[block.type][block.currentframe] == 400)
				{
					flag_down = 0;	//不能下移
					if(block.fixcounter == 0)
						block.fixcounter=1;
					break;
				}
		if(flag_down)	//如果经右移后可以继续下落
		{
			block.fixcounter = 0;
		}
	}

	else if (KEYDOWN(VK_DOWN))
	{
		//如果不能再下移
		for(i=0; i<block.cols; i++)
			for(j=0; j<block.rows; j++)	
			  if(blockgrid[block.type][block.currentframe][block.rows - j - 1][i] == 1)
				if(grid[grid_row - block.rows + j][grid_col+i] == 1 
					|| block.y+block.height[block.type][block.currentframe] == 400)
				{
					flag_down = 0;	//不能下移
					if(block.fixcounter == 0)
						block.fixcounter=1;
					break;
				}
		if(flag_down)	//如果 能下移
		{
		    block.y+=20;
			grid_row = 19-block.y/20;		//当前方块左上角所在的行号			
			downed = true;
		}
	}
	else if (KEYDOWN(VK_SPACE))
	{	//变形
		  block.currentframe++;
		  if(block.currentframe>3)
			block.currentframe=0;
		  block.rows = block.height[block.type][block.currentframe]/20;	//行数
		  block.cols = block.width[block.type][block.currentframe]/20;	//列数
		  if(block.x+block.width[block.type][block.currentframe]>419)	//如果方块超出右边
			block.x=420-block.width[block.type][block.currentframe];
	}
}// end of Keypro()


inline void no_keypro()
{
	if(flag_down)
	{
	  for(i=0; i<block.cols; i++)
		for(j=0; j<block.rows; j++)				//如果不能再下了
		  if(blockgrid[block.type][block.currentframe][block.rows-1-j][i] == 1)
			if(grid[grid_row-block.rows+j][grid_col+i] == 1 
			   || block.y+block.height[block.type][block.currentframe] >= 400)
			{
				flag_down = 0;	//不能下移
				if(block.fixcounter == 0)
					block.fixcounter=1;
				break;
			}
	}
	if(flag_down && !downed)
	{
		speed_counter++;
		if(speed_counter>5)
		{
		  block.y+=20;		//什么也没按时向下移动方块
		  speed_counter=0;
		}
	}
	downed = false;
	flag_down = true;		//
	flag_left = true;
	flag_right = true;
	if(block.fixcounter!=0)
	  block.fixcounter++;
}	//end of no_keypro()




//******************************************************************************************
//游戏逻辑*********************************************************************************
int Game_Main(void *parms = NULL, int num_parms = 0)
{// this is the main loop of the game, do all your processing here
	int index; 

	if (window_closed)
	   return(0);
	if (KEYDOWN(VK_ESCAPE) || maxtop == 400 )
	   {
	   PostMessage(main_window_handle,WM_CLOSE,0,0);
	   window_closed = 1;
	   } // end if

	// 画背景	将背景表面（包括固定的方块）  画到		后备表面
	DDraw_Draw_Surface(lpddsbackground,0,0, 640,480, lpddsback,0); 

	grid_row = 19-block.y/20;		//当前方块左上角所在的行号
	grid_col = (block.x-220)/20;	//当前方块左上角所在的列号

	if(block.fixcounter>10)	//计时完成	固定方块
	{
		//将块固定到背景表面		
		DDraw_Draw_Surface(block.frame[block.currentframe],
										block.x,block.y,
										block.width[block.type][block.currentframe],
										block.height[block.type][block.currentframe],
										lpddsbackground);

		//设置相应的gird[][] 为 1
		for(i=0; i<block.rows; i++)
		{
			 for(j=0; j<block.cols; j++)
			 {
			   if(grid[grid_row-i][grid_col+j] == 0)		//只设置没有方块的地方
				 grid[grid_row-i][grid_col+j] = blockgrid[block.type][block.currentframe][i][j];
			  }
		}
		//重设TOP
		for(j=0; j<block.cols; j++)
		{
		  top[grid_col+j] = 400 - (block.y + block.height[block.type][block.currentframe] - block.brick[block.type][block.currentframe].upheight[j]);	
		}

		for(j=0; j < 10; j++)				
		  if( maxtop < top[j] )
			maxtop = top[j];
			
		killlayer_counter = 0;
		//消层扫描
		for(i=0;i<20;i++)  // 从最下边扫到当前最高层
		{	
		  //重新设置grid
		  for(j=0; j<10 && grid[i][j]==1; j++)	//逐个查看当前行grid值是否都为1，如果都为1的话循环会进行到最后，即j会等于10
 			{}
		  if(j == 10)		//当前行要消层(即当前行grid[][]全是1
		  {
			for(k=0; k<19-i; k++)
			  for(j=0; j<10; j++)
				grid[i+k][j] = grid[i+k+1][j];	//上面层grid下移			  

			  for(j=0; j<10; j++)
				grid[19][j] = 0;			//最上面一行的grid置0
							
			//背景表面下移20个像素
			SurfacetoSurface(lpddsbackground,
								220,0,				//源X，Y
								200,400- 20*(i+1),	//源宽，高
								220,20,				//目标X，Y
								200,400 - 20*(i+1),//目标宽，高
								lpddsbackground2,0);//不带透明

			SurfacetoSurface(lpddsbackground2,
								220,20,
								200,400 - 20*(i+1),
								220,20,
								200,400 - 20*(i+1),
								lpddsbackground,0);//不带透明

			//背景表面最上面的20个像素置0
			Colore_Fill_Surface( lpddsbackground,
									220,0,         // 目标表面的X,Y
									200,20,        // 目标和宽，和高
									0);				//黑色
						
			//重置TOP值
			if(i==0)						//如果是最下面一层可以直接top-20
			{	
			  for(j=0; j < 10; j++)
				top[j] -= 20;	
			}
			else  //如果不是最下面一列，设TOP的方法
			{
			  for(j=0; j<10; j++)	//逐列检查 
			  {
				above_grid_noblockflag=true;
				for(k=1; k<20-i; k++)
				  if(grid[i+k][j]==1)
				  {
					above_grid_noblockflag=false; //如果上方有方块就将top-20
					break;
				  }
				if(above_grid_noblockflag && grid[i-1][j]==0 )//当前列上面所有行和下面一行都没有块
				{
				  for(k=1; k<=i; k++)//逐行向下找grid[][]=1的格子
				  {
					if(grid[i-k][j]==1)
					{
					  top[j]= (i-k+1)*20;
					  break;
					}
					else			//下方没有块了 =0
					  top[j]=0;
				  }
				}  
				else			//上方或邻下有块，-20
				  top[j] -= 20;
			  }
			}  // end of 重置TOP

					//重置i（行号），重新扫描
			i=(400-block.y-block.height[block.type][block.currentframe])/20-1;

			killlayer_counter = killlayer_counter+1;
		  }
		  else 					//当前行不要消层
		  {
			for(j=0; j<10 && grid[i][j]==0; j++)
			{}
 			if(j == 10)		//当前行grid[][]全是0
 		  	  break;
		  }
		}		// 消层扫描结束**************************************************

		switch(killlayer_counter)			//计分
		{
			case 0: break;
			case 1: score = score + 100; break;
			case 2: score = score + 300; break;
			case 3: score = score + 600; break;
			case 4: score = score + 1000; break;
			default : break;
		}

		//初始化新方块
		block.type = rand()%7;			//方块初始类型是随机的
		block.currentframe = rand()%3;	//方块初始帧是随机的
		if(block.width[block.type][block.currentframe]==40)
			block.x = 300;			//初始方块左上角的位置
		else block.x =280;
		block.y = 0;
		block.rows = block.height[block.type][block.currentframe]/20;	//行数
		block.cols = block.width[block.type][block.currentframe]/20;	//列数
		block.fixcounter = 0;
		// 读入方块位图到BITMAP结构
		if (!Load_Bitmap_File(&bitmap,"blocks.bmp"))
		  return(0);

		// 建立各小表面并载入相应位图
		for (int index = 0; index < 4; index++)
		{
			// 建立 4 个表面存放各帧图型
			block.frame[index] = DDraw_Create_Surface(80,80,0,0);  //int width, int height, int mem_flags, int color_key=0
			// 把位图装进相应表面
			Scan_Image_Bitmap(&bitmap,             // bitmap file to scan image data "from"
							  block.frame[index],	   // surface "to" hold data
							  index, block.type);      // cell to scan image from    	
		} // end for index
		Unload_Bitmap_File(&bitmap);	//卸载方块位图
	}	//end of if(counter>10)

	else	//继续计时(if 0 <= counter <=10)
	{
	// 移动方块
	Keypro();		//有按键时…	

	no_keypro();		//没有按键时…
	}  //end of if counter=0

	//画方块（到后备缓冲）
	DDraw_Draw_Surface(  block.frame[block.currentframe]  , 
						   block.x,block.y,
						   80,80,
						   lpddsback);

	// 用来调试时在屏幕打印即时数据
	HDC hdc;
	char buffer[80];

	if (FAILED( lpddsback->GetDC(&hdc) ))
	   return(0);
	// set the colors for the text up
	SetTextColor(hdc,RGB(255,255,255) );
	// set background mode to transparent so black isn't copied
	SetBkMode(hdc, TRANSPARENT);

	sprintf(buffer,"分数:%d",score);
	TextOut(hdc,15,SCREEN_HEIGHT-20,buffer,strlen(buffer) );

	sprintf(buffer,"maxtop:%d",maxtop);
	TextOut(hdc,15,SCREEN_HEIGHT-30,buffer,strlen(buffer) );

	lpddsback->ReleaseDC(hdc);




	// 将后备缓冲切到主表面
	while (FAILED(  lpddsprimary->Flip(NULL, DDFLIP_WAIT)));

	// wait a sec
	Sleep(100);//100为一秒10帧

	// return success or failure or your own return code here
	return(1);

} // end Game_Main

//***************************************************************************
int Game_Shutdown(void *parms = NULL, int num_parms = 0)
{
// this is called after the game is exited and the main event
// loop while is exited, do all you cleanup and shutdown here

// kill all the surfaces


// first the palette
if (lpddpal)
   {
   lpddpal->Release();
   lpddpal = NULL;
   } // end if

// now the primary surface
if (lpddsprimary)
   {
   lpddsprimary->Release();
   lpddsprimary = NULL;
   } // end if

// now blow away the IDirectDraw4 interface
if (lpdd)
   {
   lpdd->Release();
   lpdd = NULL;
   } // end if

// return success or failure or your own return code here
return(1);

} // end Game_Shutdown


//*************************************************************

//游戏主程序////////////////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE hprevinsstance,
				   LPSTR lpcmdline,
				   int ncmdshow)
{
WNDCLASSEX winclass;	//
HWND hwnd;				//
MSG msg;				//
HDC hdc;				//
PAINTSTRUCT ps;

// first fill in the window class stucture
winclass.cbSize         = sizeof(WNDCLASSEX);
winclass.style			= CS_DBLCLKS | CS_OWNDC | 
                          CS_HREDRAW | CS_VREDRAW;
winclass.lpfnWndProc	= WindowProc;
winclass.cbClsExtra		= 0;
winclass.cbWndExtra		= 0;
winclass.hInstance		= hinstance;
winclass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
winclass.hCursor		= LoadCursor(NULL, IDC_ARROW); 
winclass.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);  //黑色背景
winclass.lpszMenuName	= NULL;									//菜单
winclass.lpszClassName	= WINDOW_CLASS_NAME;					//类名
winclass.hIconSm        = LoadIcon(NULL, IDI_APPLICATION);		//

// save hinstance in global
hinstance_app = hinstance;

// 注册窗口类
if (!RegisterClassEx(&winclass))
	return(0);

// 建立窗口
if (!(hwnd = CreateWindowEx(NULL,                  // extended style
                            WINDOW_CLASS_NAME,     // class
						    "Terris Demo",         // title
						    WS_POPUP | WS_VISIBLE,
					 	    0,0,	  // initial x,y
						    SCREEN_WIDTH,SCREEN_HEIGHT,  // initial width, height
						    NULL,	  // handle to parent 
						    NULL,	  // handle to menu
						    hinstance,// instance of this application
						    NULL)))	// extra creation parms
return(0);

// save main window handle
main_window_handle = hwnd;

// initialize game here
Game_Init();

// enter main event loop
while(TRUE)
	{
    // test if there is a message in queue, if so get it
	if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
	   { 
	   // test if this is a quit
       if (msg.message == WM_QUIT)
           break;
	
	   // translate any accelerator keys
	   TranslateMessage(&msg);

	   // send the message to the window proc
	   DispatchMessage(&msg);
	   } // end if
    
       // main game processing goes here
       Game_Main();
       
	} // end while

// closedown game here
Game_Shutdown();

// return to Windows like this
return(msg.wParam);

} // end WinMain
