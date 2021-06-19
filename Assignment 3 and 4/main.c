#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main() {
	
	 unsigned int mask_offset=0x0000FF;
	 unsigned int mask_pgno=0x00FF00;
	 int offset;
	 int pgno,rw;
	int num_total=0;
	int no_pages=256;
	int ptable[no_pages];
	int faults=0,index;
	
	char arr[62][256];
	 //short pagetable[pow(2,8)];

	//char address[9];
	unsigned int address;
	unsigned char page_flag;
	int readbit;
	int i;
	for(i=0;i<no_pages;i++){
		ptable[i]=-1;
	}
	
	

	FILE* fptr;
	FILE* bptr;
	fptr=fopen("addresses.txt","r");
	bptr=fopen("BACKING_STORE_1.bin","r");
	
	//printf("   Logical	Physical	Read\Write	Value	Page Fault");
	//printf("\n");
	while(!feof(fptr))
	{  
		fscanf(fptr,"%X",&address);
		fscanf(fptr, "%d",&rw);
		
		//printf("address is:");
		//printf("%X\n",address);
		offset=address&mask_offset;
		//printf("offset is: ");
		//printf("%X\n",offset);
		pgno=address&mask_pgno;
		pgno=pgno>>8;
		//printf("page no is: ");
		//printf("%X\n",pgno);
		fscanf(fptr,"%d",&readbit);
		//printf("read bit is: ");
		//printf("%d\n",readbit);
		num_total++;
		int add=ptable[pgno];
		if(ptable[pgno]==-1){
		
			faults++;
			//add=page_flag;
			//memcpy(arr+add*256*256,bptr+pgno*256*256,256*256);
			//ptable[pgno]=add;
			//page_flag++;
			
			fseek(bptr,pgno*no_pages,SEEK_SET);
			char adddata[no_pages];
			fread(&adddata,1,256,bptr);
			for(i=0;i<256;i++){
				arr[index][i]=adddata[i];
			}
			for(i=0; i<256;i++){
				if(ptable[i]==index){
					ptable[i]=-1;	
				}			
			}
			ptable[pgno]=index;
			index=(index+1)%62;
			int value=arr[ptable[pgno]][offset];
			//printf("Logical: %X physical: %X value: %X rw: %d\n", pgno,address,value,rw);
			printf("Logical: 0x%X ",address);
			printf("Physical: 0x%X%X ", ptable[pgno],offset);
			printf("value: 0x%X ",value);
			printf("rw: %d\n ",rw);
			printf("Page fault: True ");
			printf("index: %d\n",index);

		}
		else{
			int value=arr[ptable[pgno]][offset];
			printf("Logical: 0x%X ",address);
			printf("Physical: 0x%X%X ", ptable[pgno],offset);
			printf("value: 0x%X ",value);
			printf("rw: %d\n ",rw);
			printf("Page fault: False\n");
			printf("index: %d\n",index);
		
		
		}
		//value=arr[add*256+offset];
		
		

	}
	
	
	fclose(fptr);
float rateoffault=faults/num_total*100.0;
	printf("Total number of addresses:");
	printf("%d\n", num_total);
	printf("Total number of faults:");
	printf("%d\n", faults);
printf("fault rate: %f\n", rateoffault);

}
