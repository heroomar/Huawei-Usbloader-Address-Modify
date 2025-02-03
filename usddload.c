
#include <stdio.h>
#include <stdint.h>

#include <windows.h>

FILE* ldr;
FILE* out;


void main(int argc, char* argv[]) {

unsigned int i,res,opt,datasize,pktcount,adr;
int bl; 
unsigned char c;

FILE* pt;
char ptbuf[2048];
uint8_t fileflag[41];

struct {
  int lmode;
  int size;
  int adr;
  int offset;
  char* pbuf;
} blk[10];

printf("\n Umer Zaki, 2025 Tool to change the address of usbloader");

ldr=fopen("usb.bin","rb");
if (ldr == 0) {
  printf("\n Invalid File usb.bin");
  return;
}

fread(&i,1,4,ldr);
if (i != 0x20000) {
  printf("\n Invalid usbloader\n");
  return;
}  

out=fopen("out.bin","wb+");

fseek(ldr,0,SEEK_SET);
fread(ptbuf,1,36,ldr);
fwrite(ptbuf, 1, 36, out);

fseek(ldr,36,SEEK_SET);
fread(&blk[0],1,16,ldr);
if(argv[1] && strlen(argv[1])){
  int num = (int)strtol(argv[1], NULL, 16);
  blk[0].adr = num;
}
fwrite(&blk[0], 1, 16, out);

fread(&blk[1],1,16,ldr);
if(argv[2] && strlen(argv[2])){
  int num1 = (int)strtol(argv[2], NULL, 16);
  blk[1].adr = num1;
}
fwrite(&blk[1], 1, 16, out);

printf("%s\n",blk );


if(blk[0].offset > 0){
  fread(ptbuf,1,16,ldr);
  fwrite(ptbuf, 1, 16, out);
}

for(bl=0;bl<2;bl++) {
  blk[bl].pbuf=(char*)malloc(blk[bl].size);
  fseek(ldr,blk[bl].offset,SEEK_SET);
  res=fread(blk[bl].pbuf,1,blk[bl].size,ldr);
  if (res != blk[bl].size) {
      printf("\n Usbloader Read Error %i\n",res,blk[bl].size);
      return;
  }
  fwrite(blk[bl].pbuf, 1, blk[bl].size, out);

}

printf("\n\n Part       address    size   \n------------------------------------------\n");

for(bl=0;bl<2;bl++) {

  datasize=1024;
  pktcount=1;

  for(adr=0;adr<blk[bl].size;adr+=1024) {
    if ((adr+1024)>=blk[bl].size) datasize=blk[bl].size-adr;  
    printf("\r %s    %08x %8i",bl?"usbboot":"raminit",blk[bl].adr,blk[bl].size);  
  }
  free(blk[bl].pbuf);
printf("\n");  
}
if(argv[1] && strlen(argv[1])){
  int num = (int)strtol(argv[1], NULL, 16);
  printf("New raminit address 0x%08X\n", num);
  
}
if(argv[2] && strlen(argv[2])){
  int num1 = (int)strtol(argv[2], NULL, 16);
  printf("New usbboot address 0x%08X\n", num1);
  
}


fclose(ldr);

fclose(out);
}

