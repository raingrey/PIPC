/**
*Copyright (c) 2013.dark_jadeite
*All rights reserved.
*
*文件名称: pipc.c
*文件标识: 自动创建设备节点文件
*
*当前版本：1.0
*作者：dark_jadeite
*
*2018.9.20
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>

MODULE_LICENSE("GPL");
#define PIPC_COUNT	1
dev_t dev = 0;
u32 pipc_major = 0;
u32 pipc_minor = 0;
/*定义cdev类型的变量*/
struct cdev pipc_cdev;

struct class *dev_class = NULL;
struct device *dev_device = NULL;

int pipc_open(struct inode* inode, struct file *filp)
{
	printk("enter pipc_open!\n");
	return 0;
}

long pipc_64_ioctl(struct file *fd, unsigned int cmd, unsigned long data)
{
	
	printk("enter arm64 ioctl!\n");
	return 0;
}

long pipc_32_ioctl(struct file *fd, unsigned int cmd, unsigned long data)
{
	printk("enter arm32 ioctl!\n");
	return 0;
}

struct file_operations pipc_fops = {
	.owner = THIS_MODULE,
	.open = pipc_open,
	.unlocked_ioctl=pipc_64_ioctl,
	.compat_ioctl=pipc_32_ioctl,
	};

int __init pipc_init(void)
{
	int ret = 0;
	// 动态分配设备号
	ret = alloc_chrdev_region(&dev, pipc_minor, PIPC_COUNT, "pipc");
	
	if(ret < 0){
		printk("register_chrdev_region failed!\n");
		goto failure_register_chrdev;
	}
	//获取主设备号
	pipc_major = MAJOR(dev);
	printk("pipc_major = %d\n", pipc_major);

	/*初始化cdev*/
	cdev_init(&pipc_cdev, &pipc_fops);
	/*添加cdev到内核*/
	ret = cdev_add(&pipc_cdev, dev, PIPC_COUNT);
	if(ret < 0){
		printk("cdev_add failed!\n");
		goto failure_cdev_add;
	}
	/*自动创建设备节点文件*/
	//1.注册设备类  /sys/class/pipc_class的文件夹
	dev_class = class_create(THIS_MODULE, "pipc_class");
	if(IS_ERR(dev_class)){
		printk("class_create failed!\n");
		ret = PTR_ERR("pipc_class");
		goto failure_class_create;
	}
	//2.注册设备  /sys/class/pipc_class/pipc0	  /dev/pipc0
	dev_device = device_create(dev_class, NULL, dev, NULL, "pipc_dark");
	if(IS_ERR(dev_device)){
		printk("device_create failed!\n");
		ret = PTR_ERR(dev_device);
		goto failure_device_create;
	}
	return 0;
failure_device_create:
	class_destroy(dev_class);
failure_class_create:
	cdev_del(&pipc_cdev);
failure_cdev_add:
	unregister_chrdev_region(dev, PIPC_COUNT);
failure_register_chrdev:
	return ret;

void __exit pipc_exit(void)
{
/*逆序消除*/
	//从内核中删除设备
	device_destroy(dev_class, dev);
	//从内核中删除设备类
	class_destroy(dev_class);
	//从内核中删除cdev
	cdev_del(&pipc_cdev);
	//注销设备号
	unregister_chrdev_region(dev, PIPC_COUNT);
}	

module_init(pipc_init);
module_exit(pipc_exit);
