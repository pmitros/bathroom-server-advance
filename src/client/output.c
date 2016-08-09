/* By Piotr Mitros. 
   Copyright (c) 2001. Piotr Mitros. GNU GPL. http://www.fsf.org/ */

#include <stdio.h>
#include <string.h>

#define DBG_PRN(x) printf("%s\n", x);
#define max(x,y) ((x)>(y)?(x):(y))
#define min(x,y) ((x)<(y)?(x):(y))
#define WIDTH 17
/* 5*4 */
#define HEIGHT 20

/* Building: 
   0  1  2  3
   4  5  6  7
   8  9  10 11
   12 13 14 15 */
int status[16];

#define MAPWIDTH 15

int distance[16][16] = { {0,1,4,5,6,7,8,9,2,3,10,11,12,13,14,15} ,
			 {1,0,4,5,6,7,8,9,2,3,10,11,12,13,14,15} ,
			 {2,3,6,7,5,4,10,11,0,1,8,9,15,14,13,12} ,
			 {3,2,6,7,5,4,10,11,0,1,8,9,15,14,13,12} ,
			 {4,5,6,7,0,1,8,9,2,3,10,11,12,13,14,15} ,
			 {5,4,6,7,0,1,8,9,2,3,10,11,12,13,14,15} ,
			 {6,7,5,4,10,11,2,3,9,8,1,0,14,15,13,12} ,
			 {7,6,5,4,10,11,2,3,9,8,1,0,14,15,13,12} ,
			 {8,9,12,13,4,5,6,7,14,15,0,1,10,11,2,3} ,
			 {9,8,12,13,4,5,6,7,14,15,0,1,10,11,2,3} ,
			 {10,11,14,15,6,7,12,13,4,5,2,3,8,9,0,1} ,
			 {11,10,14,15,6,7,12,13,4,5,2,3,8,9,0,1} ,
			 {12,13,14,15,8,9,10,11,4,5,6,7,0,1,2,3} ,
			 {13,12,14,15,8,9,10,11,4,5,6,7,0,1,2,3} ,
			 {14,15,13,12,10,11,8,9,6,7,5,4,2,3,0,1} ,
			 {15,14,13,12,10,11,8,9,6,7,5,4,2,3,0,1} };

/* Not currently used -- May be used in the future if we use 
   shortest-path algorithm instead of hard-coding distances to 
   give preference to bathrooms in similarly sexed floors. */
int male[]={2,3,4,5,12,13,14,15};
int female[]={6,7,8,9};
int coed[]={0,1,10,11};
int map[WIDTH][HEIGHT];

enum map_codes {
  nothing,
  marked,
  free_bath,
  closest_bath,
  no_bath, 
  use_bath_tooth_l, 
  use_bath_tooth_r, 
  use_bath_tlt_l, 
  use_bath_tlt_r, 
  use_bath_shwr_l, 
  use_bath_shwr_r, 
  use_bath_book_l,
  use_bath_book_r
};

// char *code_strings[]={" ","*", "O", "!", "X", "N"};
char *code_strings[]={"","", "empty", "close", "na", "teethl", 
		      "teethr", "tltl", "tltr", "shwrl", "shwrr", 
		      "readl", "readr"};

int get_closest_bathroom(int room)
{
  int j;
  if(room>15) {
    puts("Bad input\n");
    return 0;
  }
  for(j=0; j<16; j++)
    if(!status[distance[room][j]])
      return distance[room][j];
  return -1;
}

/* Takes 1-4 as argument. Returns position of bathroom in map. */
int bp_tmp[]={1, 5, 11, 15};
int bp(int b)
{
  if((b>=0) && (b<4))
    return bp_tmp[b];
  else {
    puts("Invalid parameter to bp\n");
    return -1;
  }
}

/* Returns the vertical coordinate of the central path on the ith floor */
int vp(int i) { return i*5+3; }

/* Vertical path in shaft s(0:1), from floor i to j (0:3) */
void mkvpath(int s, int i, int j)
{
  int t;
  /* Translate to map coordinates */
  i=vp(i); j=vp(j); s=3+s*10;
  //printf("Making v-path from %i to %i (%i)\n", i,j, s);
  /* Make i,j be min, max respectively */
  t=min(i,j); j=max(i,j); i=t;
  /* Draw path */
  for(t=i; t<=j; t++)
      map[s][t]=marked;
}

/* Horizontal path on floor f(0:3), from i to j (0:9), 
   {0:9} -> bath,room,stair,room,bath,bath,room,stair,room,bath */
int hp_tmp[]={0,1,3,5,6,8,9,11,13,14};
void mkhpath(int f, int i, int j)
{
  int t;
  f=vp(f);
  //printf("Making h-path from %i to %i (%i)\n", i,j, f);
  /* Make i,j be min, max respectively */
  t=min(i,j); j=max(i,j); i=t;
  /* Draw path */
  for(t=i; t<=j; t++)
    map[t][f]=marked;
}
     
/* h_path helpers */
int hbhlptmp[4]={0,7,9,16};
int hbath(int i) {return hbhlptmp[i]; }
int hrhlptmp[4]={1,5,11,15};
int hroom(int i) {return hrhlptmp[i]; }
int hstair(int i) {return 3+i*10; }

/* This could be cleaner. But it works. And it didn't take long
   to hack together. Whereas a real piece of code would have 
   taken longer. */
void path_closest(int room, int bath)
{
  int bx=bath%4, by=bath/4, rx=room%4, ry=room/4;

  //printf("Bath: %i (%i, %i), Room: %i (%i %i)\n", bath, bx, by, room, rx, ry);

  /* Set the corridor in front of the room and next to the bathroom */
  map[bp(bx) + (bx%2)*2-1 ][vp(by)-2] = marked;
  map[hbath(bx)][vp(by)] = marked;
  map[hbath(bx)][vp(by)-1] = marked;
  map[hbath(bx)][vp(by)-2] = marked;
  map[bp(rx)][vp(ry)] = marked;

  /* If the bathroom is on the same floor, we just draw a path from 
     here to there */
  if((by==ry) &&
     ((by == 1) || (by == 3) || (bx/2 == rx/2))) {
    //DBG_PRN("sf");
    mkhpath(by, hroom(rx), hbath(bx));
    return;
  }

  /* If the bathroom is on the same side, but a different floor, we go 
     the stairwell route */
  if((by!=ry) && (bx/2 == rx/2)) {
    //DBG_PRN("ss");
    //printf("bx:%i hbx:%i rx:%i hrx:%i\n", bx, hbath(bx), rx, hroom(rx));  
    mkhpath(by, hbath(bx), hstair(bx/2));
    mkhpath(ry, hroom(rx), hstair(rx/2));
    mkvpath(bx/2, by, ry);
    return;
  }

  /* We deal with the 110->330 contingencies, where we 
     want the stairwell of the room floor */
  if ((((ry==1) || (ry==3)) && ((by==1) || (by==3))) && 
      (((rx==0) && (bx==2)) || ((rx==3) && (bx==1)))) {
    //DBG_PRN("cont");
    mkhpath(ry, hroom(rx), hstair(rx/2));
    mkhpath(by, hbath(bx), hstair(rx/2));
    mkvpath(rx/2, ry, by);
    return;
  }

  /* If the bathroom is on a different side and different story, and 
     we're on a connecting floor, we cross and go */
  if((ry==1) || (ry==3)) {
    //DBG_PRN("cg");
    mkhpath(ry, hroom(rx), hstair(bx/2));
    mkhpath(by, hbath(bx), hstair(bx/2));
    mkvpath(bx/2, ry, by);
    return;
  }

  /* If the bathroom is on a different side and different story, and 
     it's on a connecting floor, we go and cross */
  if((by==1) || (by==3)) {
    //DBG_PRN("gc");
    mkhpath(ry, hroom(rx), hstair(rx/2));
    mkhpath(by, hbath(bx), hstair(rx/2));
    mkvpath(rx/2, ry, by);
    return;
  }

  /* Otherwise, we take the third floor route */
  //DBG_PRN("3fr");
  mkhpath(by, hbath(bx), hstair(bx/2));
  mkhpath(ry, hroom(rx), hstair(rx/2));
  mkhpath(1, hstair(bx/2), hstair(rx/2));
  mkvpath(bx/2, by, 1);
  mkvpath(rx/2, ry, 1);
  return;
}

void output(int room)
{
  int bath, i, j;
  /* Demark bathrooms as appropriate */
  for(i=0; i<4; i++)
    for(j=0; j<4; j++)
      if(!status[i+j*4]) {
	map[bp(i)][vp(j)-2]=free_bath;
      } else if(status[i+j*4]<30) {
	if(i%2)
	  map[bp(i)][vp(j)-2]=use_bath_tooth_r;
	else
	  map[bp(i)][vp(j)-2]=use_bath_tooth_l;
      } else if(status[i+j*4]<5*60) {
	if(i%2)
	  map[bp(i)][vp(j)-2]=use_bath_tlt_r;
	else
	  map[bp(i)][vp(j)-2]=use_bath_tlt_l;
      } else if(status[i+j*4]<20*60) {
	if(i%2)
	  map[bp(i)][vp(j)-2]=use_bath_shwr_r;
	else
	  map[bp(i)][vp(j)-2]=use_bath_shwr_l;
      } else {
	if(i%2)
	  map[bp(i)][vp(j)-2]=use_bath_book_l;
	else
	  map[bp(i)][vp(j)-2]=use_bath_book_r;
      }

  map[bp(2)][vp(3)-2]=no_bath;

  if((room>=0) && (room<16)) {
    /* Mark closest bathroom as closest */
    bath=get_closest_bathroom(room);
    if(bath != -1) {
      map[bp(bath%4)][vp(bath/4)-2] = closest_bath;
      path_closest(room, bath);
    }
  }
}

void load_status()
{
  /* Unix sockets require some code, so we kludge it to save 5 
     minutes. This way, iy may time out and lock up. We should
     configure Apache to kill us if that happens or write code
     that doesn't suck :) */
  FILE *socket=popen("nc bathroom.mit.edu 21314", "r");
  char bth[80], *loc, *stat, *time, *exp, *name;
  while(!feof(socket)) {
    memset(bth, 0, sizeof(bth));
    fgets(bth, 80, socket);
    strtok(bth, ":");
    loc=strtok(0, ":");
    stat=strtok(0, ":");
    time=strtok(0, ":");
    exp=strtok(0, ":");
    name=strtok(0, ":");
    if(name && *name) {
      int bath, state;
      name[strlen(name)-1]=0;
      bath=atoi(bth)-1;
      state=atoi(stat)*atoi(time);
      status[bath]=state;
      //printf("Set status of %i to %i\n", bath, state);
    }
  }
  /* No bathroom 14 */
  status[14]=1;
}

void drawmap(int room)
{
  char*s;
  int i,j;
  memset(map, 0, sizeof(map));
  load_status();
  output(room);
  printf("<table border=0 cellspacing=0 cellpadding=0>");
  for(i=0; i<HEIGHT; i++) {
    printf("<tr>");
    for(j=0; j<WIDTH; j++) {
      printf("<td>");
      switch(map[j][i]) {
      case nothing:
	printf("<img src=/bathroom/blank%ix%i.gif>", j+1, (i%5)+1);
	break;
      case marked:
	printf("<img src=/bathroom/pipe%ix%i.gif>", j+1, (i%5)+1);
	break;
      case free_bath:
      case closest_bath:
      case no_bath: 
      case use_bath_tooth_l: 
      case use_bath_tooth_r: 
      case use_bath_tlt_l: 
      case use_bath_tlt_r: 
      case use_bath_shwr_l: 
      case use_bath_shwr_r: 
      case use_bath_book_l:
      case use_bath_book_r:
	printf("<img src=/bathroom/icon_%s.gif>", code_strings[map[j][i]]);
	break;
      default:
	printf("?");
	break;
      }
      printf("</td>");
    }
    printf("</tr>\n");
  }
  printf("</table>");
}
