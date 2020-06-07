#include <linux/kernel.h>  //printk()
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>      //everything  
#include <linux/slab.h>    //kmalloc()
#include <linux/cdev.h>    //struct cdev
#include <linux/uaccess.h> //copy_*_use
#include <linux/ioctl.h>
#include <linux/device.h>

#define SCULL_BUFF_MAXSIZE 1024   //为设备分配的内存空间
#define SCULL_MAJOR 0
#define SCULL_MINOR 0
#define SCULL_CMD1  1 
#define SCULL_CMD2  2 
#define SCULL_CMD3  3
#define SCULL_NR_DEVS 3         //要请求的连续设备编号的总数

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MYSCULL-TEST");
MODULE_AUTHOR("SWG");

/*scull设备结构体,此结构体可以封装设备相关的一些信息等 
  信号量等也可以封装在此结构中，后续的设备模块一般都 
  应该封装一个这样的结构体，但此结构体中必须包含某些 
  成员，对于字符设备来说，我们必须包含struct cdev cdev*/
struct scull_dev {  
	char scull_buf[SCULL_BUFF_MAXSIZE]; //
	unsigned long size;       			// amount of data stored here */ 
	struct cdev cdev;    				//cdev结构是描述字符设备的数据结构
};
struct scull_dev *scull_devices;   //scull设备结构体指针，全局持久的scull设备变量
struct class *sculldev_class;         //创建一个class类 然后在class类下，创建class_devices，即类下面创建类的设备

int scull_nr_devs = SCULL_NR_DEVS;  //scull设备的数目
int scull_major = SCULL_MAJOR;      //主设备号12位
int scull_minor = SCULL_MINOR;      //次设备号20位

/*Open函数关键是完成:分配并填写被置于filp->private_data里的数据结构!!*/
int scull_open(struct inode *inode,struct file *filp)
{
  struct scull_dev *dev;
	dev = container_of(inode->i_cdev,struct scull_dev,cdev);//通过指针struct cdev*来获得struct scull_dev*这个新指针!注意cdev数据是scull_dev结构体的成员变量名,inode->i_cdev是指向cdev变量类型的指针.
	filp->private_data = dev;//for other methods
	if((filp->f_flags & O_ACCMODE) == O_WRONLY)//只写的方式打开设备(涉及到多进程写。。)
    {
		;
    }
	return 0;
}
int scull_release(struct inode *inode , struct file *filp)
{
	printk(KERN_NOTICE "Scull released.\n");
	return 0;
}
/*read 读取设备数据至用户空间*/
ssize_t scull_read(struct file *filp , char __user *buf, size_t count, loff_t *f_pos)
{
	ssize_t retval = 0;
	struct scull_dev *dev = filp->private_data;//将设备数据赋值给变量dev,为提取设备数据做准备!
	if (*f_pos >= dev->size)   //超过了设备数据量，文件指针的位置已经不在设备中了！
		goto out;
	if (*f_pos + count > dev->size)   //count太大，确保文件指针在文件中，数据量有效
		count = dev->size - *f_pos;
	if (copy_to_user(buf, dev->scull_buf + *f_pos , count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;   //重新计算文件指针的位置
	retval = count;
out:
	return retval;
}
/*write 将用户空间的数据写入到设备中!*/
ssize_t scull_write(struct file *filp , const char __user *buf,size_t count ,loff_t *f_pos)
{
	ssize_t retval = -ENOMEM; /* value used in "goto out" statements */
	struct scull_dev *dev = filp->private_data;
	if (*f_pos >= SCULL_BUFF_MAXSIZE)    //写的位置超出最大值
		goto out;
	if (*f_pos + count > SCULL_BUFF_MAXSIZE)  //写的位置+count不能超过最大值
		count = SCULL_BUFF_MAXSIZE - *f_pos;
	if (copy_from_user(dev->scull_buf + *f_pos , buf, count)) {
		retval = -EFAULT;
		goto out;
	}
	*f_pos += count;  //f_pos位置更新
	retval = count;
	if (dev->size < *f_pos)  //更新size大小
		dev->size = *f_pos;
out:
	return retval;
}
/*   */
long scull_ioctl(struct file *filp,    //设备文件
				unsigned int cmd,     //功能号
				unsigned long arg)    //参数，值，或者用户空间指针
{
	if (cmd == SCULL_CMD1) {
		printk("running  SCULL_CMD1 \n");
		return 0;
	}
	if (cmd == SCULL_CMD2) {
		printk("running  SCULL_CMD2 \n");
		return 0;
	}
	if (cmd == SCULL_CMD3) {
		printk("running  SCULL_CMD3 \n");
		return 0;
	}
	printk("cmd  error! \n");
	return -EFAULT;  
}
/*scull_llseek方法用来修改文件的当前读写位置,并将新位置作为返回值返回*/
loff_t scull_llseek(struct file *filp,   //设备文件
					loff_t offset,       //偏移量
					int whence)          //seek方式
{
	struct scull_dev *dev = filp->private_data; 
	loff_t tmp_pos;
	switch (whence) {
		case 0:    	//SEEK_SET 
			tmp_pos = offset;
			break;
		case 1:     // SEEK_CUR 
			tmp_pos =  filp->f_pos + offset;
			break;
		case 2: 	//SEEK_END 
			tmp_pos = dev->size + offset; 
		default:   //can't happen
			return -EINVAL;
	}
	if ((tmp_pos >  SCULL_BUFF_MAXSIZE)  || (tmp_pos < 0))
		return -EINVAL;
	return filp->f_pos = tmp_pos;  
}
/*定义struct file_operations结构对象，初始化file_operations结构中的成员，函数指针*/
struct file_operations scull_fops = {
  .owner = THIS_MODULE ,
  .read = scull_read ,
  .write = scull_write ,
  .unlocked_ioctl = scull_ioctl , 
  .llseek = scull_llseek ,
  .open = scull_open ,
  .release = scull_release ,
};
/*为scull设备建立字符设备结构体*/
static void scull_setup_cdev(struct scull_dev *dev,int index)
{
	int err,devno = MKDEV(scull_major,scull_minor + index);
	cdev_init(&dev->cdev,&scull_fops);//初始化建立cdev和file_operations之间的连接,struct file_operations scull_fops
	dev->cdev.owner = THIS_MODULE;//所属模块
	dev->cdev.ops = &scull_fops; 
	err = cdev_add(&dev->cdev,devno,1);//在字符设备中添加一个设备，注册设备,返回0表示成功,非0表示失败
	if(err)
		printk(KERN_NOTICE "Error %d adding scull%d",err,index);
}
/*卸载模块*/
void scull_cleanup_module(void)
{
	/* cleanup_module is never called if registering failed */
	int i = 0;
	dev_t dev = MKDEV(scull_major , scull_minor);  //获取设备号
	if(scull_devices) 
	{
		for(i=0;i<scull_nr_devs;i++)
		{
			cdev_del(&scull_devices[i].cdev);   ////调用cdev_del()函数移除该设备对应的字符设备结构	
		}
		kfree(scull_devices);//释放设备scull_dev结构
	}
	for(i=0;i<scull_nr_devs;i++) {
		device_destroy(sculldev_class, dev+i);         //必须先删除设备，再删除class类
    }
    class_destroy(sculldev_class);                 //删除class类
	unregister_chrdev_region(dev, scull_nr_devs);  //根据设备号注销设备
	printk(KERN_NOTICE "Scull module exit.\n");
}
/*初始化模块的函数
(1)获得内核识别的设备号，若动态分配则调用alloc_chrdev_region,若已知则调用register_chrdev_region
(2)使用kmalloc动态的给scull_dev设备结构体在普通内核内存空间分配内存空间,并使用memset初始化为0
(3)初始化每个设备的访问区块(即内存区块),（设定量子和量子集,互斥锁）,调用scull_setup_cdev向内核注册这个char device
     scull_setup_cdev注册char设备的流程:
       a.调用MKDEV由实参生成具体设备的设备号
       b.初始化cdev,主要是指定其ops cdev_init(&dev->cdev, &scull_fops).dev->cdev在设备结构体中定义.
*/
int scull_init_module(void)
{
	int result,i;
	dev_t dev = 0;//高12位为主设备号，低20位为次设备号
	char devname[10]="myscull_";  //设备节点名  
	if(scull_major)//根据已知的主设备号,静态注册设备
    {
		dev = MKDEV(scull_major,scull_minor);//调用MKDEV宏定义,利用主次设备号得到dev_t
		result = register_chrdev_region(dev,scull_nr_devs,"scull");
    }
	else  //动态注册设备
	{
		result = alloc_chrdev_region(&dev,scull_minor,scull_nr_devs,"scull");//动态申请设备号，设备名称"scull",设备个数1，次设备号scull_minor,申请到的设备号存储在dev中。该函数返回值小于0表示申请失败。
		scull_major = MAJOR(dev);
	}
	if (result < 0) { //注册失败
		printk(KERN_WARNING "scull: can't get major %d\n",scull_major);
		return result;
	}
	sculldev_class = class_create(THIS_MODULE,"sculldev");  // 创建一个类，类名为char_class
    if(IS_ERR(sculldev_class)) { //类创建失败
        printk("Err: failed in creating class.\n");
        return -1;
    }
	for(i=0;i<scull_nr_devs;i++) {
		devname[8] = (char)48+i;
		device_create(sculldev_class,NULL,dev+i,NULL,devname);  //创建设备节点
    }
   //为scull_nr_devs个设备分配scull_dev的内存空间，并清空
	scull_devices = kmalloc(scull_nr_devs * sizeof(struct scull_dev),GFP_KERNEL);
	if(!scull_devices) { //返回0,表示分配内存失败
		result = -ENOMEM;
		goto fail;
    }
	memset(scull_devices,0,scull_nr_devs * sizeof(struct scull_dev));//分配的内存空间初始化为0
     //在循环中初始化每个scull_dev设备：调用函数scull_setup_cdev()
	for(i=0;i<scull_nr_devs;i++) {
		scull_setup_cdev(&scull_devices[i],i);
    }
	return 0; /* succeed */
fail:
	scull_cleanup_module();
	return result;  
}
module_init(scull_init_module);
module_exit(scull_cleanup_module);

