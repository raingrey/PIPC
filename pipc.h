/*
 * pipc user space API
 * @dark_jadeite
 * 2018.9.21
 * i know that it is fool to create some api for a kernel module;
 * but for user's convienience and because of ioctl()'s cmd is not easy to use,
 * i do this fool.
 */
#ifndef PIPC_H
#define PIPC_H

#define MAX_PIPC_CHANNELL_PER_TASK 0x10

#ifndef EMFILE
#define EMFILE 24    /＊ Too many open files ＊/
#endif
/*
 * 31-30:0-none/1-write/2-read
 * 29-16:memory size send by arg
 * 15-8:magic code
 * 7-0:cmd number
 */
#define PIPC_CREATE_IOCTL (1 << 30) | (4 << 16) | (0xff << 8) | 0
#define PIPC_ALLOC_IOCTL (1 << 30) | (4 << 16) | (0xff << 8) | 1

/*
 * apply_pipc_block/nonblock() once can create many pipcs
 * for sender block or not is nonmeans, but for receiver will determine it's block.
 */
#define apply_pipc_block() open("/dev/pipc_dark",O_RDWR,0) 
#define apply_pipc_nonblock() open("/dev/pipc_dark",O_RDWR|O_NONBLOCK,0) 
/*
 * only one IPC channel can created between two tasks,
 * but one task can create a lot of IPC channel with different tasks.  
 */
#define create_pipc(fd,key) ioctl(fd,PIPC_CREATE_IOCTL,key)
/*
 * only one palloc can success before send out;
 * and the size of memory user can alloc is 2^x.
 */
extern void * palloc(int fd,int key,int size);
/*
 * send memory alloced by palloc on "key" out and then user can palloc another.
 */
extern void * psend(int fd,int key);
/*
 * waiting for IPC memory
 * receiver of IPC should apply_pipc_block/nonblock()
 */
extern void * precv(int fd,int key,int size);


/*
memo.md
acquire for kernel module
1. open() support block/nonblock,accept from create_pipc() by flag
2. ioctl() - PIPC_CREATE_IOCTL 
	support command PIPC_CREATE_IOCTL for pipc channel init
	key is used to identify channel


*/

#endif
