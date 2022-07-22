/*******************************************************************
                        rotate filter
 *******************************************************************/
#include <windows.h>
#include <zmouse.h>
#include "filter.h"

/*------------------------------------------------------------------
   global variables
 ------------------------------------------------------------------*/
static int initialized = 0;
static int MIN_WIDTH  =  32;
static int MIN_HEIGHT =  32;
static int MAX_WIDTH  = 856;
static int MAX_HEIGHT = 576;

/*------------------------------------------------------------------
   function prototypes
 ------------------------------------------------------------------*/
void rotate_right_90(FILTER_PROC_INFO *fpip);
void rotate_left_90(FILTER_PROC_INFO *fpip);
void rotate_180(FILTER_PROC_INFO *fpip);
void reverse_horizontal(FILTER_PROC_INFO *fpip);
void reverse_vertical(FILTER_PROC_INFO *fpip);
void setup(FILTER *fp, void *editp);

/*------------------------------------------------------------------
   assembler function
 ------------------------------------------------------------------*/

/*------------------------------------------------------------------
   filter infomation
 ------------------------------------------------------------------*/
#define	TRACK_N	0

#define CHECK_N 5
char  *check_name[] =    {"‰E 90 “x‰ñ“]", "¶ 90 “x‰ñ“]", "180 “x‰ñ“]", "¶‰E”½“]", "ã‰º”½“]"};
int    check_default[] = {1,0,0,0,0};

FILTER_DLL filter = {
	FILTER_FLAG_INFORMATION|FILTER_FLAG_RADIO_BUTTON,
	0,0,
	"‰æ‘œ‰ñ“]",
	TRACK_N,
	NULL,
	NULL,
	NULL,NULL,
	CHECK_N,
	check_name,
	check_default,
	func_proc,
	NULL,
	NULL,
	NULL,
	func_WndProc,
	NULL,NULL,
	NULL,
	0,
	"Ver 0.3.0 by ‚Ü‚é‚à»ìŠ",
	NULL,
	NULL,
};

/*------------------------------------------------------------------
   export function
 ------------------------------------------------------------------*/
FILTER_DLL __declspec(dllexport) * __stdcall GetFilterTable( void )
{
	return &filter;
}

/*------------------------------------------------------------------
   filter function
 ------------------------------------------------------------------*/
BOOL func_proc( FILTER *fp,FILTER_PROC_INFO *fpip )
{
	PIXEL_YC *swap;

	if(!initialized){
		setup(fp, fpip->editp);
	}

	if(fp->check[0]){
		rotate_right_90(fpip);
	}else if(fp->check[1]){
		rotate_left_90(fpip);
	}else if(fp->check[2]){
		rotate_180(fpip);
	}else if(fp->check[3]){
		reverse_horizontal(fpip);
	}else{
		reverse_vertical(fpip);
	}

	swap = fpip->ycp_temp;
	fpip->ycp_temp = fpip->ycp_edit;
	fpip->ycp_edit = swap;

	return TRUE;
}

/*------------------------------------------------------------------
   filter window procedure
 ------------------------------------------------------------------*/
BOOL func_WndProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam, void *editp, FILTER *fp )
{
	UNREFERENCED_PARAMETER(wparam);
	UNREFERENCED_PARAMETER(lparam);
	UNREFERENCED_PARAMETER(editp);
	UNREFERENCED_PARAMETER(fp);
	
	switch(message){
	case WM_FILTER_INIT:
		setup(fp, editp);
		break;
	case WM_KEYUP:
	case WM_KEYDOWN:
	case WM_MOUSEWHEEL:
		SendMessage(GetWindow(hwnd, GW_OWNER), message, wparam, lparam);
		break;
	}
	return FALSE;
}

/*-----------------------------------------------------------------*/
void rotate_right_90(FILTER_PROC_INFO *fpip)
{
	int x,y;
	int offset;
	int width, height;

	if(fpip->w < MAX_HEIGHT){
		offset = 0;
		height = fpip->w;
	}else{
		offset = (fpip->w - MAX_HEIGHT) / 2;
		height = MAX_HEIGHT;
	}
	width = fpip->h;

	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			fpip->ycp_temp[y*fpip->max_w+x] = fpip->ycp_edit[(fpip->h-x-1)*fpip->max_w+offset+y];
		}
	}

	fpip->w = width;
	fpip->h = height;
}

/*-----------------------------------------------------------------*/
void rotate_left_90(FILTER_PROC_INFO *fpip)
{
	int x,y;
	int offset;
	int width, height;

	if(fpip->w < MAX_HEIGHT){
		offset = 0;
		height = fpip->w;
	}else{
		offset = (fpip->w - MAX_HEIGHT) / 2;
		height = MAX_HEIGHT;
	}
	width = fpip->h;

	for(y=0;y<height;y++){
		for(x=0;x<width;x++){
			fpip->ycp_temp[y*fpip->max_w+x] = fpip->ycp_edit[x*fpip->max_w+(fpip->w-offset-y-1)];
		}
	}

	fpip->w = width;
	fpip->h = height;
}

/*-----------------------------------------------------------------*/
void rotate_180(FILTER_PROC_INFO *fpip)
{
	int x,y;

	for(y=0;y<fpip->h;y++){
		for(x=0;x<fpip->w;x++){
			fpip->ycp_temp[y*fpip->max_w+x] = fpip->ycp_edit[(fpip->h-y-1)*fpip->max_w+(fpip->w-x-1)];
		}
	}
}

/*-----------------------------------------------------------------*/
void reverse_horizontal(FILTER_PROC_INFO *fpip)
{
	int x,y;

	for(y=0;y<fpip->h;y++){
		for(x=0;x<fpip->w;x++){
			fpip->ycp_temp[y*fpip->max_w+x] = fpip->ycp_edit[y*fpip->max_w+(fpip->w-x-1)];
		}
	}
}

/*-----------------------------------------------------------------*/
void reverse_vertical(FILTER_PROC_INFO *fpip)
{
	int y;

	for(y=0;y<fpip->h;y++){
		memcpy(fpip->ycp_temp+(y*fpip->max_w), fpip->ycp_edit+((fpip->h-y-1)*fpip->max_w), sizeof(PIXEL_YC)*fpip->w);
	}
}

/*-----------------------------------------------------------------*/
void setup(FILTER *fp, void *editp)
{
	SYS_INFO si;
	
	initialized = 1;
	
	if(fp->exfunc->get_sys_info == NULL){
		/* do nothing */
		return;
	}

	if(!fp->exfunc->get_sys_info(editp, &si)){
		/* do nothing */
		return;
	}

	MIN_WIDTH  = si.min_w;
	MIN_HEIGHT = si.min_h;
	MAX_WIDTH  = si.max_w;
	MAX_HEIGHT = si.max_h;
}
