KMOD=mypci
SRCS=mypci.c device_if.h bus_if.h pci_if.h

.include <bsd.kmod.mk>
