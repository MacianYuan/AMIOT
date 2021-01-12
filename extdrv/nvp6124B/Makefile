#ifeq ($(KERNELRELEASE),)
#export MMZ_SRC_BASE=$(PWD)
#endif

ifeq ($(PARAM_FILE), )
	PARAM_FILE:=../../Makefile.param
		include $(PARAM_FILE)
endif
obj-m := nvp6124_ex.o
nvp6124_ex-objs := nvp6124_drv.o coax_protocol.o video_eq.o video_eq_6124b.o video.o motion.o audio.o


EXTRA_CFLAGS += -DHI_GPIO_I2C
EXTRA_CFLAGS += -I$(PWD)/../gpio_i2c

default:
	@make -C $(LINUX_ROOT) M=$(PWD) modules 

clean: 
	@make -C $(LINUX_ROOT) M=$(PWD) clean 


