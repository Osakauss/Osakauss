#include <types.h>
#include <libs/klibc.h>
#include <x86.h>
#include <kernel/drivers/console.h>
#include <kernel/drivers/graphics/bitmap.h>
#include <kernel/drivers/graphics/framebuffer.h>
static void  init_cursor(void);


static void update_cursor();



struct Sconsole{
	u32 offset_x;
	u32 offset_y;
	u32 barrier[2];
	struct framebuffer_pixel color;
	u32 column;
	u32 row;
	u32 width;
	u32 height;
    u32 current_x;
	u32 crws; // means: current read and write status
};


static struct Sconsole console = {
	.barrier = {0,0},
	.offset_x = 0,
	.offset_y = 0,
};

extern struct Sframebuffer framebuffer;


static void putchat(char c){ // meaning: put char at ( this puts a char a postion entered)

    bitmapPC( console.column, console.row, c, console.color);
    console.column += BITMAP_SIZE + 1;
}


static void
scroll(void)
{
	if(console.row >= console.height-2*BITMAP_SIZE-2)
	{
		// I literally have no idea why it needs to be multiplied by 4, just did some testing and it works.
		memcpy(framebuffer.buffer, &framebuffer.buffer[4*((framebuffer.pitch)/4) * (BITMAP_SIZE+1)], (framebuffer.pitch)*(console.height-((BITMAP_SIZE)+1)));
		console.row -= BITMAP_SIZE+1;
		--console.offset_y;

	}

}


bool console_require()
{

	console.height = framebuffer.height;
	console.width = framebuffer.width;
	console.row = 0;
	console.column = 0;
    console.color = GeneratePixelFG(0xff,0xff,0xff);
	init_cursor();
	return true;
}




void putch(char c)
{
	if (c == '\n'){
		console.row += BITMAP_SIZE+1;
		console.offset_y++;
		console.column=0;
		console.offset_x = 0;
	}
	else if(c == '\002'){
		struct framebuffer_pixel color = GeneratePixelFG(0x24 ,0x2D, 0xD8);
		console.color = color;
	}
	else if (c == '\001'){
		struct framebuffer_pixel color = GeneratePixelFG(0xFF, 0xFF, 0xFF);
		console.color = color;
	}
	else if (c == '\003'){
		struct framebuffer_pixel color = GeneratePixelFG (0xAF,0x11,0x16);
		console.color = color;
	}
	else if(c == '\004'){
		struct framebuffer_pixel color = GeneratePixelFG(0x25,0xCC,0x11);
		console.color = color;
	}
	else if( console.column >= console.width  - 2*BITMAP_SIZE){
		putch('\n');
		putchat(c);
		++console.offset_x;
	}
	else if (c == '\b'){
		if (console.column == 0){

		}
		else{
				if (console.barrier[0] == 0 && console.barrier[1] == 0){
					console.column -= BITMAP_SIZE+1;
					--console.offset_x;
					putch(' ');
					console.column -= BITMAP_SIZE+1;
					--console.offset_x;
				}
				else if (console.barrier[0] >= console.offset_x && console.barrier[1] == console.offset_y );
				else{
					console.column -= BITMAP_SIZE+1;
					--console.offset_x;
					putch(' ');
					console.column -= BITMAP_SIZE+1;
					--console.offset_x;

				}

		}

	}
	else if (c == '\t'){
		for (int x=0; x!=4; x++){
			putch(' ');
		}
	}
	else if (c == '\r'){
		console.column=0;
		console.offset_x=0;
	}
	else{
		putchat(c);
		++console.offset_x;
	}
	scroll();
	update_cursor(console.column,console.row);

}

static void write(string s, u32 size)
{
	for (u32 i = 0; i < size; i++)
		putch(s[i]);
}

void printk(string s)
{
	write(s, strlen(s));
}


static void
init_cursor(void){
	update_cursor();
}


static void
update_cursor()
{	/*
	console.column -= BITMAP_SIZE+1;
	--console.offset;
	putchat(' ');
	console.column -= BITMAP_SIZE+1;
	--console.offset;

    putchat((char)221);*/
}

extern void
MVCURSORC(int i){ // what it means: 'move cursor column'.
	console.column = console.column+BITMAP_SIZE + i;
	update_cursor();
}
extern void
ConsoleSetBarrier(){ //  set barrier
	console.barrier[0] = console.offset_x;
	console.barrier[1] = console.offset_y;
	return;
}
extern void
ConsoleRemBarrier(){ // remove barrier
	console.barrier[0] = 0;
	console.barrier[1] = 0;
	return;
}
