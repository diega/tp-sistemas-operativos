#include <linux/kernel.h>           //es un programa del kernel
#include <linux/module.h>           //contiene macros para hacer modulos
#include <linux/proc_fs.h>          //manejo de /proc
#include <linux/stat.h>             //constantes de archivos
#include <linux/init.h>
#include <asm/uaccess.h>            //para copiar de user memory
#include <linux/tty.h>              //para tty_struct
#include <linux/kd.h>
#include <linux/console_struct.h>   //para vc_cons, vc y vc_data
#include <linux/vt_kern.h>

#define procfs_name "keyboard_leds"

struct proc_dir_entry* proc_file;
struct tty_operations* keyboard_operations;

int procfile_write(struct file* file, const char* buffer, unsigned long count, void* data){
    char selected_led;

    if(copy_from_user(&selected_led, buffer, 1)){
        return -EFAULT;
    }
    switch(selected_led){
    case '1':
        printk(KERN_ALERT "PRESIONA 1\n");
	(keyboard_operations->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, LED_NUM);
        break;
    case '2':
        printk(KERN_ALERT "PRESIONA 2\n");
	(keyboard_operations->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, LED_CAP);
        break;
    case '3':
        printk(KERN_ALERT "PRESIONA 3\n");
	(keyboard_operations->ioctl) (vc_cons[fg_console].d->vc_tty, NULL, KDSETLED, LED_SCR);
        break;
    default:
        printk(KERN_ALERT "INPUT DESCONOCIDO\n");
    }
    return 3;
} 


static int __init load_module(void){
    if((proc_file = create_proc_entry(procfs_name, 0664, NULL)) == NULL){
        remove_proc_entry(procfs_name, NULL);
        printk(KERN_ALERT "Error: Could not initialize /proc/%s\n", procfs_name);
        return -ENOMEM;
    }

    proc_file->owner = THIS_MODULE;
    proc_file->mode  = S_IFREG | S_IRWXUGO;
    proc_file->uid   = 0;
    proc_file->gid   = 0;
    proc_file->size  = 37;
    proc_file->write_proc = procfile_write;
    printk(KERN_INFO "/proc/%s created\n", procfs_name);

    keyboard_operations = vc_cons[fg_console].d->vc_tty->ops;
    return 0;
}

static void __exit remove_module(void){
    remove_proc_entry(procfs_name, NULL);
    printk(KERN_INFO "/proc/%s removed\n", procfs_name);
}

module_init(load_module)
module_exit(remove_module)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sistemas Operativos - Grupo XX");
MODULE_DESCRIPTION("Ejercicio 3.6.6 - permite prender o apager cada LED del teclado escribiendo en /proc/keyboard_leds");
