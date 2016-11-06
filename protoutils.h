#ifndef PROTOUTILS
#define PROTOUTILS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include <ctype.h>
#include "base58.h"
#include "varhexutils.h"
#include "protocols.h"
//IP2INT

//IPv4 VALIDATOR
#define DELIM "."
 
/* return 1 if string contain only digits, else return 0 */
int valid_digit(char *ip_str)
{
	int err = 0;
    while (*ip_str) {
        if (*ip_str >= '0' && *ip_str <= '9')
            ++ip_str;
        else
            return 0;
    }
    return 1;
}
 
/* return 1 if IP string is valid, else return 0 */
int is_valid_ipv4(char *ip_str)
{
    int i, num, dots = 0;
    char *ptr;
	int err=0;
    if (ip_str == NULL)
        err = 1;
 
    // See following link for strtok()
    // http://pubs.opengroup.org/onlinepubs/009695399/functions/strtok_r.html
    ptr = strtok(ip_str, DELIM);
 
    if (ptr == NULL)
        err = 1;
 
    while (ptr) 
	{
 
        /* after parsing string, it must contain only digits */
        if (!valid_digit(ptr))
            err = 1;
 
        num = atoi(ptr);
 
        /* check for valid IP */
        if (num >= 0 && num <= 255) {
            /* parse remaining string */
            ptr = strtok(NULL, DELIM);
            if (ptr != NULL)
                ++dots;
        } else
            err = 1;
    }
 
    /* valid IP string must contain 3 dots */
    if (dots != 3)
	{
        err = 1;
	}
	if(err == 0)
	{
    return 1;
	}
	else
	{
		return 0;
	}
}


//////////////IPv6 Validator
#define MAX_HEX_NUMBER_COUNT 8 

int ishexdigit(char ch) 
{
   if((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F'))
      return(1);
   return(0);
}

int is_valid_ipv6(char *str)
{ 
   int hdcount=0;
   int hncount=0;
   int err=0;
   int packed=0;

   if(*str==':')
   {
      str++;    
      if(*str!=':')
         return(0);
      else
      {
         packed=1;
         hncount=1;
         str++;

         if(*str==0)
            return(1);
      }
   }

   if(ishexdigit(*str)==0)
   {
      return(0);        
   }

   hdcount=1;
   hncount=1;
   str++;

   while(err==0&&*str!=0)   
   {                      
      if(*str==':')
      {
         str++;
         if(*str==':')
         {
           if(packed==1)
              err=1;
           else
           {
              str++;

          if(ishexdigit(*str)||*str==0&&hncount<MAX_HEX_NUMBER_COUNT)
          {
             packed=1;
             hncount++;

             if(ishexdigit(*str))
             {
                if(hncount==MAX_HEX_NUMBER_COUNT)
                {
                   err=1;
                } else
                {
                   hdcount=1;
                   hncount++;
                   str++;   
                }
             }
          } else
          {
             err=1;
          }
       }
    }
	else
    {
           if(!ishexdigit(*str))
           {
              err=1;
           } else
           {
              if(hncount==MAX_HEX_NUMBER_COUNT)
              {
                 err=1;
              } else
              {
                  hdcount=1;
                  hncount++;
                  str++;   
              }
           }
        }
     } 
	 else
     {  
        if(ishexdigit(*str))
        {
           if(hdcount==4)
              err=1;
           else
           {
              hdcount++;          
              str++;
           }
         } else
            err=1;
     } 
   }

   if(hncount<MAX_HEX_NUMBER_COUNT&&packed==0)
      err=1;

    return(err==0);
}
///Still in work
uint64_t ip2int(char * ipconvertint)
{
	uint64_t final_result =0;
	char * iproc;
	int ipat1=0;
	int ipat2=0;
	int ipat3=0;
	int ipat4=0;
	char ip[16];
	strcpy(ip, ipconvertint);
	iproc = strtok (ip,".");
	for(int i=0; i<4;i++)
	{
		switch(i)
		{
			case 0:
			{
				ipat1 = atoi(iproc);
				break;
			}
			case 1:
			{
				ipat2 = atoi(iproc);
				break;
			}
			case 2:
			{
				ipat3 = atoi(iproc);
				break;
			}
			case 3:
			{
				ipat4 = atoi(iproc);
				break;
			}
			default:
			{
				printf("Somebody misplaced an int\n");
				break;
			}
		}
		iproc = strtok (NULL,".");
	}
	final_result =  ((ipat1*pow(2,24))+(ipat2*pow(2,16))+(ipat3*pow(2,8))+ipat4*1);
	return final_result;
}
char * int2ip(int inputintip)
{
	uint32_t ipint = inputintip;
	static char xxx_int2ip_result[16] = "\0";
	uint32_t ipint0 = (ipint >> 8*3) % 256;
	uint32_t ipint1 = (ipint >> 8*2) % 256;
	uint32_t ipint2 = (ipint >> 8*1) % 256;
	uint32_t ipint3 = (ipint >> 8*0) % 256;
	sprintf(xxx_int2ip_result, "%d.%d.%d.%d", ipint0,ipint1,ipint2,ipint3);
	return xxx_int2ip_result;
}
//I didn't feel another address_bytes_to_string was necesarry sry guys
int bytes_to_string(char * result, uint8_t * frombuf, size_t weesize)
{
	char removedlines[weesize*2];
	bzero(removedlines,sizeof(removedlines));
	strcpy(removedlines, Var_To_Hex(frombuf));
	//printf("Bytes to Hex: %s\n",removedlines);
	load_protocols();
	int thelastpos=0;
	for(int i=0;i<sizeof(removedlines);i++)
	{
		if(thelastpos!=0)
		{
			i = thelastpos;
			thelastpos=0;
		}
		char protocol[2];
		bzero(protocol,2);
		protocol[0] = removedlines[i];
		protocol[1] = removedlines[i+1];
		if(proto_with_deccode(Hex_To_Int(protocol))!=NULL)
		{
			//printf("Cproto: %s - %lu\n",protocol,Hex_To_Int(protocol));
			struct protocol * bprotox;
			bprotox = proto_with_deccode(Hex_To_Int(protocol));
			//Getting the address!
			char the_wanted_address[50];
			int ffpos=0;
			int zxp=0;
			int addrsizesofar=0;
			bzero(the_wanted_address,50);
			char isitp[2] = "\0";
			bzero(isitp,2);
			for(int zx=i+2;zx<sizeof(removedlines);zx++)
			{
				//printf("ZX: %d\n",zx);
				if(zx%2==0)
				{
					isitp[0] = removedlines[zx];
					isitp[1] = removedlines[zx+1];
					struct protocol * izp;
					izp=proto_with_deccode(Hex_To_Int(isitp));
					if(izp)
					{
						
						if(addrsizesofar == (bprotox->size/4))
						{
							printf("%d -- %d\n", addrsizesofar, (bprotox->size/4)); 
							the_wanted_address[zxp]='\0';
							thelastpos=zx;
							printf("THELASTPOS: %d ",thelastpos);
							break;
						}
						else if(addrsizesofar>(bprotox->size/4))
						{
							printf("MALFORMED STRING!!!\n");
							return 0;
						}

					}
					//printf("RL[%d]: %c\n",zx,removedlines[zx]);
					//printf("RL[%d]: %c\n",zx,removedlines[zx+1]);
				}
				addrsizesofar++;
				the_wanted_address[zxp] = removedlines[zx];
				zxp++;
			}
			
			printf("And the address:%s\n", the_wanted_address);
			printf("With size: %d\n", addrsizesofar);
			//printf("%d < %lu\n", i,sizeof(removedlines));
			strcat(result, "/");
			strcat(result,bprotox->name);
			strcat(result, "/");
			if(strcmp(bprotox->name,"ip4")==0)
			{
				strcat(result,int2ip(Hex_To_Int(the_wanted_address)));
			}
			else if(strcmp(bprotox->name,"tcp")==0)
			{
				char a[5];
				sprintf(a,"%lu",Hex_To_Int(the_wanted_address));
				strcat(result,a);
			}
			else if(strcmp(bprotox->name,"udp")==0)
			{
				char a[5];
				sprintf(a,"%lu",Hex_To_Int(the_wanted_address));
				strcat(result,a);
			}
			else
			{
				return 0;
			}
		}
		i++;
	}
	unload_protocols();
	strcat(result,"/");
	return 1;
	
}


char * address_string_to_bytes(struct protocol * xx, char * abc,size_t getsznow)
{
	static char astb__stringy[10] = "\0";
	int code = 0;
	code = xx->deccode;
	switch(code)
	{
		case 4://IPv4
		{
			char testip[16] = "\0";
			strcpy(testip,abc);
			if(is_valid_ipv4(testip)==1)
			{
				uint64_t iip = ip2int(abc);
				strcpy(astb__stringy,Int_To_Hex(iip));
				xx = NULL;
				return astb__stringy;
			}
			else
			{
				return "ERR";
			}
			break;
		}
		case 41://IPv6 Must be done
		{
			return "ERR";
			break;
		}
		case 6: //Tcp
		{
			if(atoi(abc)<65536&&atoi(abc)>0)
			{
				static char himm_woot[4] = "\0";
				bzero(himm_woot, 4);
				strcpy(himm_woot, Int_To_Hex(atoi(abc)));
				if(himm_woot[2] == '\0')
				{//manual switch
					char swap0='0';
					char swap1='0';
					char swap2=himm_woot[0];
					char swap3=himm_woot[1];
					himm_woot[0] = swap0;
					himm_woot[1] = swap1;
					himm_woot[2] = swap2;
					himm_woot[3] = swap3;
				}
				else if(himm_woot[3] == '\0')
				{
					char swap0='0';
					char swap1=himm_woot[0];
					char swap2=himm_woot[1];
					char swap3=himm_woot[2];
					himm_woot[0] = swap0;
					himm_woot[1] = swap1;
					himm_woot[2] = swap2;
					himm_woot[3] = swap3;
				}
				return himm_woot;
			}
			else
			{
				return "ERR";
			}
			break;
		}
		case 17: //Udp
		{
			if(atoi(abc)<65536&&atoi(abc)>0)
			{
				static char himm_woot2[4] = "\0";
				bzero(himm_woot2, 4);
				strcpy(himm_woot2, Int_To_Hex(atoi(abc)));
				if(himm_woot2[2] == '\0')
				{//Manual Switch2be
					char swap0='0';
					char swap1='0';
					char swap2=himm_woot2[0];
					char swap3=himm_woot2[1];
					himm_woot2[0] = swap0;
					himm_woot2[1] = swap1;
					himm_woot2[2] = swap2;
					himm_woot2[3] = swap3;
				}
				else if(himm_woot2[3] == '\0')
				{//Manual switch
					char swap0='0';
					char swap1=himm_woot2[0];
					char swap2=himm_woot2[1];
					char swap3=himm_woot2[2];
					himm_woot2[0] = swap0;
					himm_woot2[1] = swap1;
					himm_woot2[2] = swap2;
					himm_woot2[3] = swap3;
				}
				return himm_woot2;
			}
			else
			{
				return "ERR";
			}
			break;
		}
		case 33://dccp
		{
			return "ERR";
			break;
		}
		case 132://sctp
		{
			return "ERR";
			break;
		}
		case 301://udt
		{
			return "ERR";
			break;
		}
		case 302://utp
		{
			return "ERR";
			break;
		}
		case 42://IPFS - !!!
		{
			//abc=address
			break;
		}
		case 480://http
		{
			return "ERR";
			break;
		}
		case 443://https
		{
			return "ERR";
			break;
		}
		case 477://ws
		{
			return "ERR";
			break;
		}
		case 444://onion
		{
			return "ERR";
			break;
		}
		case 275://libp2p-webrtc-star
		{
			return "ERR";
			break;
		}
		default:
		{
			printf("NO SUCH PROTOCOL!\n");
			return "ERR";
			break;
		}
	}
}
int string_to_bytes(uint8_t * finalbytes,char * strx, size_t strsize)
{
	static char xxx[40] = "\0";
	bzero(xxx,40);
	int sigmalf = 0;
	char * totpch;
	char totalwordstest[strsize];
	strcpy(totalwordstest, strx);
	int totalwords = 0;
	totpch = strtok(totalwordstest, "/");
	while(totpch != NULL)
	{
		totpch = strtok (NULL, "/");
		totalwords++;
	}
	int processedwords = 0;
	char processedsofar[500] = "\0";
	char str[strsize]; //This string will be bad later.
	nextproc:
	strcpy(str,strx);
	if(str[0] == '/')
	{
		char * pch;
		pch = strtok (str,"/");
		load_protocols();
		for(int ax=0;ax<processedwords;ax++)
				{
					pch = strtok (NULL, "/");
				}
		while(pch != NULL)
		{
			if(proto_with_name(pch))
			{
				strcat(processedsofar, "/");
				strcat(processedsofar, pch);
				struct protocol * protx;
				//printf("PCH-P:%s\n",pch);
				protx = proto_with_name(pch);
				char cut[30]="\0";
				strcat(cut,Int_To_Hex(protx->deccode));
				cut[2] = '\0';
				char finipbit[2] = "\0";
				finipbit[0] = cut[0];
				finipbit[1] = cut[1];
				finipbit[2] = '\0';
				//Address
				pch = strtok (NULL, "/"); // Move to supposed address
				strcat(processedsofar, "/");
				strcat(processedsofar, pch);
				//printf("PCH-A:%s\n",pch);
				char addr[60] = "\0";
				strcpy(addr, pch);
				//If both are ok:
				strcat(xxx,finipbit);
				if(address_string_to_bytes(protx, addr,sizeof(addr)))
				{
					if(address_string_to_bytes(protx, addr,sizeof(addr)) == "ERR")
					{
						sigmalf = 1;
					}
					else
					{
						strcat(xxx,address_string_to_bytes(protx, addr,sizeof(addr)));
					}
				}
				else
				{
					sigmalf = 1;
				}
				pch = strtok (NULL, "/"); // Next step.
				processedwords++;
				processedwords++;

			}
			else
			{
				//This only gets triggered if the string has a wrong format
				//Or unknown protocol!
				printf("ERROR: MALFORMED STRING\n");
				return 0;
			}
		}
		if(processedwords != totalwords)
		{
			goto nextproc;
		}
		unload_protocols();
		if(sigmalf != 1)
		{
		//printf("S2B_RESULT: %s\n", xxx);
		//static uint8_t finalbytes[100] = {0};
		for(int i=0; i<100; i++)
		{
			finalbytes[i] = 0;
		}
		memcpy(finalbytes, Hex_To_Var(xxx), 100);
		int xtotbytes = 0;
		for(int i=0; i<100; i++)
		{
			if(finalbytes[i] != 0)
			{
				xtotbytes++;
			}
		}
		//printf("TOT BYTES: %d\n", xtotbytes);
		uint8_t finalproc[sizeof(finalbytes)/sizeof(finalbytes)[0]];
		for(int i=0;i<sizeof(finalproc); i++)
		{
			if(finalbytes[i] != 0)
			{
				finalproc[i] = '\0';
			}
		}			
		//printf("FinalBytes: %s\n",finalbytes);
		//printf("Finaltest: %s",Var_To_Hex(finalbytes));
			return 1;//success
		}
		else
		{
			printf("ERROR, MALFORMED STRING!\n");
			return 0;
		}
	}
	else
	{
		printf("ERROR, Multiaddr needs to start with '/'\n");
		return 0;
	}
}
#endif
