/*
 * pipc user space API
 * @dark_jadeite
 * 2018.9.21
 */

#include <unistd.h>
#include "pipc.h"

void * palloc(int fd,int key,int size);

/*
 * return:
 * +	pipc handler
 * -	pipc create error code
 */


/* 
 * return:
 * NULL	pipc memory alloc fault
 * 
 */
void * palloc(int fd,int key,int size)
{
	struct {
		int a,b;
		void * p;
	}data;
	data.a=key;
	data.b=flag;
	data.p=NULL;
	ioctl(fd,PIPC_ALLOC_IOCTL,data);
	return data.p;
}
