/* By Piotr Mitros. 
   Copyright (c) 2001. Piotr Mitros. GNU GPL. http://www.fsf.org/ */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define LF 10
#define CR 13

void drawmap(int room);

int get_location()
{
  char *rem_addr=getenv("REMOTE_ADDR");
  char *qs=getenv("QUERY_STRING");
  int il=-1;

  if(qs && (strstr(qs, "l=") || strstr(qs, "L="))) {
    char *loc=0;
    // Prevent possible buffer overflows
    char qsp[20];
    strncpy(qsp, qs, 15);
    memset(&qsp[7], 0, 5);
  
    loc=strstr(qsp, "l=");
    if(!loc)
      loc=strstr(qsp, "L=");
    if(!loc)
      return -1;

    loc+=2;
    il=(4-loc[0]+'0')*4+loc[1]-'1';
  }
  else {
    FILE*f;
    char ip[80];
    char *loc;

    if(!rem_addr)
      return -1;

    f=fopen("bs.db", "r");
    while (!feof(f)) {
      fgets(ip, 79, f);
      ip[70]=0;
      loc=strstr(ip, ":");
      if(!loc)
	break;
      *(loc)=0;
      loc++;
      if(!strcmp(ip, rem_addr)) {
	//printf("Found it \n");
	il=(4-loc[0]+'0')*4+loc[1]-'1';
      }
      }
    fclose(f);
    }

  if((il<0) || (il>15))
    return -1;

  return il;
}

void cat(const char*s)
{
  FILE*f=fopen(s, "r");
  char c;
  while(!feof(f)) {
    c=fgetc(f);
    if(c!=EOF)
      putchar(c);
  }
}

void main()
{
  int room=get_location();
  // Header
  printf("Content-type: text/html%c%c",LF,LF); 
  cat("laytop.html");
  drawmap(room);
  cat("laybot.html");
}

