#include"FLASH_MEM.h"

/**********************************************Nripender Code************************************************/
FLASH_AddressTypeDef write_Address,L86_Read_Address;

int L86DataCount=5;
//	FLASH_AddressTypeDef address;
//	Assign values to the address components
//	address.Page = 0;// size of one page is 2048
//	address.Block = 0;// size of block 64 page
//	address.Plane = 0;// size of 1024 block and we have 2 plane
//	unsigned long Temp_Address = SPI_FLASH_Creat_ADDR_TO_WRITE(address.Page, address.Block, address.Plane);
unsigned long caculateAddress(int Page, int Block,int Plane){
	 return SPI_FLASH_Creat_ADDR_TO_WRITE(Page,Block,Plane);
}

bool ReadFromNANDFLASH(uint8_t *readBuffer,unsigned long address,unsigned short size){
	Unlock_ALL_Blocks();
	Block_Lock_Protection_Disable();
	if(!Read_Page(readBuffer, address, size))
		return true;
	return false;
}
bool WriteInToNANDFLASH(uint8_t *pBuffer,unsigned long address,unsigned short  size){
	Unlock_ALL_Blocks();
	Block_Lock_Protection_Disable();
    return Flash_write(pBuffer,address, size);
}
bool writeL86Data(uint8_t *dataBuffer,int size){
	bool status=false;
	  if(size+1>SIZE_OF_PAGE){
		  printc(MinDelay,"Page can't be write ");

		  return false ;
	  }
	  if(write_Address.Block< INIT_WRITE_BLOCK){
		  /*INCREAMENT THE PAGE NO.*/
		  write_Address.Page  = INIT_WRITE_PAGE;                                      /* RESET THE PAGE NO.*/
		  write_Address.Block = INIT_WRITE_BLOCK;
	  }
	  if(write_Address.Page==0){
	 		  Block_Erase(caculateAddress(0,write_Address.Block,0));
	  }
	  if(write_Address.Page >= WRITE_PAGES_LIMIT)
	  {
	     write_Address.Page = INIT_WRITE_PAGE;                                       /* RESET THE PAGE NO.*/
	    (write_Address.Block)++;                                                    /* INCREAMENT THE BLOCK NO.*/
	  }
	  if(write_Address.Block > LAST_BLOCK_ADDR)
	  {
	    write_Address.Page  = INIT_WRITE_PAGE;                                      /* RESET THE PAGE NO.*/
	    write_Address.Block = INIT_WRITE_BLOCK;                                     /* RESET THE BLOCK NO.*/                                                   /* INCREAMENT THE PLANE NO.*/
	  }
	status=WriteInToNANDFLASH(dataBuffer,caculateAddress(write_Address.Page,write_Address.Block,0),size);
	 printc(MinDelay,"\n Write Address \n%d",write_Address.Page);
	(write_Address.Page)++;
	 return status;
}

/*
 * Following function(deleteL86Data) will remove data from starting as we trying to implement FIFO methods *?
 */
bool deleteL86Data(){
	     if(L86_Read_Address.Block>=write_Address.Block && L86_Read_Address.Page>=write_Address.Page){
			  /*INCREAMENT THE PAGE NO.*/
	    	 printc(MinDelay, "No data Available");
	    	return false ;
		  }
		  if(L86_Read_Address.Page > WRITE_PAGES_LIMIT)
		  {
		     L86_Read_Address.Page = INIT_WRITE_PAGE;                                       /* RESET THE PAGE NO.*/
		    (L86_Read_Address.Block)++;                                                    /* INCREAMENT THE BLOCK NO.*/
		  }
		  if(L86_Read_Address.Block > LAST_BLOCK_ADDR)
		  {
			  L86_Read_Address.Page  = INIT_WRITE_PAGE;                                      /* RESET THE PAGE NO.*/
			  L86_Read_Address.Block = INIT_WRITE_BLOCK;                                     /* RESET THE BLOCK NO.*/                                                   /* INCREAMENT THE PLANE NO.*/
		  }
		  printc(MinDelay,"\nread Address \n%d",L86_Read_Address.Page);
		  L86_Read_Address.Page++;
		  return true;
}
/*
 * Following function write configuration on page 0 block 0 and plane 0
 */
//void writeConfigPage(){
//	uint8_t configBuff[CONFIG_BUFFER_SIZE];
//	makeConfigData(configBuff);
//	Block_Erase(caculateAddress(0,0,0));
//	WriteInToNANDFLASH(configBuff,strlen(configBuff),caculateAddress(0,0,0));
//	uint8_t readBuffer[1024];
//	ReadFromNANDFLASH(readBuffer, 1024, caculateAddress(0,0,0));
//	printc(MinDelay,"%s data at config is\n",readBuffer);
//	print(1000, readBuffer, 1024);
//}


/*
 * Following function resetL86Data will use to remove all data and start for block1
 */
void resetL86Data(){
		write_Address.Page=INIT_WRITE_PAGE;
		write_Address.Block=INIT_WRITE_BLOCK;
		write_Address.Plane=INIT_WRITE_PLANE;
		L86_Read_Address.Page=INIT_WRITE_PAGE;
		L86_Read_Address.Block=INIT_WRITE_BLOCK;
		L86_Read_Address.Plane=INIT_WRITE_PLANE;
}
// this function will return the value of pa


/*
 * Following function readL86Data will read the First value of FIFO Queue
 */

bool readL86Data(uint8_t *L86BufferData,int size){
	if ((write_Address.Block <= L86_Read_Address.Block)&& (write_Address.Page <= L86_Read_Address.Page)) {
	    return false;
	}
	ReadFromNANDFLASH(L86BufferData,caculateAddress(L86_Read_Address.Page,L86_Read_Address.Block,L86_Read_Address.Plane),size);
}

/*
 *Following function will used to setUp for Flash
 *
 * */

